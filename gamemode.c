#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <dbus/dbus.h>

#ifdef NDEBUG
#define LOG(...)
#else
#define LOG(...) fprintf(stderr, __VA_ARGS__) 
#endif

__attribute__((always_inline))
static inline DBusConnection* get_session_bus() {
    DBusConnection* conn;
	DBusError err;

    dbus_error_init(&err);
	conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
	if (dbus_error_is_set(&err)) {
		LOG("DBus Connection Error:%s\n", err.message);
		dbus_error_free(&err);
	}
	if (!conn) {
		return NULL;
	}

    return conn;
}

__attribute__((always_inline))
static inline int32_t send_request(const char* restrict method) {
    DBusMessage* msg;
    DBusConnection* conn;
    DBusMessageIter args;
    DBusPendingCall* pending;
    DBusError error;

    int32_t pid = getpid();
    int32_t result = -1;
    
    if (!(conn = get_session_bus()))
        return -1;
    
    msg = dbus_message_new_method_call(
        "com.feralinteractive.GameMode",
        "/com/feralinteractive/GameMode",
        "com.feralinteractive.GameMode",
        method
    );
    if (!msg) {
        LOG("DBus create message returned NULL\n");
        return -1;
    }

    dbus_message_append_args(msg, DBUS_TYPE_INT32, &pid, DBUS_TYPE_INVALID);
    dbus_connection_send_with_reply (conn, msg, &pending, -1);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    dbus_pending_call_block(pending);
	if (!(msg = dbus_pending_call_steal_reply(pending))) {
		LOG("DBus reply message returned NULL\n");
		return -1;
	}

	dbus_pending_call_unref(pending);

	dbus_error_init(&error);
	if (dbus_set_error_from_message(&error, msg)) {
		LOG("DBus error rely: %s\n", error.message);
		dbus_error_free(&error);
	} else if (!dbus_message_iter_init(msg, &args)) {
		LOG("DBus method %s did not return anything\n");
	} else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) {
		LOG("DBus method %s return type was not INT32\n");
	} else {
		dbus_message_iter_get_basic(&args, &result);
	}

    dbus_message_unref(msg);

    return result;
}

__attribute__((constructor))
void enable_gamemode() {
    dbus_threads_init_default();
    send_request("RegisterGame");
}

__attribute__((destructor))
void disable_gamemode() {
    send_request("UnregisterGame");
}

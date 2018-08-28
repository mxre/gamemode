CFLAGS=-O3 -D_FORTIFY_SOURCE=2 -DNDEBUG
INCLUDE=-I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include
LDFLAGS=-Wl,-S,-O1,-z,relro,-z,now

STEAM_RTL=$(HOME)/.var/app/com.valvesoftware.Steam/data/Steam/ubuntu12_32/steam-runtime
XDG_DATA_PATH=$(HOME)/.var/app/com.valvesoftware.Steam/data

.PHONY: all clean

all: gamemode64.so gamemode32.so

clean:
	rm -f gamemode64.so gamemode64.o gamemode32.so gamemode32.o

gamemode64.so: gamemode64.o
	gcc -o $@ $< -shared $(LDFLAGS) $(STEAM_RTL)/amd64/lib/x86_64-linux-gnu/libdbus-1.so.3

gamemode64.o: gamemode.c
	gcc -o $@ -c $< -fPIC $(CFLAGS) $(INCLUDE)

gamemode32.so: gamemode32.o
	gcc -m32 -o $@ $< -shared $(LDFLAGS) $(STEAM_RTL)/i386/lib/i386-linux-gnu/libdbus-1.so.3

gamemode32.o: gamemode.c
	gcc -m32 -o $@ -c $< -fPIC $(CFLAGS) $(INCLUDE)

install:
	install -m644 gamemode64.so $(XDG_DATA_PATH)/gamemode/lib64/gamemode.so
	install -m644 gamemode32.so $(XDG_DATA_PATH)/gamemode/lib32/gamemode.so

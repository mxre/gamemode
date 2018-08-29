This version of `gamemode.so` aims to replace `libgamemodeauto.so` provided by
https://github.com/FeralInteractive/gamemode.

In contrast to Feral Interactive's gamemode library this shared object does not depend
on `libsystemd.so` which is not contained in the Steam runtime library. This version
is also capable to be use from inside a flatpak c.f., https://github.com/flathub/com.valvesoftware.Steam

## Use with Steam
To use a game in Steam with `gamemode.so` add `LD_PRELOAD=<path>/gamemode.so` to the *Launch Options*.
I personaly use `$XDG_DATA_HOME/gamemode/lib32/gamemode.so`, where `$XDG_DATA_HOME` is inside flatpak,
so its actually `$HOME/.var/app/com.valvesoftware.Steam/data`.

## Building
`gamemode.so` is linked against `libdbus-1.so.3` from the Steam runtime and needs this runtime
to be installed for linking.

Currently this Project doesn't use a configuration tool. All paths are set in
the Makefile.

all:
	gcc -O3 -fPIC -shared *.c -o libeps.so

windows:
	i686-w64-mingw32-gcc -O3 -shared -Wl,--add-stdcall-alias -Wl,--strip-all -Wl,--gc-sections -Wl,--discard-all -static-libgcc -D_WIN32 *.c -o libeps32.dll
	x86_64-w64-mingw32-gcc -O3 -shared -Wl,--add-stdcall-alias -Wl,--strip-all -Wl,--gc-sections -Wl,--discard-all -static-libgcc -D_WIN32 *.c -o libeps64.dll

clean:
	rm -f libeps.so
	rm -f libeps*.dll

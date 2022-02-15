%default: main

ifeq ($(OS),Windows_NT)
EXT=.exe
else
EXT=
endif

main:
	make clean
	gcc -Os -s src/*.c -o bin/flot$(EXT)

clean:
ifeq ($(OS),Windows_NT)
	- del bin/flot.exe
else
	- rm -f bin/flot
endif


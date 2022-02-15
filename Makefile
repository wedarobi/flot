BIN_DIR = bin

%default: main

ifeq ($(OS),Windows_NT)
EXT=.exe
else
EXT=
endif

main: $(BIN_DIR)
	gcc -Os -s src/*.c -o $(BIN_DIR)/flot$(EXT)

clean:
ifeq ($(OS),Windows_NT)
	- del /s /q $(BIN_DIR)
else
	- rm -rf $(BIN_DIR)
endif

$(BIN_DIR):
	mkdir $@

.PHONY: main clean

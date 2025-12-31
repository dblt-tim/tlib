

CC = clang

SRC = source
INC = include
BUILD = build

CPPFLAGS = -I$(INC) -fPIC -DTLIB_TRACK_LEAKS


COMPILE = $(CC) $(CPPFLAGS) -c $(SRC)/

.PHONY: all clean install reinstall

all: $(BUILD)/bin/tlib.so

clean:
	rm -r $(BUILD)

$(BUILD):
	mkdir -p $(BUILD)/bin

$(BUILD)/string.o: $(BUILD) $(SRC)/string.c
	$(CC) $(CPPFLAGS) -o $(BUILD)/string.o -c $(SRC)/string.c
$(BUILD)/result.o: $(BUILD) $(SRC)/result.c
	$(CC) $(CPPFLAGS) -o $(BUILD)/result.o -c $(SRC)/result.c
$(BUILD)/alloc.o: $(BUILD) $(SRC)/alloc.c
	$(CC) $(CPPFLAGS) -o $(BUILD)/alloc.o -c $(SRC)/alloc.c

$(BUILD)/bin/tlib.so: $(BUILD)/result.o $(BUILD)/string.o $(BUILD)/alloc.o $(BUILD)
	$(CC) -shared -o $(BUILD)/bin/tlib.so $(BUILD)/result.o $(BUILD)/string.o $(BUILD)/alloc.o


install: $(BUILD)/bin/tlib.so
	sudo cp $(BUILD)/bin/tlib.so /usr/local/lib/libtlib.so
	sudo cp -r include/tlib /usr/local/include/tlib

	sudo ldconfig

reinstall: $(BUILD)/bin/tlib.so
	sudo rm /usr/local/lib/libtlib.so
	sudo rm -r /usr/local/include/tlib
	$(MAKE) install

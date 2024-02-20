CC = g++ # compiler
CFLAGS = -g -Wall -Wextra # flags

# source files
SRC = src/main.cpp src/profiles.cpp src/frame.cpp src/io_helper.cpp

# target files
TARGET = build/dwarf_galaxy_analyzer.exe

.PHONY: all clean # phony targets (not files)

all: $(TARGET)

# building density calculator
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $@ $(FLAGS)
#             target ^

# cleaning
clean:
	rm -f $(TARGET)
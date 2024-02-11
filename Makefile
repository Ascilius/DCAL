CC = g++ # compiler
CFLAGS = -g -Wall -Wextra # flags

# source files
SRC1 = src/density_calculator.cpp
SRC2 = src/SIDM_profile.cpp

# target files
TARGET1 = build/density_calculator.exe
TARGET2 = build/SIDM_profile.exe

.PHONY: all auto clean # phony targets (not files)

all: $(TARGET1) $(TARGET2)

# building density calculator
$(TARGET1): $(SRC1)
	$(CC) $(SRC1) -o $@ $(FLAGS)
#              target ^

# building SIDM profiler
$(TARGET2): $(SRC2)
	$(CC) $(SRC2) -o $@ $(FLAGS)

auto: build/density_auto_calculator.exe

# building auto calculator
build/density_auto_calculator.exe: src/density_auto_calculator.cpp
	$(CC) src/density_auto_calculator.cpp -o $@ $(FLAGS)

# cleaning
clean:
	rm -f $(TARGET1) $(TARGET2)
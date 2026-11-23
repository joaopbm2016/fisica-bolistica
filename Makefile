CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)

ifeq ($(RAYLIB_LIBS),)
RAYLIB_LIBS := -lraylib -lm -lpthread -ldl
endif

SOURCE := main.c++
TARGET := wind

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(RAYLIB_CFLAGS) $(SOURCE) $(RAYLIB_LIBS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

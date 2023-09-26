CC = g++
FLAGS = -Wall -Werror -Wextra
BUILD = ../build

TARGET = $(BUILD)/interpolation.exe

all: build
build: $(TARGET)
rebuild: clean build

run: $(TARGET)
	./$(TARGET)

$(TARGET): interpolation.cpp
	$(CC) $(FLAGS) $< -o $(TARGET)

#------------------------CLEAN
clean:
	rm -rf $(BUILD)/*

#------------------------CLANG
clang_n: *.cpp
	cp ../materials/linters/.clang-format .
	clang-format -n -style=file *.cpp
	rm .clang-format
clang_i: *.cpp
	cp ../materials/linters/.clang-format .
	clang-format -i -style=file *.cpp
	rm .clang-format
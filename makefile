# simple makefile for clang-tidy and compiling
#
# Liyao Zhang 
# 10/26/2020
# CSC 250 Lab 5

SOURCE = lab4.c
EXECUTABLE = lab4
COMPILE_FLAGS = -ansi -pedantic -Wall -g

all: tidy
	gcc $(COMPILE_FLAGS) -o $(EXECUTABLE) $(SOURCE) get_image_args.c ppm_read_write.c

tidy: $(SOURCE)
	clang-tidy -checks='*' $(SOURCE) -- -std=c99

clean:
	rm -rf $(EXECUTABLE)

default:
	gcc main.c -o program -g -lm $(shell sdl2-config --cflags --libs)

.PHONY: generate_compile_commands
generate_compile_commands:
	bear -- make

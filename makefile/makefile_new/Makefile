SRC=$(wildcard ./*.c)
OBJ=$(patsubst ./%.c,./%.o,$(SRC))

main: $(OBJ)
	gcc $^ -o $@ -Wall

%.o: %.c
	gcc -c $< -o $@ -Wall

.PHONY: clean

clean:
	rm $(OBJ) main -f

SRC=$(wildcard ./*.c)
OBJ=$(patsubst ./*.c,./*.o,$(SRC))

test: $(OBJ)
	gcc $^ -o $@

%o:%c
	gcc -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o test

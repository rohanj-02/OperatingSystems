FILENAME = main.c semaphore.c
BINARY_NAME = a.out

compile:
	gcc $(FILENAME) -pthread -o $(BINARY_NAME)

run: compile
	./$(BINARY_NAME)

clean:
	rm -f $(BINARY_NAME)

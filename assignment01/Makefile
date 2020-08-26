FILENAME := hello

all: FILENAME 

FILENAME.i : 
	@echo "Pre-processing..."
	gcc -E $(FILENAME).c > $(FILENAME).i

FILENAME.s : FILENAME.i 
	@echo "Converting to Assembly Instructions(Compilation)..."
	gcc -S $(FILENAME).i

FILENAME.o : FILENAME.s 
	@echo "Converting to Object Code(Assembly)..."
	gcc -c $(FILENAME).s

FILENAME : FILENAME.o 
	@echo "Linking..."
	gcc -o $(FILENAME) $(FILENAME).o

showObject: 
	@echo "Showing Object Code"
	od -c $(FILENAME).o

clean:
	@echo "Cleaning up"
	rm $(FILENAME) $(FILENAME).s $(FILENAME).i $(FILENAME).o

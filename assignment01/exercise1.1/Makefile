# Written by Rohan Jain 2019095
FILENAME := q1

all: FILENAME 

FILENAME.i : 
	@echo "Pre-processing..."
	@gcc -E $(FILENAME).c > $(FILENAME).i
	@read -p "Press enter to continue to the next stage of compilation" next_step;

FILENAME.s : FILENAME.i 
	@echo "Converting to Assembly Instructions(Compilation)..."
	@gcc -S $(FILENAME).i
	@read -p "Press enter to continue to the next stage of compilation" next_step;

FILENAME.o : FILENAME.s 
	@echo "Converting to Object Code(Assembly)..."
	@gcc -c $(FILENAME).s
	@read -p "Press enter to continue to the next stage of compilation" next_step;

FILENAME : FILENAME.o 
	@echo "Linking..."
	@gcc -o $(FILENAME) $(FILENAME).o
	@echo "Compilation Finished!!!"
	@echo "To run the program, execute: ./$(FILENAME)"

showObject: FILENAME.o
	@echo "Showing Object Code"
	od -c $(FILENAME).o

clean:
	@echo "Cleaning up"
	rm $(FILENAME) $(FILENAME).s $(FILENAME).i $(FILENAME).o

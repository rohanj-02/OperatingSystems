default: EXTERNAL
	gcc -o main main.c internal.c

EXTERNAL:
	echo "Running"
	cd external && make

clean:
	cd external && make clean
default: EXTERNAL
	gcc -o main main.c internal.c echo.c

EXTERNAL:
	echo "Running"
	cd external && make

clean:
	cd external && make clean
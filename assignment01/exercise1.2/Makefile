default: EXTERNAL
	gcc -o main main.c internal.c echo.c

EXTERNAL:
	echo "Running"
	cd external && make

rmPopulate:
	mkdir test
	cd test && mkdir test2
	echo "test" > test.txt

clean:
	cd external && make clean
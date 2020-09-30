default: EXTERNAL
	@gcc -o main main.c cd.c echo.c ./external/printDocs.c

EXTERNAL:
	@echo "Running"
	@cd external && make

rmPopulate:
	@mkdir test
	@cd test && mkdir test2
	@echo "test" > test.txt

clean:
	@cd external && make clean
	@rm main
	@rm -rf test test.txt

run: default
	./main
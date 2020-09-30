default: EXTERNAL
	@gcc -o main main.c cd.c echo.c ./external/printDocs.c
	@echo "Run program using ./main"

EXTERNAL:
	@echo "Running"
	@cd external && make

rmPopulate:
	@mkdir test
	@cd test && mkdir test2
	@echo "test" > test.txt
	@echo "test" > test2.txt
	@echo "test" > test3.txt

clean:
	@cd external && make clean
	@rm main
	@rm -rf test test.txt test2.txt test3.txt

run: default
	./main
build:
	@gcc -c -m32 c_func.c -o c_func.o
	@g++ -c -m32 cpp_func.cpp -o cpp_func.o
	@nasm -f elf32 asm_utils.asm -o asm_utils.o
	@g++ -c -m32 main.cpp -o main.o
	@g++ -m32 c_func.o cpp_func.o asm_utils.o main.o -o main.out

run:
	@./main.out

clean:
	@rm -f *.o

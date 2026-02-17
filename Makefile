all:
	nasm -f elf64 modpow.asm -o modpow.o
	g++ -o modpow modpow.cpp modpow.o -no-pie

clean:
	rm -f modpow.o modpow

run: all
	./modpow
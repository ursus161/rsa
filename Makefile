all:
	nasm -f elf64 modpow.asm -o modpow.o
	g++ -O3 -o modpow modpow.cpp barret_mod.cpp modpow.o -no-pie

clean:
	rm -f modpow.o modpow

run: all
	./modpow
all:
	 
	g++ -O3 -o modpow modpow.cpp barret_mod.cpp  -no-pie

clean:
	rm -f modpow

run: all
	./modpow
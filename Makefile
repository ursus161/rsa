all:
	 
	g++ -O3 -o modpow modpow.cpp barret_mod.cpp  -no-pie
	g++ -O3 -o bigint bigint.cpp  -no-pie

clean:
	rm -f modpow bigint

run: all
	./modpow

run_bigint: all
	./bigint
all:
	 
	g++ -O3 -o modpow modpow.cpp barret_mod.cpp  -no-pie
	g++  -o rsakey rsakey.cpp  -no-pie

clean:
	rm -f modpow bigint rsakey

run: all
	./modpow

run_rsakey: all
	./rsakey
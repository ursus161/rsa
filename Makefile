all:
	g++ -O3 -o modpow modpow.cpp barret_mod.cpp -no-pie
	g++ -o rsa main.cpp rsaengine.cpp rsakey.cpp bigint.cpp -no-pie

clean:
	rm -f modpow rsa

run: all
	./modpow

run_rsa: all
	./rsa
all:
	
	g++ -o rsa main.cpp message.cpp rsaengine.cpp rsakey.cpp bigint.cpp -no-pie

clean:
	rm -f modpow rsa

run: all
	./modpow

run_rsa: all
	./rsa
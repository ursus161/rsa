CXX = g++
CXXFLAGS = -march=native -funroll-loops -no-pie

all:
	$(CXX) $(CXXFLAGS) -o rsa main.cpp message.cpp rsaengine.cpp rsakey.cpp bigint.cpp

clean:
	rm -f rsa

run_rsa: all
	./rsa

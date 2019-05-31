




code: code.o
	rm data/*.txt
	g++ -o code code.o  -std=c++11



code.o: front_end/code.cpp
	g++  -c front_end/code.cpp -std=c++11




clean:
	rm code *.o data/*.txt


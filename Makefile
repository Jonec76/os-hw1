CFLAGS = -B -g -Wall -O2 -std=c++11

.PHONY: clean main

main: main.cpp
	g++ -o $@ $< $(CFLAGS)
	./main data1000.txt

clean:
	find . -name "sorted*" -delete
	rm main

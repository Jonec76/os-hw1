CFLAGS = -B -g -Wall -O2 -std=c++11
OBJS := merge_sort.o min_heap.o

.PHONY: clean main

main: main.cpp $(OBJS)
	g++ -o $@ $< $(OBJS) $(CFLAGS)
	./main data_test.txt

create_data: create_data.cpp
	g++ -o $@ $< $(CFLAGS)

%.o: %.cpp *.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	find . -name "sorted*" -delete
	rm main

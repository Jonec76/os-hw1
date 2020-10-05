CFLAGS = -B -g -std=c++11
OBJS := merge_sort.o min_heap.o

.PHONY: clean main

main: main.cpp $(OBJS)
	g++ -o $@ $< $(OBJS) $(CFLAGS)

create_data: create_data.cpp
	g++ -o $@ $< $(CFLAGS)

%.o: %.cpp *.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	find . -name "sorted*" -delete
	find . -name "output*" -delete
	find . -name "tmp*" -delete
	find . -name "*.o" -delete
	find . -name "main" -delete
	find . -name "create_data" -delete

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#ifndef MIN_HEAP_H
#define MIN_HEAP_H


struct Node {
    int ID;
    int value;
};

class MinHeap {
   private:
    vector<Node> heap;
    void heapify(int i);
    // Returns the index of the Right child of the ith element
    // Returns -1 if the index > heap size
    int left(int i) ;

    // Returns the index of the Leftchild of the ith element
    // Returns -1 if the index > heap size
    int right(int i) ;

    // Returns the index of the Parent of the ith element
    // Returns -1 if parent-index < 0
    int parent(int i) ;

   public:
    //Check the size of Heap
    bool is_empty() ;
    void insert(Node val) ;
    void showheap() ;
    // take out the min and delete it
    int extract_min_value() ;
    int extract_min_id() ;
};

#endif

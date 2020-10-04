#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "min_heap.h"

using namespace std;

void MinHeap::heapify(int i) {
    int l = left(i);
    int r = right(i);

    // find the smallest amongst the parent, it's left and right child
    int smallest;
    if (l != -1 && heap[l].value < heap[i].value) {
        smallest = l;
    } else {
        smallest = i;
    }
    if (r != -1 && heap[r].value < heap[smallest].value) {
        smallest = r;
    }

    // If parent is the smallest, then we don't need to modify heap
    if (smallest != i) {
        swap(heap[i], heap[smallest]);
        heapify(smallest);
    }
}
// Returns the index of the Right child of the ith element
// Returns -1 if the index > heap size
int MinHeap::left(int i) {
    int l;
    if (((2 * i) + 1) <= heap.size() - 1) {
        l = (2 * i) + 1;
    } else {
        l = -1;
    }
    return l;
}

// Returns the index of the Leftchild of the ith element
// Returns -1 if the index > heap size
int MinHeap::right(int i) {
    int r;
    if (((2 * i) + 2) <= heap.size() - 1) {
        r = (2 * i) + 2;
    } else {
        r = -1;
    }
    return r;
}

// Returns the index of the Parent of the ith element
// Returns -1 if parent-index < 0
int MinHeap::parent(int i) {
    int p;
    if (((i - 1) / 2) >= 0) {
        p = (i - 1) / 2;
    } else {
        p = -1;
    }
    return p;
}

//Check the size of Heap
bool MinHeap::is_empty() {
    if (heap.size() == 0) {
        return true;
    } else {
        return false;
    }
}
void MinHeap::insert(Node val) {
    // insert the new element at the end of heap
    heap.push_back(val);

    // move the new value to its right position
    int i = heap.size() - 1;
    while (heap[parent(i)].value > heap[i].value) {
        swap(heap[parent(i)], heap[i]);
        i = parent(i);
    }
}
void MinHeap::showheap() {
    for (int i = 0; i < heap.size(); i++) {
        cout << heap[i].value << " ";
    }
    cout << endl;
}
// take out the min and delete it
int MinHeap::extract_min_value() {
    int min = heap[0].value;
    // let the heap[0] be the value of the last node, and modify the heap position
    if (heap.size() != 1) {
        heap[0] = heap.back();
        heap.pop_back();
        heapify(0);
    } else {
        heap.pop_back();
    }

    return min;
}
int MinHeap::extract_min_id() {
    int min = heap[0].ID;
    return min;
}
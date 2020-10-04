#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const long long int read_chunk_size = 1;
const long long int FULL_FILE_SIZE = 10;
const char* output_sorted_name = "./sorted";
const char* output_final_name = "./output.txt";
int END = 0;

long long int c = 1ULL << 32;
//Merge sort
void M(long long int *A, long long int p, long long int q, long long int r) {
    long long int n1 = q - p + 1;
    long long int n2 = r - q;
    long long int *L = new long long int[n1 + 1];
    long long int *R = new long long int[n2 + 1];

    for (long long int i = 0; i < n1; i++) {
        L[i] = A[p + i - 1];
    }
    for (long long int j = 0; j < n2; j++) {
        R[j] = A[q + j];
    }
    L[n1] = c;
    R[n2] = c;
    long long int i = 0;
    long long int j = 0;
    for (long long int k = p - 1; k < r; k++) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i = i + 1;
        } else {
            A[k] = R[j];
            j = j + 1;
        }
    }
    delete[] L;
    delete[] R;
};

void merge_sort(long long int *A, long long int p, long long int r) {
    if (p < r) {
        long long int q = (p + r) / 2;
        merge_sort(A, p, q);
        merge_sort(A, q + 1, r);
        M(A, p, q, r);
    }
};
//external sort use min heap
struct Node {
    int ID;
    int value;
};
class MinHeap {
   private:
    vector<Node> heap;
    void heapify(int i) {
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
    int left(int i) {
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
    int right(int i) {
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
    int parent(int i) {
        int p;
        if (((i - 1) / 2) >= 0) {
            p = (i - 1) / 2;
        } else {
            p = -1;
        }
        return p;
    }

   public:
    //Check the size of Heap
    bool is_empty() {
        if (heap.size() == 0) {
            return true;
        } else {
            return false;
        }
    }
    void insert(Node val) {
        // insert the new element at the end of heap
        heap.push_back(val);

        // move the new value to its right position
        int i = heap.size() - 1;
        while (heap[parent(i)].value > heap[i].value) {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
            /*if(heap[i].value>heap[i+1].value)
            {
                swap(heap[parent(i)] ,heap[i]);
			}*/
        }
    }
    void showheap() {
        for (int i = 0; i < heap.size(); i++) {
            cout << heap[i].value << " ";
        }
        cout << endl;
    }
    // take out the min and delete it
    int extract_min_value() {
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
    int extract_min_id() {
        int min = heap[0].ID;
        return min;
    }
};

void sort_sub_data(long long int *sub_data, int data_size, long long int data_ctr){
    merge_sort(sub_data, 1, data_size);
    fstream sorted_file;

    sorted_file.open(output_sorted_name + to_string((int)ceil((double)data_ctr / FULL_FILE_SIZE)) + ".txt", ios::out);

    if (sorted_file.fail()) {
        cout << "Can't open sorted_file" << endl;
    } else {
        for (long long int i = 0; i < data_size; ++i) {
            sorted_file << sub_data[i] << endl;
        }

        sorted_file.close();
    }
}

void external_sort(long long sorted_index){
    MinHeap min_heap;
    fstream sorted_file[sorted_index];
    for (int i = 1; i <= sorted_index; i++) {
        sorted_file[i - 1].open(output_sorted_name + to_string(i) + ".txt");
        string line;
        Node node;
        getline(sorted_file[i - 1], line);
        node.ID = i;
        node.value = stoi(line);
        min_heap.insert(node);
    }
    fstream big_f;
    big_f.open(output_final_name, ios::out);
    do {
        long long next_file_id = min_heap.extract_min_id();
        string line;
        Node node;

        big_f << min_heap.extract_min_value() << endl;
        getline(sorted_file[next_file_id - 1], line);
        if (line != "") {
            node.ID = next_file_id;
            node.value = stoi(line);
            min_heap.insert(node);
        } else {
            sorted_file[next_file_id - 1].close();
            string path = output_sorted_name + to_string(next_file_id) + ".txt";
            const char *p = path.c_str();
            remove(p);
        }
    } while (!min_heap.is_empty());
    big_f.close();
}

//main function
int main(int argc, char *argv[]) {

    if(argc != 2)
        printf("Wrong parameters for main");

    vector<char> number;
    long long data_size = 10, sorted_index = 0;
    long long int *sub_data = new long long int[data_size];
    string tmp_str;
    long long int data_ctr = 0;
    FILE *fp = freopen(argv[1], "rb", stdin);
    if(!fp){
        printf("File %s is not exist.", argv[1]);
    }
    do {
        char *buf = (char *)malloc(read_chunk_size);
        int read_len = fread(buf, 1, read_chunk_size, stdin);
        for (int i = 0; i < read_len; i++) {
            number.push_back(buf[i]);
            if (buf[i] == '\n') {
                tmp_str = "";
                for (int j = 0; j < number.size(); j++) {
                    tmp_str += number[j];
                }
                number.clear();
                sub_data[data_ctr % data_size] = (stoi(tmp_str));
                data_ctr++;
                if (data_ctr % data_size == 0) {
                    sort_sub_data(sub_data, data_size, data_ctr);
                    sorted_index = data_ctr / data_size;
                }
            }
        }
        if (read_len != read_chunk_size) { 
            END = true;// Set END=true at last step.
            if (data_ctr % data_size != 0) {
                sort_sub_data(sub_data, data_ctr % data_size, data_ctr);
                sorted_index++;
            }
        }
        free(buf);
    } while (!END);
    fclose(fp);
    delete[] sub_data;

    external_sort(sorted_index);
    return 0;
}

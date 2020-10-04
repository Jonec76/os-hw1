#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "merge_sort.h"
#include "min_heap.h"

using namespace std;

const long long int read_chunk_size = 1;
const long long int FULL_FILE_SIZE = 10;
const char *output_sorted_name = "./sorted";
const char *output_final_name = "./output.txt";
int END = 0;

void sort_sub_data(long long int *sub_data, int data_size, long long int data_ctr) {
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

void external_sort(long long sorted_index) {
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
    if (argc != 2)
        printf("Wrong parameters for main");

    vector<char> number;
    long long data_size = 10, sorted_index = 0;
    long long int *sub_data = new long long int[data_size];
    string tmp_str;
    long long int data_ctr = 0;
    FILE *fp = freopen(argv[1], "rb", stdin);
    if (!fp) {
        printf("File %s is not exist.", argv[1]);
    }
    do {
        char *buf = (char *)malloc(read_chunk_size);
        int read_len = fread(buf, 1, read_chunk_size, stdin);
        for (int i = 0; i < read_len; i++) {
            number.push_back(buf[i]);
            if (buf[i] == '\n') {
                tmp_str = "";
                for (size_t j = 0; j < number.size(); j++) {
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
            END = true;  // Set END=true at last step.
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

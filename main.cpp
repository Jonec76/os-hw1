#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 

#include "merge_sort.h"
#include "min_heap.h"

using namespace std;

const long long int read_chunk_size = 200000000;
long long int FULL_FILE_SIZE;
const int fstream_size = 4000;
const char *output_sorted_name = "./sorted";
string output_final_name = "./output.txt";
char TIME_RESULT[50] ;
int END = 0;
int file_idx = 0;


void sort_sub_data(long long int *sub_data, int data_size, long long int data_ctr) {
    merge_sort(sub_data, 1, data_size);
    fstream sorted_file;
    sorted_file.open(output_sorted_name + to_string((int)ceil((double)data_ctr / FULL_FILE_SIZE)-1) + ".txt", ios::out);
    if (sorted_file.fail()) {
        cout<<to_string((int)ceil((double)data_ctr / FULL_FILE_SIZE)-1)<<"??";
        // cout << "Can't open sorted_file" << endl;
    } else {
        for (long long int i = 0; i < data_size; ++i) {
            sorted_file << sub_data[i] << endl;
        }
        sorted_file.close();
    }
}

int e_ctr = 0;
void external_sort(long long sorted_index, int start, int end, bool finish) {
    FILE* pFile = fopen (TIME_RESULT, "a");
    if (pFile == NULL) {
        printf("Failed to open file %s.", TIME_RESULT);
        exit(EXIT_FAILURE);
    }

    clock_t external_begin_time = clock();

    MinHeap min_heap;
    fstream sorted_file[end - start +1];
    for (long long int i = 0; i < end - start; i++) {
        sorted_file[i].open(output_sorted_name + to_string(i+start) + ".txt");
        string line;
        Node node;
        getline(sorted_file[i], line);
        node.ID = i;
        node.value = stoi(line);
        min_heap.insert(node);
    }

    if(start != 0){
        sorted_file[end-start].open("tmp"+to_string(file_idx)+".txt");
        file_idx++;
        string line;
        Node node;
        getline(sorted_file[end-start], line);
        node.ID = end-start;
        node.value = stoi(line);
        min_heap.insert(node);
    }
    fstream big_f;
    if(finish)
        output_final_name = "output.txt";
    else{
        output_final_name = "tmp"+to_string(file_idx)+".txt";
    }
    big_f.open(output_final_name,ios::out);
    do {
        long long next_file_id = min_heap.extract_min_id();
        string line;
        Node node;

        big_f << min_heap.extract_min_value() << endl;
        getline(sorted_file[next_file_id], line);
        if (line != "") {
            node.ID = next_file_id;
            node.value = stoi(line);
            min_heap.insert(node);
        } else {
            sorted_file[next_file_id].close();
            if(next_file_id != end-start){
                string path = output_sorted_name + to_string(next_file_id + start) + ".txt";
                const char *p = path.c_str();
                remove(p);
            }
        }
    } while (!min_heap.is_empty());
    big_f.close();
    fprintf (pFile, "external: %f", float( clock () - external_begin_time ) /  CLOCKS_PER_SEC);
    fprintf (pFile, "\n");
    fclose(pFile);
}

//main function
int main(int argc, char *argv[]) {
    if (argc != 2)
        printf("Wrong parameters for main");

    vector<char> number;
    long long sorted_index = 0;
    string tmp_str;
    long long int data_ctr = 0;
    string input_data(argv[1], 30);

    sprintf(TIME_RESULT, "time_%lld_%s", FULL_FILE_SIZE, argv[1]);
    FILE* pFile = fopen (TIME_RESULT, "a");
    if (pFile == NULL) {
        printf("Failed to open file %s.", TIME_RESULT);
        exit(EXIT_FAILURE);
    }

    FILE *fp = freopen(argv[1], "rb", stdin);
    if (!fp) {
        printf("File %s is not exist.", argv[1]);
    }

    fseek(fp, 0L, SEEK_END);
    long long sz = ftell(fp);
    fclose(fp);
    FULL_FILE_SIZE = sz / 10000;

    fp = freopen(argv[1], "rb", stdin);
    if (!fp) {
        printf("File %s is not exist.", argv[1]);
    }

    long long int *sub_data = new long long int[FULL_FILE_SIZE];

    clock_t program_begin_time = clock();

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
                sub_data[data_ctr % FULL_FILE_SIZE] = (stoi(tmp_str));
                data_ctr++;
                if (data_ctr % FULL_FILE_SIZE == 0) {
                    sort_sub_data(sub_data, FULL_FILE_SIZE, data_ctr);
                    sorted_index = data_ctr / FULL_FILE_SIZE;
                }
            }
        }
        if (read_len != read_chunk_size) {
            END = true;  // Set END=true at last step.
            if (data_ctr % FULL_FILE_SIZE != 0) {
                sort_sub_data(sub_data, data_ctr % FULL_FILE_SIZE, data_ctr);
                sorted_index++;
            }
        }
        free(buf);
    } while (!END);
    fclose(fp);
    delete[] sub_data;


    fprintf (pFile, "merge: %f", float( clock () - program_begin_time ) /  CLOCKS_PER_SEC);
    fprintf (pFile, "\n");
    fclose(pFile);

    for(int i=0;i<ceil((double)sorted_index / fstream_size);i++){
        int end;
        bool finish;
        if((i+1)*fstream_size > sorted_index){
            finish = true;
            end = sorted_index;
        }else{
            end = (i+1)*fstream_size;
            finish = false;
        }
        external_sort(sorted_index, i * fstream_size, end, finish);
    }
    for(int i=0;i<ceil((double)sorted_index / fstream_size);i++){
        string path = "tmp" + to_string(i) + ".txt";
        const char *p = path.c_str();
        remove(p);
    }
    pFile = fopen (TIME_RESULT, "a");
    if (pFile == NULL) {
        printf("Failed to open file %s.", TIME_RESULT);
        exit(EXIT_FAILURE);
    }
    fprintf (pFile, "total: %f", float( clock () - program_begin_time ) /  CLOCKS_PER_SEC);
    fprintf (pFile, "\n");
    return 0;
}

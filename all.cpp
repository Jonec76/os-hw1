#include<iostream>
#include<fstream>
#include <stdio.h>
#include<malloc.h>
#include<string>
#include <stdlib.h>
#include<vector>
//4567sec
using namespace std;

//everytime read 200000000 char
const long long int MAXS = 200000000;
int END=0;

long long int c=1ULL<<32;
//Merge sort
void M(long long int *A,long long int p,long long int q,long long int r)
{
    long long int n1=q-p+1;
    long long int n2=r-q;
    long long int *L=new long long int [n1+1];
    long long int *R=new long long int [n2+1];

    for(long long int i=0;i<n1;i++)
    {
        L[i]=A[p+i-1];
    }
    for(long long int j=0;j<n2;j++)
    {
        R[j]=A[q+j];
    }
    L[n1]=c;
    R[n2]=c;
    long long int i=0;
    long long int j=0;
    for (long long int k=p-1;k<r;k++)
    {
        if(L[i]<=R[j])
        {
            A[k]=L[i];
            i=i+1;
        }
        else
        {
            A[k]=R[j];
            j=j+1;
        }
    }
    delete [] L;
    delete [] R;
};

void MS(long long int *A,long long int p,long long int r)
{
    if(p<r)
    {
        long long int q=(p+r)/2;
        MS(A,p,q);
        MS(A,q+1,r);
        M(A,p,q,r);
    }
};
//external sort use min heap
struct H_D
{
	int ID;
	int value;
};
class MinHeap
{
private:
	vector<H_D> heap;
	void heapify(int i)
	{
		int l = left(i);
		int r = right(i);

		// find the smallest amongst the parent, it's left and right child
		int smallest;
		if (l != -1 && heap[l].value < heap[i].value)
		{
			smallest = l;
		}
		else
		{
			smallest = i;
		}
		if (r != -1 && heap[r].value < heap[smallest].value)
		{
			smallest = r;
		}

		// If parent is the smallest, then we don't need to modify heap
		if (smallest != i)
		{
			swap(heap[i], heap[smallest]);
			heapify(smallest);
		}
	}
	// Returns the index of the Right child of the ith element
	// Returns -1 if the index > heap size
	int left(int i)
	{
		int l;
		if (((2 * i) + 1) <= heap.size() - 1)
		{
			l = (2 * i) + 1;
		}
		else
		{
			l = -1;
		}
		return l;
	}

	// Returns the index of the Leftchild of the ith element
	// Returns -1 if the index > heap size
	int right(int i)
	{
		int r;
		if (((2 * i) + 2) <= heap.size() - 1)
		{
			r = (2 * i) + 2;
		}
		else
		{
			r = -1;
		}
		return r;
	}

	// Returns the index of the Parent of the ith element
	// Returns -1 if parent-index < 0
	int parent(int i)
	{
		int p;
		if (((i-1)/2) >= 0)
		{
			p = (i-1)/2;
		}
		else
		{
			p = -1;
		}
		return p;
	}

public:
	//Check the size of Heap
	bool Heap_size()
	{
		if (heap.size() != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void insert(H_D val)
	{
		// insert the new element at the end of heap
		heap.push_back(val);

		// move the new value to its right position
		int i = heap.size() - 1;
		while (heap[parent(i)].value > heap[i].value)
		{
			swap(heap[parent(i)] ,heap[i]);
			i = parent(i);
			/*if(heap[i].value>heap[i+1].value)
            {
                swap(heap[parent(i)] ,heap[i]);
			}*/
		}
	}
	void showheap()
    {
        for(int i=0;i<heap.size();i++)
        {
            cout<<heap[i].value<<" ";
        }
        cout<<endl;
	}
	// take out the min and delete it
	int extractMin_Value()
	{

		int min = heap[0].value;
		// let the heap[0] be the value of the last node, and modify the heap position
		if (heap.size() != 1)
		{
			heap[0] = heap.back();
			heap.pop_back();
			heapify(0);
		}
		else
		{
			heap.pop_back();
		}

		return min;
	}
	int extractMin_ID()
	{

		int min = heap[0].ID;
		return min;
	}
};

//main function
int main()
{
    vector<char> number;
    //how many numbers you want to sort in one file=================================================================================only need to change this
    int sort_n=10;
    int inputfiles;
    long long int *B = new long long int [sort_n];
    string real;
    long long int num=0;
    int have=0;
    freopen("data_512M.txt","rb",stdin);//===========================================================================================input file
    do
    {
        char * buf= (char*) malloc (MAXS);
        int len = fread(buf,1,MAXS,stdin);
        for (int i=0;i<MAXS;i++)
        {

            if(buf[i]=='\0')
            {
                END=1;
                if(have==1)
                {
                    MS(B,1,num%sort_n);
                    inputfiles=(num/sort_n)+1;
                    fstream file;

                    file.open("./output"+to_string(inputfiles)+".txt",ios::out);//======================you will have specific path

                    if(file.fail())
                    {
                        cout<<"Can't open file"<<endl;
                        cout<<inputfiles<<endl;
                    }
                    else
                    {
                        for (long long int i=0; i<(num%sort_n); ++i)
                        {
                            file << B[i] << endl;
                        }

                        file.close();
                    }
                }


                break;
            }
            number.push_back(buf[i]);
            if(buf[i]=='\n')
            {
                real=number[0];
                for(int j=1;j<number.size();j++)
                {
                    real=real+number[j];
                }
                number.clear();

                B[num%sort_n]=(stoi(real));
                num=num+1;
                have=1;
                if(num%sort_n==0)
                {
                    have=0;
                    MS(B,1,sort_n);
                    fstream file;

                    file.open("./output"+to_string(num/sort_n)+".txt",ios::out);

                    if(file.fail())
                    {
                        cout<<"Can't open file"<<endl;
                        cout<<to_string(num/sort_n)<<endl;

                    }
                    else
                    {
                        for (long long int i=0; i<sort_n; ++i)
                        {
                            file << B[i] << endl;
                        }

                        file.close();
                    }
                    inputfiles=num/sort_n;
                }
            }
        }
        free(buf);
        //cout<<num<<endl;
    }while(END==0);
     fclose(stdin);

    //cout<<numbers[0];
    delete [] B;

    MinHeap O;
    fstream f[inputfiles];
    for(int i=1;i<=inputfiles;i++)
    {
        f[i-1].open("./output"+to_string(i)+".txt");
        string line;
        H_D node;
        getline(f[i-1],line);
        node.ID=i;
        node.value=stoi(line);
        O.insert(node);
    }
    fstream big_f;
    big_f.open("./output.txt",ios::out);
    do
    {
        O.showheap();
        int next=O.extractMin_ID();
        big_f << O.extractMin_Value() << endl;
        string line;
        H_D node;
        getline(f[next-1],line);
        if(line!="")
        {
            node.ID=next;
            node.value=stoi(line);
            O.insert(node);
        }
        else
        {
            f[next-1].close();
            string path="./output"+to_string(next)+".txt";
            const char *p=path.c_str();
            remove(p);
            cout<<"output"+to_string(next)+"is empty"<<endl;
        }

    }while(O.Heap_size());
    big_f.close();
    return 0;
}


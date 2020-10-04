#include<iostream>
#include<fstream>
#include <random>

using namespace std;

int main()
{
    long long int a=1ULL<<31;
    a=a*-1;
    long long int b=1ULL<<31;
    b=b-1;
    long long int c=1ULL<<32;
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_int_distribution<int> dis(a,b);
    fstream file;
    file.open("data1000.txt",ios::out);
    if(file.fail())
    {
        cout<<"Can't open file"<<endl;
    }
    else
    {
        for (int i=0; i<77; ++i)
        {
            file << dis(gen) << endl;
        }

        cout<<"file is done!!"<<endl;
        file.close();
    }
    return 0;
}

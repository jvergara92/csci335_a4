#include "BinomialQueue.h"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;


/*BinomialQueue<string> populate(istream & in)
{
    string word;
    BinomialQueue<string> q = new BinomialQueue<string>();
    int line = 0;
    
    while(in>>word)
    {
        q.insert(word);
    }
    
    return q;
}*/

int main()
{
    map<string, BinomialQueue<string> *> hashTable;
    ifstream s;
    string d;
    BinomialQueue<string> q;
    s.open("words.txt");
    //BinomialQueue<string> q=populate(s);
    return 0;
}

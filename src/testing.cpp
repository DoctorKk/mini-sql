#include"minisql.h"
#include"BufferManager.h"
#include"CatalogManager.h"
#include"interpreter.h"
#include"RecordManager.h"
#include<fstream>

using namespace std;

int main() {
    //Interpreter ip;
    //ip.mainFunction();
    /*
    string record = "test/test/test\nthis/is/a/test";
    string temp = "\n";
    vector<string>* test = new vector<string>;
    split(record, temp, test);
    for (auto i: *test) {
        cout << i << endl;
    }
    */

    //IndexManager im;
    //BPlusTree<int> * btree = new BPlusTree<int>;
    //im.init_index("teacher", btree, 0);
    //cout << btree -> Search(32498) << endl;
    BufferManager bm;
    File* temp = bm.loadFile("student2");
}

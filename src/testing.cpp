#include"minisql.h"
#include"BufferManager.h"
#include"CatalogManager.h"
#include"interpreter.h"
#include"RecordManager.h"
#include<fstream>

using namespace std;

int main() {
    //BufferManager bm;
    //RecordManager rm;
    //rm.record_insert("teacher", "GIGI!");
    //rm.record_insert("teacher", "test!");
    //rm.record_insert("teacher", "skdfja----s=-d", 28);
    Interpreter ip;
    Catalog cm;
    RecordManager rm;
    ip.mainFunction();
    //rm.record_showall("teacher");
}

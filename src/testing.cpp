#include"minisql.h"
#include"BufferManager.h"
#include"CatalogManager.h"
#include"RecordManager.h"
#include<fstream>

using namespace std;

int main() {
    BufferManager bm;
    RecordManager rm;
    rm.record_insert("teacher", "777shit88", 28);
}

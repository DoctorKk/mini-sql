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
    //rm.record_insert("teacher", "test/sdf/sdfsd/af");
    Interpreter ip;
    //Catalog cm;
    ip.mainFunction();
    //rm.record_showall("teacher");
    //Catalog cm;
    //vector<Attribute> * at = new vector<Attribute>;
    //cm.attributeGet("teacher", at);
    //BufferManager bm;
    //File * temp = bm.loadFile("student2");
    //cout << "stop" << endl;

}

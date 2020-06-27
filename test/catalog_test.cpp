// Minisql.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include"catalog.h"
#include <iostream>

using namespace std;

int main()
{
    Catalog tem;
    string name;
    int res;

    /*
    //test findTable
    name = "student";
    res = tem.findTable(name);
    tem.print(res);     //success

    name = "teacher";
    res = tem.findTable(name);
    tem.print(res);     //fail

    //test dropTable
    name = "student_tem";
    res = tem.dropTable(name);
    tem.print(res);     //success

    name = "student_tem_tem";
    res = tem.dropTable(name);
    tem.print(res);     //success
   
    //test addTable
    
    name = "student";
    Attribute a("name", 1, false);
    Attribute b("age", 0, false,"tag");
    vector<Attribute>* attributeVector = new vector<Attribute>;
    attributeVector->push_back(a);
    attributeVector->push_back(b);
    res = tem.createTable(name, attributeVector,"name");
    tem.print(res);     //fail

    name = "student_new";
    res = tem.createTable(name, attributeVector, "name");
    tem.print(res);     //success
    
  
    // test getRecordNum
    name = "student";
    res = tem.getRecordNum(name);
    if (res < 0)
        tem.print(res);
    else
        cout << name << " file contains " << res << " records" << endl;

    //test update ++
    name = "student";
    res = tem.insertRecordNum(name, 2);
    tem.print(res);
    

    //test update --
    name = "student";
    res = tem.deleteRecordNum(name, 2);
    tem.print(res);

    //test find index
    name = "new";
    res = tem.findIndex(name);
    tem.print(res);

    //test add index
    string indexName = "tag2";
    string tableName = "teacher";
    string attributeName = "num";
    int type = 0;
    res = tem.addIndex(indexName, tableName, attributeName, type);
    tem.print(res);

    
    //test drop index
    name = "tag2";
    res = tem.dropIndex(name);
    tem.print(res);

    //test revoke on attribute
    string tableName = "student";
    string AttributeName = "name";
    string indexName = "tag";

    res = tem.revokeIndexOnAttribute(tableName,AttributeName,indexName);
    tem.print(res);
    
    //test indexNameListGet
    name = "student";
    vector<string>* indexNameVector = new vector<string>;
    res = tem.indexNameListGet(name, indexNameVector);
    tem.print(res);
    for(auto i = (*indexNameVector).begin();i!=(*indexNameVector).end();i++)
        cout << *i << " ";

    //test getIndexType
    name = "tag";
    res = tem.getIndexType(name);
    cout << "type:" << res << endl;

    //test attributeget
    name = "student";
    vector<Attribute>* attributeVector = new vector<Attribute>;
    res = tem.attributeGet(name, attributeVector);
    for (auto i = (*attributeVector).begin(); i != (*attributeVector).end(); i++)
        (*i).print();
    */
    
    //test getlength
    name = "student";
    res = tem.calculateLength(name);
    cout << res;
}


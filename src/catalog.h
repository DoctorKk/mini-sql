#ifndef CATALOG_H
#define CATALOG_H
#include<iostream>
#include<string>
#include<vector>
using namespace std;

//1.数据库中所有表的定义信息，包括表的名称、表中字段（列）数、主键、定义在该表上的索引。
//stored in each file

//2.表中每个字段的定义信息，包括字段类型、是否唯一等。

class Attribute
{
public:
    string name;
    int type, size;              //-1为float，0为int，其余整数对应varchar(type)    
    bool ifUnique;          //是否唯一
    string index;

    Attribute(string n, int t, bool i,string in = "") {
        name = n;
        type = t;
        ifUnique = i;
        index = in;
    }
    string getName() {
        return name;
    }
    int getType() {
        return type;
    }
    bool getUnique() {
        return ifUnique;
    }
    string getIndex() {
        return index;
    }
    void print()
    {
        cout << "name:" << name << ";type: " << type << ";ifUnique: " << ifUnique << endl;
    }
    ~Attribute() {};
};


//3.数据库中所有索引的定义，包括所属表、索引建立在那个字段上等。
class IndexInfo{
public:
    string indexName;
    string tableName;
    string Attribute;
    int type;
    IndexInfo(string i, string t, string a, int ty)
    {
        indexName = i; tableName = t; Attribute = a; type = ty;
    }
    void print() {
        cout << "index:" << indexName << ";table:" << tableName << ";Attribute:" << Attribute << endl;
    }
};

//Catalog Manager还必需提供访问及操作上述信息的接口，供Interpreter和API模块使用。
class Catalog {
public:
    Catalog();
    virtual ~Catalog();
    
    //table
    int findTable(string tableName);
    int dropTable(string tableName);
    int createTable(string tableName, vector<Attribute>* attributeVector, string primaryKeyName);
    
    //record
    int getRecordNum(string tableName);
    int insertRecordNum(string tableName, int recordNum);   // increment the number of record
    int deleteRecordNum(string tableName, int deleteNum);   // delete the number of record

    //attribute
    int attributeGet(string tableName, vector<Attribute>* attributeVector);

    //index
    int findIndex(string indexName);
    int addIndex(string indexName, string tableName, string attributeName, int type);
    int dropIndex(string index);
    int setIndexOnAttribute(string tableName, string AttributeName, string indexName);
    int revokeIndexOnAttribute(string tableName, string AttributeName, string indexName);
    int indexNameListGet(string tableName, vector<string>* indexNameVector);    //all indexes of one table
    int getIndexType(string indexName);
    
    //supplement 
    int calculateLength(string tableName);
    int calculateLength2(int type);
    //void recordStringGet(string tableName, vector<string>* recordContent, char* recordResult); 
    void print(int i);
};

int char2int(char type[], int len);

#endif CATALOG_H
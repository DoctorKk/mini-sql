#ifndef CATALOG_H
#define CATALOG_H
#include<iostream>
#include<string>
#include<vector>
using namespace std;

//1.数据库中所有表的定义信息，包括表的名称、表中字段（列）数、主键、定义在该表上的索引。
class Table {
public:
    string name;
    int column;
    string primary_key;
    string index;   // default value is "", representing no index
    Table(string n,int c,string p) {
        name = n;
        column = c;
        primary_key = p;
        index = "";
    }
    string indexNameGet() {
        return index;
    }
    void print() {
        cout << "name:" << name << ";column:" << column << "primary_key:" << primary_key << ";index:" << index << endl;
    }
};

//2.表中每个字段的定义信息，包括字段类型、是否唯一等。
class Attribute {
public:
    //int static const TYPE_FLOAT = -1;
    //int static const TYPE_INT = 0;
    int type;       //the type of the attribute,-1 represents float, 0 represents int, other positive integer represents char and the value is the number of char)
    bool ifUnique;
    Attribute(int t, bool i) {
        type = t;
        ifUnique = i;
    }
    void print()
    {
        cout << "type: " << type << ";ifUnique: " << ifUnique << endl;
    }
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
    Catalog();
    virtual ~Catalog();
    
    //table
    int findTable(string tableName);
    int dropTable(string tableName);
    int addTable(string tableName, vector<Attribute>* attributeVector, string primaryKeyName, int primaryKeyLocation);
    
    //record
    int getRecordNum(string tableName);
    int insertRecordNum(string tableName, int recordNum);  // increment the number of record
    int deleteRecordNum(string tableName, int deleteNum);// delete the number of record

    //attribute
    int attributeGet(string tableName, vector<Attribute>* attributeVector);

    //index
    int findIndex(string indexName);
    int addIndex(string indexName, string tableName, string attributeName, int type);
    int revokeIndexOnAttribute(string tableName, string AttributeName, string indexName);
    int indexNameListGet(string tableName, vector<string>* indexNameVector);
    int getAllIndex(vector<IndexInfo>* indexs);
    int setIndexOnAttribute(string tableName, string AttributeName, string indexName);
    int getIndexType(string indexName);
    
    //length 
    int calcuteLenth(string tableName);
    int calcuteLenth2(int type);
};

#endif CATALOG_H
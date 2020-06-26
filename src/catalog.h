#ifndef CATALOG_H
#define CATALOG_H
#include<iostream>
#include<string>
#include<vector>
using namespace std;

//1.���ݿ������б�Ķ�����Ϣ������������ơ������ֶΣ��У����������������ڸñ��ϵ�������
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

//2.����ÿ���ֶεĶ�����Ϣ�������ֶ����͡��Ƿ�Ψһ�ȡ�
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

//3.���ݿ������������Ķ��壬���������������������Ǹ��ֶ��ϵȡ�
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

//Catalog Manager�������ṩ���ʼ�����������Ϣ�Ľӿڣ���Interpreter��APIģ��ʹ�á�
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
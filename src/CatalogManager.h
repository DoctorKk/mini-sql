/*
 * @Author: Holly
 * @Date: 2020-06-25
 * @LastEditTime: 2020-06-29
 * @LastEditors: Please set LastEditors
 * @Description: verison 2.0
 */

#ifndef CATALOGMANAGER_H
#define CATALOGMANAGER_H

#include<iostream>
#include<string>
#include<vector>
#include"IndexInfo.h"
#include"Attribute.h"

using namespace std;


//Catalog Manager必需提供访问及操作上述信息的接口，供Interpreter和API模块使用。
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
    string findIndex(string indexName);
    int addIndex(string indexName, string tableName, string attributeName, int type);
    int dropIndex(string index);
    int setIndexOnAttribute(string tableName, string AttributeName, string indexName);
    int revokeIndexOnAttribute(string tableName, string AttributeName, string indexName);
    int indexNameListGet(string tableName, vector<string>* indexNameVector);    //all indexes of one table
    int getIndexType(string indexName);
    string getPrimaryKeyName(string tableName);

    //get length
    int calculateLength_table(string tableName);
    int calculateLength_type(int type);
    int calculateLength_attribute(string tableName);
    int getoffset(string tableName, string attributeName);

    //void recordStringGet(string tableName, vector<string>* recordContent, char* recordResult); 
    void print(int i);
};

int char2int(char type[], int len);

#endif

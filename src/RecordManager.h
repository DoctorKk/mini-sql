/*
 * @Author: 小文
 * @Date: 2020-06-21 15:26:20
 * @LastEditTime: 2020-06-28 12:10:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \新建文件夹\RecordManager.h
 */
#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H
#include"Attribute.h"
#include"Condition.h"
#include"minisql.h"
#include"BufferManager.h"
#include"CatalogManager.h"
#include"IndexManager.h"
#include<string>
#include<vector>

using namespace std;

class RecordManager
{
public:
    RecordManager() {}
    ~RecordManager() {
        buffer.writeAlltoDisk();
        //delete & buffer;
        //delete & catalog;
    }
    BufferManager buffer;
    Catalog catalog;
    IndexManager im;
    //int table_create(string tableName);
    //int table_drop(string tableName);

    int index_drop(string indexName);
    int index_create(string indexName);

    int record_insert(string tableName, char *record);

    //print all record of a table meet requirement
    int record_showall(string tableName, vector<string> *attributeNameVector, vector<Condition> *conditionVector);
    //int record_showall(string tableName);

    //delete all record of a table meet requirement
    int record_deleteall(string tableName, vector<Condition> *conditionVector);
    

    //int indexRecordAllAlreadyInsert(string tableName, string indexName);

private:
    int record_indexshow(string tableName, vector<string> *attributeNameVector, Block* block, int offset);
    int record_blockshow(string tableName, vector<string> *attributeNameVector, vector<Condition> *conditionVector, Block* block);
    //int record_blockshow(string tableName, Block *block);

    int record_blockdelete(string tableName, vector<Condition> *conditionVector, Block* block);
    bool record_conditionfit(string recordBegin, int recordSize, vector<Attribute> *attributeVector, vector<Condition> *conditionVector);
    bool content_conditionfit(string content, int type, Condition *condition);

    void record_print(string recordBegin, int recordSize, vector<Attribute> *attributeVector, vector<string> *attributeNameVector);
    //void record_print(string recordBegin, int recordSize, vector<Attribute> *attributeVector);

    void content_print(char *content, int type);
};

void split(std::string &s, std::string &delim, std::vector<std::string> *ret);
#endif

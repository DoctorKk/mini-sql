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
#include<string>
#include<vector>

using namespace std;

class RecordManager
{
public:
    RecordManager() {}
    BufferManager buffer;
    Catalog catalog;
    //int table_create(string tableName);
    //int table_drop(string tableName);

    int index_drop(string indexName);
    int index_create(string indexName);

    int record_insert(string tableName, char *record);

    //print all record of a table meet requirement
    int record_showall(string tableName, vector<string> *attributeNameVector, vector<Condition> *conditionVector);
    
    //delete all record of a table meet requirement
    int record_deleteall(string tableName, vector<Condition> *conditionVector);
    

    //int indexRecordAllAlreadyInsert(string tableName, string indexName);

private:
    
    int record_blockshow(string tableName, vector<string> *attributeNameVector, vector<Condition> *conditionVector, Block* block);
    int record_blockdelete(string tableName, vector<Condition> *conditionVector, Block* block);
    bool record_conditionfit(char *recordBegin, int recordSize, vector<Attribute> *attributeVector, vector<Condition> *conditionVector);
    bool content_conditionfit(char *content, int type, Condition *condition);
    void record_print(char *recordBegin, int recordSize, vector<Attribute> *attributeVector, vector<string> *attributeNameVector);
    void content_print(char *content, int type);
};

#endif
/*
 * @Author: 小文
 * @Date: 2020-06-30 04:51:51
 * @LastEditTime: 2020-06-30 04:52:25
 * @LastEditors: Please set LastEditors
 * @Description: index
 * @FilePath: \新建文件夹\mini-sql-master\src\IndexManager.cpp
 */ 
#include"IndexManager.h"

void IndexManager::create_index(string indexName, string tableName, int type, int offset)
{
    //stringstream ss;
    //string temp;
    //ss >> temp;
    if (type == -1)
    {
        BPlusTree<float> *tree = new BPlusTree<float>;
        init_index(tableName, tree, offset);
        //==========before inserting check if it already exists
        indexFloatMap.insert(floatMap::value_type(indexName, tree));
    }
    else if (type == 0)
    {
        BPlusTree<int> *tree = new BPlusTree<int>;
        init_index(tableName, tree, offset);
        indexIntMap.insert(intMap::value_type(indexName, tree));
    }
    else
    {
        BPlusTree<string> *tree = new BPlusTree<string>;
        init_index(tableName, tree, offset);
        indexStringMap.insert(stringMap::value_type(indexName, tree));
    }
}

//===========also need change here: since a table can have different indexes
void IndexManager::drop_index(string tableName, int type)
{
    if (type == -1)
    {
        floatMap::iterator itFloat = indexFloatMap.find(tableName);
        if (itFloat == indexFloatMap.end())
        {
            cout << "Error:in drop index, no index " << tableName << " exits" << endl;
            return;
        }
        else
        {
            delete itFloat->second;
            indexFloatMap.erase(itFloat);
        }
    }
    else if (type == 0)
    {
        intMap::iterator itInt = indexIntMap.find(tableName);
        if (itInt == indexIntMap.end())
        {
            cout << "Error:in drop index, no index " << tableName << " exits" << endl;
            return;
        }
        else
        {
            delete itInt->second;
            indexIntMap.erase(itInt);
        }
    }
    else
    {
        stringMap::iterator itString = indexStringMap.find(tableName);
        if (itString == indexStringMap.end())
        {
            cout << "Error:in drop index, no index " << tableName << " exits" << endl;
            return;
        }
        else
        {
            delete itString->second;
            indexStringMap.erase(itString);
        }
    }
}

int IndexManager::search_index(string tableName, float key)
{
    floatMap::iterator itFloat = indexFloatMap.find(tableName);
    if (itFloat == indexFloatMap.end())
    {
        cout << "Error:in search index, no index " << tableName << " exits" << endl;
        return -1;
    }
    else
    {
        return itFloat->second->Search(key);
    }
}

int IndexManager::search_index(string tableName, int key)
{
    intMap::iterator itInt = indexIntMap.find(tableName);
    if (itInt == indexIntMap.end())
    {
        cout << "Error:in search index, no index " << tableName << " exits" << endl;
        return -1;
    }
    else
    {
        return itInt->second->Search(key);
    }
}

int IndexManager::search_index(string tableName, string key)
{
    stringMap::iterator itString = indexStringMap.find(tableName);
    if (itString == indexStringMap.end())
    {
        cout << "Error:in search index, no index " << tableName << " exits" << endl;
        return -1;
    }
    else
    {
        return itString->second->Search(key);
    }
}

void IndexManager::init_index(string tableName, BPlusTree<float> *tree, int offset)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());
    char *indexBegin = b->data;
    int begin = 0;
    float value;
    int count = 0;
    int recordSize = 0;
    
    for(int i = 0;i<=offset;i++){
        while(b->data[begin]!='/'){
            begin++;
        }
    }
    begin++;

    while (1)
    {
        if (b == NULL)
        {
            return;
        }
        while ( begin < b->blockSize)
        {
            value = *(float *)indexBegin;
            tree->Insert(make_pair(count++, value));
            while(b->data[begin]!='\n'){
                begin++;
            }
            begin++;
            for(int i = 0;i<offset;i++){
                while(b->data[begin]!='/' && begin<b->blockSize){
                    begin++;
                }
            }
            begin++;
            indexBegin = b->data + begin;
        }
        b = buffer.getNextBlock(tableName.c_str(), b);
    }
}

void IndexManager::init_index(string tableName, BPlusTree<int> *tree, int offset)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());
    char *indexBegin = b->data;
    string content(indexBegin);
    int end = 0;
    int pre = 0;
    int value = 0;
    string temp;
    while (1) {
        if (b == NULL)
            return;
        else {
            int i = 0;
            for (; i <= offset; i++) {
                temp = content.substr(pre, end);
                value = stoi(temp);
                end = content.find("/", pre);
                pre = end+1;
            }
        }
    }




    /*
    int value;
    int count = 0;
    string temp;

    for(int i = 0;i<=offset;i++){
        while(b->data[end]!='/'){
            end++;
        }
    }
    //end++;


    while (1)
    {
        if (b == NULL)
        {
            return;
        }
        while (end < b->blockSize)
        {
            temp = content.substr(pre, end);
            pre = end;
            value = stoi(temp);
            //value = *(int *)indexBegin;
            tree->Insert(make_pair(count++, value));
            while(b->data[end]!='\n'){
                end++;
            }
            end++;
            for(int i = 0;i<=offset;i++){
                while(b->data[end]!='/' && end<b->blockSize){
                    end++;
                }
            }
            end++;
            //indexBegin = b->data + end;
        }
        b = buffer.getNextBlock(tableName.c_str(), b);
    }
     */
}

void IndexManager::init_index(string tableName, BPlusTree<string> *tree, int offset)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());
    char *indexBegin = b->data;
    int begin = 0;
    string value;
    int count = 0;
    int size = 0;

    for(int i = 0;i<offset;i++){
        while(b->data[begin]!='/'){
            begin++;
        }
    }
    begin++;


    while (1)
    {
        if (b == NULL)
        {
            return;
        }
        while (indexBegin - indexBegin < b->blockSize)
        {   
            size = 0;
            while(indexBegin[size]!='/'){
                size++;
            }
            memcpy(&value, indexBegin, size + 1);
            tree->Insert(make_pair(count++, value));

            while(b->data[begin]!='\n'){
                begin++;
            }
            begin++;
            for(int i = 0;i<offset;i++){
                while(b->data[begin]!='/' && begin<b->blockSize){
                    begin++;
                }
            }
            begin++;
            indexBegin = b->data + begin;
        }
        b = buffer.getNextBlock(tableName.c_str(), b);
    }
}

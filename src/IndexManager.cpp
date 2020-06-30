/*
 * @Author: 小文
 * @Date: 2020-06-30 04:51:51
 * @LastEditTime: 2020-06-30 04:52:25
 * @LastEditors: Please set LastEditors
 * @Description: index
 * @FilePath: \新建文件夹\mini-sql-master\src\IndexManager.cpp
 */ 
#include"IndexManager.h"

void IndexManager::create_index(string tableName, int type)
{
    if (type == -1)
    {
        BPlusTree<float> *tree;
        init_index(tableName, tree);
        indexFloatMap.insert(floatMap::value_type(tableName, tree));
    }
    else if (type == 0)
    {
        BPlusTree<int> *tree;
        init_index(tableName, tree);
        indexIntMap.insert(intMap::value_type(tableName, tree));
    }
    else
    {
        BPlusTree<string> *tree;
        init_index(tableName, tree);
        indexStringMap.insert(stringMap::value_type(tableName, tree));
    }
}

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

int search_index(string tableName, float key)
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

int search_index(string tableName, int key)
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

int search_index(string tableName, string key)
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

void IndexManager::init_index(string tableName, BPlusTree<float> *tree)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());
    char *indexBegin;

    float value;
    int count = 0;
    int recordSize;

    while (1)
    {
        if (b == NULL)
        {
            return;
        }
        while (indexBegin - indexBegin < b->blockSize)
        {
            value = *(float *)indexBegin;
            tree->Insert(pair(count++, value));
            indexBegin += recordSize;
        }
        b = buffer.getNextBlock(tableName.c_str(), b);
    }
}

void IndexManager::init_index(string tableName, BPlusTree<int> *tree)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());
    char *indexBegin;

    int value;
    int count = 0;
    int recordSize;

    while (1)
    {
        if (b == NULL)
        {
            return;
        }
        while (indexBegin - indexBegin < b->blockSize)
        {
            value = *(int *)indexBegin;
            tree->Insert(pair(count++, value));
            indexBegin += recordSize;
        }
        b = buffer.getNextBlock(tableName.c_str(), b);
    }
}

void IndexManager::init_index(string tableName, BPlusTree<string> *tree, int type)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());
    char *indexBegin;

    string value;
    int count = 0;
    int recordSize;

    while (1)
    {
        if (b == NULL)
        {
            return;
        }
        while (indexBegin - indexBegin < b->blockSize)
        {
            memccpy(&value, indexBegin, type + 1);
            tree->Insert(pair(count++, value));
            indexBegin += recordSize;
        }
        b = buffer.getNextBlock(tableName.c_str(), b);
    }
}

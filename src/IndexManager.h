/*
 * @Author: 小文
 * @Date: 2020-06-29 22:44:49
 * @LastEditTime: 2020-06-30 04:52:43
 * @LastEditors: Please set LastEditors
 * @Description: Index manage
 * @FilePath: \新建文件夹\mini-sql-master\src\IndexManager.h
 */

#ifndef __Minisql__IndexManager__
#define __Minisql__IndexManager__

#include <stdio.h>
#include <map>
#include <string>
#include <sstream>

#include "Attribute.h"
#include "BPlusTree.cpp"
#include "BufferManager.h"
#include"CatalogManager.h"

class API;

class IndexManager
{
private:
    BufferManager buffer;
    Catalog cm;

    typedef map<string, BPlusTree<int> *> intMap;
    typedef map<string, BPlusTree<string> *> stringMap;
    typedef map<string, BPlusTree<float> *> floatMap;

    int static const TYPE_FLOAT = Attribute::TYPE_FLOAT;
    int static const TYPE_INT = Attribute::TYPE_INT;

    intMap indexIntMap;
    stringMap indexStringMap;
    floatMap indexFloatMap;

    void init_index(string tableName, BPlusTree<int> *tree, int offset);
    void init_index(string tableName, BPlusTree<float> *tree, int offset);
    void init_index(string tableName, BPlusTree<string> *tree, int offset);

    
    // other values mean the size of the char.Eg, 4 means char(4);
public:
    void create_index(string indexName, string tableName, int type, int offset);

    void drop_index(string tableName, int type);

    int search_index(string tableName, float key);
    int search_index(string tableName, int key);
    int search_index(string tableName, string key);
};

#endif /* defined(__Minisql__IndexManager__) */

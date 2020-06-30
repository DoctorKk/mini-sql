#ifndef INDEXINFO_H
#define INDEXINFO_H

#include<iostream>
#include<string>

using namespace std;

/* 数据库中所有索引的定义，包括所属表、索引建立在那个字段上等 */

class IndexInfo {
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

#endif

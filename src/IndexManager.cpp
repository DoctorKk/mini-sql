/*
 * @Author: 小文
 * @Date: 2020-06-30 04:51:51
 * @LastEditTime: 2020-06-30 04:52:25
 * @LastEditors: Please set LastEditors
 * @Description: index
 * @FilePath: \新建文件夹\mini-sql-master\src\IndexManager.cpp
 */ 
#include"IndexManager.h"

extern void split(std::string &s, std::string &delim, std::vector<std::string> *ret);



bool IndexManager::check_index(string indexName) {
    return (indexFloatMap.find(indexName) != indexFloatMap.end() || indexIntMap.find(indexName) != indexIntMap.end() || indexStringMap.find(indexName) != indexStringMap.end());
}

void IndexManager::create_index(string indexName, string tableName, int type, int offset)
{
    //stringstream ss;
    //string temp;
    //ss >> temp;
    if (check_index(indexName)) {
        cout << "There is already a " << indexName << " !" << endl;
        return;
    }
    if (type == -1)
    {
        BPlusTree<float> *tree = new BPlusTree<float>;
        init_index(tableName, indexName, tree, offset);
        indexFloatMap.insert(floatMap::value_type(indexName, tree));
    }
    else if (type == 0)
    {
        BPlusTree<int> *tree = new BPlusTree<int>;
        init_index(tableName, indexName, tree, offset);
        indexIntMap.insert(intMap::value_type(indexName, tree));
    }
    else
    {
        BPlusTree<string> *tree = new BPlusTree<string>;
        init_index(tableName, indexName, tree, offset);
        indexStringMap.insert(stringMap::value_type(indexName, tree));
    }
    cout << "Index " << indexName << " created successfully!" << endl;
}

//===========also need change here: since a table can have different indexes
void IndexManager::drop_index(string indexName)
{
    bool flag = false;
    {
        floatMap::iterator itFloat = indexFloatMap.find(indexName);
        if (itFloat == indexFloatMap.end())
        {
        }
        else
        {
            delete itFloat->second;
            indexFloatMap.erase(itFloat);
            flag = true;
        }
    }
    {
        intMap::iterator itInt = indexIntMap.find(indexName);
        if (itInt == indexIntMap.end())
        {
            //cout << "Error:in drop index, no index " << indexName << " exits" << endl;
        }
        else
        {
            delete itInt->second;
            indexIntMap.erase(itInt);
            flag = true;
        }
    }
    {
        stringMap::iterator itString = indexStringMap.find(indexName);
        if (itString == indexStringMap.end())
        {
            //cout << "Error:in drop index, no index " << indexName << " exits" << endl;
        }
        else
        {
            delete itString->second;
            indexStringMap.erase(itString);
            flag = true;
        }
    }
    if (!flag) {
        cout << "Error:in drop index, no index " << indexName << " exits" << endl;
        return;
    }
    cout << "Index " << indexName << " dropped successfully!" << endl;
}

int IndexManager::search_index(string tableName, string attr, float key)
{
    if (indexPool.find(tableName+attr) == indexPool.end()) // when not found
        return INT_MIN;
    string indexName = indexPool[tableName+attr];
    if (indexFloatMap.find(indexName)==indexFloatMap.end())
        return INT_MIN;
    return indexFloatMap[indexName] -> Search(key);
    /*
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
     */
}

int IndexManager::search_index(string tableName, string attr, int key)
{
    if (indexPool.find(tableName+attr) == indexPool.end()) // when not found
        return INT_MIN;
    string indexName = indexPool[tableName+attr];
    if (indexIntMap.find(indexName)==indexIntMap.end())
        return INT_MIN;
    return indexIntMap[indexName] -> Search(key);
}

int IndexManager::search_index(string tableName, string attr, string key)
{
    if (indexPool.find(tableName+attr) == indexPool.end()) // when not found
        return INT_MIN;
    string indexName = indexPool[tableName+attr];
    if (indexStringMap.find(indexName)==indexStringMap.end())
        return INT_MIN;
    return indexStringMap[indexName] -> Search(key);
}

void IndexManager::init_index(string tableName, string indexName, BPlusTree<float> *tree, int offset)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());

    string temp1 = "/", temp2 = "\n";
    string begin;
    float value;
    int count = 0;

    vector<Attribute> *atemp = new vector<Attribute>;
    cm.attributeGet(tableName, atemp);
    int attributesum = atemp->size();

    while (1) {
        if (b == nullptr) break;
        begin = b->data;

        vector<string> records(0), at(0);


        split(begin, temp2, &records);

        for (auto item: records) {
            vector<string> temp(0);
            split(item, temp1, &temp);
            if (item.empty())
                break;
            value = stof(temp[offset]);
            tree -> Insert(make_pair(count++, value));

            temp.clear();
        }
        b = b -> nextBlock;

    }
    indexPool.insert(make_pair(tableName+(*atemp)[offset].name, indexName));
}

void IndexManager::init_index(string tableName, string indexName, BPlusTree<int> *tree, int offset) {
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());

    string temp1 = "/", temp2 = "\n";
    string begin;
    int value;
    int count = 0;
    vector<Attribute> *atemp = new vector<Attribute>;
    cm.attributeGet(tableName, atemp);
    int attributesum = atemp->size();


    while (1) {
        if (b == nullptr) break;
        begin = b->data;

        vector<string> records(0), at(0);


        split(begin, temp2, &records);

        for (auto item: records) {
            vector<string> temp(0);
            split(item, temp1, &temp);
            if (item.empty())
                break;
            value = stoi(temp[offset]);
            tree -> Insert(make_pair(count++, value));

            temp.clear();
        }
        b = b -> nextBlock;
    }
    indexPool.insert(make_pair(tableName+(*atemp)[offset].name, indexName));

}


void IndexManager::init_index(string tableName, string indexName, BPlusTree<string> *tree, int offset)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());

    string temp1 = "/", temp2 = "\n";
    string begin;
    int count = 0;
    vector<Attribute> *atemp = new vector<Attribute>;
    cm.attributeGet(tableName, atemp);
    int attributesum = atemp->size();


    while (1) {
        if (b == nullptr) break;
        begin = b->data;

        vector<string> records(0), at(0);

        split(begin, temp2, &records);

        for (auto item: records) {
            vector<string> temp(0);
            split(item, temp1, &temp);
            if (item.empty())
                break;
            tree -> Insert(make_pair(count++, temp[offset]));

            temp.clear();
        }
        b = b -> nextBlock;

    }
    indexPool.insert(make_pair(tableName+(*atemp)[offset].name, indexName));
}

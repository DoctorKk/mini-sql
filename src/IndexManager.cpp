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
        init_index(tableName, tree, offset);
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
    cout << "Index " << indexName << " created successfully!" << endl;
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

    string temp1 = "/", temp2 = "\n";
    string begin;
    float value;
    int count = 0;


    while (1) {
        if (b == NULL) return;
        begin = b->data;

        vector<string> records(0), at(0);

        vector<Attribute> *atemp = new vector<Attribute>;
        cm.attributeGet(tableName, atemp);
        int attributesum = atemp->size();

        split(begin, temp2, &records);

        for (auto item: records) {
            vector<string> temp(0);
            split(item, temp1, &temp);
            value = stof(temp[offset]);
            tree -> Insert(make_pair(count++, value));

            temp.clear();
        }
        b = b -> nextBlock;

    }
}

void IndexManager::init_index(string tableName, BPlusTree<int> *tree, int offset) {
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());

    string temp1 = "/", temp2 = "\n";
    string begin;
    int value;
    int count = 0;


    while (1) {
        if (b == NULL) return;
        begin = b->data;

        vector<string> records(0), at(0);

        vector<Attribute> *atemp = new vector<Attribute>;
        cm.attributeGet(tableName, atemp);
        int attributesum = atemp->size();

        split(begin, temp2, &records);

        for (auto item: records) {
            vector<string> temp(0);
            split(item, temp1, &temp);
            value = stoi(temp[offset]);
            tree -> Insert(make_pair(count++, value));

            temp.clear();
        }
        b = b -> nextBlock;

    }
}
    /*
    int count = 0, num = 0;
    if(offset!=attributesum-1 && offset!=0){
        for(int i = offset;i<records.size();i = i+offset-1) {
            at[count++] = records[i];
        }
    }
    else if(offset==attributesum-1){
        for(int i = offset;i<records.size();i = i+offset-1) {
            num = 0;
            while(records[i][num]!='\n'){
                at[count][num] = records[i][num];
                num++;
            }
            count++;

            //at[count++] = records[i];
        }
    }
    else{
        for(int i = offset;i<records.size();i = i+offset-1) {
            num = 0;
            while(records[i][num]!='\n'){
                //at[count][num] = records[i][num];
                num++;
            }
            for(int j = num+1;j<records[i].length();j++) {
                at[count][j - num - 1] = records[i][j];
            }
            count++;

            //at[count++] = records[i];
        }
    }

    count = 0;

        for(int i = 0;i<at.size();i++) {
            tree->Insert(make_pair(count++, stoi(at[i])));
        }
        b = buffer.getNextBlock(tableName.c_str(), b);
    }

    /*
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
     */


void IndexManager::init_index(string tableName, BPlusTree<string> *tree, int offset)
{
    File *f = buffer.getFile(tableName.c_str());

    Block *b = buffer.getFirstBlock(tableName.c_str());

    string temp1 = "/", temp2 = "\n";
    string begin;
    int count = 0;


    while (1) {
        if (b == NULL) return;
        begin = b->data;

        vector<string> records(0), at(0);

        vector<Attribute> *atemp = new vector<Attribute>;
        cm.attributeGet(tableName, atemp);
        int attributesum = atemp->size();

        split(begin, temp2, &records);

        for (auto item: records) {
            vector<string> temp(0);
            split(item, temp1, &temp);
            tree -> Insert(make_pair(count++, temp[offset]));

            temp.clear();
        }
        b = b -> nextBlock;

    }
}

/*
 * @Author: 小文
 * @Date: 2020-06-21 15:26:33
 * @LastEditTime: 2020-07-04 12:13:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \新建文件夹\RecordManger.cpp
 */ 

#include "RecordManager.h"
#include "CatalogManager.h"

/*
************** Already in catalog **************
int RecordManager::table_create(string tableName)
{
    FILE *fp;
    fp = fopen(tableName.c_str(), "w+");
    if (fp == NULL)
    {
        return 1;
    }
    fclose(fp);
    return 0;
}


int RecordManager::table_drop(string tableName)
{
    if (remove(tableName.c_str()))
    {
        return 1;
    }

    return 0;
}
*/

void split(std::string &s, std::string &delim, std::vector<std::string> *ret)
{
    size_t last = 0;
    size_t index = s.find_first_of(delim, last);
    while (index != string::npos)
    {
        ret->push_back(s.substr(last, index - last));
        last = index + 1;
        index = s.find_first_of(delim, last);
    }
    if (index - last > 0)
    {
        ret->push_back(s.substr(last, index - last));
    }

}

int RecordManager::record_insert(string tableName, char *record)
{
    string stemp(record);
    stemp.append("\n");
    record = (char*) stemp.c_str();
    int recordSize = strlen(record);
    File *temp = buffer.getFile(tableName.c_str());

    if(temp==NULL){
        return 0;
    }

    Block* f = buffer.getLastBlock(tableName.c_str());
    if (f->blockSize + recordSize > BLOCK_SIZE - 1){
        f = buffer.createBlock(tableName.c_str());
    }

    buffer.setDirty(f);
    memcpy(f->data+strlen(f->data), record, recordSize);
    f->blockSize = strlen(f -> data);
    //buffer.writeFiletoDisk(temp);
    return 1;
}




/*
int RecordManager::record_showall(string tableName)
{
    File *temp = buffer.getFile(tableName.c_str());
    Block *f = buffer.getFirstBlock(tableName.c_str());

    cout<<"test"<<endl;

    int Num = 0;

    if(temp == NULL){
        cout<<"The table doesn't exist!"<<endl;
        return -1;
    }

    while(1){
        if (f == buffer.getLastBlock(tableName.c_str())){
            Num += record_blockshow(tableName,f);
            break;
        }
        else{
            Num += record_blockshow(tableName,f);
            f = buffer.getNextBlock(tableName.c_str(), f);
        }
    }
    return Num;
}

int RecordManager::record_blockshow(string tableName, Block *block)
{
    cout<<block->data<<endl;
    if(block==NULL){
        return -1;
    }
    //printf("%s\n",block->data);
    int count = 0;
    int begin = 0;
    vector<Attribute> at;
    string recordbegin = block->data;
    //cout<<"record: "<<recordbegin<<endl;
    catalog.attributeGet(tableName, &at);
    int record_size;

    vector<string> records;
    records.reserve(11);
    string temp = "\n";

    split(recordbegin, temp, &records);

    for(int i = 0;i<records.size();i++){
        //ssscout<<"record: "<<records[i]<<endl;
        record_print(records[i], records[i].length(), &at);
        cout<<endl;
        count++;
    }


    return count;
}

void RecordManager::record_print(string recordBegin, int recordSize, vector<Attribute> *attributeVector)
{
    //cout<<"testing"<<endl;
    int type;
    string attributeName;
    int typeSize = -1;
    int preSize = -1;
    char content[255];

    string begin = recordBegin;
    
    string temp("/");

    vector<string> t(0);

    split(begin, temp, &t);

    string t1 = t[attributeVector->size()-1];
    temp = "\n";
    split(t1,temp,&T);
    

    for (int i = 0; i < attributeVector->size()-1; i++)
    {

        type = (*attributeVector)[i].type;
                if (type == Attribute::TYPE_INT) {
                    //int tmp = *((int *)content);
                    //printf("%d ", tmp);
                    cout << t[i] << " " << flush;
                } else if (type == Attribute::TYPE_FLOAT) {
                    //float tmp = *((float *)content);
                    //printf("%f ", tmp);
                    cout << t[i] << " " << flush;
                } else {
                    //string tmp = content;
                    //printf("%s ", tmp.c_str());
                    cout << t[i] << " " << flush;
                }
                //break;
            //}
        //}
        
    }
    cout<<T[0]<<" "<<flush;
    //cout<<"over"<<endl;
}

*/
int RecordManager::record_showall(string tableName, vector<string> *attributeNameVector, vector<Condition> *conditionVector)
{
    File *temp = buffer.getFile(tableName.c_str());
    Block *f = buffer.getFirstBlock(tableName.c_str());

    int Num = 0;

    if(temp == NULL){
        cout<<"The table doesn't exist!"<<endl;
        return -1;
    }

    while(1){
        if (f == buffer.getLastBlock(tableName.c_str())){

            Num += record_blockshow(tableName, attributeNameVector, conditionVector, f);

            break;
        }
        else{

            Num += record_blockshow(tableName, attributeNameVector, conditionVector, f);
            f = buffer.getNextBlock(tableName.c_str(), f);
        }
    }

    return Num;
}

int RecordManager::record_deleteall(string tableName, vector<Condition> *conditionVector)
{
    File *f = buffer.getFile(tableName.c_str());
    Block *block = buffer.getFirstBlock(tableName.c_str());
    
    if(f==NULL){
        cout << "The table doesn't exist!" << endl;
        return -1;
    }

    int count = 0;
    
    while(1){

        if (block == buffer.getLastBlock(tableName.c_str())){
            count += record_blockdelete(tableName, conditionVector, block);
            return count;
        }
        else{
            count += record_blockdelete(tableName, conditionVector, block);
            block = buffer.getNextBlock(tableName.c_str(), block);
        }
    }
}

int RecordManager::record_blockdelete(string tableName, vector<Condition> *conditionVector, Block* block)
{
    if (block == NULL)
    {
        return -1;
    }
    int count = 0, i = 0;

    char *recordBegin = block->data;
    vector<Attribute> attributeVector;

    catalog.attributeGet(tableName, &attributeVector);
    
    int recordSize = attributeVector.size();
    int begin = 0;
    
    while (begin < block->blockSize)
    {
        //if the recordBegin point to a record

        if (record_conditionfit(recordBegin, recordSize, &attributeVector, conditionVector))
        {
            count++;

            //api->recordIndexDelete(recordBegin, recordSize, &attributeVector, block->offsetNum);
            
            for (i = 0; i + recordSize + begin < block->blockSize; i++)
            {
                recordBegin[i] = recordBegin[i + recordSize];
            }
            memset(recordBegin + i, 0, recordSize);
            block->blockSize = block->blockSize-recordSize;
            buffer.setDirty(block);
        }
        else
        {
            while(block->data[begin]!='\n')
            {
                begin++;
            }
            begin++;
            recordBegin = block->data + begin;
        }
    }

    return count;
}



int RecordManager::record_blockshow(string tableName, vector<string> *attributeNameVector, vector<Condition> *conditionVector, Block *block)
{
    if(block==NULL){
        return -1;
    }
    
    int count = 0;
    int begin = 0;
    vector<Attribute> at;

    catalog.attributeGet(tableName, &at);
    int record_size = 0;

    vector<string> records;
    at.reserve(11);

    string recordbegin = block->data;
    string temp = "\n";

    split(recordbegin, temp, & records);

    for(int i = 0; i < records.size();i++){
        if (record_conditionfit(records[i], record_size, &at, conditionVector)) {
            count++;
            record_print(records[i], record_size, &at, attributeNameVector);
        }
    }

    //delete &records;
    records.clear();
    vector<string>().swap(records);
    return count;

}

void RecordManager::record_print(string recordBegin, int recordSize, vector<Attribute> *attributeVector, vector<string> *attributeNameVector)
{
    string attributeName;
    
    string begin = recordBegin;

    string temp("/");

    vector<string> t;
    t.reserve(attributeVector->size()+1);

    split(begin, temp, &t);

    string t1 = t[attributeVector->size()-1];

    for(int i = 0; i < attributeVector->size(); i++){

        
        for (int j = 0; j < (*attributeNameVector).size(); j++){
            if ((*attributeNameVector)[j] == (*attributeVector)[i].name){

                cout<<t[i]<<" "<<flush;
                break;
            }
        }
    }
    //cout<<T[0]<<flush;
    //delete &t;
    t.clear();
    vector<string>().swap(t);
    cout<<endl;

    return;
    
}

bool RecordManager::record_conditionfit(string recordBegin, int recordSize, vector<Attribute> *attributeVector, vector<Condition> *conditionVector)
{
    if (conditionVector==NULL){
        return true;
    }
    int type, typeSize;
    string attributeName;
    string content;

    string contentBegin = recordBegin;
    string temp("/");
    vector<string> contents;
    contents.reserve(attributeVector->size()+1);
    split(contentBegin, temp, &contents);

    for (int i = 0; i < attributeVector->size(); i++)
    {
        content = contents[i];

        for (int j = 0; j < (*conditionVector).size(); j++)
        {

            if ((*conditionVector)[j].attributeName == (*attributeVector)[i].name)
            {
                type = (*attributeVector)[i].type;
                if (!content_conditionfit(content, type, &(*conditionVector)[j]))
                {

                    return false;
                }
            }
        }
    }
    //delete &contents;
    contents.clear();
    vector<string>().swap(contents);
    return true;
}

void RecordManager::content_print(char * content, int type)
{
    if (type == 0)
    {
        //if the content is a int
        int tmp = *((int *)content); //get content value by point
        printf("%d ", tmp);
    }
    else if (type == -1)
    {
        //if the content is a float
        float tmp = *((float *)content); //get content value by point
        printf("%f ", tmp);
    }
    else
    {
        //if the content is a string
        string tmp = content;
        printf("%s ", tmp.c_str());
    }
}

bool RecordManager::content_conditionfit(string content, int type, Condition *condition)
{
    if (type == 0)
    {
        int tmp = atoi(content.c_str()); //get content value by point
        return condition->ifRight(tmp);
    }
    else if (type == -1)
    {
        //if the content is a float
        float tmp = atof(content.c_str()); //get content value by point
        return condition->ifRight(tmp);
    }
    else
    {
        //if the content is a string
        return condition->ifRight(content);
    }
    return true;
}

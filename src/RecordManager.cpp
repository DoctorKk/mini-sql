/*
 * @Author: 小文
 * @Date: 2020-06-21 15:26:33
 * @LastEditTime: 2020-06-30 19:10:11
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
    //buffer.writeBlocktoDisk(f);
    return 1;
}





int RecordManager::record_showall(string tableName)
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
            Num += record_blockshow(tableName,f);
            break;
        }
        else{
            Num += record_blockshow(tableName,f);
            f = buffer.getNextBlock(tableName.c_str(), f);
        }
    }

}

int RecordManager::record_blockshow(string tableName, Block *block)
{
    if(block==NULL){
        return -1;
    }
    
    int count = 0;
    int begin = 0;
    vector<Attribute> at;

    catalog.attributeGet(tableName, &at);
    int record_size;

    char* recordbegin = block->data;

    while(begin < block->blockSize){
        count++;
        record_size = 0;
        while(recordbegin[record_size]!='\n'){
            record_size++;
        }

        record_print(recordbegin, record_size, &at);
        printf("\n");
        while(block->data[begin]!='\n' && begin < block->blockSize){
            begin++;
        }
        begin++;
        recordbegin = block->data + begin;
    }
    return count;
}

void RecordManager::record_print(char *recordBegin, int recordSize, vector<Attribute> *attributeVector)
{
    int type;
    string attributeName;
    int typeSize = 0;
    char content[255];

    char *begin = recordBegin;
        
    for(int i = 0; i < attributeVector->size(); i++){
        type = (*attributeVector)[i].type;
        while(begin[typeSize]!='/'){
            typeSize++;
        }
        typeSize++;   
        /*
        if(type==-1){
            typeSize = sizeof(float);
        }
        else if(type==0){
            typeSize = sizeof(int);
        }
        else{
            typeSize = type + 1;
        }
        */

        memset(content, 0, 255);

        memcpy(content, begin, typeSize);

        for (int j = 0; j < (*attributeNameVector).size(); j++){
            
            if (type == Attribute::TYPE_INT)
            {
                int tmp = *((int *)content); 
                printf("%d ", tmp);
            }
            else if (type == Attribute::TYPE_FLOAT)
            {
                float tmp = *((float *)content); 
                printf("%f ", tmp);
            }
            else
            {
                string tmp = content;
                printf("%s ", tmp.c_str());
            }
            break;
        }
    }   
}














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
        if(block==NULL){
            return -1;
        }

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


    char* recordbegin = block->data;

    while(begin < block->blockSize){

        if (record_conditionfit(recordbegin, record_size, &at, conditionVector)){
            count++;
            record_size = 0;
            while(recordbegin[record_size]!='\n'){
                record_size++;
            }
            record_print(recordbegin, record_size, &at, attributeNameVector);
            printf("\n");
        }

        while(block->data[begin]!='\n' && begin < block->blockSize){
            begin++;
        }

        begin++;
        recordbegin = block->data + begin;
    }
    return count;

}

void RecordManager::record_print(char *recordBegin, int recordSize, vector<Attribute> *attributeVector, vector<string> *attributeNameVector)
{
    int type;
    string attributeName;
    int typeSize = 0;
    char content[255];

    char *begin = recordBegin;
        
    for(int i = 0; i < attributeVector->size(); i++){
        type = (*attributeVector)[i].type;
        while(begin[typeSize]!='/'){
            typeSize++;
        }
        typeSize++;   
        /*
        if(type==-1){
            typeSize = sizeof(float);
        }
        else if(type==0){
            typeSize = sizeof(int);
        }
        else{
            typeSize = type + 1;
        }
        */

        memset(content, 0, 255);

        memcpy(content, begin, typeSize);

        for (int j = 0; j < (*attributeNameVector).size(); j++){
            if ((*attributeNameVector)[j] == (*attributeVector)[i].name){

                if (type == Attribute::TYPE_INT)
                {
                    int tmp = *((int *)content); 
                    printf("%d ", tmp);
                }
                else if (type == Attribute::TYPE_FLOAT)
                {
                    float tmp = *((float *)content); 
                    printf("%f ", tmp);
                }
                else
                {
                    string tmp = content;
                    printf("%s ", tmp.c_str());
                }

                break;
            }
        }
    }

    
}

bool RecordManager::record_conditionfit(char *recordBegin, int recordSize, vector<Attribute> *attributeVector, vector<Condition> *conditionVector)
{
    if (conditionVector==NULL){
        return true;
    }
    int type, typeSize;
    string attributeName;
    char content[255];

    char *contentBegin = recordBegin;
    for (int i = 0; i < attributeVector->size(); i++)
    {
        type = (*attributeVector)[i].type;
        attributeName = (*attributeVector)[i].name;

        if (type == -1)
        {
            typeSize = sizeof(float);
        }
        else if (type == 0)
        {
            typeSize = sizeof(int);
        }
        else
        {
            typeSize = type + 1;
        }

        //init content (when content is string , we can get a string easily)
        memset(content, 0, 255);
        memcpy(content, contentBegin, typeSize);
        for (int j = 0; j < (*conditionVector).size(); j++)
        {
            if ((*conditionVector)[j].attributeName == attributeName)
            {
                //if this attribute need to deal about the condition
                if (!content_conditionfit(content, type, &(*conditionVector)[j]))
                {
                    //if this record is not fit the conditon
                    return false;
                }
            }
        }

        contentBegin += typeSize;
    }
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

bool RecordManager::content_conditionfit(char *content, int type, Condition *condition)
{
    if (type == 0)
    {
        //if the content is a int
        int tmp = *((int *)content); //get content value by point
        return condition->ifRight(tmp);
    }
    else if (type == -1)
    {
        //if the content is a float
        float tmp = *((float *)content); //get content value by point
        return condition->ifRight(tmp);
    }
    else
    {
        //if the content is a string
        return condition->ifRight(content);
    }
    return true;
}

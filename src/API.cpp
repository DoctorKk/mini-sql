#include"API.h"

/*** table operation ***/
/* 1.drop table */
/* @param:table name */
void API::tableDrop(string tableName) {
    if (!tableExist(tableName)) return;

    vector<string> indexNameVector;

    //get all index in the table, and drop them all
    indexNameListGet(tableName, &indexNameVector);

    for (int i = 0; i < indexNameVector.size(); i++)
    {
        indexDrop(indexNameVector[i]);
    }

    //delete a table file
    if (cm->dropTable(tableName) == 2)
    {
        cout << "Drop table " << tableName << " successfully." << endl;
    }
}

/* 2.create table */
/* @param: table name,attributes,primary key & location */
void API::tableCreate(string tableName, vector<Attribute>* attributeVector, string primaryKeyName, int primaryKeyLocation) {
    if (cm->findTable(tableName) == 1)
    {
        cout << "There is a table " << tableName << " already" << endl;
        return;
    }

    //CatalogManager to create a table information
    if (cm->createTable(tableName, attributeVector, primaryKeyName) == 3)
    {
       cout << "Create table " << tableName << " successfully" << endl;
    }

    if (primaryKeyName != "")
    {
        //get a primary key
        //string indexName = primaryIndexNameGet(tableName);
        //indexCreate(indexName, tableName, primaryKeyName);
    }
}


/* drop index */
/* @param: indexName */
void API::indexDrop(string indexName) {
    rm -> im.drop_index(indexName);
    return;

}


/* create index */
void API::indexCreate(string indexName, string tableName, string attributeName)
{
    vector<Attribute>* at = new vector<Attribute>;
    cm->attributeGet(tableName, at);
    int size = (*at).size();
    int i = 0;
    for (; i < size; i++) {
        if (attributeName==(*at)[i].name)
            break;
    }
    rm->im.create_index(indexName, tableName, (*at)[i].type, i);

    return;

}

/* check if table exist,input: table name */
/* if exist, return 1 */
/* else return 0 */
int API::tableExist(string tableName)
{
    if (cm->findTable(tableName) != 1)
    {
        cout << "There is no table " << tableName << endl;
        return 0;
    }
    else
    {
        return 1;
    }
}

/* get all indexes of the table,input:table name,index vector */
/* store index names in vector and return 10 */
int API::indexNameListGet(string tableName, vector<string>* indexNameVector) {
    if (!tableExist(tableName)) return 0;

    return cm->indexNameListGet(tableName, indexNameVector);
}

string API::primaryIndexNameGet(string tableName) {
    cm->getPrimaryKeyName(tableName);
}

/*
void API::recordShow(string tableName, vector<string>* attributeNameVector){
    if(rm->record_showall(tableName)==0){
        cout<<"Empty table!"<<endl;
    }
    return;
}
*/

void API::recordShow(string tableName, vector<string>* attributeNameVector, vector<Condition>* conditionVector){
    vector<Attribute> temp;
    vector<string> at;
    at.reserve(10);
    cm->attributeGet(tableName,&temp);
    for(int i = 0;i<temp.size();i++){
        at.push_back(temp[i].getName());
        cout<<at[i]<<" ";
    }
    cout<<endl;

    if(attributeNameVector==NULL){
        if(rm->record_showall(tableName, &at, conditionVector)==0){
            cout<<"No records meet the needs."<<endl;
        }
        //cout<<"done"<<endl;
        return;
    }
    else if(rm->record_showall(tableName, attributeNameVector, conditionVector)==0){
        cout<<"No records meet the needs."<<endl;
    }
    temp.clear();
    vector<Attribute>().swap(temp);
    at.clear();
    vector<string>().swap(at);
    return;
}

void API::recordInsert(string tableName, vector<string> recordContent){
    string content;
    int size = recordContent.size();
    for (int i = 0; i < size-1; i++) {
        content += recordContent[i];
        //content.append("/");
    }
    //content.insert(0, "\n");
    char* temp = new char[content.size()];
    temp = (char*) content.c_str();

    if(rm->record_insert(tableName, temp)==0){
        cout<<"Table doesn't exist"<<endl;
    }
    return;
}

void API::recordDelete(string tableName, vector<Condition>* conditionVector){
    int num = rm->record_deleteall(tableName, conditionVector);
    if(num>=0) cout<<"Delete "<<num<<" records."<<endl;
    return;
}

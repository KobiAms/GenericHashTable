#include "GenericHashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table *createTable(int size, int dType, int listLength)
{
    if(size <= 0 || listLength <= 0 || !(dType == STR_TYPE || dType == INT_TYPE)){
        fprintf( stderr, "ERR: input to create Table is incorrect\n");
        return NULL;
    }
    Table *table = (Table *)malloc(sizeof(Table));
    if(table == NULL)
        return NULL;
    table->arr = (Object **)calloc(size, sizeof(Object *));
    if(table->arr == NULL)
        return NULL;
    table->dataType = dType;
    table->numOfMultiply = 0;
    table->originalSize = size;
    table->currentSize = size;
    table->listLength = listLength;
    return table;
}

void freeTable(Table *table)
{
    if(!table)
        return;
    for (int i = 0; i < table->currentSize; i++)
        freeRoot(table->arr[i]);
    free(table->arr);
    free(table);
}

void freeRoot(Object *obj)
{
    if (!obj)
        return;
    if (obj->next)
        freeRoot(obj->next);
    free(obj->data);
    free(obj);
}

int physicalIndex(Table *table, void *data)
{

    int index;
    if (table->dataType)
        index = strHashFun(data, table->originalSize);
    else
        index = intHashFun(data, table->originalSize);
    index *= table->currentSize / table->originalSize;
    return index;
}

int add(Table *table, void *data)
{
    if(!table || !data)
        return -1;

    int index = physicalIndex(table, data);
    void *dynamicCpyOfData;
    if (table->dataType)
    {
        int len = strlen(data);
        dynamicCpyOfData = calloc(len+1, sizeof(char));
        if(dynamicCpyOfData == NULL)
            return -1;
        strncpy(dynamicCpyOfData, data, len);
    }
    else
    {
        dynamicCpyOfData = calloc(1, sizeof(int));
        if(dynamicCpyOfData == NULL)
            return -1;
        *((int *)dynamicCpyOfData) = *((int *)data);
    }
    Object *obj = createObject(dynamicCpyOfData);
    int sectionSize = table->currentSize / table->originalSize;
    if (!insertToSection(&(table->arr[index]), obj, sectionSize, table->listLength))
    {
        multiplyTable(table);
        insertToSection(&(table->arr[index * 2]), obj, sectionSize * 2, table->listLength);
    }
    return 0;
}

int insertToSection(Object **enterance, Object *obj, int sectionSize, int depth)
{
    int inserted = 0;
    for (int i = 0; i < sectionSize && !inserted; i++, enterance++)
    {
        if (*enterance)
            inserted = insertToRoot(*enterance, obj, depth);
        else
        {
            *enterance = obj;
            inserted = 1;
        }
    }
    return inserted;
}

int insertToRoot(Object *root, Object *obj, int depth)
{
    if (depth - 1 == 0)
        return 0;
    if (root->next)
        return insertToRoot(root->next, obj, depth - 1);
    root->next = obj;
    return 1;
}

void multiplyTable(Table *table)
{
    table->arr = (Object **)realloc(table->arr, sizeof(Object *) * table->currentSize * 2);
    if(table->arr == NULL)
            return;
    table->currentSize *= 2;
    table->numOfMultiply++;
    for(int i = table->currentSize/2; i < table->currentSize; i++)
        table->arr[i] = NULL;
    for(int i = (table->currentSize/2) - 1; i >= 0; i--){
        table->arr[i * 2] = table->arr[i];
        if(i != 0)
            table->arr[i] = NULL;
    }


}

int removeFromRoot(Object **root, void *data, int type)
{

    if (!(*root))
        return 0;
    if (isEqual(type, data, (*root)->data) == 0)
    {
        void *temp = (*root)->next;
        freeObject((*root), type);
        (*root) = temp;
        return 1;
    }
    return removeFromRoot(&(*root)->next, data, type);
}

int removeObj(Table *table, void *data)
{
    if(!table || !data)
        return -1;
    int index = physicalIndex(table, data);
    int deleted = 0, deletedIdx = 0, section = table->currentSize / table->originalSize;
    for (int i = 0; i < section && !deleted; i++){
        deleted = removeFromRoot(&table->arr[index + i], data, table->dataType);
        deletedIdx = i;
    }
    return deletedIdx;
}

Object *search(Table *table, void *data)
{
    if(!table || !data)
        return 0;
    int index = physicalIndex(table, data);
    Object *obj = NULL;
    int section = table->currentSize / table->originalSize;
    for (int i = 0; i < section && !obj; i++)
        obj = searchInRoot(&table->arr[index + i], data, table->dataType);
    return obj;
}

Object *searchInRoot(Object **root, void *data, int type)
{
    if (!(*root))
        return NULL;
    if (isEqual(type, data, (*root)->data) == 0)
        return (*root);
    return searchInRoot(&(*root)->next, data, type);
}

int isEqual(int type, void *data1, void *data2)
{
    if(!data1 || !data2)
        return 0;
    if(type){
        int lenData1 = strlen(data1);
        if(lenData1 != strlen(data2))
            return 1;
        return strncmp(data1, data2, lenData1);
    }
    return (*((int *)data1) - *((int *)data2));
}

void printTable(Table *table)
{
    if(!table)
        return;
    for (int i = 0; i < (table->currentSize); i++)
    {
        printf("[%d]", i);
        if (table->arr[i])
            printRoot(table->arr[i], table->dataType);
        printf("\n");
    }
}

void printRoot(Object *obj, int dataType)
{
    if (dataType)
        printf("\t%s\t-->", (char *)obj->data);
    else
        printf("\t%d\t-->", *((int *)obj->data));
    if (!obj->next)
        return;
    printRoot(obj->next, dataType);
}

Object *createObject(void *data)
{
    if(!data)
        return NULL;
    Object *obj = (Object *)calloc(1, sizeof(Object));
    if(obj == NULL)
        return NULL;
    obj->data = data;
    obj->next = NULL;
    return obj;
}

void freeObject(Object *obj, int type)
{
    if(!obj)
        return;
    free(obj->data);
    free(obj);
}

int intHashFun(int *key, int origSize)
{
    if(!key)
        return -1;
    if(*key < 0)
        return (int)(*key) % origSize +origSize;
    return (int)(*key) % origSize;
}

int strHashFun(char *key, int origSize)
{
    if(!key)
        return -1;
    int len = strlen(key);
    int sum = 0;
    for (int i = 0; i < len; i++)
        sum += key[i];
    return (int)sum % origSize;
}
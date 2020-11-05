#include "GenericHashTable.c"
int main()
{

    
    Table *t = createTable(4, 1, 2);
    printf("\n\t\t\t - TEST: add Function\n\n");
    add(t, "4444");
    add(t, "44444");
    add(t, "4445");
    add(t, "44445");
    add(t, "4446");
    add(t, "44446");
    add(t, "4447");
    add(t, "44447");
    printTable(t);
    printf("\n\t\t\t - TEST: multiTable Function\n\n");
    add(t, "444444");
    add(t, "444447");
    printTable(t);
    printf("\n\t\t\t - TEST: removeObj Function\n\n");
    removeObj(t, "444444");
    removeObj(t, "444444");
    removeObj(t, "444447");
    printTable(t);
    printf("\n\t\t\t - TEST: search Function\n\n");
    if(search(t, "44444"))
        printf("*-*-* 44444 found\n");
    if(!search(t, "444444"))
        printf("*-*-* 444444 NOT found\n");
    printf("\n\t\t\t - TEST: createObject Function\n\n");
    char *str = (char*)calloc(6, sizeof(char));
    strncpy(str, "Hello", 5);
    Object *obj = createObject(str);
    printf("\n\t\t\t - TEST: freeObject Function\n\n");
    freeObject(obj, 1);
    printf("\n\t\t\t - TEST: isEqual Function\n\n");
    int x = 5, y = 5, z = 6;
    printf("*-*-* 0 == %d\n", isEqual(1, "equal", "equal"));
    printf("*-*-* 1 == %d\n", isEqual(1, "equal", "not equal"));
    printf("*-*-* 0 == %d\n", isEqual(0, &x, &y));
    printf("*-*-* -1 == %d\n", isEqual(0, &y, &z));
    printf("\n\t\t\t - TEST: HashFunc\n\n");
    printf("*-*-*  0 = %d\n", strHashFun("4", 4));
    printf("*-*-*  1 = %d\n", strHashFun("5", 4));
    printf("*-*-*  2 = %d\n", strHashFun("6", 4));
    printf("*-*-*  3 = %d\n", strHashFun("7", 4));

    freeTable(t);
}
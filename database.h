// database.h
#ifndef DATABASE_H
#define DATABASE_H
#define NAME_LENGTH 50
#define MAX_TABLES 10
#include "database.h"

#define MAX_RECORDS 100
#define MAX_FIELDS 10
#define NAME_LENGTH 50
#define MAX_TABLES 10


typedef enum {
    TYPE_INT,
    TYPE_STRING,
    TYPE_FLOAT,
    TYPE_UNKNOWN
} FieldType;

typedef struct {
    char name[NAME_LENGTH];
    FieldType type;
} Field;

typedef struct {
    int intValue;
    char stringValue[NAME_LENGTH];
    float floatValue;
} FieldValue;

typedef struct {
    FieldValue values[MAX_FIELDS];
} Record;

typedef struct {
    char name[NAME_LENGTH];
    Field fields[MAX_FIELDS];
    int field_count;
    Record records[MAX_RECORDS];
    int record_count;
} Table;
typedef struct {
    char name[NAME_LENGTH];
    Table *tables[MAX_TABLES];
    int table_count;
} Database;



int add_table_to_database(Database *db, const Table *table);
void list_tables_in_database(const Database *db);

#endif // DATABASE_H

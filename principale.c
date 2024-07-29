#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define MAX_FIELDS 10
#define NAME_LENGTH 50

typedef enum {
    TYPE_INT,
    TYPE_STRING,
    TYPE_FLOAT
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




void insert_into_table(Table *table, const char *command) {
    if (table->record_count >= MAX_RECORDS) {
        printf("Table %s is full.\n", table->name);
        return;
    }

    Record *record = &table->records[table->record_count];
    const char *value_str = strstr(command, "AVEC") + 5;
    char field_name[NAME_LENGTH];
    char value[NAME_LENGTH];

    for (int i = 0; i < table->field_count; i++) {
        sscanf(value_str, "%s %s", field_name, value);
        value_str = strstr(value_str, value) + strlen(value) + 1;

        if (strcmp(field_name, table->fields[i].name) == 0) {
            switch (table->fields[i].type) {
                case TYPE_INT:
                    record->values[i].intValue = atoi(value);
                    break;
                case TYPE_STRING:
                    strncpy(record->values[i].stringValue, value, NAME_LENGTH);
                    break;
                case TYPE_FLOAT:
                    record->values[i].floatValue = atof(value);
                    break;
            }
        }
    }

    table->record_count++;
}




void print_table_records(const Table *table) {
    printf("Table: %s\n", table->name);
    printf("Records:\n");
    for (int i = 0; i < table->record_count; i++) {
        for (int j = 0; j < table->field_count; j++) {
            switch (table->fields[j].type) {
                case TYPE_INT:
                    printf("%s: %d ", table->fields[j].name, table->records[i].values[j].intValue);
                    break;
                case TYPE_STRING:
                    printf("%s: %s ", table->fields[j].name, table->records[i].values[j].stringValue);
                    break;
                case TYPE_FLOAT:
                    printf("%s: %.2f ", table->fields[j].name, table->records[i].values[j].floatValue);
                    break;
            }
        }
        printf("\n");
    }
}


void create_table_from_command(const char *command, Table *table) {
    const char *type_str;
    char field_name[NAME_LENGTH];

    table->field_count = 0;
    table->record_count = 0;

    sscanf(command, "CREER TABLE %s AVEC", table->name);

    type_str = strstr(command, "AVEC") + 5;
    while (sscanf(type_str, "%s %s", field_name, table->fields[table->field_count].name) == 2) {
        if (strcmp(field_name, "ENTIER") == 0) {
            table->fields[table->field_count].type = TYPE_INT;
        } else if (strcmp(field_name, "CHAINE") == 0) {
            table->fields[table->field_count].type = TYPE_STRING;
        } else if (strcmp(field_name, "FLOTTANT") == 0) {
            table->fields[table->field_count].type = TYPE_FLOAT;
        }
        table->field_count++;
        type_str = strstr(type_str, table->fields[table->field_count - 1].name) + strlen(table->fields[table->field_count - 1].name) + 1;
    }
}
void print_table_structure(const Table *table) {
    printf("Table: %s\n", table->name);
    printf("Fields:\n");
    for (int i = 0; i < table->field_count; i++) {
        printf("  Name: %s, Type: ", table->fields[i].name);
        switch (table->fields[i].type) {
            case TYPE_INT:
                printf("INTEGER\n");
                break;
            case TYPE_STRING:
                printf("STRING\n");
                break;
            case TYPE_FLOAT:
                printf("FLOAT\n");
                break;
            default:
                printf("UNKNOWN\n");
                break;
        }
    }
}



int main() {
    Table voiture;
    const char *create_command = "CREER TABLE VOITURE AVEC ENTIER AGE CHAINE NOM FLOTTANT SOLDE ENTIER PRIX";
    const char *insert_command = "INSERER DANS VOITURE AVEC AGE 10 NOM LAMBORGHINI SOLDE 10.5 PRIX 10 SUR TROIS";

    create_table_from_command(create_command, &voiture);
   

    insert_into_table(&voiture, insert_command);
    print_table_records(&voiture);
    print_table_structure(&voiture);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include <stdbool.h>





int create_database(Database *db, const char *name) {
    if (strlen(name) >= NAME_LENGTH) {
        printf("Error: Database name is too long.\n");
        return -1;
    }
    strncpy(db->name, name, NAME_LENGTH);
    db->table_count = 0;
    printf("Database '%s' created successfully.\n", db->name);
    return 0;
}

void list_tables_in_database(const Database *db) {
    printf("Tables in Database '%s':\n", db->name);
    for (int i = 0; i <= db->table_count; i++) {
        printf("  - %s\n", db->tables[i]->name);
    }
}


int add_table_to_database(Database *db, const Table *table) {
    if (db->table_count >= MAX_TABLES) {
        fprintf(stderr, "Error: Database is full, cannot add more tables.\n");
        return -1;
    }
    db->tables[db->table_count++] = (Table *)table;
    return 0;
}


int execute_create_database_command(const char *command, Database *db) {
    char db_name[NAME_LENGTH];

    if (sscanf(command, "CREER BASE DE DONNEE %s", db_name) != 1) {
        printf("Error: Invalid command format.\n");
        return -1;
    }

    return create_database(db, db_name);
}




//inserer dans la table
int insert_into_table(Table *table, const char *command) {
    if (table->record_count >= MAX_RECORDS) {
        printf("Error: Table %s is full.\n", table->name);
        return -1;
    }

    Record *record = &table->records[table->record_count];
    const char *value_str = strstr(command, "AVEC") + 5;
    char field_name[NAME_LENGTH];
    char value[NAME_LENGTH];
    int found_fields = 0;

    for (int i = 0; i < table->field_count; i++) {
        if (sscanf(value_str, "%s %s", field_name, value) != 2) {
            printf("Error: Invalid insertion format for field '%s'.\n", field_name);
            return -1;
        }
        value_str = strstr(value_str, value) + strlen(value) + 1;

        if (strcmp(field_name, table->fields[i].name) == 0) {
            found_fields++;
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
                default:
                    printf("Error: Unknown field type.\n");
                    return -1;
            }
        }
    }

    if (found_fields != table->field_count) {
        printf("Error: Missing or extra fields in insertion command.\n");
        return -1;
    }

    table->record_count++;
    return 0;
}


void print_table_with_records(const Table *table) {
    // Affichage des noms des champs
    printf("---------------------------------------------------------\n");
    printf("| ");
    for (int i = 0; i < table->field_count; i++) {
        printf("%-15s | ", table->fields[i].name);
    }
    printf("\n---------------------------------------------------------\n");

    // Affichage des enregistrements
    for (int i = 0; i < table->record_count; i++) {
        printf("| ");
        for (int j = 0; j < table->field_count; j++) {
            switch (table->fields[j].type) {
                case TYPE_INT:
                    printf("%-15d | ", table->records[i].values[j].intValue);
                    break;
                case TYPE_STRING:
                    printf("%-15s | ", table->records[i].values[j].stringValue);
                    break;
                case TYPE_FLOAT:
                    printf("%-15.2f | ", table->records[i].values[j].floatValue);
                    break;
                default:
                    printf("%-15s | ", "UNKNOWN");
                    break;
            }
        }
        printf("\n");
    }
    printf("---------------------------------------------------------\n");
}



//Affichage du contenue de la table 

void print_table_records(const Table *table) {
    printf("Affichage de la table\n");
    printf("---------------------------------------------------------\n");
    printf("Table: %s - Records\n", table->name);
    printf("---------------------------------------------------------\n");

    if (table->record_count == 0) {
        printf("No records available.\n");
        return;
    }

    for (int i = 0; i < table->record_count; i++) {
        printf("| Record %d\n", i + 1);
        for (int j = 0; j < table->field_count; j++) {
            printf("| %-20s : ", table->fields[j].name);
            switch (table->fields[j].type) {
                case TYPE_INT:
                    printf("%d\n", table->records[i].values[j].intValue);
                    break;
                case TYPE_STRING:
                    printf("%s\n", table->records[i].values[j].stringValue);
                    break;
                case TYPE_FLOAT:
                    printf("%.2f\n", table->records[i].values[j].floatValue);
                    break;
                default:
                    printf("UNKNOWN\n");
                    break;
            }
        }
        printf("---------------------------------------------------------\n");
    }
}



//fonction pour la creation de la table de la base de donnees
int create_table_from_command(const char *command, Table *table) {
    const char *type_str;
    char field_name[NAME_LENGTH];

    table->field_count = 0;
    table->record_count = 0;

    if (sscanf(command, "CREER TABLE %s AVEC", table->name) != 1) {
        printf("Error: Invalid command format.\n");
        return -1; // Erreur de syntaxe
    }

    type_str = strstr(command, "AVEC") + 5;
    while (sscanf(type_str, "%s %s", field_name, table->fields[table->field_count].name) == 2) {
        if (strcmp(field_name, "ENTIER") == 0) {
            table->fields[table->field_count].type = TYPE_INT;
        } else if (strcmp(field_name, "CHAINE") == 0) {
            table->fields[table->field_count].type = TYPE_STRING;
        } else if (strcmp(field_name, "FLOTTANT") == 0) {
            table->fields[table->field_count].type = TYPE_FLOAT;
        } else {
            printf("Error: Unknown field type '%s'.\n", field_name);
            return -1; // Type inconnu
        }
        table->field_count++;
        type_str = strstr(type_str, table->fields[table->field_count - 1].name) + strlen(table->fields[table->field_count - 1].name) + 1;
    }
    return 0; // Succès
}


//structure de la table 
void print_table_structure(const Table *table) {
    printf("Table: %s\n", table->name);
    printf("Champs:\n");
    //printf("----------------------------------------\n");
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

//commande Afficher
void execute_select_command(Table *table, const char *command) {
    char table_name[NAME_LENGTH];
    char field_name[NAME_LENGTH];
    char value[NAME_LENGTH];

    if (sscanf(command, "AFFICHER TOUS DANS %s SI %s = %s", table_name, field_name, value) != 3) {
        printf("Error: Invalid command format.\n");
        return;
    }

    if (strcmp(table->name, table_name) != 0) {
        printf("Error: Table %s not found.\n", table_name);
        return;
    }

    // Removing potential quotes around value
    if (value[0] == '\'' || value[0] == '"') {
        memmove(value, value + 1, strlen(value));
    }
    if (value[strlen(value) - 1] == '\'' || value[strlen(value) - 1] == '"') {
        value[strlen(value) - 1] = '\0';
    }

    select_and_print_records(table, field_name, value);
}



void select_and_print_records(const Table *table, const char *field_name, const char *value) {
    printf("\n");
    printf("\n---------------------------------------------------------\n");
    printf("| ");
    for (int i = 0; i < table->field_count; i++) {
        printf("%-15s | ", table->fields[i].name);
    }
    printf("\n---------------------------------------------------------\n");

    for (int i = 0; i < table->record_count; i++) {
        int match = 0;
        for (int j = 0; j < table->field_count; j++) {
            if (strcmp(table->fields[j].name, field_name) == 0) {
                switch (table->fields[j].type) {
                    case TYPE_INT:
                        if (atoi(value) == table->records[i].values[j].intValue) {
                            match = 1;
                        }
                        break;
                    case TYPE_STRING:
                        if (strcmp(value, table->records[i].values[j].stringValue) == 0) {
                            match = 1;
                        }
                        break;
                    case TYPE_FLOAT:
                        if (atof(value) == table->records[i].values[j].floatValue) {
                            match = 1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        if (match) {
            printf("| ");
            for (int j = 0; j < table->field_count; j++) {
                switch (table->fields[j].type) {
                    case TYPE_INT:
                        printf("%-15d | ", table->records[i].values[j].intValue);
                        break;
                    case TYPE_STRING:
                        printf("%-15s | ", table->records[i].values[j].stringValue);
                        break;
                    case TYPE_FLOAT:
                        printf("%-15.2f | ", table->records[i].values[j].floatValue);
                        break;
                    default:
                        printf("%-15s | ", "UNKNOWN");
                        break;
                }
            }
            printf("\n---------------------------------------------------------\n");
        }
    }
}


// Fonction pour supprimer les enregistrements correspondant à une condition
int delete_from_table(Table *table, const char *condition) {
    bool found = false;
    for (int i = 0; i < table->record_count; i++) {
        // Extraire le nom du record pour vérifier la condition
        if (strstr(table->records[i].values, condition) != NULL) {
            // Décaler tous les enregistrements suivants pour écraser le record supprimé
            for (int j = i; j < table->record_count - 1; j++) {
                table->records[j] = table->records[j + 1];
            }
            table->record_count--;
            found = true;
            i--; // Revoir l'index pour le prochain élément à vérifier
        }
    }
    return found ? 0 : -1;
}

int execute_delete_command(Table *table, const char *command) {
    char condition[NAME_LENGTH];
    if (sscanf(command, "SUPPRIMER DANS %49s SI NOM = '%49[^']'", table->name, condition) != 2) {
        fprintf(stderr, "Error: Invalid delete command format.\n");
        return -1;
    }

    return delete_from_table(table, condition);
}



int main() {
    Database db;
    const char *create_db_command    = "CREER BASE DE DONNEE USERS";
     const char *create_db_command1    = "CREER BASE DE DONNEE ETUDIANTS";
    const char *create_table_command = "CREER TABLE USER AVEC ENTIER AGE CHAINE NOM CHAINE MATRICULE";
    const char *insert_command1      = "INSERER DANS USER AVEC AGE 10 NOM LMR MATRICULE 21MB34";
    const char *insert_command2      = "INSERER DANS USER AVEC AGE 5 NOM CONSO MATRICULE 21MM38";
    const char *insert_command3      = "INSERER DANS USER AVEC AGE 10 NOM LMR3 MATRICULE 22MM38";
    const char *select_command       = "AFFICHER TOUS DANS USER SI NOM = 'LMR'";
    const char *delete_command       = "SUPPRIMER DANS user SI NOM = 'LMR'";

    // Créer la base de données
    if (execute_create_database_command(create_db_command, &db) == 0) {
        execute_create_database_command(create_db_command1, &db);
        Table voiture;

        // Créer la table voiture
        if (create_table_from_command(create_table_command, &voiture) == 0) {
            // Ajouter la table à la base de données
            add_table_to_database(&db, &voiture);

            // Insérer des enregistrements dans la table voiture
            insert_into_table(&voiture, insert_command1);
            insert_into_table(&voiture, insert_command2);
            insert_into_table(&voiture, insert_command3);

            // Lister les tables dans la base de données
            list_tables_in_database(&db);

            // Exécuter la commande de sélection
            print_table_with_records(&voiture);
            print_table_structure(&voiture);
           // print_table_records(&voiture);
            execute_select_command(&voiture, select_command);
             if (execute_delete_command(&voiture, delete_command) == 0) {
                printf("Records deleted successfully.\n");
            } else {
                printf("No matching records found to delete.\n");
            }
        }
    }

    return 0;
}




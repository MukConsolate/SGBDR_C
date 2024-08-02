#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include <stdbool.h>

#define MAX_COMMAND_LENGTH 256

// Function prototypes
void load_database_state(Database *db);
int table_exists(const Database *db, const char *table_name);
void append_table_to_state_file(const Table *table);
void update_table_in_state_file(const Table *table);

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
    for (int i = 0; i < db->table_count; i++) {
        printf("  - %s\n", db->tables[i]->name);
    }
}

int add_table_to_database(Database *db, const Table *table) {
    if (db->table_count >= MAX_TABLES) {
        fprintf(stderr, "Error: Database is full, cannot add more tables.\n");
        return -1;
    }
  
    db->tables[db->table_count] = (Table *)malloc(sizeof(Table));
    if (db->tables[db->table_count] == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for table.\n");
        return -1;
    }
    memcpy(db->tables[db->table_count], table, sizeof(Table));
    db->table_count++;
    append_table_to_state_file(table);  // Save the new table to file
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

int table_exists(const Database *db, const char *table_name) {
    for (int i = 0; i < db->table_count; i++) {
        if (strcmp(db->tables[i]->name, table_name) == 0) {
            return 1; // Table exists
        }
    }
    return 0; // Table does not exist
}

int create_table_from_command(const char *command, Table *table) {
    const char *type_str;
    char field_name[NAME_LENGTH];

    table->field_count = 0;
    table->record_count = 0;
    Database *db = (Database *)malloc(sizeof(Database));
    if (db == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for database.\n");
        return -1;
    }
    if (db->table_count >= MAX_TABLES) {
        fprintf(stderr, "Erreur : la base de données est pleine, impossible d'ajouter d'autres tables.\n");
        return -1;
    }

    if (sscanf(command, "CREER TABLE %s AVEC", table->name) != 1) {
        printf("Error: Invalid command format.\n");
        return -1; // Syntax error
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
            return -1; // Unknown type
        }
        table->field_count++;
        type_str = strstr(type_str, table->fields[table->field_count - 1].name) + strlen(table->fields[table->field_count - 1].name) + 1;
    }

    if (table_exists(db, table->name)) {
        printf("Error: Table '%s' already exists.\n", table->name);
        return -1; // Table already exists
    }

    add_table_to_database(db, table);
    return 0; // Success
}

void print_table_with_records(const Table *table) {
    printf("---------------------------------------------------------\n");
    printf("| ");
    for (int i = 0; i < table->field_count; i++) {
        printf("%-15s | ", table->fields[i].name);
    }
    printf("\n---------------------------------------------------------\n");

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

int delete_from_table(Table *table, const char *condition) {
    bool found = false;
    for (int i = 0; i < table->record_count; i++) {
        if (strstr(table->records[i].values, condition) != NULL) {
            for (int j = i; j < table->record_count - 1; j++) {
                table->records[j] = table->records[j + 1];
            }
            table->record_count--;
            found = true;
            i--;
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

    int result = delete_from_table(table, condition);
    update_table_in_state_file(table); // Update table in file after deletion
    return result;
}

void log_message(const char *message) {
    FILE *log_file = fopen("logs.txt", "a");
    if (log_file != NULL) {
        fprintf(log_file, "%s\n", message);
        fclose(log_file);
    } else {
        printf("Error: Unable to open log file.\n");
    }
}

void save_database_state(const Database *db) {
    // Ouvrir le fichier en mode écriture pour écraser le contenu existant
    FILE *state_file = fopen("database_states.txt", "w");
    if (state_file != NULL) {
        // Sauvegarder toutes les tables de la base de données
        for (int i = 0; i < db->table_count; i++) {
            Table *table = db->tables[i];
            fprintf(state_file, "Table: %s\n", table->name);
            fprintf(state_file, "Fields: ");
            for (int j = 0; j < table->field_count; j++) {
                fprintf(state_file, "%s (%d) ", table->fields[j].name, table->fields[j].type);
            }
            fprintf(state_file, "\nRecords:\n");
            for (int k = 0; k < table->record_count; k++) {
                for (int l = 0; l < table->field_count; l++) {
                    FieldType type = table->fields[l].type;
                    if (type == TYPE_INT) {
                        fprintf(state_file, "%d ", table->records[k].values[l].intValue);
                    } else if (type == TYPE_STRING) {
                        fprintf(state_file, "%s ", table->records[k].values[l].stringValue);
                    } else if (type == TYPE_FLOAT) {
                        fprintf(state_file, "%.2f ", table->records[k].values[l].floatValue);
                    }
                }
                fprintf(state_file, "\n");
            }
        }
        fclose(state_file);
    } else {
        printf("Error: Unable to open state file.\n");
    }
}

void load_database_state(Database *db) {
    FILE *state_file = fopen("database_states.txt", "r");
    if (state_file != NULL) {
        char line[MAX_COMMAND_LENGTH];
        Table *temp_table = NULL;
        int table_index = -1;

        while (fgets(line, sizeof(line), state_file)) {
            if (strncmp(line, "Table:", 6) == 0) {
                // Si une table précédente était en cours, l'ajouter à la base de données
                if (temp_table != NULL) {
                    db->tables[++table_index] = temp_table;
                }
                temp_table = (Table *)malloc(sizeof(Table));
                if (temp_table == NULL) {
                    fprintf(stderr, "Error: Failed to allocate memory for table.\n");
                    break;
                }
                sscanf(line, "Table: %s", temp_table->name);
                temp_table->field_count = 0;
                temp_table->record_count = 0;
            } else if (strncmp(line, "Fields:", 7) == 0 && temp_table != NULL) {
                char field_name[NAME_LENGTH];
                int field_type;
                char *ptr = line + 7;
                while (sscanf(ptr, "%s (%d)", field_name, &field_type) == 2) {
                    strcpy(temp_table->fields[temp_table->field_count].name, field_name);
                    temp_table->fields[temp_table->field_count].type = field_type;
                    temp_table->field_count++;
                    ptr = strstr(ptr, " ") + 1;
                }
            } else if (strncmp(line, "Records:", 8) == 0 && temp_table != NULL) {
                int record_index = 0;
                while (fgets(line, sizeof(line), state_file) && strlen(line) > 1) {
                    char *ptr = line;
                    for (int j = 0; j < temp_table->field_count; j++) {
                        if (temp_table->fields[j].type == TYPE_INT) {
                            temp_table->records[record_index].values[j].intValue = atoi(ptr);
                        } else if (temp_table->fields[j].type == TYPE_STRING) {
                            sscanf(ptr, "%s", temp_table->records[record_index].values[j].stringValue);
                        } else if (temp_table->fields[j].type == TYPE_FLOAT) {
                            temp_table->records[record_index].values[j].floatValue = atof(ptr);
                        }
                        ptr = strstr(ptr, " ") + 1;
                    }
                    temp_table->record_count++;
                    record_index++;
                }
            }
        }
        if (temp_table != NULL) {
            db->tables[++table_index] = temp_table;
        }
        db->table_count = table_index + 1;
        fclose(state_file);
    } else {
        printf("Error: Unable to open state file.\n");
    }
}

void append_table_to_state_file(const Table *table) {
    FILE *state_file = fopen("database_states.txt", "a");
    if (state_file != NULL) {
        fprintf(state_file, "Table: %s\n", table->name);
        fprintf(state_file, "Fields: ");
        for (int j = 0; j < table->field_count; j++) {
            fprintf(state_file, "%s (%d) ", table->fields[j].name, table->fields[j].type);
        }
        fprintf(state_file, "\nRecords:\n");
        for (int k = 0; k < table->record_count; k++) {
            for (int l = 0; l < table->field_count; l++) {
                FieldType type = table->fields[l].type;
                if (type == TYPE_INT) {
                    fprintf(state_file, "%d ", table->records[k].values[l].intValue);
                } else if (type == TYPE_STRING) {
                    fprintf(state_file, "%s ", table->records[k].values[l].stringValue);
                } else if (type == TYPE_FLOAT) {
                    fprintf(state_file, "%.2f ", table->records[k].values[l].floatValue);
                }
            }
            fprintf(state_file, "\n");
        }
        fclose(state_file);
    } else {
        printf("Error: Unable to open state file.\n");
    }
}

void update_table_in_state_file(const Table *table) {
    // Lire le contenu existant du fichier
    FILE *state_file = fopen("database_states.txt", "r");
    if (state_file == NULL) {
        printf("Error: Unable to open state file.\n");
        return;
    }

    // Stocker le contenu du fichier dans une mémoire temporaire
    char *file_content = NULL;
    size_t file_size = 0;
    fseek(state_file, 0, SEEK_END);
    file_size = ftell(state_file);
    fseek(state_file, 0, SEEK_SET);
    file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL) {
        printf("Error: Failed to allocate memory for file content.\n");
        fclose(state_file);
        return;
    }
    fread(file_content, 1, file_size, state_file);
    fclose(state_file);
    file_content[file_size] = '\0';

    // Ouvrir le fichier en mode écriture pour le mettre à jour
    state_file = fopen("database_states.txt", "w");
    if (state_file == NULL) {
        printf("Error: Unable to open state file.\n");
        free(file_content);
        return;
    }

    // Écrire le contenu modifié
    char *ptr = file_content;
    while (*ptr) {
        // Chercher la table à mettre à jour
        if (strncmp(ptr, "Table: ", 7) == 0) {
            char table_name[NAME_LENGTH];
            sscanf(ptr, "Table: %s", table_name);
            if (strcmp(table_name, table->name) == 0) {
                // Écrire la table mise à jour
                fprintf(state_file, "Table: %s\n", table->name);
                fprintf(state_file, "Fields: ");
                for (int j = 0; j < table->field_count; j++) {
                    fprintf(state_file, "%s (%d) ", table->fields[j].name, table->fields[j].type);
                }
                fprintf(state_file, "\nRecords:\n");
               
                for (int k = 0; k < table->record_count; k++) {
                    for (int l = 0; l < table->field_count; l++) {
                        FieldType type = table->fields[l].type;
                        if (type == TYPE_INT) {
                            fprintf(state_file, "%d ", table->records[k].values[l].intValue);
                        } else if (type == TYPE_STRING) {
                            fprintf(state_file, "%s ", table->records[k].values[l].stringValue);
                        } else if (type == TYPE_FLOAT) {
                            fprintf(state_file, "%.2f ", table->records[k].values[l].floatValue);
                        }
                    }
                    fprintf(state_file, "\n");
                }
                // Passer au bloc suivant
                while (*ptr && *ptr != '\n') {
                    ptr++;
                }
                continue;
            }
        }
        // Copier le reste du contenu inchangé
        fprintf(state_file, "%c", *ptr);
        ptr++;
    }

    // Ajouter la table si elle n'existe pas
    fprintf(state_file, "Table: %s\n", table->name);
    fprintf(state_file, "Fields: ");
    for (int j = 0; j < table->field_count; j++) {
        fprintf(state_file, "%s (%d) ", table->fields[j].name, table->fields[j].type);
    }
    fprintf(state_file, "\nRecords:\n");
    for (int k = 0; k < table->record_count; k++) {
        for (int l = 0; l < table->field_count; l++) {
            FieldType type = table->fields[l].type;
            if (type == TYPE_INT) {
                fprintf(state_file, "%d ", table->records[k].values[l].intValue);
            } else if (type == TYPE_STRING) {
                fprintf(state_file, "%s ", table->records[k].values[l].stringValue);
            } else if (type == TYPE_FLOAT) {
                fprintf(state_file, "%.2f ", table->records[k].values[l].floatValue);
            }
        }
        fprintf(state_file, "\n");
    }

    fclose(state_file);
    free(file_content);
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

void free_table(Table *table) {
    if (table != NULL) {
        free(table);
    }
}

void free_database(Database *db) {
    for (int i = 0; i < db->table_count; i++) {
        free_table(db->tables[i]);
    }
}


int main() {
    Database db;
    Table table;
    char command[MAX_COMMAND_LENGTH];

    // Load the database state from the file
    load_database_state(&db);

    // Loop to receive user commands
    while (1) {
        printf("Entrez une commande SQL : ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;  // Exit on read error
        }

        // Remove newline character at the end of the command
        size_t length = strlen(command);
        if (length > 0 && command[length - 1] == '\n') {
            command[length - 1] = '\0';
        }

        // Execute the appropriate command
        if (strncmp(command, "CREER BASE DE DONNEE", 20) == 0) {
            execute_create_database_command(command, &db);
            log_message(command);
        } else if (strncmp(command, "CREER TABLE", 11) == 0) {
            create_table_from_command(command, &table);
            add_table_to_database(&db, &table);
            log_message(command);
        } else if (strncmp(command, "INSERER DANS", 12) == 0) {
            insert_into_table(&table, command);
            log_message(command);
        } else if (strncmp(command, "AFFICHER TOUS DANS", 18) == 0) {
            execute_select_command(&table, command);
            log_message(command);
        } else if (strncmp(command, "SUPPRIMER DANS", 14) == 0) {
            execute_delete_command(&table, command);
            log_message(command);
        } else if (strcmp(command, "LISTE DES TABLES") == 0) {
            list_tables_in_database(&db);
            log_message(command);
        } else if (strcmp(command, "TOUT AFFICHER") == 0) {
            print_table_with_records(&table);
            log_message(command);
        } else {
            printf("Commande inconnue ou mal formatée.\n");
            log_message("Commande inconnue ou mal formatée.");
        }

        // Save the state of the database after each command
        save_database_state(&db);
    }
free_table(&table);
free_database(&db);

    return 0;
}


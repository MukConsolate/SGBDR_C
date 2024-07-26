#ifndef __TABLE_H
#define __TABLE_H

#define MAX_TABLES 100


typedef struct _champ Champ; 
struct  _champ{

};
typedef struct _enregistrement Enregistrement;
struct _enregistrement
{
    
};

typedef struct _table Table; 
struct  _table
{
   Champ attribut;
   Enregistrement ligne[];

};

Table *creerTable(Champ * attribut, Enregistrement * ligne);

#endif
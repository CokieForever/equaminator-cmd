/*

Equaminator CMD - A mathematic parser with interface in "command line" mode
Copyright (C) 2012 Cokie (cokie.forever@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/


#define COMMANDES

typedef struct Commande Commande;

#ifndef CONSTANTES
#include "constantes.h"
#endif

#ifndef MAIN
#include "main.h"
#endif

#ifndef OBJETS
#include "objets.h"
#endif

struct Commande
{
    char nom[MAX_NOM],
         description[MAX_EXPR];
    int (*fonction)(const char[]);
};

int ExecCommande(const char expr[]);
int EstCommande(const char expr[]);
void ExtraireNom(const char args[], char nom[], int toUpper, int garderEsp);


int Exit(const char[]);
int Help(const char[]);
int Reset(const char[]);
int VarList(const char[]);
int OpList(const char[]);
int Load(const char args[]);
int Clear(const char args[]);



//Résultats de commande

#define TERMINERPRGM        1
#define CMDEXECUTEE         2
#define CMDNONTROUVEE       0

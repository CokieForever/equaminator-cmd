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


#define MAIN

#ifndef CONSTANTES
#include "constantes.h"
#endif

#ifndef OBJETS
#include "objets.h"
#endif

#ifndef OPERATIONS
#include "operations.h"
#endif

#ifndef COMMANDES
#include "commandes.h"
#endif

int ChargerParam(void);
int Executer(const char instruction[]);
Objet Calculer(const char expr[], Objet locVar[]);
int Preparer(char expr[]);
Objet ErreurIntFn(const char expr[], const char fichier[], int ligne);
Objet ErreurExt(const char expr[], const char expl[]);
void EnleverParentheses(char expr[]);
int InsererCaractere(char expr[], unsigned int i, char c);
int EnleverCaractere(char expr[], char c);
char* StrchrRev(const char expr[], char c);
char* StrstrRev(const char expr0[], const char expr1[]);
char* StrchrEx(const char expr[], char c);
char* StrchrRevEx(const char expr[], char c);
char* StrstrRevEx(const char expr0[], const char expr1[]);
unsigned int StrCount(const char expr[], char c);
unsigned int StrCountEx(const char expr[], char c);
char* StrstrEx(const char expr[], const char str[]);
int StrcmpEx(const char expr1[], const char expr2[]);
int EstNombre(char c);
int EstLettre(char c);
void SaisirTexte(char buf[], unsigned int taille);
void Purger(void);
void Clean(char chaine[]);
int AfficherObjet(Objet obj);
void AfficherMatrice(Objet obj);
int SprintObjet(char buf[], Objet obj);
int PrintObjet(Objet obj);
void SprintComplexe(char buf[], Objet obj);
void SprintFloat(char buf[], double num);
void PrintFloat(double num);
void SprintType(char buf[], Objet obj);
void PrintType(Objet obj);
void SprintTypeOp(char buf[], Operation op);
void PrintTypeOp(Operation op);
void SprintObjetStr(char buf[], Objet obj);
#ifdef MEMMGR
void FreeEverything(void);
#endif

#define ErreurInt(a)    ErreurIntFn(a, __FILE__, __LINE__)

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


#define OBJETS

#ifndef CONSTANTES
#include "constantes.h"
#endif


#define ERREUR    (-1)
#define VIDE       0
#define COMPLEXE   1
#define FONCTION   2
#define VARIABLE   3
#define SUITE      4
#define MATRICE    5
#define UPLET      6
#define BOOLEEN    7
#define TEXTE      8

#define NORMAL     0
#define LOCAL      1
#define PERMANENT  2
#define USERVAR    4

typedef struct Objet
{
    int type, special;
    char nom[MAX_NOM],
         *description;

    void *data;
    size_t size;
} Objet;

typedef struct Complexe
{
    double re, im;
} Complexe;

typedef struct Fonction
{
    Objet (*fonction)(Objet);

    char expr[MAX_EXPR], var[MAX_VAR+1];
} Fonction;

/*typedef struct Variable
{
    Objet val;

    char expr[MAX_EXPR];
} Variable;*/

typedef struct Suite
{
    Objet (*suite)(int);

    Objet terme1;
    int indice1;
    char exprRec[MAX_EXPR], var;
} Suite;

typedef struct Matrice
{
    Objet **matrice;
    unsigned int lignes, col;
} Matrice;

typedef struct Uplet
{
    Objet *uplet;
    unsigned int taille;
} Uplet;

#define VRAI 1
#define FAUX 0
typedef struct Booleen
{
    int val;
} Booleen;


#ifndef MAIN
#include "main.h"
#endif

int InitialiserObjets(void);
void Desinitialiser(void);
void AffecterAns(Objet obj);
Objet RetournerAns(void);
Objet* RecupTabObj(void);
Objet CreerObjet(int type, const char nom[], const char descr[], void *data, size_t size);
void LibererObjet(Objet* a);
Objet CopierObjet(Objet a);
Objet CreerFonction(Objet (*fonction)(Objet), const char expr[], const char var[], const char nom[], const char descr[]);
Objet CreerComplexe(double re, double im, const char nom[], const char descr[]);
Objet CreerComplexe2(double mod, double arg, const char nom[], const char descr[]);
double Mod(Objet x);
double Arg(Objet x);
Objet CreerBooleen(int val, const char nom[], const char descr[]);
Matrice CreerMatrice(unsigned int l, unsigned int c);
Uplet CreerUplet(unsigned int n);
void LibererMatrice(Matrice mat);
void LibererUplet(Uplet upl);
Objet CreerSuite(Objet (*suite)(int), Objet terme1, int indice1, const char exprRec[], const char var, const char nom[], const char descr[]);
int NumeroObjet(const char nom[]);
int NumeroLocVar(const char nom[]);
Objet IdentifierObjet(const char expr[], Objet locVar[]);
Objet RecupererFonction(const char expr[], Objet locVar[]);
Objet RecupererMatrice(const char expr[]);
Objet RecupererUplet(const char expr[]);
int ExtraireParametres(const char expr[], char *tab[], unsigned int n, unsigned int l);
char** ExtraireParamAuto(const char expr[], unsigned int* n, unsigned int* l);
int CompterParametres(const char expr[], unsigned int *lmax);
void LibererTab(char *tab[], unsigned int n);
Objet MatIdentite(unsigned int n);
int EstEntier(double a);
int IsParenth(const char expr[]);
Objet CreerObjetTexte(const char nom[], const char texte[]);
int PivotGauss(Objet a, unsigned int dim);
size_t TailleDataObjet(Objet obj);
int EnregistrerObjet(Objet obj);
int OublierObjet(const char nom[]);
int MAJTabObjets(void);


Objet Exp(Objet a);
Objet Sin(Objet a);
Objet Cos(Objet a);
Objet Tan(Objet a);
Objet Abs(Objet a);
Objet Ach(Objet a);
Objet Ln(Objet a);
Objet Acos(Objet a);
Objet Asin(Objet a);
Objet Ash(Objet a);
Objet Atan(Objet a);
Objet Ath(Objet a);
Objet Ceil(Objet a);
Objet Cosh(Objet a);
Objet Frac(Objet a);
Objet Trunc(Objet a);
Objet Floor(Objet a);
Objet Round(Objet a);
Objet Sinh(Objet a);
Objet Sqrt(Objet a);
Objet Tanh(Objet a);
Objet Norme(Objet a);
Objet Det(Objet a);
Objet Si(Objet a);
Objet Drv(Objet a);
Objet Integ(Objet a);
Objet Zero(Objet o);
Objet RePart(Objet a);
Objet ImPart(Objet a);
Objet ArgCp(Objet a);
Objet ModCp(Objet a);


//Macros pour accéder DIRECTEMENT aux données enregistrées dans un objet
#define Re(a)                (((Complexe*)((a).data))->re)
#define Im(a)                (((Complexe*)((a).data))->im)
#define ExprFn(a)            (((Fonction*)((a).data))->expr)
#define VarFn(a)             (((Fonction*)((a).data))->var)
#define IndSuite(a)          (((Suite*)((a).data))->indice1)
#define TermeSuite(a)        (((Suite*)((a).data))->terme1)
#define ExprSuite(a)         (((Suite*)((a).data))->exprRec)
#define VarSuite(a)          (((Suite*)((a).data))->var)
#define LignesMat(a)         (((Matrice*)((a).data))->lignes)
#define ColMat(a)            (((Matrice*)((a).data))->col)
#define MatriceObj(a)        (((Matrice*)((a).data))->matrice)
#define TailleUplet(a)       (((Uplet*)((a).data))->taille)
#define UpletObj(a)          (((Uplet*)((a).data))->uplet)
#define ValBool(a)           (((Booleen*)((a).data))->val)
#define TexteObj(a)          ((a).description)
#define TexteEstim(a)        (*((Objet*)((a).data)))

#define IsPermanent(a)       (((a).special & PERMANENT) == PERMANENT)
#define IsUserVar(a)         (((a).special & USERVAR) == USERVAR)
#define IsLocal(a)           (((a).special & LOCAL) == LOCAL)

#define EstNul(a)            (fabs(a)<1./pow(10,PRECISION))

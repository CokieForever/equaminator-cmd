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


#define OPERATIONS

typedef struct Operation Operation;

#ifndef CONSTANTES
#include "constantes.h"
#endif

#ifndef OBJETS
#include "objets.h"
#endif

#define OPBIN  1   //operateur binaire
#define OPUND  2   //operateur unaire à droite
#define OPUNG  4   //operateur unaire à gauche

#define OPTXD  8    //operateur demandant du texte (situé) à droite
#define OPTXG  16   //operateur demandant du texte (situé) à gauche

#define MAX_OPERATEUR 5   //taille maximale de la chaine d'un opérateur

struct Operation
{
    int type;
    Objet (*fonction)(Objet, Objet);
    char nom[MAX_NOM],
         description[MAX_EXPR];

    char operateur[MAX_OPERATEUR];
    int priorite;   //de 0 (basse) à 5 (haute)
};

Objet Add(Objet a, Objet b);
Objet AddFonctions(Objet a, Objet b);
Objet AddSuites(Objet a, Objet b);
Objet AddMatrices(Objet a, Objet b);
Objet AddUplets(Objet a, Objet b);

Objet Opp(Objet a, Objet b);
Objet OppFonction(Objet a);
Objet OppSuite(Objet a);
Objet OppMatrice(Objet a);
Objet OppUplet(Objet a);

Objet Sous(Objet a, Objet b);

Objet Mult(Objet a, Objet b);
Objet MultSuites(Objet a, Objet b);
Objet MultMatrices(Objet a, Objet b);

Objet ScalMult(Objet a, Objet b);
Objet ScalMultFonction(Objet a, Objet b);
Objet ScalMultSuite(Objet a, Objet b);
Objet ScalMultMatrice(Objet a, Objet b);
Objet ScalMultUplet(Objet a, Objet b);

Objet Inv(Objet a, Objet b);
Objet InvSuite(Objet a);
Objet InvMatrice(Objet a);

Objet Div(Objet a, Objet b);

Objet Puiss(Objet a, Objet b);
Objet Compo(Objet a, Objet b);
Objet Fact(Objet a, Objet b);

Objet Fonc(Objet a, Objet b);
Objet NewVar(Objet a, Objet b);
Objet NewCst(Objet a, Objet b);

Objet OuEx(Objet a, Objet b);
Objet Et(Objet a, Objet b);
Objet Ou(Objet a, Objet b);
Objet Neg(Objet a, Objet b);

Objet OpCentralComp(Objet a, Objet b, int(*fonction)(double,double), const char operateur[]);
Objet SupStrict(Objet a, Objet b);
Objet SupEgal(Objet a, Objet b);
Objet InfStrict(Objet a, Objet b);
Objet InfEgal(Objet a, Objet b);

Objet Egal(Objet a, Objet b);
Objet Egal2(Objet a, Objet b, int ghostErrors);
Objet Diff(Objet a, Objet b);
Objet EgalMatrice(Objet a, Objet b, int ghostErrors);
Objet EgalUplet(Objet a, Objet b, int ghostErrors);

Objet Eval(Objet a, Objet b);
Objet EvalFn(Objet a, Objet b);
Objet EvalMat(Objet a, Objet b);
Objet EvalUplet(Objet a, Objet b);

Objet EchangerLignes(Objet a, unsigned int l1, unsigned int l2);
Objet ScalMultLigne(Objet a, unsigned int l0, double re, double im);
Objet OpLigne(Objet a, unsigned int l1, unsigned int l2, double re, double im);

#ifndef MAIN
#include "main.h"
#endif

int TrouverOperateur(const char expr[], Operation *op);
int EstDansOperateur(const char expr[], int p);
Operation IdentifierOperation(const char op[], int type);
int RemplacerVar(char expr[], const char avar[], const char nvar[]);
int RemplacerVarsGlob(char expr[], Objet locVar[]);
int AutreOperation(Operation *op);
int VerifNomVar(const char nom[]);
void AffecterTabLocVar(Objet locVar[]);
Objet* EliminerLocVars(Objet locVar[], const char vars[]);
Operation* RecupTabOp(void);

#define EstOperateurUG(op)      (((op).type & OPUNG) == OPUNG)
#define EstOperateurUD(op)      (((op).type & OPUND) == OPUND)
#define EstOperateurBinaire(op) (((op).type & OPBIN) == OPBIN)

#define EstDansOperateurUG(a,b) ((EstDansOperateur(a,b) & OPUNG) == OPUNG)
#define EstDansOperateurUD(a,b) ((EstDansOperateur(a,b) & OPUND) == OPUND)
#define EstDansOperateurB(a,b)  ((EstDansOperateur(a,b) & OPBIN) == OPBIN)

#define EstOperateurTexteDroite(op)   (((op).type & OPTXD) == OPTXD)
#define EstOperateurTexteGauche(op)   (((op).type & OPTXG) == OPTXG)

#define OperationsIdentiques(a,b)  ((a).type==(b).type && !strcmp((a).operateur,(b).operateur) && (a).fonction==(b).fonction && (a).priorite==(b).priorite)

#define Descr(a) ((a).special == PERMANENT ? (a).nom : (a).description)

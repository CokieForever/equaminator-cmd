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


#include "operations.h"


Operation tabOperations[] =
{
    {OPBIN, Add,        "Addition",                 "Loi d'addition usuelle [Complexes, Fonctions, Matrices, Uplets]",                 "+",    0},
    {OPBIN, Sous,       "Soustraction",             "Addition de l'oppose [Complexes, Fonctions, Matrices, Uplets]",                   "-",    0},
    {OPBIN, Mult,       "Multiplication",           "Loi de multiplication usuelle [Complexes, Matrices]",                             "*",    1},
    {OPBIN, Mult,       "Multiplication implicite", "! Systeme seulement !",                                                           "#",    2},
    {OPBIN, Div,        "Division",                 "Multiplication par l'inverse [Complexes]",                                        "/",    1},
    {OPBIN, Puiss,      "Puissance",                "Fonction puissance usuelle [Complexes, Matrices]",                                "^",    3},
    {OPBIN, Compo,      "Composition",              "Loi de composition usuelle [Fonctions]",                                          "@",    1},
    {OPBIN, Eval,       "Evaluation",               "Operateur d'evaluation [Fonctions, Matrices, Uplets]",                            ":",    4},

    {OPBIN, OuEx,       "Ou exclusif",              "Operateur binaire/logique OU exclusif [Entiers, Booleens]",              /* ¤ */  {-49}, -3},
    {OPBIN, Et,         "Et",                       "Operateur binaire/logique ET [Entiers, Booleens]",                                "&",   -2},
    {OPBIN, Et,         "Et",                       "Operateur logique ET [Booleens]",                                                 "*",    1},
    {OPBIN, Ou,         "Ou",                       "Operateur binaire/logique OU [Entiers, Booleens]",                                "|",   -3},
    {OPBIN, Ou,         "Ou",                       "Operateur logique OU [Booleens]",                                                 "+",    0},

    {OPBIN, SupStrict,  "Superieur a",              "Operateur de comparaison \"Superieur a\" [Reels]",                                ">",   -1},
    {OPBIN, SupEgal,    "Superieur ou egal a",      "Operateur de comparaison \"Superieur ou egal a\" [Reels]",                        ">=",  -1},
    {OPBIN, InfStrict,  "Inferieur a",              "Operateur de comparaison \"Inferieur a\" [Reels]",                                "<",   -1},
    {OPBIN, InfEgal,    "Inferieur ou egal a",      "Operateur de comparaison \"Inferieur ou egal a\" [Reels]",                        "<=",  -1},
    {OPBIN, Egal,       "Egal a",                   "Operateur de comparaison \"Egal a\" [Complexes, Uplets, Matrices]",               "==",  -1},
    {OPBIN, Diff,       "Different de",             "Operateur de comparaison \"Different de\" [Complexes, Uplets, Matrices]",         "~=",  -1},

    {OPUND, Fact,       "Factorielle",              "Fonction factorielle usuelle [Entiers naturels]",                                 "!",    4},

    {OPUNG, Opp,        "Oppose",                   "Oppose par la loi d'addition usuelle [Complexes, Fonctions, Matrices, Uplets]",   "-",    0},
    {OPUNG, Inv,        "Inverse",                  "Inverse par la loi de multiplication usuelle [Complexes, Matrices]",              "/",    4},

    {OPUNG, Neg,        "Negation",                 "Operateur logique/binaire NON [Entiers, Booleens]",                               "~",    4},

    {OPBIN | OPTXD | OPTXG,
            Fonc,       "Creation de fonction",     "Operateur de creation de fonctions [Texte]",                                      "->",   9},
    {OPBIN | OPTXD | OPTXG,
            NewVar,     "Affectation variable",     "Operateur d'affectation de variable (sans estimation) [Texte]",                   "=",   -9},
    {OPBIN | OPTXD | OPTXG,
            NewCst,     "Affectation constante",    "Operateur d'affectation de constante (avec estimation) [Texte]",                  ":=",  -9},

    {0, NULL, "", "", "", 0}
};

Objet *tabLocVar_op = NULL;


//Additionne deux objets a et b selon leur type
Objet Add(Objet a, Objet b)
{
    char buf[MAX_EXPR];

    snprintf(buf, MAX_EXPR-1, "(%s)+(%s)", Descr(a), Descr(b));

    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);
    if (a.type == VIDE)  //le vide joue le rôle de neutre
        return CopierObjet(b);
    if (b.type == VIDE)
        return CopierObjet(a);
    if (a.type != b.type)
        return ErreurExt("Impossible d'additionner : Types incompatibles", buf);

    switch(a.type)
    {
        case COMPLEXE:
            return CreerComplexe(Re(a)+Re(b), Im(a)+Im(b), NULL, buf);
        case FONCTION:
            return AddFonctions(a, b);
        case SUITE:
            return AddSuites(a, b);
        case MATRICE:
            return AddMatrices(a, b);
        case UPLET:
            return AddUplets(a, b);
        default:
            break;
    }

    return ErreurExt("Impossible d'additionner : Type non adapte", buf);
}

//Additionne deux objets de type fonction
Objet AddFonctions(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != FONCTION || b.type != FONCTION)
        return ErreurInt("Type inattendu");

    char *expr1 = ExprFn(a),
         *expr2 = ExprFn(b),
         *var1, *var2,
         buf[MAX_EXPR], buf2[MAX_EXPR],
         c1[2] = "", c2[2] = "";
    int i;

    snprintf(buf, MAX_EXPR-1, "(%s)+(%s)", Descr(a), Descr(b));

    if (expr2)
        expr2 = malloc(sizeof(char)*(strlen(expr2)+1));

    var1 = VarFn(a);
    var2 = VarFn(b);

    if (!expr1 || !expr2 || !var1 || !var2)
        return ErreurInt("Objet incorrect");

    if (strlen(var1) != strlen(var2))
    {
        free(expr2);
        return ErreurExt("Impossible d'additionner : Fonctions incompatibles", buf);
    }

    strcpy(expr2, ExprFn(b));

    for (i=0 ; var1[i] ; i++)
    {
        if (var1[i] != var2[i])
        {
            c1[0] = var1[i];
            c2[0] = var2[i];
            RemplacerVar(expr2, c2, c1);
        }
    }

    snprintf(buf2, MAX_EXPR-1, "((%s)+(%s))", expr1, expr2);
    free(expr2);
    return CreerFonction(NULL, buf2, var1, NULL, buf);
}


//Additionne deux objets de type suite
Objet AddSuites(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != SUITE || b.type != SUITE)
        return ErreurInt("Type inattendu");

    int indice = IndSuite(a);
    Objet terme = Add(TermeSuite(a),TermeSuite(b)), obj;
    char *expr1 = ExprSuite(a), *expr2 = ExprSuite(b),
         buf[MAX_EXPR], buf2[MAX_EXPR],
         var1[2] = "", var2[2] = "";

    if (terme.type == ERREUR)
        return terme;
    if (expr2)
        expr2 = malloc(sizeof(char)*(strlen(expr2)+1));

    var1[0] = VarSuite(a);
    var2[0] = VarSuite(b);

    if (!expr1 || !expr2)
        return ErreurInt("Objet incorrect");

    strcpy(expr2, ExprFn(b));

    if (var1[0] != var2[0])
        RemplacerVar(expr2, var2, var1);

    snprintf(buf, MAX_EXPR-1, "(%s)+(%s)", expr1, expr2);
    snprintf(buf2, MAX_EXPR-1, "(%s)+(%s)", Descr(a), Descr(b));

    if (IndSuite(b)>indice)
        indice = IndSuite(b);

    free(expr2);

    obj = CreerSuite(NULL, terme, indice, buf, var1[0], NULL, buf2);
    LibererObjet(&terme);
    return obj;
}

//Additionne deux objets de type matrice
Objet AddMatrices(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != MATRICE || b.type != MATRICE)
        return ErreurInt("Type inattendu");

    unsigned int i, j,
                l = LignesMat(a),
                c = ColMat(a);
    Objet **mat1 = MatriceObj(a),
          **mat2 = MatriceObj(b),
          obj;
    Matrice matR;
    char buf[MAX_EXPR];

    if (!mat1 || !mat2)
        return ErreurInt("Objet incorrect");
    if (l != LignesMat(b) || c != ColMat(b))
        return ErreurExt("Impossible d'additionner : Matrices de tailles différentes", NULL);

    if (l>MAX_MAT)
        l = MAX_MAT;
    if (c>MAX_MAT)
        c = MAX_MAT;

    matR = CreerMatrice(l, c);
    if (!matR.matrice)
        return ErreurInt("Erreur d'allocation");

    for (i=0 ; i<l ; i++)
    {
        for (j=0 ; j<c ; j++)
        {
            matR.matrice[i][j] = Add(mat1[i][j], mat2[i][j]);
            if (matR.matrice[i][j].type == ERREUR)
            {
                obj = CopierObjet(matR.matrice[i][j]);
                LibererMatrice(matR);
                return obj;
            }
        }
    }

    snprintf(buf, MAX_EXPR-1, "(%s)+(%s)", Descr(a), Descr(b));
    return CreerObjet(MATRICE, NULL, buf, &matR, sizeof(Matrice));
}


//Additionne deux objets de type uplet
Objet AddUplets(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != UPLET || b.type != UPLET)
        return ErreurInt("Type inattendu");

    unsigned int i, n = TailleUplet(a);
    Objet *upl1 = UpletObj(a),
          *upl2 = UpletObj(b),
          obj;
    Uplet uplR;
    char buf[MAX_EXPR];

    if (!upl1 || !upl2)
        return ErreurInt("Objet incorrect");
    if (n != TailleUplet(b))
        return ErreurExt("Impossible d'additionner : Uplets de tailles différentes", NULL);

    if (n>MAX_UPLET)
        n = MAX_UPLET;

    uplR = CreerUplet(n);
    if (!uplR.uplet)
        return ErreurInt("Erreur d'allocation");

    for (i=0 ; i<n ; i++)
    {
        uplR.uplet[i] = Add(upl1[i],upl2[i]);
        if (uplR.uplet[i].type == ERREUR)
        {
            obj = CopierObjet(uplR.uplet[i]);
            LibererUplet(uplR);
            return obj;
        }
    }

    snprintf(buf, MAX_EXPR-1, "(%s)+(%s)", Descr(a), Descr(b));
    return CreerObjet(UPLET, NULL, buf, &uplR, sizeof(Uplet));
}


//Donne l'opposé d'un objet selon son type
Objet Opp(Objet a, Objet b)
{
    char buf[MAX_EXPR];

    snprintf(buf, MAX_EXPR-1, "-(%s)", Descr(a));
    b = a; // Evite un warning sur "unused parameter"

    if (a.type == ERREUR)
        return CopierObjet(a);

    switch(a.type)
    {
        case COMPLEXE:
            return CreerComplexe(-Re(a), -Im(a), NULL, buf);
        case FONCTION:
            return OppFonction(a);
        case SUITE:
            return OppSuite(a);
        case MATRICE:
            return OppMatrice(a);
        case UPLET:
            return OppUplet(a);
        default:
            break;
    }

    return ErreurExt("Impossible de calculer l'oppose : Type non adapte", buf);
}

//Donne l'opposé d'un objet de type fonction
Objet OppFonction(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != FONCTION)
        return ErreurInt("Type inattendu");

    char buf[MAX_EXPR],
         buf2[MAX_EXPR];

    snprintf(buf, MAX_EXPR-1, "-(%s)", ExprFn(a));
    snprintf(buf2, MAX_EXPR-1, "-(%s)", Descr(a));
    return CreerFonction(NULL, buf, VarFn(a), NULL, buf2);
}

//Donne l'opposé d'un objet de type suite
Objet OppSuite(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != SUITE)
        return ErreurInt("Type inattendu");

    char buf[MAX_EXPR],
         buf2[MAX_EXPR];
    Objet obj, obj2;

    snprintf(buf, MAX_EXPR-1, "-(%s)", ExprSuite(a));
    snprintf(buf2, MAX_EXPR-1, "-(%s)", Descr(a));
    obj = Opp(TermeSuite(a), TermeSuite(a));
    if (obj.type == ERREUR)
        return obj;

    obj2 = CreerSuite(NULL, obj, IndSuite(a), buf, VarSuite(a), NULL, buf2);
    LibererObjet(&obj);
    return obj2;
}

//Donne l'opposé d'un objet de type matrice
Objet OppMatrice(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != MATRICE)
        return ErreurInt("Type inattendu");

    Objet **mat1 = MatriceObj(a), obj;
    int c = ColMat(a), l = LignesMat(a),
        i,j;
    Matrice mat2;
    char buf[MAX_EXPR];

    if (!mat1)
        return ErreurInt("Objet incorrect");

    mat2 = CreerMatrice(l, c);
    if (!mat2.matrice)
        return ErreurInt("Erreur d'allocation");

    for (i=0 ; i<l ; i++)
    {
        for (j=0 ; j<c ; j++)
        {
            mat2.matrice[i][j] = Opp(mat1[i][j], mat1[i][j]);
            if (mat2.matrice[i][j].type == ERREUR)
            {
                obj = CopierObjet(mat2.matrice[i][j]);
                LibererMatrice(mat2);
                return obj;
            }
        }
    }

    snprintf(buf, MAX_EXPR-1, "-(%s)", Descr(a));
    return CreerObjet(MATRICE, NULL, buf, &mat2, sizeof(Matrice));
}

//Donne l'opposé d'un objet de type uplet
Objet OppUplet(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != UPLET)
        return ErreurInt("Type inattendu");

    int i, n = TailleUplet(a);
    Objet *upl1 = UpletObj(a), obj;
    Uplet upl2;
    char buf[MAX_EXPR];

    upl2 = CreerUplet(n);
    if (!upl2.uplet)
        return ErreurInt("Erreur d'allocation");

    for (i=0 ; i<n ; i++)
    {
        upl2.uplet[i] = Opp(upl1[i], upl1[i]);
        if (upl2.uplet[i].type == ERREUR)
        {
            obj = CopierObjet(upl2.uplet[i]);
            LibererUplet(upl2);
            return obj;
        }
    }

    snprintf(buf, MAX_EXPR-1, "-(%s)", Descr(a));
    return CreerObjet(UPLET, NULL, buf, &upl2, sizeof(Uplet));
}


//Soustrait deux objets
Objet Sous(Objet a, Objet b)
{
    Objet obj = Opp(b, b),
          obj2 = Add(a, obj);

    LibererObjet(&obj);
    return obj2;
}


//Multiplie deux objets selon leur type
Objet Mult(Objet a, Objet b)
{
    char buf[MAX_EXPR];

    snprintf(buf, MAX_EXPR-1, "(%s)*(%s)", Descr(a), Descr(b));

    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);
    if (a.type == VIDE)  //le vide joue le rôle de neutre
        return CopierObjet(b);
    if (b.type == VIDE)
        return CopierObjet(a);
    if ((a.type == COMPLEXE && b.type != COMPLEXE) || (b.type == COMPLEXE && a.type != COMPLEXE))
        return ScalMult(a, b);
    if (a.type != b.type)
        return ErreurExt("Impossible de multiplier : Types incompatibles", buf);

    switch(a.type)
    {
        case COMPLEXE:
            return CreerComplexe(Re(a)*Re(b)-Im(a)*Im(b), Im(a)*Re(b)+Im(b)*Re(a), NULL, buf);
        case SUITE:
            return MultSuites(a, b);
        case MATRICE:
            return MultMatrices(a, b);
        default:
            break;
    }

    return ErreurExt("Impossible de multiplier : Type non adapte", buf);
}

//Multiplie un objet quelconque par un objet de type complexe (multiplication scalaire)
Objet ScalMult(Objet a, Objet b)
{
    Objet obj;
    char buf[MAX_EXPR];

    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);
    if (a.type == VIDE)  //le vide joue le rôle de neutre
        return b;
    if (b.type == VIDE)
        return a;

    if (a.type != COMPLEXE && b.type != COMPLEXE)
        return ErreurInt("Type inattendu");

    if (a.type != COMPLEXE)
    {
        obj = b;
        b = a;
        a = obj;
    }

    switch (b.type)
    {
        case COMPLEXE:
            return Mult(a,b);
        case FONCTION:
            return ScalMultFonction(a,b);
        case SUITE:
            return ScalMultSuite(a,b);
        case MATRICE:
            return ScalMultMatrice(a,b);
        case UPLET:
            return ScalMultUplet(a,b);
        default:
            break;
    }

    snprintf(buf, MAX_EXPR-1, "(%s)*(%s)", Descr(a), Descr(b));
    return ErreurExt("Impossible de multiplier (scalaire) : Type non adapte", buf);
}



//Multiplie deux objets de type suite
Objet MultSuites(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != SUITE || b.type != SUITE)
        return ErreurInt("Type inattendu");

    int indice = IndSuite(a);
    Objet terme = Mult(TermeSuite(a),TermeSuite(b)), obj;
    char *expr1 = ExprSuite(a), *expr2 = ExprSuite(b),
         buf[MAX_EXPR], buf2[MAX_EXPR],
         var1[2] = "", var2[2] = "";

    if (terme.type == ERREUR)
        return terme;
    if (expr2)
        expr2 = malloc(sizeof(char)*(strlen(expr2)+1));

    var1[0] = VarSuite(a);
    var2[0] = VarSuite(b);

    if (!expr1 || !expr2)
        return ErreurInt("Objet incorrect");

    strcpy(expr2, ExprFn(b));

    if (var1[0] != var2[0])
        RemplacerVar(expr2, var2, var1);

    snprintf(buf, MAX_EXPR-1, "(%s)*(%s)", expr1, expr2);
    snprintf(buf2, MAX_EXPR-1, "(%s)*(%s)", Descr(a), Descr(b));

    if (IndSuite(b)>indice)
        indice = IndSuite(b);

    free(expr2);

    obj = CreerSuite(NULL, terme, indice, buf, var1[0], NULL, buf2);
    LibererObjet(&terme);
    return obj;
}

//Multiplie deux objets de type matrice
Objet MultMatrices(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != MATRICE || b.type != MATRICE)
        return ErreurInt("Type inattendu");

    int i, j, k,
        l1 = LignesMat(a), l2 = LignesMat(b),
        c1 = ColMat(a), c2 = ColMat(b);
    Objet **mat1 = MatriceObj(a),
          **mat2 = MatriceObj(b),
          tmp, tmp2;
    Matrice matR;
    char buf[MAX_EXPR];

    if (!mat1 || !mat2)
        return ErreurInt("Objet incorrect");
    if (c1 != l2)
        return ErreurExt("Impossible de multiplier : Matrices de tailles incompatibles", NULL);

    matR = CreerMatrice(l1, c2);
    if (!matR.matrice)
        return ErreurInt("Erreur d'allocation");

    for (i=0 ; i<l1 ; i++)
    {
        for (j=0 ; j<c2 ; j++)
        {
            matR.matrice[i][j].type = VIDE;
            for (k=0 ; k<c1 ; k++)
            {
                tmp = Mult(mat1[i][k], mat2[k][j]);
                tmp2 = matR.matrice[i][j];
                matR.matrice[i][j] = Add(tmp2, tmp);
                if (matR.matrice[i][j].description)
                    SprintObjet(matR.matrice[i][j].description, matR.matrice[i][j]);

                tmp2.type = tmp.type;
                LibererObjet(&tmp);
                LibererObjet(&tmp2);
                if (matR.matrice[i][j].type == ERREUR)
                {
                    tmp2 = CopierObjet(matR.matrice[i][j]);
                    LibererMatrice(matR);
                    return tmp2;
                }
            }
        }
    }

    snprintf(buf, MAX_EXPR-1, "(%s)*(%s)", Descr(a), Descr(b));
    return CreerObjet(MATRICE, NULL, buf, &matR, sizeof(Matrice));
}

//Effectue la multiplication d'une fonction par un scalaire complexe
Objet ScalMultFonction(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);

    Objet obj;
    char *expr, buf[MAX_EXPR], buf2[MAX_EXPR], descrCp[MAX_EXPR];

    if (a.type != COMPLEXE)
    {
        obj = a;
        a = b;
        b = obj;
    }

    if (a.type != COMPLEXE || b.type != FONCTION)
        return ErreurInt("Type inattendu");

    expr = ExprFn(b);
    if (!expr)
        return ErreurInt("Objet incorrect");

    if (a.nom[0])
        snprintf(buf, MAX_EXPR-1, "%s#(%s)", a.nom, expr);
    else if (Descr(a))
        snprintf(buf, MAX_EXPR-1, "(%s)#(%s)", Descr(a), expr);
    else
    {
        SprintComplexe(descrCp, a);
        snprintf(buf, MAX_EXPR-1, "(%s)#(%s)", descrCp, expr);
    }

    snprintf(buf2, MAX_EXPR-1, "(%s)*(%s)", Descr(a), Descr(b));

    return CreerFonction(NULL, buf, VarFn(b), NULL, buf2);
}

//Effectue la multiplication d'une suite par un scalaire complexe
Objet ScalMultSuite(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);

    Objet obj, terme;
    char *expr, buf[MAX_EXPR], buf2[MAX_EXPR], descrCp[MAX_EXPR];

    if (a.type != COMPLEXE)
    {
        obj = a;
        a = b;
        b = obj;
    }

    if (a.type != COMPLEXE || b.type != SUITE)
        return ErreurInt("Type inattendu");

    expr = ExprSuite(b);
    if (!expr)
        return ErreurInt("Objet incorrect");

    terme = ScalMult(a, TermeSuite(b));
    if (terme.type == ERREUR)
        return terme;

    if (a.nom[0])
        snprintf(buf, MAX_EXPR-1, "%s#(%s)", a.nom, expr);
    else if (Descr(a))
        snprintf(buf, MAX_EXPR-1, "(%s)#(%s)", Descr(a), expr);
    else
    {
        SprintComplexe(descrCp, a);
        snprintf(buf, MAX_EXPR-1, "(%s)#(%s)", descrCp, expr);
    }

    snprintf(buf2, MAX_EXPR-1, "(%s)*(%s)", Descr(a), Descr(b));

    obj = CreerSuite(NULL, terme, IndSuite(b), expr, VarSuite(b), NULL, buf2);
    LibererObjet(&terme);
    return obj;
}

//Effectue la multiplication d'une matrice par un scalaire complexe
Objet ScalMultMatrice(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);

    char buf[MAX_EXPR];
    int c, l, i, j;
    Matrice mat2;
    Objet obj, **mat1;

    if (a.type != COMPLEXE)
    {
        obj = a;
        a = b;
        b = obj;
    }

    if (a.type != COMPLEXE || b.type != MATRICE)
        return ErreurInt("Type inattendu");

    mat1 = MatriceObj(b);
    if (!mat1)
        return ErreurInt("Objet incorrect");

    c = ColMat(b);
    l = LignesMat(b);

    mat2 = CreerMatrice(l, c);

    for (i=0 ; i<l; i++)
    {
        for (j=0 ; j<c ; j++)
        {
            mat2.matrice[i][j] = ScalMult(a, mat1[i][j]);
            if (mat2.matrice[i][j].type == ERREUR)
            {
                obj = CopierObjet(mat2.matrice[i][j]);
                LibererMatrice(mat2);
                return obj;
            }
        }
    }

    snprintf(buf, MAX_EXPR-1, "(%s)*(%s)", Descr(a), Descr(b));

    return CreerObjet(MATRICE, NULL, buf, &mat2, sizeof(Matrice));
}

//Effectue la multiplication d'un uplet par un scalaire complexe
Objet ScalMultUplet(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);

    char buf[MAX_EXPR];
    int i, n;
    Objet obj, *upl1;
    Uplet upl2;

    if (a.type != COMPLEXE)
    {
        obj = a;
        a = b;
        b = obj;
    }

    if (a.type != COMPLEXE || b.type != UPLET)
        return ErreurInt("Type inattendu");

    upl1 = UpletObj(b);
    if (!upl1)
        return ErreurInt("Objet incorrect");

    n = TailleUplet(b);
    upl2 = CreerUplet(n);

    for (i=0 ; i<n; i++)
    {
        upl2.uplet[i] = ScalMult(a, upl1[i]);
        if (upl2.uplet[i].type == ERREUR)
        {
            obj = CopierObjet(upl2.uplet[i]);
            LibererUplet(upl2);
            return obj;
        }
    }

    snprintf(buf, MAX_EXPR-1, "(%s)*(%s)", Descr(a), Descr(b));

    return CreerObjet(UPLET, NULL, buf, &upl2, sizeof(Uplet));
}




//Donne l'inverse d'un objet selon son type
Objet Inv(Objet a, Objet b)
{
    char buf[MAX_EXPR];

    snprintf(buf, MAX_EXPR-1, "/(%s)", Descr(a));
    b = a; // Evite un warning sur "unused parameter"

    if (a.type == ERREUR)
        return CopierObjet(a);

    switch(a.type)
    {
        case COMPLEXE:
        {
            if (EstNul(Re(a)) && EstNul(Im(a)))
                return ErreurExt("Division par zero", buf);
            else
            {
                double n = (Re(a)*Re(a)+Im(a)*Im(a));
                return CreerComplexe(Re(a)/n, -Im(a)/n, NULL, buf);
            }
        }
        case SUITE:
            return InvSuite(a);
        case MATRICE:
            return InvMatrice(a);
        default:
            break;
    }

    return ErreurExt("Impossible d'inverser : Type non adapte", buf);
}

//Donne l'inverse d'un objet de type suite
Objet InvSuite(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != SUITE)
        return ErreurInt("Type inattendu");

    char buf[MAX_EXPR],
         buf2[MAX_EXPR];
    Objet obj, obj2;

    snprintf(buf, MAX_EXPR-1, "/(%s)", ExprSuite(a));
    snprintf(buf2, MAX_EXPR-1, "/(%s)", Descr(a));
    obj = Inv(TermeSuite(a), TermeSuite(a));
    if (obj.type == ERREUR)
        return obj;

    obj2 = CreerSuite(NULL, obj, IndSuite(a), buf, VarSuite(a), NULL, buf2);
    LibererObjet(&obj);
    return obj2;
}

//Donne l'inverse d'un objet de type matrice (algorithme de Gauss-Jordan)
Objet InvMatrice(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != MATRICE)
        return ErreurInt("Type inattendu");

    Objet mat1, mat2, obj, **mat3 = MatriceObj(a);
    Objet **m1, **m2;
    unsigned int c = ColMat(a),
                 i,j;
    double re, im;
    char buf[MAX_EXPR];

    snprintf(buf, MAX_EXPR-1, "/(%s)", Descr(a));

    if (!mat3)
        return ErreurInt("Objet incorrect");
    if (c!=LignesMat(a))
        return ErreurExt("Impossible d'inverser une matrice non carree", buf);
    for (i=0 ; i<c ; i++)
    {
        for (j=0 ; j<c ; j++)
        {
            if (mat3[i][j].type != COMPLEXE)
                return ErreurExt("Impossible d'inverser une matrice contenant autre chose que des complexes", buf);
        }
    }

    mat2 = MatIdentite(c);
    if (mat2.type == ERREUR)
        return mat2;

    mat1 = CopierObjet(a);
    if (mat1.type == ERREUR)
    {
        LibererObjet(&mat2);
        return mat1;
    }

    m1 = MatriceObj(mat1);
    m2 = MatriceObj(mat2);
    if (!m1 || !m2)
    {
        LibererObjet(&mat1);
        LibererObjet(&mat2);
        return ErreurInt("Objet incorrect");
    }

    for (i=0 ; i<c ; i++)
    {
        for (j=i ; j<c && (EstNul(Re(m1[i][j])) && EstNul(Im(m1[i][j]))) ; j++);
        if (j>=c)
            return ErreurExt("La matrice n'est pas inversible", buf);

        EchangerLignes(mat1, i, j);
        EchangerLignes(mat2, i, j);

        obj = Inv(m1[i][i], m1[i][i]);
        ScalMultLigne(mat1, i, Re(obj), Im(obj));
        ScalMultLigne(mat2, i, Re(obj), Im(obj));
        LibererObjet(&obj);

        for (j=0 ; j<c ; j++)
        {
            if (j != i)
            {
                re = -Re(m1[j][i]);
                im = -Im(m1[j][i]);
                OpLigne(mat1, j, i, re, im);
                OpLigne(mat2, j, i, re, im);
            }
        }
    }

    LibererObjet(&mat1);
    return mat2;
}

/*
    Operations élémentaires sur les matrices ("sur lignes" uniquement, "sur colonnes" disponibles par transposée)
    Fonctionnent "en place" (effet de bord)
*/

//Echange de lignes
Objet EchangerLignes(Objet a, unsigned int l1, unsigned int l2)  //renvoie un objet uniquement dans le but de gérer les erreurs
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != MATRICE)
        return ErreurInt("Type inattendu");

    unsigned int i,
                 l = LignesMat(a), c = ColMat(a);
    Objet obj, **mat = MatriceObj(a);

    if (l1>=l || l2>=l)
        return ErreurInt("Index de ligne hors limite");

    for (i=0 ; i<c ; i++)
    {
        obj = mat[l1][i];
        mat[l1][i] = mat[l2][i];
        mat[l2][i] = obj;
    }

    return a;  //inutile, seulement pour le typage
}

//Multiplication d'une ligne par un scalaire
Objet ScalMultLigne(Objet a, unsigned int l0, double re, double im)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != MATRICE)
        return ErreurInt("Type inattendu");

    unsigned int i, c = ColMat(a);
    Objet **mat = MatriceObj(a),
          cp = CreerComplexe(re, im, NULL, NULL),
          obj;

    if (!mat)
        return ErreurInt("Objet incorrect");

    if (l0>=LignesMat(a))
        return ErreurInt("Index de ligne hors limite");

    for (i=0 ; i<c ; i++)
    {
        obj = mat[l0][i];
        mat[l0][i] = ScalMult(cp, obj);
        LibererObjet(&obj);
        if (mat[l0][i].type == ERREUR)
        {
            LibererObjet(&cp);
            return CopierObjet(mat[l0][i]);
        }
    }

    LibererObjet(&cp);
    return a;
}

//Ajout à une ligne d'une autre, multipliée par un scalaire
//l1 <- l1+(re+i*im)l2
Objet OpLigne(Objet a, unsigned int l1, unsigned int l2, double re, double im)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (a.type != MATRICE)
        return ErreurInt("Type inattendu");

    unsigned int i, c = ColMat(a), l = LignesMat(a);
    Objet **mat = MatriceObj(a),
          cp = CreerComplexe(re, im, NULL, NULL),
          obj, obj2;

    if (!mat)
        return ErreurInt("Objet incorrect");

    if (l1>=l || l2>=l)
        return ErreurInt("Index de ligne hors limite");

    for (i=0 ; i<c ; i++)
    {
        obj = mat[l1][i];
        obj2 = ScalMult(cp, mat[l2][i]);
        mat[l1][i] = Add(obj, obj2);
        LibererObjet(&obj);
        LibererObjet(&obj2);
        if (mat[l1][i].type == ERREUR)
        {
            LibererObjet(&cp);
            return mat[l1][i];
        }
    }

    LibererObjet(&cp);
    return a;
}


//Divise un objet par un autre : a / b
Objet Div(Objet a, Objet b)
{
    if (a.type != MATRICE && b.type != MATRICE)
    {
        Objet obj = Inv(b, b),
              obj2 = Mult(a, obj);

        LibererObjet(&obj);
        return obj2;
    }

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "(%s)/(%s)", Descr(a), Descr(b));
    return ErreurExt("Impossible de diviser des matrices", buf);
}


//Calcule la puissance d'un objet par un autre : a ^ b
Objet Puiss(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);

    Objet obj, obj2;
    char buf[MAX_EXPR];
    int i;
    snprintf(buf, MAX_EXPR-1, "(%s)^(%s)", Descr(a), Descr(b));

    switch (a.type)
    {
        case COMPLEXE:
        {
            if (b.type != COMPLEXE || !EstNul(Im(b)))
                return ErreurExt("Impossible de calculer la puissance", buf);
            else return CreerComplexe2(pow(Mod(a), Re(b)), Arg(a)*Re(b), NULL, buf);
        }
        case MATRICE:
        {
            if (b.type != COMPLEXE || !EstNul(Im(b)) || !EstEntier(Re(b)) || ColMat(a) != LignesMat(a))
                return ErreurExt("Impossible de calculer la puissance", buf);
            if (Re(b) == 0)
            {
                obj = MatIdentite(ColMat(a));
                if (obj.description)
                    strncpy(obj.description, buf, MAX_EXPR-1);
                return obj;
            }
            if (Re(b) < 0)
                obj = Inv(a, a);
            else obj = CopierObjet(a);

            for (i=1 ; i<abs(Re(b)) && obj.type != ERREUR ; i++)
            {
                obj2 = Mult(obj, a);
                LibererObjet(&obj);
                obj = obj2;
                if (obj.description)
                    strncpy(obj.description, buf, MAX_EXPR-1);
            }

            return obj;
        }
        default:
            break;
    }

    return ErreurExt("Impossible de calculer la puissance", buf);
}

//Composition de l'objet a (type fonction) par l'objet b (type fonction) : a o b
Objet Compo(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);

    char buf[MAX_EXPR],
         expr1[MAX_EXPR], expr2[MAX_EXPR],
         *var1, *var2;
    snprintf(buf, MAX_EXPR-1, "(%s)@(%s)", Descr(a), Descr(b));

    if (a.type != FONCTION || b.type != FONCTION)
        return ErreurExt("Impossible de composer des objets autre que des fonctions", buf);

    var1 = VarFn(a);
    var2 = VarFn(b);
    if (!var1 || !var2 || !ExprFn(a) || !ExprFn(b))
        return ErreurInt("Objet incorrect");
    if (var1[1])
        return ErreurExt("Impossible de composer : Nombre de parametres incompatible", buf);

    snprintf(expr2, MAX_EXPR-1, "(%s)", ExprFn(b));
    strcpy(expr1, ExprFn(a));
    RemplacerVar(expr1, var1, expr2);

    return CreerFonction(NULL, expr1, var2, NULL, buf);
}

//Factorielle d'entiers naturels uniquement
Objet Fact(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    b = a; // Evite un warning sur "unused parameter"

    char buf[MAX_EXPR];
    double r = 1;
    int i;

    snprintf(buf, MAX_EXPR-1, "(%s)!", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)) || !EstEntier(Re(a)) || Re(a)<0)
        return ErreurExt("Impossible de calculer la factorielle", buf);

    for (i=0 ; i<Re(a) ; i++)
        r*=(i+1);

    return CreerComplexe(r,0,NULL,buf);
}


//Effectue un ou exclusif binaire ou logique entre les objets
Objet OuEx(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "(%s)%c(%s)", Descr(a), 15, Descr(b));

    if (a.type != b.type)
        return ErreurExt("Impossible d'effectuer un OU exclusif : type incompatibles", buf);

    switch(a.type)
    {
        case COMPLEXE:
            if (!EstNul(Im(a)) || !EstEntier(Re(a)) || !EstNul(Im(b)) || !EstEntier(Re(b)))
                break;
            else return CreerComplexe( ((int)Re(a))^((int)Re(b)), 0, NULL, buf);
        case BOOLEEN:
            return CreerBooleen( ValBool(a)!=ValBool(b) ? VRAI : FAUX, NULL, buf);
        default:
            break;
    }

    return ErreurExt("Impossible d'effectuer un OU exclusif : Type non adapte", buf);
}

//Effectue un et binaire ou logique entre les objets
Objet Et(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "(%s)&(%s)", Descr(a), Descr(b));

    if (a.type != b.type)
        return ErreurExt("Impossible d'effectuer un ET : type incompatibles", buf);

    switch(a.type)
    {
        case COMPLEXE:
            if (!EstNul(Im(a)) || !EstEntier(Re(a)) || !EstNul(Im(b)) || !EstEntier(Re(b)))
                break;
            else return CreerComplexe( ((int)Re(a))&((int)Re(b)), 0, NULL, buf);
        case BOOLEEN:
            return CreerBooleen( ValBool(a)&&ValBool(b) ? VRAI : FAUX, NULL, buf);
        default:
            break;
    }

    return ErreurExt("Impossible d'effectuer un ET : Type non adapte", buf);
}

//Effectue un ou binaire ou logique entre les objets
Objet Ou(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "(%s)|(%s)", Descr(a), Descr(b));

    if (a.type != b.type)
        return ErreurExt("Impossible d'effectuer un OU : Types incompatibles", buf);

    switch(a.type)
    {
        case COMPLEXE:
            if (!EstNul(Im(a)) || !EstEntier(Re(a)) || !EstNul(Im(b)) || !EstEntier(Re(b)))
                break;
            else return CreerComplexe( ((int)Re(a))|((int)Re(b)), 0, NULL, buf);
        case BOOLEEN:
            return CreerBooleen( ValBool(a)||ValBool(b) ? VRAI : FAUX, NULL, buf);
        default:
            break;
    }

    return ErreurExt("Impossible d'effectuer un OU : Type non adapte", buf);
}

//Effectue un non binaire ou logique sur l'objet
Objet Neg(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    b = a; // Evite un warning sur "unused parameter"

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "~(%s)", Descr(a));

    switch(a.type)
    {
        case COMPLEXE:
            if (!EstNul(Im(a)) || !EstEntier(Re(a)))
                break;
            else return CreerComplexe( ((int)Re(a))^(-1), 0, NULL, buf);
        case BOOLEEN:
            return CreerBooleen( ValBool(a) ? FAUX : VRAI, NULL, buf);
        default:
            break;
    }

    return ErreurExt("Impossible d'effectuer un NON : Type non adapte", buf);
}


//Fonction générique capable de creer un objet booléen à partir de deux objets et d'une fonction de comparaison, gestion des erreurs incluse
Objet OpCentralComp(Objet a, Objet b, int(*fonction)(double,double), const char operateur[])
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    if (b.type == ERREUR)
        return CopierObjet(b);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "(%s)%s(%s)", Descr(a), operateur, Descr(b));

    if (a.type != COMPLEXE || b.type != COMPLEXE || !EstNul(Im(a)) || !EstNul(Im(b)))
        return ErreurExt("Impossible de comparer : objets non reels", buf);

    return CreerBooleen(fonction(Re(a),Re(b)) ? VRAI : FAUX, NULL, buf);
}


//Compare les objets passés ; renvoie un objet booléen VRAI si a > b, FAUX sinon
int Fn_SupStrict(double nb1, double nb2)
{
    return !EstNul(nb1-nb2) && nb1 > nb2;
}
Objet SupStrict(Objet a, Objet b)
{
    return OpCentralComp(a, b, Fn_SupStrict, ">");
}

//Compare les objets passés ; renvoie un objet booléen VRAI si a >= b, FAUX sinon
int Fn_SupEgal(double nb1, double nb2)
{
    return EstNul(nb1-nb2) || nb1 >= nb2;
}
Objet SupEgal(Objet a, Objet b)
{
    return OpCentralComp(a, b, Fn_SupEgal, ">=");
}

//Compare les objets passés ; renvoie un objet booléen VRAI si a < b, FAUX sinon
int Fn_InfStrict(double nb1, double nb2)
{
    return !EstNul(nb1-nb2) && nb1 < nb2;
}
Objet InfStrict(Objet a, Objet b)
{
    return OpCentralComp(a, b, Fn_InfStrict, "<");
}

//Compare les objets passés ; renvoie un objet booléen VRAI si a <= b, FAUX sinon
int Fn_InfEgal(double nb1, double nb2)
{
    return EstNul(nb1-nb2) || nb1 <= nb2;
}
Objet InfEgal(Objet a, Objet b)
{
    return OpCentralComp(a, b, Fn_InfEgal, "<=");
}

Objet Egal(Objet a, Objet b)
{
    return Egal2(a, b, 0);
}
Objet Egal2(Objet a, Objet b, int ghostErrors)
{
    if (a.type == ERREUR && !ghostErrors)
        return CopierObjet(a);
    if (b.type == ERREUR && !ghostErrors)
        return CopierObjet(b);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "(%s)==(%s)", Descr(a), Descr(b));

    if (a.type != b.type)
        return ErreurExt("Impossible de comparer : type incompatibles", buf);
    if (a.type == ERREUR && ghostErrors)
        return CreerBooleen(VRAI, NULL, buf);

    switch(a.type)
    {
        case COMPLEXE:
            return CreerBooleen(EstNul(Re(a)-Re(b)) && EstNul(Im(a)-Im(b)) ? VRAI : FAUX, NULL, buf);
        case MATRICE:
            return EgalMatrice(a, b, ghostErrors);
        case UPLET:
            return EgalUplet(a, b, ghostErrors);
        default:
            break;
    }

    return ErreurExt("Impossible de comparer : Type non adapte", buf);

}

Objet EgalMatrice(Objet a, Objet b, int ghostErrors)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != MATRICE)
        return ErreurInt("Type inattendu");

    Objet **mat1 = MatriceObj(a),
          **mat2 = MatriceObj(b), r;
    unsigned int c = ColMat(a),
                 l = LignesMat(a),
                 i,j;
    int egal=1;
    char buf[MAX_EXPR];

    snprintf(buf, MAX_EXPR-1, "(%s)==(%s)", Descr(a), Descr(b));

    if (!mat1 || !mat2)
        return ErreurInt("Objet incorrect");

    if (c != ColMat(b) || l != LignesMat(b))
        return CreerBooleen(FAUX, NULL, buf);

    for (i=0 ; i<l && egal ; i++)
    {
        for (j=0 ; j<c && egal ; j++)
        {
            r = Egal2(mat1[i][j], mat2[i][j], ghostErrors);
            if (r.type == ERREUR || !ValBool(r))
                egal = 0;
            LibererObjet(&r);
        }
    }

    return CreerBooleen(egal ? VRAI : FAUX, NULL, buf);
}

Objet EgalUplet(Objet a, Objet b, int ghostErrors)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != UPLET)
        return ErreurInt("Type inattendu");

    Objet *upl1 = UpletObj(a),
          *upl2 = UpletObj(b), r;
    unsigned int n = TailleUplet(a), i;
    int egal=1;
    char buf[MAX_EXPR];

    snprintf(buf, MAX_EXPR-1, "(%s)==(%s)", Descr(a), Descr(b));

    if (!upl1 || !upl2)
        return ErreurInt("Objet incorrect");

    if (n != TailleUplet(b))
        return CreerBooleen(FAUX, NULL, buf);

    for (i=0 ; i<n && egal ; i++)
    {
        r = Egal2(upl1[i], upl2[i], ghostErrors);
        if (r.type == ERREUR || !ValBool(r))
            egal = 0;
        LibererObjet(&r);
    }

    return CreerBooleen(egal ? VRAI : FAUX, NULL, buf);
}

Objet Diff(Objet a, Objet b)
{
    Objet r = Egal(a,b);
    int egal;
    char buf[MAX_EXPR];

    if (r.type == ERREUR)
        return r;
    else egal = ValBool(r);

    LibererObjet(&r);
    snprintf(buf, MAX_EXPR-1, "(%s)~=(%s)", Descr(a), Descr(b));
    return CreerBooleen(egal ? FAUX : VRAI, NULL, buf);
}


Objet Eval(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);

    char buf[MAX_EXPR];

    switch (a.type)
    {
        case FONCTION :
            return EvalFn(a,b);
        case MATRICE:
            return EvalMat(a,b);
        case UPLET:
            return EvalUplet(a,b);
        default:
            break;
    }

    snprintf(buf, MAX_EXPR-1, "(%s):(%s)", Descr(a), Descr(b));
    return ErreurExt("Impossible d'evaluer autre chose que des fonctions / matrices / uplets", buf);
}

Objet EvalFn(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != FONCTION)
        return ErreurInt("Type inattendu");

    Fonction *fn;
    Objet tab[MAX_VAR+1], *upl, r;
    char buf[MAX_EXPR];
    int i;

    snprintf(buf, MAX_EXPR-1, "(%s):(%s)", Descr(a), Descr(b));

    fn = (Fonction*)(a.data);
    if (fn->fonction)
        return fn->fonction(b);

    if (fn->var[1]) //la fonction attend plusieurs arguments
    {
        if (b.type != UPLET)
            return ErreurExt("Impossible d'evaluer, manque d'arguments", buf);

        upl = UpletObj(b);

        if (strlen(fn->var) != TailleUplet(b))
            return ErreurExt("Impossible d'evaluer, nombre d'arguments incorrect", buf);

        for (i=0 ; i<=MAX_VAR && fn->var[i] ; i++)
        {
            tab[i] = CopierObjet(upl[i]);
            if (IsPermanent(tab[i]))
                strcpy(tab[i].description, tab[i].nom);
            tab[i].nom[0] = fn->var[i];
            tab[i].nom[1] = '\0';
            tab[i].special = LOCAL;
        }
        tab[i].type = VIDE;
    }
    else
    {
        tab[0] = CopierObjet(b);
        if (IsPermanent(tab[0]))
            strcpy(tab[0].description, tab[0].nom);
        tab[0].nom[0] = fn->var[0];
        tab[0].nom[1] = '\0';
        tab[0].special = LOCAL;
        tab[1].type = VIDE;
    }

    r = Calculer(fn->expr, tab);
    for (i=0 ; i<MAX_VAR && tab[i].type != VIDE ; i++)
        LibererObjet(&(tab[i]));

    return r;
}

Objet EvalMat(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != MATRICE)
        return ErreurInt("Type inattendu");

    Objet *upl, **mat;
    char buf[MAX_EXPR];
    int c,l;

    snprintf(buf, MAX_EXPR-1, "(%s):(%s)", Descr(a), Descr(b));

    if (b.type != UPLET || TailleUplet(b) != 2)
        return ErreurExt("Format incorrect pour l'extraction d'elements", buf);

    upl = UpletObj(b);
    mat = MatriceObj(a);

    if (   upl[0].type != COMPLEXE || upl[1].type != COMPLEXE
        || !EstNul(Im(upl[0])) || !EstEntier(Re(upl[0])) || (l = round(Re(upl[0]))) < 0 || l >= (int)LignesMat(a)
        || !EstNul(Im(upl[1])) || !EstEntier(Re(upl[1])) || (c = round(Re(upl[1]))) < 0 || c >= (int)ColMat(a)     )
        return ErreurExt("Extraction d'elements : hors limites", buf);
    else return CopierObjet(mat[l][c]);
}

Objet EvalUplet(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != UPLET)
        return ErreurInt("Type inattendu");

    Objet *upl;
    char buf[MAX_EXPR];
    int i;

    snprintf(buf, MAX_EXPR-1, "(%s):(%s)", Descr(a), Descr(b));

    if (b.type != COMPLEXE)
        return ErreurExt("Format incorrect pour l'extraction d'elements", buf);
    if (!EstNul(Im(b)) || !EstEntier(Re(b)) || (i = round(Re(b))) < 0 || i >= (int)TailleUplet(a))
        return ErreurExt("Extraction d'elements : hors limites", buf);

    upl = UpletObj(a);
    return CopierObjet(upl[i]);
}

Objet Fonc(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != TEXTE || b.type != TEXTE)
        return ErreurInt("Type inattendu");

    char buffer[MAX_EXPR];

    snprintf(buffer, MAX_EXPR-1, "%s->%s", TexteObj(a), TexteObj(b));
    return RecupererFonction(buffer, tabLocVar_op);
}

//Créé un objet de type texte avec le texte de l'objet b et comme nom le texte de l'objet a, et l'enregistre dans la liste générale des objets
Objet NewVar(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != TEXTE || b.type != TEXTE)
        return ErreurInt("Type inattendu");

    char buf[MAX_EXPR],
         *nom = TexteObj(a);
    int r = VerifNomVar(nom);
    Objet obj;

    snprintf(buf, MAX_EXPR-1, "%s=%s", Descr(a), Descr(b));

    if (!r)
        return ErreurExt("Nom de variable trop long", buf);
    else if (r < 0)
        return ErreurExt("Nom de variable incorrect", buf);

    obj = CreerObjetTexte(TexteObj(a), TexteObj(b));

    if ((r = EnregistrerObjet(obj)) <= 0)
        LibererObjet(&obj);
    if (!r)
        return ErreurExt("Plus de place pour enregistrer l'objet", buf);
    else if (r == -1)
        return ErreurExt("Le nom specifie est celui d'un objet systeme", Descr(a));
    else if (r < 0)
        return ErreurExt("Erreur de recursivite", buf);

    if (obj.data && TexteEstim(obj).type == ERREUR)
    {
        LibererObjet(&TexteEstim(obj));
        free(obj.data);
        obj.data = NULL;
        obj.size = 0;
        return obj;
    }
    else if (obj.data)
    {
        Objet obj2 = CopierObjet(TexteEstim(obj));
        LibererObjet(&obj);
        return obj2;
    }
    else return obj;
}

//Créé un nouvel objet, estimation explicitée de l'objet texte b, avec comme nom l'objet texte a, et l'enregistre dans la liste générale des objets
Objet NewCst(Objet a, Objet b)
{
    if (a.type == ERREUR)
        return CopierObjet(a);
    else if (b.type == ERREUR)
        return CopierObjet(b);
    else if (a.type != TEXTE || b.type != TEXTE)
        return ErreurInt("Type inattendu");

    char buf[MAX_EXPR], descr[MAX_EXPR],
         *nom = TexteObj(a);
    int r = VerifNomVar(nom), i;
    Objet obj, obj2,
          *vars = RecupTabObj(),
          *locVar;

    snprintf(buf, MAX_EXPR-1, "%s:=%s", Descr(a), Descr(b));

    if (!r)
        return ErreurExt("Nom de variable trop long", Descr(a));
    else if (r < 0)
        return ErreurExt("Nom de variable incorrect", Descr(a));

    if (!(locVar = malloc(sizeof(Objet)*(MAX_OBJETS))))
        return ErreurInt("Erreur d'allocation");

    for (i=0 ; vars && i < MAX_OBJETS && vars[i].type != VIDE ; i++)
    {
        locVar[i] = CopierObjet(vars[i]);
        if (!IsUserVar(locVar[i]))
            strcpy(locVar[i].description, locVar[i].nom);
    }
    if (i < MAX_OBJETS)
        locVar[i].type = VIDE;

    obj = Calculer(TexteObj(b), locVar);
    if (obj.type != ERREUR)
        SprintObjetStr(descr, obj);
    else strncpy(descr, obj.description, MAX_EXPR-1);
    obj2 = CreerObjet(obj.type, TexteObj(a), descr, obj.data, obj.size);

    if (obj.data)
        free(obj.data);
    if (obj.description)
        free(obj.description);
    for (i=0 ; i < MAX_OBJETS && locVar[i].type != VIDE ; i++)
        LibererObjet(&(locVar[i]));
    free(locVar);

    if ((r = EnregistrerObjet(obj2)) <= 0)
        LibererObjet(&obj2);
    if (!r)
        return ErreurExt("Plus de place pour enregistrer l'objet", buf);
    else if (r == -1)
        return ErreurExt("Le nom specifie est celui d'un objet systeme", Descr(a));
    else if (r < 0)
        return ErreurExt("Erreur de recursivite", buf);

    return obj2;
}

//Vérifie la validité d'un nom en tant que nom de variable
int VerifNomVar(const char nom[])
{
    int i;

    if (strlen(nom) >= MAX_NOM)
        return 0;
    if (EstCommande(nom))
        return -2;
    for (i=0 ; nom[i] ; i++)
    {
        if (EstNombre(nom[0]) || (!EstLettre(nom[i]) && !EstNombre(nom[i])))
            return -1;
    }

    return 1;
}


//Trouve dans une chaine de calcul l'opérateur principal en fonction des positions et des priorités de tous ceux présents et retourne sa position, éventuellement la structure entière.
int TrouverOperateur(const char expr[], Operation *op)
{
    int priorite = 10, i, j, n;
    const char *p = NULL, *p2 = expr-1;

    if (StrchrEx(expr, ',')) //pour éviter les problèmes avec les n-uplets non parenthésés : -1,2,3 = (-1,2,3) != -(1,2,3)
        return -1;

    for (i=0 ; tabOperations[i].type ; i++)
    {
        n = strlen(tabOperations[i].operateur);
        p2 = expr-1;
        while ( (p2 = StrstrEx(p2+1, tabOperations[i].operateur)) )
        {
            if (tabOperations[i].priorite < priorite || (tabOperations[i].priorite == priorite && p2>p))
            {
                j = p2-expr;
                if (   (EstOperateurUG(tabOperations[i]) && !j)
                    || (EstOperateurUD(tabOperations[i]) && !expr[j+n])
                    || (EstOperateurBinaire(tabOperations[i]) && j>0 && expr[j+n] && (EstDansOperateurUD(expr, j-1) || !EstDansOperateur(expr, j-1)) && (EstDansOperateurUG(expr, j+n) || !EstDansOperateur(expr, j+n))) )
                {
                    /*if (expr[j+1] != '>' || expr[j] != '-')
                    {*/
                        priorite = tabOperations[i].priorite;
                        p = p2;
                        if (op)
                            *op = tabOperations[i];
                    /*}*/
                }
            }
        }
    }

    if (!p)
        return -1;
    else return (int)(p-expr);
}

//Dit si un emplacement d'une chaine donnée est situé dans un opérateur ou non
//Renvoie une combinaison | des types si oui, 0 si non
int EstDansOperateur(const char expr[], int p)
{
    int i, j, k, n, type = 0;
    for (i=0 ; tabOperations[i].type ; i++)
    {
        n = strlen(tabOperations[i].operateur);
        for (j=0 ; j<n && p-j>=0 ; j++)
        {
            for (k=0 ; expr[p-j+k] && tabOperations[i].operateur[k] && expr[p-j+k] == tabOperations[i].operateur[k] ; k++);
            if (!tabOperations[i].operateur[k])
                type |= tabOperations[i].type;
        }
    }

    return type;
}

//Identifie une opération à partir de son opérateur et de son type (facultatif, type peut être 0). L'opérateur peut être en tête d'une chaine de caractères quelconque, ex "+23".
Operation IdentifierOperation(const char op[], int type)
{
    int i, j;

    for (i=0 ; tabOperations[i].type ; i++)
    {
        if (tabOperations[i].type == type || !type)
        {
            for (j=0 ; tabOperations[i].operateur[j] && op[j] && op[j] == tabOperations[i].operateur[j] ; j++);
            if (!tabOperations[i].operateur[j])
                return tabOperations[i];
        }
    }

    return tabOperations[i];
}

//Remplace dans expr toutes les occurences de la variable avar par nvar
//Exemple : RemplacerVar("x1*cos(x12)", "x1", "y1") => "y1*cos(x12)"  (et non pas "y1*cos(y12)" pour un remplacement 'brut')
int RemplacerVar(char expr[], const char avar[], const char nvar[])
{
    if (!expr || !avar || !avar[0] || (nvar && !strcmp(avar, nvar)))
        return 0;

    char *p = expr,
         expr1[MAX_EXPR],
         expr2[MAX_EXPR];
    int i, j, l1 = strlen(avar), l2 = strlen(nvar);

    while ( (p = strstr(p, avar)) )
    {
        i = p-expr;
        j = i+strlen(avar)-1;

        if ((i<=0 || (!EstLettre(expr[i-1]) && !EstNombre(expr[i-1]))) && !EstLettre(expr[j+1]) && !EstNombre(expr[j+1]))
        {
            strcpy(expr1, expr);
            expr1[i] = '\0';
            strcpy(expr2, expr+j+1);

            if (nvar)
                snprintf(expr, MAX_EXPR-1, "%s%s%s", expr1, nvar, expr2);
            else snprintf(expr, MAX_EXPR-1, "%s%s", expr1, expr2);

            p += l2;
        }
        else p += l1;
    }

    return 1;
}

//Remplace toutes les variables de l'expression dont le nom figure dans le tableau locVar par leur description, en épargnant les variables locales définies par des expressions de fonction
int RemplacerVarsGlob(char expr[], Objet locVar[])
{
    int i, j, k, replace, r=0;
    char *p = expr,
         *p2, *p3, *p4,
         buf[MAX_EXPR];

    for (i=0 ; locVar && locVar[i].type != VIDE ; i++)
    {
        if (!locVar[i].nom[0] || locVar[i].nom[1])
            RemplacerVar(expr, locVar[i].nom, Descr(locVar[i]));
        else
        {
            p = expr;
            while ( (p = strstr(p, locVar[i].nom)) )
            {
                j = p-expr;
                if ((j<=0 || (!EstLettre(expr[j-1]) && !EstNombre(expr[j-1]))) && !EstLettre(expr[j+1]) && !EstNombre(expr[j+1]))
                {
                    replace = 1;

                    *p = '\0';
                    p3 = NULL;
                    p4 = StrchrRevEx(expr, ',');
                    while ( (p2 = StrstrRevEx(expr, "->")) && p2>p4 )
                    {
                        if (p3)
                            *p3 = '-';

                        j = p2-expr;
                        if (j > 0 && expr[j-1] != ')')
                        {
                            if (expr[j-1] == locVar[i].nom[0])
                                replace = 0;
                        }
                        else
                        {
                            for (; j >= 0 && expr[j] != '(' && expr[j] != locVar[i].nom[0] ; j--);
                            if (j >= 0 && expr[j] == locVar[i].nom[0])
                                replace = 0;
                        }

                        p3 = p2;
                        *p2 = '\0';
                    }
                    if (p3)
                        *p3 = '-';

                    if (replace)
                    {
                        k = p+1-expr;
                        if (expr[k] == '-' && expr[k+1] == '>')
                            replace = 0;
                        else if ( (p2 = StrchrEx(p+1, ')')) )
                        {
                            for (k = p2-expr ; expr[k] == ')' ; k++);
                            if (expr[k] == '-' && expr[k+1] == '>')
                                replace = 0;
                        }
                    }

                    if (replace)
                    {
                        strncpy(buf, p+1, MAX_EXPR-1);
                        strcat(expr, Descr(locVar[i]));
                        strcat(expr, buf);
                        r = 1;
                    }
                    else *p = locVar[i].nom[0];
                }

                p++;
            }
        }
    }

    return r;
}

//Renvoie une copie épurée du tableau locVar, sans les variables de nom un caractère de la chaîne vars
//la copie doit être libérée avec free(), les objets eux-mêmes ne sont PAS copiés (donc pas de libération avec LibererObjet() à faire !)
//la copie possède systématiquement une case de plus, une variable de type VIDE
Objet* EliminerLocVars(Objet locVar[], const char vars[])
{
    int i, j, ok, k=0;
    Objet *locVar2;

    for (i=0 ; locVar && locVar[i].type != VIDE ; i++);
    if (!(locVar2 = malloc(sizeof(Objet)*(i+1))))
        return NULL;

    for (j=0 ; locVar && locVar[j].type != VIDE ; j++)
    {
        ok = 1;
        for (i=0 ; vars[i] ; i++)
        {
            if (!locVar[j].nom[1] && locVar[j].nom[0] == vars[i])
                ok = 0;
        }
        if (ok)
        {
            locVar2[k] = locVar[j];
            k++;
        }
    }
    locVar2[k].type = VIDE;

    return locVar2;
}

//Trouve une autre opération ayant même opérateur, même priorité et même type (mais fonction différente) que celle donnée et l'enregistre à la même adresse
int AutreOperation(Operation *op)
{
    int i;

    if (!op)
        return 0;

    for (i=0 ; tabOperations[i].type && !OperationsIdentiques(*op, tabOperations[i]) ; i++);
    i++;
    for (; tabOperations[i].type && (tabOperations[i].type != op->type || strcmp(tabOperations[i].operateur,op->operateur) || tabOperations[i].priorite != op->priorite) ; i++);

    if (!tabOperations[i].type)
        return 0;
    else *op = tabOperations[i];
    return 1;
}

void AffecterTabLocVar(Objet locVar[])
{
    tabLocVar_op = locVar;
}

Operation* RecupTabOp(void)
{
    return tabOperations;
}

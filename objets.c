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


#include "objets.h"


Objet tabObjets[MAX_OBJETS];
Objet *tabLocVar;
Objet *Ans;

//Initialise le tableau des objets
int InitialiserObjets(void)
{
    int i;

    for (i=0 ; i<MAX_OBJETS ; i++)
        tabObjets[i].type = VIDE;


    tabObjets[0] =  CreerFonction(Abs, "abs:(x)", "x", "abs", "Valeur absolue [Reels]");
    tabObjets[1] =  CreerFonction(Ach, "acosh:(x)", "x", "acosh", "Argcosinus hyperbolique [1;+I[");
    tabObjets[2] =  CreerFonction(Acos, "acos:(x)", "x", "acos", "Arccosinus [-1;1]");
    tabObjets[3] =  CreerFonction(ArgCp, "arg:(z)", "z", "arg", "Argument [Complexes]");
    tabObjets[4] =  CreerFonction(Asin, "asin:(x)", "x", "asin", "Arcsinus [-1;1]");
    tabObjets[5] =  CreerFonction(Ash, "asinh:(x)", "x", "asinh", "Argsinus hyperbolique [Reels]");
    tabObjets[6] =  CreerFonction(Atan, "atan:(x)", "x", "atan", "Arctangente [Reels]");
    tabObjets[7] =  CreerFonction(Ath, "atanh:(x)", "x", "atanh", "Argtangente hyperbolique ]-1;1[");
    tabObjets[8] =  CreerFonction(Ceil, "ceil:(x)", "x", "ceil", "Entier directement superieur [Reels]");
    tabObjets[9] =  CreerFonction(Cos, "cos:(x)", "x", "cos", "Cosinus [Reels]");
    tabObjets[10] = CreerFonction(Cosh, "cosh:(x)", "x", "cosh", "Cosinus hyperbolique [Reels]");
    tabObjets[11] = CreerFonction(Det, "det:(m)", "m", "det", "Determinant [Matrices carrees]");
    tabObjets[12] = CreerFonction(Drv, "drv:(f,x)", "fx", "drv", "Derivee de la fonction f estimee en x [Fonction x Complexe]");
    tabObjets[13] = CreerFonction(EigenDec, "eig:(x)", "x", "eig", "Decomposition en Valeurs/Vecteurs Propres [Matrices]");
    tabObjets[14] = CreerFonction(Exp, "exp:(z)", "z", "exp", "Fonction exponentielle [Complexes]");
    tabObjets[15] = CreerFonction(Frac, "frac:(x)", "x", "frac", "Partie decimale [Reels]");
    tabObjets[16] = CreerFonction(ImPart, "im:(z)", "z", "im", "Partie imaginaire [Complexes]");
    tabObjets[17] = CreerFonction(Trunc, "int:(x)", "x", "int", "Troncature entiere [Reels]");
    tabObjets[18] = CreerFonction(Integ, "integ:(f,a,b)", "fab", "integ", "Integrale de la fonction f entre a et b [Fonction x Reel x Reel]");
    tabObjets[19] = CreerFonction(Floor, "intg:(x)", "x", "intg", "Partie entiere [Reels]");
    tabObjets[20] = CreerFonction(Ln, "ln:(x)", "x", "ln", "Logarithme neperien ]0;+I[");
    tabObjets[21] = CreerFonction(ModCp, "mod:(z)", "z", "mod", "Module [Complexes]");
    tabObjets[22] = CreerFonction(Norme, "norm:(x)", "x", "norm", "Norme [Uplets de reels]");
    tabObjets[23] = CreerFonction(RePart, "re:(z)", "z", "re", "Partie reelle [Complexes]");
    tabObjets[24] = CreerFonction(Round, "rnd:(x)", "x", "rnd", "Arrondi a l'unite [Reels]");
    tabObjets[25] = CreerFonction(Si, "si:(b,v,f)", "bvf", "si", "Condition : si b alors v sinon f [Booleen x Objet x Objet]");
    tabObjets[26] = CreerFonction(Sin, "sin:(x)", "x", "sin", "Sinus [Reels]");
    tabObjets[27] = CreerFonction(Sinh, "sinh:(x)", "x", "sinh", "Sinus hyperbolique [Reels]");
    tabObjets[28] = CreerFonction(Svd, "svd:(x)", "x", "svd", "Decomposition en Valeurs Singulieres [Matrices]");
    tabObjets[29] = CreerFonction(Sqrt, "sqrt:(x)", "x", "sqrt", "Racine carree [0;+I[");
    tabObjets[30] = CreerFonction(Tan, "tan:(x)", "x", "tan", "Tangente [R \\ (Pi/2 + Pi.Z)]");
    tabObjets[31] = CreerFonction(Tanh, "tanh:(x)", "x", "tanh", "Tangente hyperbolique [Reels]");
    tabObjets[32] = CreerFonction(Trace, "tr:(x)", "x", "tr", "Trace [Matrices]");
    tabObjets[33] = CreerFonction(Transposee, "trsp:(x)", "x", "trsp", "Transposee [Matrices]");
    tabObjets[34] = CreerFonction(Zero, "zero:(f,a,b)", "fab", "zero", "Premier zero de la fonction f dans l'intervalle [a;b] [Fonction x Reel x Reel]");

    i=35;
    tabObjets[i] = CreerComplexe(0, 1, "i", "Complexe (Re=0 ; Im=1)");
    tabObjets[i+1] = CreerComplexe(2.71828182845904523536028747135266249776, 0, "e", "Valeur prise par l'exponentielle en 1");
    tabObjets[i+2] = CreerComplexe(PI, 0, "pi", "Nombre pi");
    tabObjets[i+3] = CreerBooleen(VRAI, "VRAI", "Booleen vrai");
    tabObjets[i+4] = CreerBooleen(FAUX, "FAUX", "Booleen faux");
    tabObjets[i+5] = CreerComplexe(0, 0, "ans", "Resultat precedent");
    Ans = &(tabObjets[i+5]);

    for (i=0 ; i<MAX_OBJETS ; i++)
        tabObjets[i].special = PERMANENT;

    return 1;
}

//Libère tous les objets du tableau d'objets
void Desinitialiser(void)
{
    int i;
    for (i=0 ; i<MAX_OBJETS && tabObjets[i].type != VIDE ; i++)
        LibererObjet(&(tabObjets[i]));
    LibererObjet(Ans);
    Ans = NULL;
}

void AffecterAns(Objet obj)
{
    LibererObjet(Ans);
    *Ans = CopierObjet(obj);
    strcpy(Ans->nom, "ans");
    if (!Ans->description)
        Ans->description = malloc(sizeof(char)*MAX_EXPR);
    strncpy(Ans->description, "Resultat precedent", MAX_EXPR-1);
}

Objet RetournerAns(void)
{
    return *Ans;
}

Objet* RecupTabObj(void)
{
    return tabObjets;
}

//Permet d'enregistrer un nouvel objet dans le tableau des objets, remet également à jour toutes les variables du tableau
int EnregistrerObjet(Objet obj)
{
    int i, done=0;
    Objet oldObj;

    for (i=0 ; !done && i<MAX_OBJETS && tabObjets[i].type != VIDE ; i++)
    {
        if (!strcmp(tabObjets[i].nom, obj.nom))
        {
            if (!IsUserVar(tabObjets[i]))
                return -1;
            else done = 1;
        }
    }

    if (!done)
    {
        for (i=0 ; i<MAX_OBJETS && tabObjets[i].type != VIDE ; i++);
        if (i >= MAX_OBJETS)
            return 0;
    }
    else i--;

    oldObj = tabObjets[i];
    tabObjets[i] = CopierObjet(obj);
    tabObjets[i].special = PERMANENT | USERVAR;

    if (MAJTabObjets())
    {
        LibererObjet(&oldObj);
        return 1;
    }
    else
    {
        LibererObjet(&(tabObjets[i]));
        tabObjets[i] = oldObj;
        MAJTabObjets();
        return -2;
    }
}

//Permet d'effacer un objet du tableau des objets à partir de son nom, remet également à jour toutes les variables du tableau
int OublierObjet(const char nom[])
{
    int i;
    for (i=0 ; i<MAX_OBJETS && tabObjets[i].type != VIDE && strcmp(tabObjets[i].nom, nom) ; i++);
    if (i >= MAX_OBJETS || tabObjets[i].type == VIDE)
        return 0;

    LibererObjet(&(tabObjets[i]));
    for (; i<MAX_OBJETS-1 && tabObjets[i+1].type != VIDE ; i++)
        tabObjets[i] = tabObjets[i+1];

    MAJTabObjets();
    return 1;
}

//Remet à jour toutes les variables du tableau
int MAJTabObjets(void)
{
    int i, startAgain = 1, count;
    Objet obj, oldObj, objBool;

    for (count=0 ; count < 50 && startAgain ; count++)
    {
        startAgain = 0;
        for (i=0 ; i<MAX_OBJETS && tabObjets[i].type != VIDE ; i++)
        {
            if (tabObjets[i].type == TEXTE)
            {
                oldObj = tabObjets[i];
                tabObjets[i] = ErreurExt("Variable inconnue dans l'expression", tabObjets[i].nom);
                strcpy(tabObjets[i].nom, oldObj.nom);
                obj = Calculer(TexteObj(oldObj), NULL);
                LibererObjet(&(tabObjets[i]));
                tabObjets[i] = oldObj;

                if (!tabObjets[i].data)
                    tabObjets[i].data = malloc(sizeof(Objet));
                else
                {
                    objBool = Egal2(TexteEstim(tabObjets[i]), obj, 1);
                    if (objBool.type != BOOLEEN || !ValBool(objBool))
                        startAgain = 1;
                    LibererObjet(&objBool);
                    LibererObjet((Objet*)tabObjets[i].data);
                }

                if (tabObjets[i].data)
                    memcpy(tabObjets[i].data, &obj, sizeof(Objet));
                else LibererObjet(&obj);
                tabObjets[i].type = TEXTE;
            }
        }
    }

    return !startAgain;
}

/*
    Créée un objet avec des données spécifiques
    L'objet devra être libéré avec la fonction LibererObjet
    data et descr, si non NULL, doivent pointer sur de la mémoire flottante (et non déjà allouée),
    la fonction s'occupant d'allouer manuellement de la mémoire pour le stockage
    En revanche les éventuels pointeurs contenus dans data doivent pointer sur de la mémoire allouée !
*/
Objet CreerObjet(int type, const char nom[], const char descr[], void *data, size_t size)
{
    Objet obj;
    memset(&obj, 0, sizeof(Objet));
    obj.type = type;
    obj.special = NORMAL;
    if (nom)
        strncpy(obj.nom, nom, MAX_NOM-1);
    else obj.nom[0] = '\0';
    if (descr && (obj.description = malloc(sizeof(char)*MAX_EXPR)) )
        strncpy(obj.description, descr, MAX_EXPR-1);
    if (data && size>0)
    {
        obj.data = malloc(size);
        if (!obj.data)
        {
            if (obj.description)
                free(obj.description);
            return ErreurInt("Erreur d'allocation");
        }
        memcpy(obj.data, data, size);
        obj.size = size;
    }

    return obj;
}

//Donne la taille des données d'un objet en fonction de son type
size_t TailleDataObjet(Objet obj)
{
    switch (obj.type)
    {
        case COMPLEXE:
            return sizeof(Complexe);
        case FONCTION:
            return sizeof(Fonction);
        case SUITE:
            return sizeof(Suite);
        case MATRICE:
            return sizeof(Matrice);
        case UPLET:
            return sizeof(Uplet);
        case BOOLEEN:
            return sizeof(Booleen);
        case TEXTE:
        case VIDE:
            return 0;
        default:
            break;
    }

    return -1; //inconnu !
}


//Libère entièrement un objet précédemment créé
//Récursive
void LibererObjet(Objet* a)
{
    if (!a)
        return;

    int i,j;

    if (a->type == MATRICE)
    {
        int l = LignesMat(*a), c = ColMat(*a);
        Objet **mat = MatriceObj(*a);

        if (mat)
        {
            for(i=0 ; i<l ; i++)
            {
                if (mat[i])
                {
                    for (j=0 ; j<c ; j++)
                        LibererObjet(&(mat[i][j]));
                    free(mat[i]);
                }
            }
            free(mat);
        }
    }
    else if (a->type == UPLET)
    {
        int n = TailleUplet(*a);
        Objet *upl = UpletObj(*a);

        if (upl)
        {
            for (i=0 ; i<n ; i++)
                LibererObjet(&(upl[i]));
            free(upl);
        }
    }
    else if (a->type == SUITE)
    {
        Objet terme = TermeSuite(*a);
        LibererObjet(&terme);
    }
    else if (a->type == TEXTE && a->data)
    {
        Objet estim = TexteEstim(*a);
        LibererObjet(&estim);
    }

    if (a->type != VIDE && a->size > 0 && a->data)
        free(a->data);
    if (a->type != VIDE && a->description)
        free(a->description);
    a->type = VIDE;
    a->data = NULL;
    a->size = 0;
}

//Copie intégralement un objet : toutes les zones de mémoires sont copiées en entier, pas seulement les pointeurs
//Récursive
Objet CopierObjet(Objet a)
{
    Objet b = a;

    if (a.size > 0 && a.data)
    {
        b.data = malloc(a.size);
        if (!b.data)
            return ErreurInt("Erreur d'allocation");
        memcpy(b.data, a.data, a.size);
    }

    if (a.description && (b.description = malloc(sizeof(char)*MAX_EXPR)) )
        strncpy(b.description, a.description, MAX_EXPR-1);

    if (a.type == TEXTE)
    {
        if (b.data)
        {
            Objet obj = CopierObjet(TexteEstim(a));
            memcpy(b.data, &obj, sizeof(Objet));
        }
    }
    else if (a.type == MATRICE)
    {
        int i, j,
            c = ColMat(a),
            l = LignesMat(a);
        Objet **mat1 = MatriceObj(a);
        Matrice mat2 = CreerMatrice(l, c);

        if (!mat2.matrice)
            return ErreurInt("Erreur d'allocation");

        for (i=0 ; i<l ; i++)
        {
            for (j=0 ; j<c ; j++)
                mat2.matrice[i][j] = CopierObjet(mat1[i][j]);
        }

        if (b.data)
            memcpy(b.data, &mat2, sizeof(Matrice));
    }
    else if (a.type == UPLET)
    {
        int i, n = TailleUplet(a);
        Objet *upl1 = UpletObj(a);
        Uplet upl2 = CreerUplet(n);

        if (!upl2.uplet)
            return ErreurInt("Erreur d'allocation");

        for (i=0 ; i<n ; i++)
            upl2.uplet[i] = CopierObjet(upl1[i]);

        if (b.data)
            memcpy(b.data, &upl2, sizeof(Uplet));
    }
    else if (a.type == SUITE && b.data)
        TermeSuite(b) = CopierObjet(TermeSuite(a));

    return b;
}


//Créée un objet de type fonction, il devra être libéré avec LibererObjet
Objet CreerFonction(Objet (*fonction)(Objet), const char expr[], const char var[], const char nom[], const char descr[])
{
    Fonction fn;
    fn.fonction = fonction;
    if (expr)
        strncpy(fn.expr, expr, MAX_EXPR-1);
    else fn.expr[0] = '\0';
    if (var)
        strncpy(fn.var, var, MAX_VAR);
    else fn.var[0] = '\0';

    return CreerObjet(FONCTION, nom, descr, (void*)&fn, sizeof(Fonction));
}

//Créée un objet de type complexe, il devra être libéré avec LibererObjet
Objet CreerComplexe(double re, double im, const char nom[], const char descr[])
{
    Complexe cp;
    cp.im = im;
    cp.re = re;

    return CreerObjet(COMPLEXE, nom, descr, (void*)&cp, sizeof(Complexe));
}

//Créée un objet de type complexe à partir d'un module et d'un argument, il devra être libéré avec LibererObjet
Objet CreerComplexe2(double mod, double arg, const char nom[], const char descr[])
{
    Complexe cp;
    cp.re = mod*cos(arg);
    cp.im = mod*sin(arg);

    return CreerObjet(COMPLEXE, nom, descr, (void*)&cp, sizeof(Complexe));
}

double Mod(Objet x)
{
    //retourne le module d'un nombre complexe
    return sqrt(Re(x)*Re(x)+Im(x)*Im(x));
}

double Arg(Objet x)
{
    //retourne l'argument entre -PI et PI

    if (EstNul(Re(x)))
    {
        if (Im(x)>0)
            return PI/2;
        else return -PI/2;
    }

    double arg=atan(Im(x)/Re(x));
    if (Re(x)<0 && Im(x)>=0)
        return arg+PI;
    else if (Re(x)<0 && Im(x)<=0)
        return arg-PI;
    else return arg;
}

//Créée un objet de type booléen, il devra être libéré avec LibererObjet
Objet CreerBooleen(int val, const char nom[], const char descr[])
{
    Booleen bol;
    bol.val = val;

    return CreerObjet(BOOLEEN, nom, descr, (void*)&bol, sizeof(Booleen));
}

//Créée un objet de type suite, il devra être libéré avec LibererObjet
Objet CreerSuite(Objet (*suite)(int), Objet terme1, int indice1, const char exprRec[], char var, const char nom[], const char descr[])
{
    Suite su;
    su.suite = suite;
    su.terme1 = CopierObjet(terme1);
    su.indice1 = indice1;
    if (exprRec)
        strncpy(su.exprRec, exprRec, MAX_EXPR-1);
    else su.exprRec[0] = '\0';
    su.var = var;

    return CreerObjet(SUITE, nom, descr, (void*)&su, sizeof(Suite));
}

//Alloue une matrice vide des dimensions spécifiées
Matrice CreerMatrice(unsigned int l, unsigned int c)
{
    Matrice mat;
    unsigned int i, j;

    mat.matrice = malloc(sizeof(Objet*)*l);
    if (!mat.matrice)
        return mat;

    for (i=0 ; i<l ; i++)
    {
        mat.matrice[i] = malloc(sizeof(Objet)*c);
        if (!mat.matrice[i])
        {
            for (j=0 ; j<i ; j++)
                free(mat.matrice[i]);
            free(mat.matrice);
            mat.matrice = NULL;
            return mat;
        }
        else
        {
            for (j=0 ; j<c ; j++)
            {
                mat.matrice[i][j].type = VIDE;
                mat.matrice[i][j].data = NULL;
                mat.matrice[i][j].size = 0 ;
                mat.matrice[i][j].description = NULL;
            }
        }
    }

    mat.col = c;
    mat.lignes = l;

    return mat;
}

//Alloue un uplet vide de la taille spécifiée
Uplet CreerUplet(unsigned int n)
{
    Uplet upl;
    unsigned int i;

    upl.uplet = malloc(sizeof(Objet)*n);
    if (!upl.uplet)
        return upl;

    for (i=0 ; i<n ; i++)
    {
        upl.uplet[i].type = VIDE;
        upl.uplet[i].data = NULL;
        upl.uplet[i].size = 0;
        upl.uplet[i].description = NULL;
    }

    upl.taille = n;
    return upl;
}

void LibererMatrice(Matrice mat)
{
    Objet obj = CreerObjet(MATRICE, NULL, NULL, (void*)&mat, sizeof(Matrice));
    LibererObjet(&obj);
}

void LibererUplet(Uplet upl)
{
    Objet obj = CreerObjet(UPLET, NULL, NULL, (void*)&upl, sizeof(Uplet));
    LibererObjet(&obj);
}

Objet CreerObjetTexte(const char nom[], const char texte[])
{
    Objet obj = Calculer(texte, NULL);
    return CreerObjet(TEXTE, nom, texte, &obj, sizeof(Objet));
}


//Donne le numéro d'un objet à partir de son nom
int NumeroObjet(const char nom[])
{
    int i;

    for (i=0 ; i<MAX_OBJETS && tabObjets[i].type != VIDE ; i++)
    {
        if (!strcmp(tabObjets[i].nom, nom))
            return i;
    }

    return -1;
}

//Donne le numéro d'une variable locale à partir de son nom
int NumeroLocVar(const char nom[])
{
    int i;

    for (i=0 ; tabLocVar && tabLocVar[i].type != VIDE ; i++)
    {
        if (!strcmp(tabLocVar[i].nom, nom))
            return i;
    }

    return -1;
}


//Identifie un objet à partir d'une expression (l'objet est une copie et doit donc être libéré avec LibererObjet())
//pour la signification de locVar, se reporter à la fonction Calculer()
Objet IdentifierObjet(const char expr[], Objet locVar[])
{
    int n;
    double d;
    char *p;
    Objet r, result, *oldTab = tabLocVar;

    result.type = VIDE;
    tabLocVar = locVar;

    if ( (n = NumeroLocVar(expr)) >=0)
        r = CopierObjet(tabLocVar[n]);
    else if ( (n = NumeroObjet(expr)) >=0)  //Objet pré-enregistré
        r = CopierObjet(tabObjets[n]);

    if (n >= 0 && r.type == TEXTE && r.data && TexteEstim(r).type != ERREUR)
    {
        Objet r2 = CopierObjet(TexteEstim(r));
        LibererObjet(&r);
        result = r2;
    }
    else if (n >= 0)
        result = r;

    else if (StrchrEx(expr, ','))  //N-uplet
        result = RecupererUplet(expr);

    else if (expr[0] == '[')  //Matrice
        result = RecupererMatrice(expr);

    else
    {
        d = strtod(expr, &p);
        if (!p || !(*p))  //Complexe
            result = CreerComplexe(d, 0, NULL, expr);
    }

    tabLocVar = oldTab;  //Pour éviter que d'éventuels calculs intermédiaires ne modifient définitivement le tableau,
                         //il revient systématiquement dans son état d'origine après calcul

    if (result.type == VIDE)
        return ErreurExt("Objet inconnu / Erreur de syntaxe", expr);
    else return result;
}

//Transforme une expr du type "(a,b,c)->f(a,b,c)" (où f() est une fonction quelconque explicite ou non) en un objet de type fonction
//Procède également à un remplacement, dans l'expression, du nom des variables du tableau locVar par leur description, en épargnant toutefois les variables redéfinies localement par une expression de fonction
Objet RecupererFonction(const char expr0[], Objet locVar[])
{
    char *p, **tab, buf[MAX_VAR+1], expr[MAX_EXPR];
    unsigned int n, l, i;
    int ok=1;
    Objet *locVar2;

    strncpy(expr, expr0, MAX_EXPR-1);

    if ( !(p = StrstrEx(expr, "->")) )
        return ErreurExt("L'objet n'est pas une fonction correcte", expr);

    *p = '\0';
    if ( !(tab = ExtraireParamAuto(expr, &n, &l)) )
        return ErreurInt("Erreur d'allocation");
    for (i=0 ; i<n ; i++)
    {
        if (!EstLettre(tab[i][0]))
            ok = 0;
    }
    if (!ok || n>MAX_VAR || l>2)
    {
        LibererTab(tab, n);
        return ErreurExt("Variable(s) incorrecte(s) specifiee(s)", expr);
    }

    for (i=0 ; i<n ; i++)
        buf[i] = tab[i][0];
    buf[i] = '\0';

    *p = '-';
    if ( locVar && (locVar2 = EliminerLocVars(locVar, buf)) )
    {
        RemplacerVarsGlob(expr, locVar2);
        free(locVar2);
    }

    LibererTab(tab, n);
    return CreerFonction(NULL, p+2, buf, NULL, expr);
}

//Transforme une expr du type "(a,cb,er,df)" ou "a,cb,er,df" en un objet de type n-uplet
Objet RecupererUplet(const char expr[])
{
    char **tab;
    unsigned int n, i;
    Uplet upl;
    Objet obj;

    tab = ExtraireParamAuto(expr, &n, NULL);
    if (n>MAX_UPLET)
        n = MAX_UPLET;
    upl = CreerUplet(n);

    for(i=0 ; i<n ; i++)
    {
        upl.uplet[i] = Calculer(tab[i], tabLocVar);
        if (upl.uplet[i].type == ERREUR)
        {
            LibererTab(tab, n);
            obj = CopierObjet(upl.uplet[i]);
            LibererUplet(upl);
            return obj;
        }
    }

    LibererTab(tab, n);
    return CreerObjet(UPLET, NULL, expr, &upl, sizeof(Uplet));
}

//Transforme une expr du type "[[ab,c][ze,cfg][e,f]]" en un objet de type matrice (ici 3x2) (lignes x col)
Objet RecupererMatrice(const char expr[])
{
    const char *expr0 = expr;
    char *p, *buf, **tab;
    Matrice mat = CreerMatrice(MAX_MAT, MAX_MAT);
    int done = 0, n, n0=-1, j;
    unsigned int i, n2;
    Objet obj1, obj2;

    if (expr[0] != '[' || expr[1] != '[' || !(p=StrchrEx(expr+1, ']')) || *(p+1) )
        return ErreurExt("Matrice : Syntaxe incorrecte", expr);
    if (!mat.matrice)
        return ErreurInt("Erreur d'allocation");

    buf = malloc(sizeof(char)*(strlen(expr)+1));
    if (!buf)
    {
        LibererMatrice(mat);
        return ErreurInt("Erreur d'allocation");
    }

    expr = expr+1;
    p = StrchrEx(expr+1, ']');

    for (i=0 ; i<MAX_MAT && !done ; i++)
    {
        if (!p)
        {
            free(buf);
            LibererMatrice(mat);
            return ErreurExt("Matrice : Syntaxe incorrecte", expr);
        }

        strcpy(buf, expr);
        buf[0] = '(';
        buf[p-expr] = ')';

        if (buf[p-expr+1] == ']')
            done = 1;
        else if (buf[p-expr+1] != '[')
        {
            free(buf);
            LibererMatrice(mat);
            return ErreurExt("Matrice : Syntaxe incorrecte", expr);
        }

        buf[p-expr+1] = '\0';
        tab = ExtraireParamAuto(buf, &n2, NULL);
        if (!tab)
        {
            LibererMatrice(mat);
            free(buf);
            return ErreurInt("Erreur d'allocation");
        }

        n = n2>MAX_MAT ? MAX_MAT : n2;

        if (n0<=0)
            n0 = n;
        else if (n != n0)
        {
            LibererTab(tab, n2);
            LibererMatrice(mat);
            free(buf);
            return ErreurExt("La matrice n'a pas une largeur constante", expr0);
        }

        for (j=0 ; j<n ; j++)
        {
            mat.matrice[i][j] = Calculer(tab[j], tabLocVar);
            if (mat.matrice[i][j].type == ERREUR)
            {
                LibererTab(tab, n2);
                free(buf);
                obj1 = CopierObjet(mat.matrice[i][j]);
                LibererMatrice(mat);
                return obj1;
            }
        }

        LibererTab(tab, n2);

        if (!done)
        {
            expr = p+1;
            p = StrchrEx(expr+2, ']');
        }
    }

    mat.lignes = i;
    mat.col = n0;

    free(buf);
    obj1 = CreerObjet(MATRICE, NULL, expr0, &mat, sizeof(Matrice));
    obj2 = CopierObjet(obj1);

    for (i=0 ; i<MAX_MAT ; i++)
    {
        for (j=0 ; j<MAX_MAT ; j++)
            LibererObjet(&(mat.matrice[i][j]));
        if (i>=mat.lignes)
            free(mat.matrice[i]);
    }
    LibererObjet(&obj1);

    return obj2;
}


//Dit si une expression est entourée de parenthèses
int IsParenth(const char expr[])
{
    int i;
    char *p;

    if (expr[0] == '(' && (p=StrchrEx(expr+1, ')')) ) //si parenthèses
    {
        i = p-expr;
        if (!expr[i+1])
            return 1;
    }

    return 0;
}


/*
   Extrait les paramètres passés à une fonction, type de chaine (expr) : "(a,xd,aze)" ou "a,xd,aze"
   et les place dans un tableau (tab), n désignant le nombre de cases et l la taille d'une case (chaine de caractères)
   [par exemple ici tab[0] = "a" ; tab[1] = "xd" ; tab[2] = "aze" ; on doit avoir n>=3 et l>=4]
   Retour : -1 = erreur fatale, 0 = erreur de taille (=> chaines incomplètes), 1 = ok
   Rq : pour faire comprendre qu'il s'agit, par exemple, d'un seul argument n-uplet, passer ((a,xd,az))
*/

int ExtraireParametres(const char expr[], char *tab[], unsigned int n, unsigned int l)
{
    char buffer[MAX_EXPR], *p;
    int done = 0, r = 1, parenth;
    unsigned int  i = 0;

    if (!expr || !tab || n<=0 || l<=0)
        return -1;

    parenth = IsParenth(expr);
    expr = expr+parenth;

    while (!done && i<n)
    {
        strncpy(buffer, expr, MAX_EXPR-1);
        p = StrchrEx(buffer, ',');
        if (!p)
        {
            p = StrchrEx(buffer, ')');
            done = 1;
        }
        else expr = (char*) ((int)expr + ((int)p+1 - (int)buffer));
        if (p)
            *p = '\0';

        if (strlen(buffer)>=l)
            r = 0;
        strncpy(tab[i], buffer, l);

        i++;
    }

    return r && done;
}

//Meme fonction que ExtraireParametres mais alloue automatiquement la mémoire nécessaire
char** ExtraireParamAuto(const char expr[], unsigned int* n, unsigned int* l)
{
    int n2, i;
    unsigned l2;
    char **tab;

    n2 = CompterParametres(expr, &l2);
    if (n2>0)
    {
        tab = malloc(sizeof(char*)*n2);
        for (i=0 ; i<n2 ; i++)
            tab[i] = malloc(sizeof(char)*l2);
        if (n)
            *n = n2;
        if (l)
            *l = l2;

        if (ExtraireParametres(expr, tab, n2, l2)>0)
            return tab;
        else LibererTab(tab, n2);
    }

    return NULL;
}


/*
    Fonction comptant le nombre de paramètres dans expr (voir la fonction ExtraireParametres).
    lmax est éventuellement la taille max d'un paramètre (\0 compris).
*/
int CompterParametres(const char expr[], unsigned int *lmax)
{
    if (!expr)
        return -1;

    int i, m=0, l, parenth = IsParenth(expr);
    const char *p, *pNext;

    p = expr+parenth;

    if (!lmax)
        return StrCountEx(expr, ',')+1;

    for (i=1 ; (pNext=StrchrEx(p, ',')) ; i++)
    {
        l = pNext-p;
        if (l>m)
            m = l;
        p = pNext+1;
    }

    if (!m)
        m = strlen(expr)-2*parenth;
    else if ( (l=strlen(p)-parenth) > m )
        m = l;

    *lmax = m+1;
    return i;
}


//Libère un tableau de pointeurs sur char (typiquement créé par la fonction ExtraireParamAuto() )
void LibererTab(char *tab[], unsigned int n)
{
    unsigned int i;
    for (i=0 ; i<n ; i++)
    {
        if (tab[i])
            free(tab[i]);
    }

    free(tab);
}

//Donne la matrice identité (complexe) de taille n
Objet MatIdentite(unsigned int n)
{
    char buf[MAX_EXPR];
    unsigned int i, j;
    Objet error;
    Matrice mat;

    error.type = VIDE;
    if (n<=0 || n>MAX_MAT)
    {
        snprintf(buf, MAX_EXPR-1, "MatIdentite(%d)", n);
        return ErreurExt("Taille de matrice incorrecte",  buf);
    }

    mat = CreerMatrice(n,n);
    if (!mat.matrice)
        return ErreurInt("Erreur d'allocation");

    for (i=0 ; i<n ; i++)
    {
        for (j=0 ; error.type == VIDE && j<n ; j++)
        {
            if (i!=j)
                mat.matrice[i][j] = CreerComplexe(0,0,NULL,"0");
            else mat.matrice[i][j] = CreerComplexe(1,0,NULL,"1");

            if (mat.matrice[i][j].type == ERREUR)
                error = CopierObjet(mat.matrice[i][j]);
        }
    }

    if (error.type == VIDE)
    {
        snprintf(buf, MAX_EXPR-1, "Matrice identite de taille %d", n);
        return CreerObjet(MATRICE, NULL, buf, &mat, sizeof(Matrice));
    }
    else
    {
        LibererMatrice(mat);
        return error;
    }
}

//Dit si un nombre est entier ou non
int EstEntier(double a)
{
    return a == floor(a);
}



//Fonctions...

//Exponentielle
Objet Exp(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "exp(%s)", Descr(a));

    if (a.type != COMPLEXE)
        return ErreurExt("Exponentielle : hors domaine", buf);

    return CreerComplexe2(exp(Re(a)), Im(a), NULL, buf);
}

//Sinus
Objet Sin(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "sin(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Sinus : hors domaine", buf);

    return CreerComplexe(sin(Re(a)), 0, NULL, buf);
}

//Cosinus
Objet Cos(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "cos(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Cosinus : hors domaine", buf);

    return CreerComplexe(cos(Re(a)), 0, NULL, buf);
}

//Tangente
Objet Tan(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "tan(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Tangente : hors domaine", buf);
    else if ( EstNul(cos(Re(a))) )
        return ErreurExt("Tangente : hors domaine", buf);

    return CreerComplexe(tan(Re(a)), 0, NULL, buf);
}


//Valeur absolue
Objet Abs(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "abs(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Valeur absolue : hors domaine", buf);

    return CreerComplexe(fabs(Re(a)), 0, NULL, buf);
}

//Argcosinus hyperbolique
Objet Ach(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "ach(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Argcosinus : hors domaine", buf);
    else if (Re(a) < 1)
        return ErreurExt("Argcosinus : hors domaine", buf);

    return CreerComplexe(acosh(Re(a)), 0, NULL, buf);
}

//Arccosinus
Objet Acos(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "acos(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Arccosinus : hors domaine", buf);
    else if ( Re(a) < -1 || Re(a) > 1)
        return ErreurExt("Arccosinus : hors domaine", buf);

    return CreerComplexe(acos(Re(a)), 0, NULL, buf);
}

//Arcsinus
Objet Asin(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "asin(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Arcsinus : hors domaine", buf);
    else if ( Re(a) < -1 || Re(a) > 1)
        return ErreurExt("Arcsinus : hors domaine", buf);

    return CreerComplexe(asin(Re(a)), 0, NULL, buf);
}

//Argsinus hyperbolique
Objet Ash(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "asinh(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Argsinus : hors domaine", buf);

    return CreerComplexe(asinh(Re(a)), 0, NULL, buf);
}

//Arctangente
Objet Atan(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "atan(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Arctangente : hors domaine", buf);

    return CreerComplexe(atan(Re(a)), 0, NULL, buf);
}

//Argtangente hyperbolique
Objet Ath(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "atanh(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Argtangente : hors domaine", buf);
    else if ( Re(a) >= 1 || Re(a) <= -1)
        return ErreurExt("Argtangente : hors domaine", buf);

    return CreerComplexe(atanh(Re(a)), 0, NULL, buf);
}

//Entier immédiatement supérieur
Objet Ceil(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "ceil(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Ceil : hors domaine", buf);

    return CreerComplexe(ceil(Re(a)), 0, NULL, buf);
}

//Cosinus hyperbolique
Objet Cosh(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "cosh(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Cosinus hyperbolique : hors domaine", buf);

    return CreerComplexe(cosh(Re(a)), 0, NULL, buf);
}

//Partie fractionnaire
Objet Frac(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "frac(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Frac : hors domaine", buf);

    return CreerComplexe(Re(a)-trunc(Re(a)), 0, NULL, buf);
}

//Troncature
Objet Trunc(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "int(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Troncature : hors domaine", buf);

    return CreerComplexe(trunc(Re(a)), 0, NULL, buf);
}

//Partie entière
Objet Floor(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "intg(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Partie entiere : hors domaine", buf);

    return CreerComplexe(floor(Re(a)), 0, NULL, buf);
}

//Arrondi
Objet Round(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "rnd(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Arrondi : hors domaine", buf);

    return CreerComplexe(round(Re(a)), 0, NULL, buf);
}

//Sinus hyperbolique
Objet Sinh(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "sinh(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Sinus hyperbolique : hors domaine", buf);

    return CreerComplexe(sinh(Re(a)), 0, NULL, buf);
}

//Racine carrée
Objet Sqrt(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "sqrt(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Racine : hors domaine", buf);
    else if (Re(a) < 0)
        return ErreurExt("Racine : hors domaine", buf);

    return CreerComplexe(sqrt(Re(a)), 0, NULL, buf);
}

//Tangente hyperbolique
Objet Tanh(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "tanh(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Tangente hyperbolique : hors domaine", buf);

    return CreerComplexe(tanh(Re(a)), 0, NULL, buf);
}


//Logarithme népérien
Objet Ln(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "ln(%s)", Descr(a));

    if (a.type != COMPLEXE || !EstNul(Im(a)))
        return ErreurExt("Logarithme neperien : hors domaine", buf);
    else if ( Re(a) <= 0)
        return ErreurExt("Logarithme neperien : hors domaine", buf);

    return CreerComplexe(log(Re(a)), 0, NULL, buf);
}

//Norme 2, vecteurs en dimension finie
Objet Norme(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    int n, i;
    double norm = 0, m;
    Objet* uplet;

    snprintf(buf, MAX_EXPR-1, "norm(%s)", Descr(a));

    if (a.type == UPLET)
    {
        n = TailleUplet(a);
        uplet = UpletObj(a);
        for (i=0 ; i < n ; i++)
        {
            if (uplet[i].type != COMPLEXE)
                return ErreurExt("Norme : hors domaine", buf);
            m = Mod(uplet[i]);
            norm += m*m;
        }
        return CreerComplexe(sqrt(norm), 0, NULL, buf);
    }
    else if (a.type == COMPLEXE)
        return CreerComplexe(Mod(a), 0, NULL, buf);
    else return ErreurExt("Norme : hors domaine", buf);
}

//Déterminant, matrices carrées
Objet Det(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    unsigned int l, i;
    int r;
    Objet **mat, det, det2;
    snprintf(buf, MAX_EXPR-1, "det(%s)", Descr(a));

    if ( a.type != MATRICE || ColMat(a) != (l=LignesMat(a)) )
        return ErreurExt("Determinant : hors domaine", buf);

    if (!(mat = MatriceObj(a)))
        return ErreurInt("Objet incorrect");
    if (CheckMatConsistency(mat, l, l) != COMPLEXE)
        return ErreurExt("Impossible de calculer le determinant d'une matrice contenant autre chose que des complexes", buf);

    if (!(r = PivotGauss(a, l)))
        return CreerComplexe(0, 0, NULL, buf);

    det = CreerComplexe(r, 0, NULL, buf);
    for (i=0 ; i<l ; i++)
    {
        det2 = Mult(det, mat[i][i]);
        LibererObjet(&det);
        det = det2;
    }

    return det;
}


//Applique un pivot de Gauss pour triangularisation inférieure sur une matrice carrée de complexes, récursivement
int PivotGauss(Objet a, unsigned int dim)
{
    if (dim < 2)
        return 1;

    Objet **mat = MatriceObj(a), nb, nb2;
    int l = dim-1, c = dim-1, exch = 0, i;

    while (l >= 0 && EstNul(Re(mat[l][c])) && EstNul(Im(mat[l][c])))
        l--;

    if (l < 0)
        return 0;
    else if (l < (int)dim-1)
    {
        exch = 1;
        EchangerLignes(a, dim-1, l);
    }

    for (i=dim-2 ; i >= 0 ; i--)
    {
        nb = Div(mat[i][dim-1], mat[dim-1][dim-1]);
        nb2 = Opp(nb, nb);
        LibererObjet(&nb);
        if (nb2.type == ERREUR)
        {
            LibererObjet(&nb2);
            return 0;
        }

        OpLigne(a, i, dim-1, Re(nb2), Im(nb2));
        LibererObjet(&nb2);
    }

    return exch ? -PivotGauss(a, dim-1) : PivotGauss(a, dim-1);
}

//Fonction conditionnelle
Objet Si(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    Objet *upl;
    snprintf(buf, MAX_EXPR-1, "Si(%s)", Descr(a));

    if ( a.type != UPLET || TailleUplet(a) != 3 )
        return ErreurExt("Fonction Si : hors domaine", buf);

    if (!(upl = UpletObj(a)))
        return ErreurInt("Objet incorrect");

    if (upl[0].type != BOOLEEN)
        return ErreurExt("Fonction Si : parametre 1 hors domaine", buf);

    if (ValBool(upl[0]))
        return CopierObjet(upl[1]);
    else return CopierObjet(upl[2]);
}

//Dérivée par approximation de la pente
Objet Drv(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR], descrCp[MAX_EXPR];
    Objet *upl, obj1, obj2, cp1, cp2, cp3, cp4, result;
    snprintf(buf, MAX_EXPR-1, "drv(%s)", Descr(a));

    if ( a.type != UPLET || TailleUplet(a) != 2 )
        return ErreurExt("Fonction derivee : hors domaine", buf);

    if (!(upl = UpletObj(a)))
        return ErreurInt("Objet incorrect");

    if (upl[0].type != FONCTION || (VarFn(upl[0]))[1])
        return ErreurExt("Fonction derivee : parametre 1 hors domaine", buf);

    if (upl[1].type != COMPLEXE || !EstNul(Im(upl[1])))
        return ErreurExt("Fonction derivee : parametre 2 hors domaine", buf);

    snprintf(descrCp, MAX_EXPR-1, "(%s)+10^-7", Descr(upl[1]));
    cp1 = CreerComplexe(Re(upl[1]) + 10e-7, 0, NULL, descrCp);
    snprintf(descrCp, MAX_EXPR-1, "(%s)-10^-7", Descr(upl[1]));
    cp2 = CreerComplexe(Re(upl[1]) - 10e-7, 0, NULL, descrCp);

    obj1 = EvalFn(upl[0], cp1);
    obj2 = EvalFn(upl[0], cp2);

    cp3 = Sous(obj1, obj2);
    cp4 = CreerComplexe(2*10e-7, 0, NULL, "2*10^-7");

    result = Div(cp3, cp4);

    LibererObjet(&cp3);
    LibererObjet(&cp4);

    LibererObjet(&obj1);
    LibererObjet(&obj2);

    LibererObjet(&cp1);
    LibererObjet(&cp2);

    return result;
}


//Intégrale de Riemann
Objet Integ(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    Objet *upl, obj1, obj2, cp;
    int nbSteps = 5000, i;
    double step, reInteg, imInteg, x;
    snprintf(buf, MAX_EXPR-1, "integ(%s)", Descr(a));

    if ( a.type != UPLET || TailleUplet(a) != 3 )
        return ErreurExt("Fonction integrale : hors domaine", buf);

    if (!(upl = UpletObj(a)))
        return ErreurInt("Objet incorrect");

    if (upl[0].type != FONCTION || (VarFn(upl[0]))[1])
        return ErreurExt("Fonction integrale : parametre 1 hors domaine", buf);

    if (upl[1].type != COMPLEXE || !EstNul(Im(upl[1])))
        return ErreurExt("Fonction integrale : parametre 2 hors domaine", buf);

    if (upl[2].type != COMPLEXE || !EstNul(Im(upl[2])))
        return ErreurExt("Fonction integrale : parametre 3 hors domaine", buf);

    if (EstNul(Re(upl[2])-Re(upl[1])))
        return CreerComplexe(0, 0, NULL, buf);

    step = (Re(upl[2])-Re(upl[1])) / nbSteps;
    x = Re(upl[1]);
    cp = CopierObjet(upl[1]);
    obj1 = EvalFn(upl[0], cp);
    reInteg = 0; imInteg = 0;

    for (i=0 ; obj1.type == COMPLEXE && i < nbSteps ; i++)
    {
        x += step;
        LibererObjet(&cp);
        cp = CreerComplexe(x, 0, NULL, NULL);
        obj2 = EvalFn(upl[0], cp);
        if (obj2.type == COMPLEXE)
        {
            reInteg += (Re(obj2)+Re(obj1))/2*step;
            imInteg += (Im(obj2)+Im(obj1))/2*step;
        }
        LibererObjet(&obj1);
        obj1 = obj2;
    }

    LibererObjet(&cp);

    if (obj1.type == ERREUR)
        return obj1;
    else if (obj1.type != COMPLEXE)
    {
        LibererObjet(&obj1);
        return ErreurExt("La fonction n'est pas a valeurs complexes dans tout l'intervalle", buf);
    }
    LibererObjet(&obj1);

    return CreerComplexe(reInteg, imInteg, NULL, buf);
}

Objet Zero(Objet o)
{
    if (o.type == ERREUR)
        return CopierObjet(o);

    char buf[MAX_EXPR];
    Objet *upl, obj1, obj2, obj3, cp;
    int nbSteps = 1000, i, done=0;
    double a, b, x1, x2, x3, precision, precisionMin, step;
    snprintf(buf, MAX_EXPR-1, "zero(%s)", Descr(o));

    if ( o.type != UPLET || TailleUplet(o) != 3 )
        return ErreurExt("Fonction zero : hors domaine", buf);

    if (!(upl = UpletObj(o)))
        return ErreurInt("Objet incorrect");

    if (upl[0].type != FONCTION || (VarFn(upl[0]))[1])
        return ErreurExt("Fonction zero : parametre 1 hors domaine", buf);

    if (upl[1].type != COMPLEXE || !EstNul(Im(upl[1])))
        return ErreurExt("Fonction zero : parametre 2 hors domaine", buf);

    if (upl[2].type != COMPLEXE || !EstNul(Im(upl[2])))
        return ErreurExt("Fonction zero : parametre 3 hors domaine", buf);

    a = Re(upl[2]);
    b = Re(upl[1]);
    if (EstNul(a-b))
        return ErreurExt("Fonction zero : intervalle incorrect", buf);

    if (b < a)
    {
        double tmp = a;
        a = b;
        b = tmp;
    }

    for (x1=ceil(a) ; x1 <= b ; x1++)
    {
        cp = CreerComplexe(x1, 0, NULL, buf);
        obj1 = EvalFn(upl[0], cp);
        if (obj1.type == COMPLEXE && EstNul(Im(obj1)) && EstNul(Re(obj1)))
        {
            LibererObjet(&obj1);
            return cp;
        }
        LibererObjet(&obj1);
        LibererObjet(&cp);
    }

    step = (b-a) / nbSteps;
    x2 = a;
    cp = CopierObjet(upl[1]);
    obj1 = EvalFn(upl[0], cp);

    if (obj1.type == COMPLEXE && EstNul(Im(obj1)) && EstNul(Re(obj1)))
    {
        LibererObjet(&cp);
        LibererObjet(&obj1);
        return CreerComplexe(a, 0, NULL, buf);
    }

    for (i=0 ; !done && obj1.type == COMPLEXE && EstNul(Im(obj1)) && i < nbSteps ; i++)
    {
        x2 += step;
        LibererObjet(&cp);
        cp = CreerComplexe(x2, 0, NULL, NULL);
        obj2 = EvalFn(upl[0], cp);
        if (obj2.type == COMPLEXE && EstNul(Im(obj2)) && Re(obj1)*Re(obj2) <= 0)
            done = 1;
        else
        {
            LibererObjet(&obj1);
            obj1 = obj2;
        }
    }

    LibererObjet(&cp);

    if (obj1.type == ERREUR)
        return obj1;
    else if (obj1.type != COMPLEXE || !EstNul(Im(obj1)))
    {
        LibererObjet(&obj1);
        return ErreurExt("La fonction n'est pas a valeurs reelles dans tout l'intervalle", buf);
    }

    if (!done)
    {
        LibererObjet(&obj1);
        return ErreurExt("Pas de zero trouve dans l'intervalle specifie", buf);
    }

    if (EstNul(Re(obj2)))
    {
        LibererObjet(&obj1);
        LibererObjet(&obj2);
        return CreerComplexe(x2, 0, NULL, buf);
    }

    x1 = x2-step;
    precision = step;
    precisionMin = 1./pow(10,PRECISION);
    done = 0;
    while(!done && precision > precisionMin)
    {
        x3 = (x1+x2) / 2;
        cp = CreerComplexe(x3, 0, NULL, NULL);
        obj3 = EvalFn(upl[0], cp);
        LibererObjet(&cp);

        if (obj3.type == COMPLEXE && EstNul(Im(obj3)))
        {
            if (EstNul(Re(obj3)))
                done = 1;
            else if (Re(obj3)*Re(obj1) >= 0)
            {
                LibererObjet(&obj1);
                obj1 = obj3;
                x1 = x3;
            }
            else
            {
                LibererObjet(&obj2);
                obj2 = obj3;
                x2 = x3;
            }
        }
        precision /= 2;
    }

    LibererObjet(&obj1);
    LibererObjet(&obj2);

    if (done)
    {
        LibererObjet(&obj3);
        return CreerComplexe(x3, 0, NULL, buf);
    }

    return CreerComplexe((x1+x2)/2, 0, NULL, buf);
}

//Partie relle
Objet RePart(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "re(%s)", Descr(a));

    if (a.type != COMPLEXE)
        return ErreurExt("Impossible d'extraire la partie reelle d'un objet non complexe", buf);

    return CreerComplexe(Re(a), 0, NULL, buf);
}

//Partie imaginaire
Objet ImPart(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "im(%s)", Descr(a));

    if (a.type != COMPLEXE)
        return ErreurExt("Impossible d'extraire la partie imaginaire d'un objet non complexe", buf);

    return CreerComplexe(Im(a), 0, NULL, buf);
}

//Argument
Objet ArgCp(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "arg(%s)", Descr(a));

    if (a.type != COMPLEXE)
        return ErreurExt("Argument : hors domaine", buf);

    return CreerComplexe(Arg(a), 0, NULL, buf);
}

//Module
Objet ModCp(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    snprintf(buf, MAX_EXPR-1, "mod(%s)", Descr(a));

    if (a.type != COMPLEXE)
        return ErreurExt("Module : hors domaine", buf);

    return CreerComplexe(Mod(a), 0, NULL, buf);
}

//SVD
Objet Svd(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    unsigned int nc, nl, i, j;
    Objet **mat;
    Matrice matU, matV, matS;
    Uplet result;
    snprintf(buf, MAX_EXPR-1, "svd(%s)", Descr(a));

    if (a.type != MATRICE)
        return ErreurExt("SVD : hors domaine", buf);

    if (!(mat = MatriceObj(a)))
        return ErreurInt("Objet incorrect");

    nc = ColMat(a); nl = LignesMat(a);
    if (CheckMatConsistency(mat, nc, nl) != COMPLEXE)
        return ErreurExt("Impossible d'effectuer une SVD sur une matrice contenant autre chose que des complexes", buf);

    DMat dmat = svdNewDMat(nl, nc);
    if (!dmat)
        return ErreurInt("Erreur d'allocation");
    for (i=0 ; i < nl ; i++)
    {
        for (j=0 ; j < nc ; j++)
            dmat->value[i][j] = Re(mat[i][j]);
    }

    SMat smat = svdConvertDtoS(dmat);
    svdFreeDMat(dmat);
    if (!smat)
        return ErreurInt("Erreur de conversion");

    SVDRec svdRec = svdLAS2A(smat, 0);
    svdFreeSMat(smat);
    if (!svdRec)
        return ErreurInt("Erreur lors du calcul de la SVD");

    matU = CreerMatrice(nl, nl);
    if (!matU.matrice)
    {
        svdFreeSVDRec(svdRec);
        return ErreurInt("Erreur d'allocation");
    }
    for (i=0 ; i < matU.lignes ; i++)
    {
        for (j=0 ; j < matU.col ; j++)
            matU.matrice[i][j] = CreerComplexe(svdRec->Ut->value[j][i], 0, NULL, NULL);
    }

    matV = CreerMatrice(nc, nc);
    if (!matV.matrice)
    {
        svdFreeSVDRec(svdRec);
        LibererMatrice(matU);
        return ErreurInt("Erreur d'allocation");
    }
    for (i=0 ; i < matV.lignes ; i++)
    {
        for (j=0 ; j < matV.col ; j++)
            matV.matrice[i][j] = CreerComplexe(svdRec->Vt->value[j][i], 0, NULL, NULL);
    }

    matS = CreerMatrice(nl, nc);
    if (!matS.matrice)
    {
        svdFreeSVDRec(svdRec);
        LibererMatrice(matU); LibererMatrice(matV);
        return ErreurInt("Erreur d'allocation");
    }
    for (i=0 ; i < matS.lignes ; i++)
    {
        for (j=0 ; j < matS.col ; j++)
            matS.matrice[i][j] = CreerComplexe(i==j && i < (unsigned int)svdRec->d ? svdRec->S[i] : 0, 0, NULL, NULL);
    }

    svdFreeSVDRec(svdRec);

    result = CreerUplet(3);
    if (!result.uplet)
        return ErreurInt("Erreur d'allocation");
    result.uplet[0] = CreerObjet(MATRICE, NULL, NULL, &matU, sizeof(Matrice));
    result.uplet[1] = CreerObjet(MATRICE, NULL, NULL, &matS, sizeof(Matrice));
    result.uplet[2] = CreerObjet(MATRICE, NULL, NULL, &matV, sizeof(Matrice));

    return CreerObjet(UPLET, NULL, buf, &result, sizeof(Uplet));
}

Objet Trace(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    unsigned int n, i;
    Objet **mat, trace, trace2;
    snprintf(buf, MAX_EXPR-1, "trace(%s)", Descr(a));

    if (a.type != MATRICE)
        return ErreurExt("Trace : hors domaine", buf);

    if (!(mat = MatriceObj(a)))
        return ErreurInt("Objet incorrect");

    trace = CopierObjet(mat[0][0]);
    n = ColMat(a) > LignesMat(a) ? LignesMat(a) : ColMat(a);
    for (i=1 ; i < n ; i++)
    {
        trace2 = Add(trace, mat[i][i]);
        LibererObjet(&trace);
        trace = trace2;
    }

    if (trace.description)
        free(trace.description);
    trace.description = malloc(sizeof(char) * (strlen(buf)+1));
    if (!trace.description)
        return ErreurInt("Erreur d'allocation");
    strcpy(trace.description, buf);

    return trace;
}

Objet Transposee(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    unsigned int nc, nl, i, j;
    Objet **mat;
    Matrice transp;
    snprintf(buf, MAX_EXPR-1, "trace(%s)", Descr(a));

    if (a.type != MATRICE)
        return ErreurExt("Transposee : hors domaine", buf);

    if (!(mat = MatriceObj(a)))
        return ErreurInt("Objet incorrect");

    nc = ColMat(a); nl = LignesMat(a);
    transp = CreerMatrice(nc, nl);
    if (!transp.matrice)
        return ErreurInt("Erreur d'allocation");

    for (i=0 ; i < nl ; i++)
    {
        for (j=0 ; j < nc ; j++)
            transp.matrice[j][i] = CopierObjet(mat[i][j]);
    }

    return CreerObjet(MATRICE, NULL, buf, &transp, sizeof(Matrice));
}

Objet EigenDec(Objet a)
{
    if (a.type == ERREUR)
        return CopierObjet(a);

    char buf[MAX_EXPR];
    unsigned int nl, i, j, k=0;
    Objet **mat;
    double *matrixData = NULL, *eigenVal = NULL, *eigenVec = NULL;
    Matrice eigenValMat, eigenVecMat;
    Uplet upl;
    snprintf(buf, MAX_EXPR-1, "eig(%s)", Descr(a));

    if (a.type != MATRICE)
        return ErreurExt("EVD : hors domaine", buf);

    if (!(mat = MatriceObj(a)))
        return ErreurInt("Objet incorrect");

    nl = LignesMat(a);
    if (ColMat(a) != nl)
        return ErreurExt("Impossible d'effectuer une EVD sur une matrice non carree", buf);

    if (CheckMatConsistency(mat, nl, nl) != COMPLEXE)
        return ErreurExt("Impossible d'effectuer une EVD sur une matrice contenant autre chose que des complexes", buf);

    matrixData = malloc(sizeof(double) * nl *nl);
    if (!matrixData)
        return ErreurInt("Erreur d'allocation");
    for (i=0 ; i < nl ; i++)
    {
        for (j=0 ; j <= i ; j++)
        {
            matrixData[k] = Re(mat[i][j]);
            k++;
        }
        for (j=i+1 ; j < nl ; j++)
        {
            if (!EstNul(Re(mat[i][j]) - Re(mat[j][i])))
                return ErreurExt("Impossible d'effectuer une EVD sur une matrice non symetrique", buf);
            matrixData[k] = Re(mat[i][j]);
            k++;
        }
    }

    eigenVal = malloc(sizeof(double) * nl);
    if (!eigenVal)
    {
        free(matrixData);
        return ErreurInt("Erreur d'allocation");
    }
    eigenVec = malloc(sizeof(double) * nl * nl);
    if (!eigenVec)
    {
        free(matrixData); free(eigenVal);
        return ErreurInt("Erreur d'allocation");
    }

    rs(nl, matrixData, eigenVal, 1, eigenVec);
    free(matrixData);

    eigenValMat = CreerMatrice(nl, nl);
    if (!eigenValMat.matrice)
    {
        free(eigenVal); free(eigenVec);
        return ErreurInt("Erreur d'allocation");
    }
    eigenVecMat = CreerMatrice(nl, nl);
    if (!eigenVecMat.matrice)
    {
        free(eigenVal); free(eigenVec);
        LibererMatrice(eigenValMat);
        return ErreurInt("Erreur d'allocation");
    }
    k = 0;
    for (i=0 ; i < nl ; i++)
    {
        for (j=0 ; j < nl ; j++)
        {
            eigenValMat.matrice[i][j] = CreerComplexe(i==j ? eigenVal[i] : 0, 0, NULL, NULL);
            eigenVecMat.matrice[j][i] = CreerComplexe(eigenVec[k], 0, NULL, NULL);
            k++;
        }
    }

    free(eigenVal);
    free(eigenVec);

    upl = CreerUplet(2);
    if (!upl.uplet)
    {
        LibererMatrice(eigenVecMat); LibererMatrice(eigenValMat);
        return ErreurInt("Erreur d'allocation");
    }
    upl.uplet[0] = CreerObjet(MATRICE, NULL, NULL, &eigenValMat, sizeof(Matrice));
    upl.uplet[1] = CreerObjet(MATRICE, NULL, NULL, &eigenVecMat, sizeof(Matrice));

    return CreerObjet(UPLET, NULL, buf, &upl, sizeof(Uplet));
}

int CheckMatConsistency(Objet **mat, int nc, int nl)
{
    int i, j;
    int type = mat[0][0].type;

    for (i=0 ; i<nl ; i++)
    {
        for (j=0 ; j<nc ; j++)
        {
            if (mat[i][j].type != type)
                return -1;
        }
    }

    return type;
}

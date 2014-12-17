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


#include "main.h"

int main(void)
{
    char instruction[MAX_EXPR];
    int done = 0;

    printf(" -----------------\n| EQUAMINATOR CMD |\n -----------------\n\n");
    printf("Auteur :\tCokie\nVersion :\t1.0.7 beta\nMise a jour :\t%s\n", __DATE__);
    printf("License :\tGNU GPL version 2.0\n\t\thttp://www.gnu.org/licenses/gpl-2.0.html\n\n\n");
    printf("Initialisation... ");

    InitialiserObjets();
    #ifndef MEMMGR
    atexit(Desinitialiser);
    #endif
    printf("terminee.\n");

    ChargerParam();

    printf("Veuillez entrer une commande ou un calcul :\n(Tapez help pour de l'aide !)\n\n> ");

    while (!done)
    {
        SaisirTexte(instruction, MAX_EXPR);
        done = Executer(instruction);
    }

    #ifdef MEMMGR
    Desinitialiser();
    FreeEverything();
    #endif
    return 0;
}


//Charge le fichier EquaCMD.ini et exécute les instruction contenues.
int ChargerParam(void)
{
    FILE *fichierINI;
    char c;
    printf("Chargement des parametres... ");

    if ( !(fichierINI = fopen("EquaCMD.ini", "r")) )
    {
        printf("Echec - fichier introuvable ou inaccessible.\n\n");
        return -1;
    }
    else
    {
        while((c = fgetc(fichierINI)) == ' ' || c == '\n');
        if (fgetc(fichierINI) == EOF)
        {
            fclose(fichierINI);
            printf("Pas de parametre a charger.\n\n");
            return 0;
        }
        else
        {
            fclose(fichierINI);
            printf("\n\n");
            Load("EquaCMD.ini");
            printf("Chargement termine.\n\n\n");
        }
    }

    return 1;
}


//Execute une instruction
int Executer(const char instruction0[])
{
    char instruction[MAX_EXPR];
    int r;
    Objet obj;

    strncpy(instruction, instruction0, MAX_EXPR-1);
    r = ExecCommande(instruction);

    if (r != TERMINERPRGM)
    {
        if (r == CMDNONTROUVEE)
        {
            Preparer(instruction);
            obj = Calculer(instruction, NULL);
            if (obj.type != TEXTE && obj.type != ERREUR && obj.type != VIDE)
                AffecterAns(obj);
            AfficherObjet(obj);
            LibererObjet(&obj);
        }
        else printf("$ Commande executee");

        printf("\n\n> ");
        return 0;
    }
    else return 1;
}


/*
    Effectue un calcul donné par une chaine de calcul
    La chaine doit être sans espace et tous les opérateurs doivent être explicites
    locVar représente éventuellement un tableau de variables locales, prioritaires sur les globales
    Par convention le tableau de variables doit se terminer par une variable de type VIDE
    Recursive
*/
Objet Calculer(const char expr0[], Objet locVar[])
{
    if (!expr0)
        return ErreurInt("Erreur de parametre");

    int i, done;
    char expr[MAX_EXPR],
         expr1[MAX_EXPR], expr2[MAX_EXPR], buf[MAX_EXPR],
         chaineErreur[MAX_EXPR] = "";
    Operation operation;
    Objet obj1, obj2, obj;

    strncpy(expr, expr0, MAX_EXPR);
    EnleverParentheses(expr);

    if ( (i= TrouverOperateur(expr, &operation)) <0)
        return IdentifierObjet(expr, locVar);

    do
    {
        done = 1;

        strcpy(expr1, expr);
        expr1[i] = '\0';
        strcpy(expr2, expr+i+strlen(operation.operateur));

        if (!expr1[0])
        {
            obj1 = EstOperateurTexteDroite(operation) ? CreerObjetTexte(NULL, expr2) : Calculer(expr2, locVar);
            obj2.type = VIDE;
        }
        else if (!expr2[0])
        {
            obj1 = EstOperateurTexteGauche(operation) ? CreerObjetTexte(NULL, expr1) : Calculer(expr1, locVar);
            obj2.type = VIDE;
        }
        else
        {
            obj1 = EstOperateurTexteGauche(operation) ? CreerObjetTexte(NULL, expr1) : Calculer(expr1, locVar);
            obj2 = EstOperateurTexteDroite(operation) ? CreerObjetTexte(NULL, expr2) : Calculer(expr2, locVar);
        }

        if (!operation.fonction)
        {
            buf[0] = expr[i]; buf[1] = '\0';
            obj = ErreurExt("Operateur inconnu dans l'expression", buf);
        }
        else if (obj1.type == ERREUR)
            obj = CopierObjet(obj1);
        else if (obj2.type == ERREUR)
            obj = CopierObjet(obj2);
        else
        {
            AffecterTabLocVar(locVar);
            obj = operation.fonction(obj1, obj2);
            AffecterTabLocVar(NULL);
            if (obj.type == ERREUR)
            {
                if (chaineErreur[0])
                    snprintf(chaineErreur, MAX_EXPR-1, "%s\n# %s", chaineErreur, obj.description);
                else strncpy(chaineErreur, obj.description, MAX_EXPR-1);

                LibererObjet(&obj);

                if (AutreOperation(&operation))
                    done = 0;
                else obj = CreerObjet(ERREUR, NULL, chaineErreur, NULL, 0);
            }
        }

        LibererObjet(&obj1);
        LibererObjet(&obj2);

    } while (!done);

    return obj;
}

//Prépare une expression au calcul : enlève les espaces, rajoute les opérateurs manquants...
int Preparer(char expr[])
{
    if (!expr)
        return 0;

    Objet *tabObj = RecupTabObj();
    int i, j, type;
    char buf[MAX_EXPR], *p, *p2, *p3, *p4;

    EnleverCaractere(expr, ' ');

    for (i=0 ; i<MAX_OBJETS && tabObj[i].type != VIDE ; i++)
    {
        type = tabObj[i].type;
        if (tabObj[i].type == TEXTE && tabObj[i].data)
            type = TexteEstim(tabObj[i]).type;
        if (type == FONCTION || type == MATRICE || type == UPLET)
        {
            snprintf(buf, MAX_EXPR-1, "%s(", tabObj[i].nom);
            if ( (p=strstr(expr, buf)) )
            {
                InsererCaractere(expr, p-expr + strlen(tabObj[i].nom), ':');
                i--;
            }
        }
    }

    while ( (p=strstr(expr, ")(")) )
        InsererCaractere(expr, p-expr+1, ':');
    while ( (p=strstr(expr, "](")) )
        InsererCaractere(expr, p-expr+1, ':');

    p = expr;
    while ( (p=strstr(p, "->")) )
    {
        j = p-expr+1;
        InsererCaractere(expr, j+1, '(');
        p4=StrchrEx(p+3, ']');
        p3=StrchrEx(p+3, ')');
        p2=StrchrEx(p+3, ',');

        p3 = (p4 && (!p3 || p4 < p3)) ? p4 : p3;
        p2 = (p3 && (!p2 || p3 < p2)) ? p3 : p2;

        if (p2)
            InsererCaractere(expr, p2-expr, ')');
        else strcat(expr, ")");
        p = expr+j;
    }

    return 1;
}


//Renvoie un objet de type erreur à partir d'une chaine, d'un nom de fichier et d'un numero de ligne
Objet ErreurIntFn(const char expr[], const char fichier[], int ligne)
{
    Objet obj;
    char nom[MAX_EXPR], *p = StrchrRev(fichier, '\\');

    strncpy(nom, p ? p+1 : fichier, MAX_EXPR-1);

    obj.type = ERREUR;
    obj.description = malloc(sizeof(char)*MAX_EXPR);
    if (obj.description)
        snprintf(obj.description, MAX_EXPR-1, "Erreur : \"%s\" (%s, ligne %d)", expr, nom, ligne);

    obj.nom[0] = '\0';
    obj.data = NULL;
    obj.size = 0;
    return obj;
}

//Renvoie un objet de type erreur à partir de deux chaines
Objet ErreurExt(const char expr[], const char expl[])
{
    Objet obj;
    obj.type = ERREUR;
    obj.description = malloc(sizeof(char)*MAX_EXPR);
    if (obj.description)
        snprintf(obj.description, MAX_EXPR-1, "Erreur : \"%s\"\n\t-> %s", expr, expl);

    obj.nom[0] = '\0';
    obj.data = NULL;
    obj.size = 0;
    return obj;
}

//Enlève, si elles existent, les parenthèses autour d'une expression : "((f(x)+2))" => "f(x)+2"
//Recursive
void EnleverParentheses(char expr[])
{
    if (!expr || expr[0] != '(')
        return;

    char *c = StrchrEx(expr+1, ')');
    int i;

    if (c)
    {
        i = c-expr;
        if (!expr[i+1])
        {
            expr[i] = '\0';
            strcpy(expr, expr+1);
            EnleverParentheses(expr);
        }
    }

    return;
}

//Insère un caractère à la position i dans la chaine
int InsererCaractere(char expr[], unsigned int i, char c)
{
    if (!expr || i>strlen(expr))
        return 0;

    char expr1[MAX_EXPR], expr2[MAX_EXPR];
    strcpy(expr1, expr);
    expr1[i] = '\0';
    strcpy(expr2, expr+i);

    snprintf(expr, MAX_EXPR-1, "%s%c%s", expr1, c, expr2);
    return 1;
}

//Enlève toute occurence du caractere c dans la chaine expr
int EnleverCaractere(char expr[], char c)
{
    if (!expr || !c)
        return 0;

    char *p;
    while ( (p=strchr(expr, c)) )
        strcpy(p, p+1);

    return 1;
}

//Cherche le caractère c dans la chaine expr en partant de la fin
char* StrchrRev(const char expr[], char c)
{
    if (!expr)
        return NULL;

    int i = strlen(expr)-1;
    for (; i >= 0 && expr[i] != c ; i--);

    if (i < 0)
        return NULL;
    else return (char*)(&(expr[i]));
}

//Cherche la chaîne expr1 dans la chaine expr0 en partant de la fin
char* StrstrRev(const char expr0[], const char expr1[])
{
    if (!expr0 || !expr1)
        return NULL;

    int j, i = strlen(expr0)-1;
    for(; i >= 0 ; i--)
    {
        if (expr0[i] == expr1[0])
        {
            for (j=0 ; expr1[j] && expr0[i+j] && expr1[j] == expr0[i+j] ; j++);
            if (!expr1[j])
                return (char*)(&(expr0[i]));
        }
    }

    return NULL;
}

//Cherche le caractère c dans la chaine expr en sautant toutes les expression parenthésées intermédiaires () ou []
char* StrchrEx(const char expr[], char c)
{
    if (!expr || c=='(' || c=='[')
        return NULL;

    int i, t=0;
    for (i=0 ; expr[i] ; i++)
    {
        if (expr[i] == '(' || expr[i] == '[')
            t++;

        if (t==0 && expr[i] == c)
            return (char*)(expr+i);

        if ((expr[i] == ')' || expr[i] == ']')&& t>0)
            t--;
    }

    return NULL;
}

//Cherche le caractère c dans la chaine expr en partant de la fin et en sautant toutes les expression parenthésées intermédiaires () ou []
char* StrchrRevEx(const char expr[], char c)
{
    if (!expr || c=='(' || c=='[')
        return NULL;

    int i=strlen(expr), t=0;
    for (; i >= 0 ; i--)
    {
        if (expr[i] == ')' || expr[i] == ']')
            t++;

        if (t==0 && expr[i] == c)
            return (char*)(expr+i);

        if ((expr[i] == '(' || expr[i] == '[')&& t>0)
            t--;
    }

    return NULL;
}

//Cherche la chaîne expr1 dans la chaine expr0 en partant de la fin et en sautant toutes les expression parenthésées intermédiaires () ou []
char* StrstrRevEx(const char expr0[], const char expr1[])
{
    if (!expr0 || !expr1)
        return NULL;

    int i=strlen(expr0), j, t=0;
    for (; i >= 0 ; i--)
    {
        if (expr0[i] == ')' || expr0[i] == ']')
            t++;

        if (t==0 && expr0[i] == expr1[0])
        {
            for (j=0 ; expr1[j] && expr0[i+j] && expr1[j] == expr0[i+j] ; j++);
            if (!expr1[j])
                return (char*)(&(expr0[i]));
        }

        if ((expr0[i] == '(' || expr0[i] == '[')&& t>0)
            t--;
    }

    return NULL;
}


//Compte la totalité des caractères c présents dans la chaine expr
unsigned int StrCount(const char expr[], char c)
{
    unsigned int i;
    const char *p=expr-1;

    for (i=0 ; (p=strchr(p+1,c)) ; i++);

    return i;
}


//Compte la totalité des caractères c présents dans la chaine expr en sautant toutes les expression parenthésées intermédiaires
unsigned int StrCountEx(const char expr[], char c)
{
    unsigned int i;
    const char *p=expr-1;

    for (i=0 ; (p=StrchrEx(p+1,c)) ; i++);

    return i;
}


//Chercher la chaine str dans expr en sautant toutes les expressions parenthésées intermédiaires
char* StrstrEx(const char expr[], const char str[])
{
    char *p = StrchrEx(expr, str[0]);
    int ok, i;

    while (p)
    {
        ok = 1;
        for (i=0 ; str[i] && ok ; i++)
        {
            if (str[i] != *(p+i))
                ok = 0;
        }

        if (ok)
            return p;
        else p = StrchrEx(p+1, str[0]);
    }

    return NULL;
}

//Compare des chaines de caractères indépendamment de la casse
int StrcmpEx(const char expr1[], const char expr2[])
{
    char buf1[MAX_EXPR],
         buf2[MAX_EXPR];
    int i;

    strncpy(buf1, expr1, MAX_EXPR-1);
    strncpy(buf2, expr2, MAX_EXPR-1);

    for (i=0 ; buf1[i] ; i++)
        buf1[i] = toupper(buf1[i]);

    for (i=0 ; buf2[i] ; i++)
        buf2[i] = toupper(buf2[i]);

    return strcmp(buf1, buf2);
}


//Entrée de texte sécurisée en console
void SaisirTexte(char buf[], unsigned int taille)
{
    fgets(buf, taille-1, stdin);
    Clean(buf);
}

//Vide le buffer d'entrée
void Purger(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Nettoie une chaine et si nécessaire le buffer après une saisie
void Clean(char chaine[])
{
    char *p = strchr(chaine, '\n');

    if (p)
        *p = 0;
    else Purger();
}

//Dit si c est un nombre
int EstNombre(char c)
{
    return c>='0' && c<='9';
}

//Dit si c est une lettre
int EstLettre(char c)
{
    return (c>='a' && c<='z') || (c>='A' && c<='Z');
}

//Affiche un objet à l'écran en détail
int AfficherObjet(Objet obj)
{
    unsigned int i;

    switch (obj.type)
    {
        case ERREUR:
            printf("# %s", obj.description);
            return 1;
        case COMPLEXE:
        {
            char n1[51], n2[51];
            SprintFloat(n1, Re(obj));
            SprintFloat(n2, Im(obj));

            if (n2[0] == '0' && !n2[1])  // si nul
                printf("= %s", n1);
            else printf("= Complexe\n  Re = %s\n  Im = %s", n1, n2);
            return 1;
        }
        case FONCTION:
            #if (OS == LINUX)
            printf("= %s -> %s", VarFn(obj), ExprFn(obj));
            #else
            printf("= %s %c %s", VarFn(obj), FLECHE, ExprFn(obj));
            #endif
            return 1;
        case UPLET:
        {
            Objet *upl = UpletObj(obj);
            printf("= ( ");
            for (i=0 ; i<TailleUplet(obj) ; i++)
            {
                PrintObjet(upl[i]);
                if (i<TailleUplet(obj)-1)
                    printf(" , ");
            }
            printf(" )");
            return 1;
        }
        case MATRICE:
            AfficherMatrice(obj);
            return 1;
        case BOOLEEN:
            printf("= %s", ValBool(obj)?"VRAI":"FAUX");
            return 1;
        case TEXTE:
            if (obj.data && TexteEstim(obj).type != ERREUR)
                AfficherObjet(TexteEstim(obj));
            else printf("~ %s", TexteObj(obj));
            return 1;
        default:
            printf("# Erreur : Objet non reconnu");
            break;
    }

    return 0;
}

//Affiche un objet de type matrice
#if (OS == LINUX)

void AfficherMatrice(Objet obj)
{
    Objet **mat = MatriceObj(obj);
    unsigned int l = LignesMat(obj), c = ColMat(obj),
                 i,j,l1;
    int lmax = floor((76.0/c) - 3),
        lmaj = -1;
    char buf[MAX_EXPR];

    if (lmax <= 0)
        lmax = 1;

    for (i=0 ; i<l ; i++)
    {
        for (j=0 ; j<c ; j++)
        {
            SprintObjet(buf, mat[i][j]);
            if ( (int)(l1=strlen(buf)) > lmaj )
                lmaj = l1;
        }
    }

    if (lmaj > lmax)
        lmaj = lmax;

    printf("= Matrice\n  ");
    for (i=0 ; i<l ; i++)
    {
        for (j=0 ; j<c ; j++)
        {
            SprintObjet(buf, mat[i][j]);
            if ((int)strlen(buf)>lmaj)
                buf[lmaj] = '\0';
            printf(" %*s ", lmaj, buf);
            if (j<c-1)
                printf(" ");
        }
        if (i<l-1)
            printf("\n  ");
    }
}

#else

void AfficherMatrice(Objet obj)
{
    Objet **mat = MatriceObj(obj);
    unsigned int l = LignesMat(obj), c = ColMat(obj),
                 i,j,k,l1;
    int lmax = floor((76.0/c) - 3),
        lmaj = -1, m;
    char buf[MAX_EXPR];

    if (lmax <= 0)
        lmax = 1;

    for (i=0 ; i<l ; i++)
    {
        for (j=0 ; j<c ; j++)
        {
            SprintObjet(buf, mat[i][j]);
            if ( (int)(l1=strlen(buf)) > lmaj )
                lmaj = l1;
        }
    }

    if ((int)lmaj > lmax)
        lmaj = lmax;

    printf("= ");
    for (i=0 ; i<c ; i++)
    {
        printf("%c", i ? 194:218);
        for (j=0 ; (int)j<lmaj+2 ; j++)
            printf("%c", 196);
    }
    printf("%c\n  ", 191);

    for (i=0 ; i<l ; i++)
    {
        printf("%c", 179);
        for (j=0 ; j<c ; j++)
        {
            SprintObjet(buf, mat[i][j]);
            if ((int)strlen(buf)>lmaj)
                buf[lmaj] = '\0';
            printf(" %*s ", lmaj, buf);
            if (j<c-1)
                printf("%c", 179);
        }
        printf("%c\n  ", 179);

        if (i<l-1)
        {
            for (k=0 ; k<c ; k++)
            {
                printf("%c", k ? 197:195);
                for (m=0 ; m<lmaj+2 ; m++)
                    printf("%c", 196);
            }
            printf("%c\n  ", 180);
        }
    }

    for (i=0 ; i<c ; i++)
    {
        printf("%c", i ? 193:192);
        for (j=0 ; (int)j<lmaj+2 ; j++)
            printf("%c", 196);
    }
    printf("%c", 217);
}

#endif


//Imprime la description simplifiée d'un objet dans une chaine
int SprintObjet(char buf[], Objet obj)
{
    int ok = 1;

    switch (obj.type)
    {
        case ERREUR:
            strncpy(buf, "#ERR", MAX_EXPR-1);
            break;
        case COMPLEXE:
            SprintComplexe(buf, obj);
            break;
        case FONCTION:
            #if (OS == LINUX)
            snprintf(buf, MAX_EXPR-1, "%s->%s", VarFn(obj), ExprFn(obj));
            #else
            snprintf(buf, MAX_EXPR-1, "%s%c%s", VarFn(obj), FLECHE, ExprFn(obj));
            #endif
            break;
        case UPLET:
            snprintf(buf, MAX_EXPR-1, "%d-Uplet", TailleUplet(obj));
            break;
        case MATRICE:
            snprintf(buf, MAX_EXPR-1, "Matrice %dx%d", LignesMat(obj), ColMat(obj));
            break;
        case BOOLEEN:
            strncpy(buf, ValBool(obj)?"VRAI":"FAUX", MAX_EXPR-1);
            break;
        case TEXTE:
            if (obj.data)
                SprintObjet(buf, TexteEstim(obj));
            else strncpy(buf, "Texte", MAX_EXPR-1);
            return 1;
        default:
            strncpy(buf, "Inconnu", MAX_EXPR-1);
            ok = 0;
            break;
    }

    if (!IsLocal(obj) && obj.nom[0] && (strlen(obj.nom)<=strlen(buf) || !ok))
        strncpy(buf, obj.nom, MAX_EXPR-1);
    if (!IsPermanent(obj) && obj.type != MATRICE && obj.description && obj.description[0] && strlen(obj.description)<strlen(buf))
    {
        if (obj.type == UPLET)
            snprintf(buf, MAX_EXPR-1, "(%s)", obj.description);
        else strncpy(buf, obj.description, MAX_EXPR-1);
    }

    return ok;
}

//Imprime la description simplifiée d'un objet à l'écran
int PrintObjet(Objet obj)
{
    char buf[MAX_EXPR];
    int ok = SprintObjet(buf, obj);
    printf("%s", buf);
    return ok;
}

//Imprime un complexe dans une chaine
void SprintComplexe(char buf[], Objet obj)
{
    char n1[MAX_EXPR], n2[MAX_EXPR];
    SprintFloat(n1, Re(obj));
    SprintFloat(n2, Im(obj));
    if (n2[0] == '0' && !n2[1]) // si nul
        strncpy(buf, n1, MAX_EXPR-1);
    else if (n1[0] == '0' && !n1[1])
    {
        if (n2[0] == '1' && !n2[1])
            strcpy(buf, "i");
        else if (n2[0] == '-' && n2[1] == '1' && !n2[2])
            strcpy(buf, "-i");
        else snprintf(buf, MAX_EXPR-1, "%s*i", n2);
    }
    else
    {
        if (n2[0] == '-')
        {
            if (n2[1] == '1' && !n2[2])
                snprintf(buf, MAX_EXPR-1, "%s-i", n1);
            else snprintf(buf, MAX_EXPR-1, "%s%s*i", n1, n2);
        }
        else if (n2[0] == '1' && !n2[1])
            snprintf(buf, MAX_EXPR-1, "%s+i", n1);
        else snprintf(buf, MAX_EXPR-1, "%s+%s*i", n1, n2);
    }
}


//Imprime un flottant dans une chaine
void SprintFloat(char buf[], double num)
{
    /*snprintf(buf, MAX_EXPR-1, "%.20f", num);
    return;*/

    char n1[50], n2[50], *p;
    char string0[PRECISION+1], string9[PRECISION+1];
    int i, f = floor(fabs(num));

    if (fabs(num) >= 1e9)
    {
        snprintf(buf, MAX_EXPR-1, "%.*e", PRECISION, num);
        if ( (p = strchr(buf, 'e')) )
        {
            for (i = p-buf-1 ; i>=0 && buf[i]=='0' ; i--);
            if (i>=0 && buf[i]=='.')
                i--;
            strcpy(buf+i+1, p);
            return;
        }
    }

    for (i=0 ; i<PRECISION ; i++)
    {
        string0[i] = '0';
        string9[i] = '9';
    }
    string0[PRECISION] = '\0';
    string9[PRECISION] = '\0';

    snprintf(n2, MAX_EXPR-1, "%.20f", fabs(num)-f);
    strcpy(n2, n2+2);

    if ( (p=strstr(n2, string0)) )
        *p = '\0';
    else if ( (p=strstr(n2, string9)) )
    {
        if (p == n2)
            f += 1;
        else *(p-1) += 1;
        *p = '\0';
    }

    n2[PRECISION+2] = '\0';
    i = strlen(n2)-1;
    while (i>=0 && n2[i] == '0')
        i--;
    n2[i+1] = '\0';

    if (num>=0 || (!n2[0] && f<1))
        snprintf(n1, MAX_EXPR-1, "%d", f);
    else snprintf(n1, MAX_EXPR-1, "-%d", (int)f);
    //n1[PRECISION+2] = '\0';

    if (n2[0])
        snprintf(buf, MAX_EXPR-1, "%s.%s", n1, n2);
    else strcpy(buf, n1);
}

//Imprime un flottant à l'écran
void PrintFloat(double num)
{
    char buf[MAX_EXPR];
    SprintFloat(buf, num);
    printf("%s", buf);
}

//Imprime le type d'un objet dans une chaine
void SprintType(char buf[], Objet obj)
{
    switch (obj.type)
    {
        case ERREUR:
            strncpy(buf, "Erreur", MAX_EXPR-1);
            return;
        case VIDE:
            strncpy(buf, "Vide", MAX_EXPR-1);
            return;
        case COMPLEXE:
            strncpy(buf, "Complexe", MAX_EXPR-1);
            return;
        case FONCTION:
            strncpy(buf, "Fonction", MAX_EXPR-1);
            return;
        case MATRICE:
            strncpy(buf, "Matrice", MAX_EXPR-1);
            return;
        case UPLET:
            strncpy(buf, "Uplet", MAX_EXPR-1);
            return;
        case BOOLEEN:
            strncpy(buf, "Booleen", MAX_EXPR-1);
            return;
        case TEXTE:
            strncpy(buf, "Texte", MAX_EXPR-1);
            return;
        default:
            break;
    }

    strncpy(buf, "Inconnu", MAX_EXPR-1);
}

//Imprime le type d'un objet à l'écran
void PrintType(Objet obj)
{
    char buf[MAX_EXPR];
    SprintType(buf, obj);
    printf("%s", buf);
}

//Imprime le type d'une operation dans une chaine
void SprintTypeOp(char buf[], Operation op)
{
    if (EstOperateurBinaire(op))
        strncpy(buf, "Binaire", MAX_EXPR-1);
    else if (EstOperateurUD(op))
        strncpy(buf, "Unaire droite", MAX_EXPR-1);
    else if (EstOperateurUG(op))
        strncpy(buf, "Unaire gauche", MAX_EXPR-1);
    else strncpy(buf, "Inconnu", MAX_EXPR-1);
}

//Imprime le type d'un objet à l'écran
void PrintTypeOp(Operation op)
{
    char buf[MAX_EXPR];
    SprintTypeOp(buf, op);
    printf("%s", buf);
}


//Imprime un objet dans une chaine, sous sa forme d'entrée (par ex, une matrice sera imprimée sous la forme [[a,b][c,d]])
void SprintObjetStr(char buf[], Objet obj)
{
    int i,j;
    char buf2[MAX_EXPR];
    Objet obj2;

    switch (obj.type)
    {
        case UPLET:
        {
            int n = TailleUplet(obj);
            Objet *upl = UpletObj(obj);
            strcpy(buf, "(");
            for (i=0 ; i<n ; i++)
            {
                obj2 = upl[i];
                obj2.special = PERMANENT;
                SprintObjetStr(buf2, obj2);
                snprintf(buf, MAX_EXPR-1, "%s%s%c", buf, buf2, i+1<n ? ',' : ')');
            }
            break;
        }
        case MATRICE:
        {
            int l = LignesMat(obj),
                c = ColMat(obj);
            Objet **mat = MatriceObj(obj);
            strcpy(buf, "[");
            for (i=0 ; i<l ; i++)
            {
                strcat(buf, "[");
                for (j=0 ; j<c ; j++)
                {
                    obj2 = mat[i][j];
                    obj2.special = PERMANENT;
                    SprintObjetStr(buf2, mat[i][j]);
                    snprintf(buf, MAX_EXPR-1, "%s%s%c", buf, buf2, j+1<c ? ',' : ']');
                }
            }
            strcat(buf, "]");
            break;
        }
        default:
            obj2 = obj;
            obj2.special = PERMANENT;
            SprintObjet(buf, obj2);
            break;
    }

    return;
}


#ifdef MEMMGR
void FreeEverything(void)
{
    printf("\n");
    MEMMGR_freeEverything(1, 1);
}
#endif

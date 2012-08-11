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


#include "commandes.h"

Commande tabCommandes[] =
{
    //Les noms de commandes doivent etre en majuscules
    {"EXIT",    "Libere toute la memoire et quitte le programme\n\t[Sans argument]",                                    Exit    },
    {"HELP",    "Liste les commandes disponibles\n\t[Argument facultatif : nom partiel de la commande]",                Help    },
    {"RESET",   "Reinitialise toute la memoire\n\t[Sans argument]",                                                     Reset   },
    {"VARLIST", "Liste les variables actuellement en memoire\n\t[Argument facultatif : nom partiel de la variable]",    VarList },
    {"OPLIST",  "Liste les operateurs disponibles\n\t[Argument facultatif : nom partiel de l'operateur]",               OpList  },
    {"LOAD",    "Execute les instructions ecrites dans un fichier\n\t[Argument obligatoire : adresse du fichier]",      Load    },
    {"CLEAR",   "Efface completement l'ecran de la console\n\t[Sans argument]",                                         Clear   },

    {"", "", NULL}
};

//Exécute une commande avec arguments passée en paramètre
int ExecCommande(const char expr[])
{
    int i, r = CMDNONTROUVEE;
    char *p = strchr(expr, ' ');

    if (p)
        *p = '\0';

    for (i=0 ; r == CMDNONTROUVEE && tabCommandes[i].fonction ; i++)
    {
        if (tabCommandes[i].fonction && !StrcmpEx(expr, tabCommandes[i].nom))
            r = tabCommandes[i].fonction(p ? p+1 : NULL);
    }

    if (p)
        *p = ' ';
    return r;
}

//Dit si une expression est une commande valide (sans argument)
int EstCommande(const char expr[])
{
    int i;
    for (i=0 ; tabCommandes[i].fonction ; i++)
    {
        if (tabCommandes[i].fonction && !StrcmpEx(expr, tabCommandes[i].nom))
            return 1;
    }

    return 0;
}

//Copie dans nom la chaine args sans ses espaces si garderEsp = 0 et tout en majuscules si toUpper != 0
void ExtraireNom(const char args[], char nom[], int toUpper, int garderEsp)
{
    int i, j=0;
    if (args)
    {
        for (i=0 ; args[i] && i < MAX_EXPR; i++)
        {
            if (garderEsp || args[i] != ' ')
            {
                nom[j] = toUpper ? toupper(args[i]) : args[i];
                j++;
            }
        }
    }

    nom[j] = '\0';
    return;
}



//Commandes...
int Exit(const char args[])
{
    const char *p = args;  //Evite un warning sur "unused parameter"
    p++;

    return TERMINERPRGM;
}

//Listes les commandes dont le nom contient l'argument passé
int Help(const char args[])
{
    int i, c=0;
    char nom[MAX_EXPR];

    ExtraireNom(args, nom, 1, 0);

    printf("\n$ Liste des commandes\n\n");
    for (i=0 ; tabCommandes[i].fonction ; i++)
    {
        if (!args || strstr(tabCommandes[i].nom, nom))
        {
            c++;
            printf("  %s\n\t%s\n\n", tabCommandes[i].nom, tabCommandes[i].description);
        }
    }

    switch (c)
    {
        case 0:
            printf("  Aucune commande trouvee, verifiez votre argument\n\n");
            break;
        case 1:
            printf("  1 commande trouvee\n\n");
            break;
        default:
            printf("  %d commandes trouvees\n\n", c);
            break;
    }

    return CMDEXECUTEE;
}

//Remet à zéro toutes les variables enregistrées, efface les variables utilisateur
int Reset(const char args[])
{
    const char *p = args;  //Evite un warning sur "unused parameter"
    p++;

    Desinitialiser();
    InitialiserObjets();
    return CMDEXECUTEE;
}

//Liste les variables dont le nom contient l'argument passé
int VarList(const char args[])
{
    Objet *vars = RecupTabObj();
    int i, c=0;
    char type[MAX_EXPR],
         type2[MAX_EXPR],
         nom[MAX_EXPR];

    ExtraireNom(args, nom, 0, 0);

    printf("\n$ Liste des objets enregistres\n\n");
    for (i=0 ; i < MAX_OBJETS && vars[i].type != VIDE ; i++)
    {
        if (!args || strstr(vars[i].nom, nom))
        {
            c++;
            SprintType(type, vars[i]);
            if (vars[i].type == TEXTE)
            {
                if (vars[i].data)
                    SprintType(type2, TexteEstim(vars[i]));
                else strncpy(type2, "Aucune", MAX_EXPR-1);
                printf("  %s\n\t%s\n\t%s\n\tEstimation actuelle : %s\n\n", vars[i].nom, type, vars[i].description, type2);
            }
            else
            {
                printf("  %s\n\t%s\n\t%s\n", vars[i].nom, type, vars[i].description);
                if (vars[i].type == FONCTION && !IsUserVar(vars[i]))
                    printf("\t%s\n", ExprFn(vars[i]));
                printf ("\n");
            }
        }
    }

    switch (c)
    {
        case 0:
            printf("  Aucune variable trouvee, verifiez votre argument\n\n");
            break;
        case 1:
            printf("  1 variable trouvee\n\n");
            break;
        default:
            printf("  %d variables trouvees\n\n", c);
            break;
    }

    return CMDEXECUTEE;
}

//Liste les opérateurs dont le nom contient l'argument passé
int OpList(const char args[])
{
    Operation *ops = RecupTabOp();
    int i, c=0;
    char type[MAX_EXPR],
         nom[MAX_EXPR],
         nomOp[MAX_EXPR];

    ExtraireNom(args, nom, 1, 1);

    printf("\n$ Liste des operations disponibles\n\n");
    for (i=0 ; ops[i].type ; i++)
    {
        ExtraireNom(ops[i].nom, nomOp, 1, 1);
        if (!args || strstr(nomOp, nom))
        {
            c++;
            SprintTypeOp(type, ops[i]);
            printf("  %s : %s\n\t%s\n\t%s\n\tPriorite : %d\n\n", ops[i].nom, ops[i].operateur, type, ops[i].description, ops[i].priorite);
        }
    }

    switch (c)
    {
        case 0:
            printf("  Aucun operateur trouve, verifiez votre argument\n\n");
            break;
        case 1:
            printf("  1 operateur trouve\n\n");
            break;
        default:
            printf("  %d operateurs trouves\n\n", c);
            break;
    }

    return CMDEXECUTEE;
}

//Charge le fichier dont l'adresse est passée en argument et exécute les instructions qu'il contient
int nbFichiersOuverts = 0;
int Load(const char args[])
{
    FILE *fichier;
    char instruction[MAX_EXPR],
         instrInterdite[MAX_EXPR], *p;
    int i;

    if (!args)
    {
        printf("# Veuillez specifier un nom de fichier a charger\n\n");
        return CMDEXECUTEE;
    }

    if (!(fichier = fopen(args, "r")))
    {
        printf("# Impossible d'ouvrir le fichier, verifiez l'adresse\n\t-> \"%s\"\n\n", args);
        return CMDEXECUTEE;
    }

    if (nbFichiersOuverts >= MAX_FICHIERS_OUVERTS)
    {
        printf("# Impossible de charger encore un fichier\n\n");
        fclose(fichier);
        return CMDEXECUTEE;
    }

    nbFichiersOuverts++;
    sprintf(instrInterdite, "load %s", args);

    printf("\n$ Execution des instructions du fichier...\n\n");
    for (i=0 ; i < 50 ; i++)
        #if (OS == LINUX)
        printf("-");
        #else
        printf("%c", 196);
        #endif
    printf("\n\n");

    for (i=1 ; fgets(instruction, MAX_EXPR-1, fichier) ; i++)
    {
        if ( (p=strchr(instruction, '\n')) )
            *p = '\0';
        if (instruction[0])
        {
            printf("\r(%d)\n> %s\n", i, instruction);
            if (StrcmpEx(instrInterdite, instruction))
                Executer(instruction);
            else printf("# Chargement du meme fichier non autorise\n\n");
        }
    }

    printf("\r");
    for (i=0 ; i < 50 ; i++)
        #if (OS == LINUX)
        printf("-");
        #else
        printf("%c", 196);
        #endif
    printf("\n\n$ Execution terminee\n\n");

    fclose(fichier);
    nbFichiersOuverts--;
    return CMDEXECUTEE;
}


int Clear(const char args[])
{
    const char *p = args;  //Evite un warning sur "unused parameter"
    p++;

    #if (OS == LINUX)
    printf("\033[2J");
    #else
    system("cls");
    #endif

    return CMDEXECUTEE;
}


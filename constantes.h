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


#define CONSTANTES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#ifndef NO_MEMMGR
#define MEMMGR_PRINT_WARNINGS
#include "memmgr.h"   //Commentez cette ligne pour désactiver le Memory Manager
#endif

//Choix de l'OS
#define AUTO       0
#define WINDOWS    1
#define LINUX      2
#define MAC        2

#define OS         AUTO    //Changez la valeur !

#if (OS == AUTO)
#undef OS
#ifdef _WIN32
#define OS WINDOWS
#else
#define OS LINUX
#endif
#endif //OS==AUTO


//Constantes utiles
#define MAX_EXPR                1000
#define MAX_NOM                 50
#define MAX_MAT                 50
#define MAX_VAR                 26       //nombre de variables max pour une fonction à plusieurs variables
#define MAX_UPLET               100
#define MAX_OBJETS              200      //doit être strictement supérieur à MAX_VAR
#define MAX_FICHIERS_OUVERTS    20

#define PRECISION  10   //Précision des résultats affichés : 10e-(PRECISION)

#define FLECHE     26   //Le caractère "->" sous Windows...

#define PI         3.1415926535897932384626433832795028841971693993751058209  //Oui, c'est un peu exagéré, mais on s'en fiche pas mal.

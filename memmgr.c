/*

File distributed as part of the "Equaminator CMD" project - A mathematic parser with interface in "command line" mode
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


#define MEMMGR_NO_MACRO
#include "memmgr.h"

MemZone *tabMemZones = NULL;
unsigned int nbMemZones = 0;

#define zoneBegin(z)    ((z).pBegin)
#define zoneSize(z)     ((z).size)
#define zoneNumLine(z)  ((z).numLine)
#define zoneFileName(z) ((z).fileName)

int NewMemZone(void* p, size_t size, unsigned int numLine, const char fileName[]);
int DeleteMemZone(unsigned int i);


void* MEMMGR_malloc(size_t size, unsigned int numLine, const char fileName[])
{
    void* p = malloc(size);
    if (p && NewMemZone(p, size, numLine, fileName))
        return p;
    else return NULL;
}

void MEMMGR_free(void* p, int print, unsigned int numLine, const char fileName[])
{
    unsigned int i;
    int ok=0;
    char *pChar;

    for (i=0 ; i<nbMemZones ; i++)
    {
        void* pBegin = zoneBegin(tabMemZones[i]);
        if ((char*)pBegin < (char*)p && (char*)p < (char*)pBegin + zoneSize(tabMemZones[i]))
        {
            zoneSize(tabMemZones[i]) = (char*)p - (char*)pBegin;
            ok = 1;
        }
        else if (pBegin == p)
            ok = DeleteMemZone(i);
    }

    if (ok)
        free(p);
    else if (print)
    {
        while ( (pChar = strchr(fileName, '\\')) )
            fileName = pChar+1;
        printf("\nMEMMGR: Warning: (%s ; %d) tried to free non allocated memory at adress 0x%x\n", fileName, numLine, (unsigned int)p);
    }
}

void MEMMGR_freeEverything(int print, int wait)
{
    unsigned int i;
    char c;

    if (!nbMemZones)
    {
        if (print)
            printf("MEMMGR: No remaining allocated memory found, good job!\n");
    }
    else
    {
        for (i=0 ; i<nbMemZones ; i++)
        {
            free(zoneBegin(tabMemZones[i]));
            if (print)
                printf("MEMMGR: Freed %d bytes allocated by (%s ; %d).\n", zoneSize(tabMemZones[i]), zoneFileName(tabMemZones[i]), zoneNumLine(tabMemZones[i]));
        }

        if (tabMemZones)
            free(tabMemZones);
        tabMemZones = NULL;
        nbMemZones = 0;
    }

    while (wait && (c=getchar()) != EOF && c != '\n');
}

int MEMMGR_isPointerOK(void* p)
{
    unsigned int i;
    int ok=0;

    for (i=0 ; i<nbMemZones ; i++)
    {
        void* pBegin = zoneBegin(tabMemZones[i]);
        if ((char*)(pBegin) <= (char*)(p) && (char*)(p) < (char*)(pBegin) + zoneSize(tabMemZones[i]))
            ok = 1;
    }

    return ok;
}


int NewMemZone(void* p, size_t size, unsigned int numLine, const char fileName[])
{
    MemZone *tmz = malloc(sizeof(MemZone)*(nbMemZones+1));
    char *pChar;

    if (!tmz)
        return 0;

    if (tabMemZones)
    {
        memcpy(tmz, tabMemZones, sizeof(MemZone)*nbMemZones);
        free(tabMemZones);
    }

    tabMemZones = tmz;
    zoneBegin(tabMemZones[nbMemZones]) = p;
    zoneSize(tabMemZones[nbMemZones]) = size;
    zoneNumLine(tabMemZones[nbMemZones]) = numLine;
    while ( (pChar = strchr(fileName, '\\')) )
        fileName = pChar+1;
    strncpy(zoneFileName(tabMemZones[nbMemZones]), fileName, MEMMGR_MAX_FILENAME-1);
    nbMemZones++;

    return 1;
}

int DeleteMemZone(unsigned int i)
{
    MemZone *tmz = NULL;
    if (i>=nbMemZones)
        return 0;

    if (nbMemZones == 1)
    {
        free(tabMemZones);
        tabMemZones = NULL;
        nbMemZones = 0;
        return 1;
    }

    if (!(tmz = malloc(sizeof(MemZone)*(nbMemZones-1))))
        return 0;

    if (i>0)
        memcpy(tmz, tabMemZones, sizeof(MemZone)*i);
    if (i+1<nbMemZones)
        memcpy(&(tmz[i]), &(tabMemZones[i+1]), sizeof(MemZone)*(nbMemZones-(i+1)));
    free(tabMemZones);
    tabMemZones = tmz;
    nbMemZones--;

    return 1;
}

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


#ifndef MEMMGR

#define MEMMGR

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MEMMGR_MAX_FILENAME     25

typedef struct
{
    void* pBegin;
    size_t size;
    unsigned int numLine;
    char fileName[MEMMGR_MAX_FILENAME];
} MemZone;

void* MEMMGR_malloc(size_t size, unsigned int numLine, const char fileName[]);
void MEMMGR_free(void* p, int print, unsigned int numLine, const char fileName[]);
void MEMMGR_freeEverything(int print, int wait);
int MEMMGR_isPointerOK(void* p);

#ifndef MEMMGR_NO_MACRO
#define malloc(s)   MEMMGR_malloc(s,__LINE__,__FILE__)
#ifdef MEMMGR_PRINT_WARNINGS
#define free(p)     MEMMGR_free(p,1,__LINE__,__FILE__)
#else
#define free(p)     MEMMGR_free(p,0,__LINE__,__FILE__)
#endif
#endif

#endif //ifndef MEMMGR

/***************************************************************************
                          utils.h  -  description
                             -------------------
    begin                : Thu Oct 18 2001
    copyright            : (C) 2001 by Steeve Beaudin
    email                : steevebeaudin@netscape.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>

char * format_time(time_t cal_time);
void * xmalloc (size_t size);
void * xrealloc (void *ptr, size_t size);
char * savestring (const char *ptr, size_t len);
bool stringCompare(char *s1, char *s2);

//void  meminfo();

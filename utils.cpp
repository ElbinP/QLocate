/***************************************************************************
                          utils.c  -  description
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
#include "utils.h"

char * format_time(time_t cal_time)
{
    struct tm *time_struct;
    static char string[30];
    /* Put the calendar time into a structure  if type 'tm'.			*/
    time_struct=localtime(&cal_time);
    /* Build a formatted date from the  structure.				*/
    strftime(string, sizeof string, "%b %d %Y %H:%M:%S", time_struct);
    /* Return the date/time			*/
    return(string);
}

void * xmalloc (size_t size)
{
    register void *value = malloc (size);
    if (value == 0)
        fprintf ( stderr, "virtual memory exhausted\n");
    return value;
}

char * savestring (const char *ptr, size_t len)
{
    register char *value = (char *) xmalloc (len + 1);
    value[len] = '\0';
    return (char *) memcpy (value, ptr, len);
}

void * xrealloc (void *ptr, size_t size)
{
    register void *value = realloc (ptr, size);
    if (value == 0)
        fprintf ( stderr, "Virtual memory exhausted\n");
    return value;
}

bool stringCompare(char *s1, char *s2)
{
    bool Ret;

    if (strcmp(s1, s2))
    {
        Ret = 0;
    }
    else
    {
        Ret = 1;
    }

    return (Ret);
}

/*        // the end of files is useful for debug , uncomment before use....
void  meminfo()
{
	struct mallinfo themem;

	themem = mallinfo();

	printf("============== meminfo =============\n");
	printf("total space allocated from system : %d\n",themem.arena);
	printf("number of non-inuse chunks : %d\n",themem.ordblks);
	printf("number of mmapped regions : %d\n",themem.hblks);
	printf("total space in mmapped regions : %d\n",themem.hblkhd);
	printf("total allocated space : %d\n",themem.uordblks);
	printf("total non-inuse space : %d\n",themem.fordblks);
	printf("top-most, releasable (via malloc_trim) space : %d\n",themem.keepcost);

}
*/

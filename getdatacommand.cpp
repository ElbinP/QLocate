/***************************************************************************
                          getdatacommand.cpp  -  description
                             -------------------
    begin                : Thu Nov 22 2001
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

#include "getdatacommand.h"

getdatacommand::getdatacommand(QListWidget *ListBox, QString Query)
{
    inRun = FALSE;

    theListBoxResults = ListBox;
    theQuery = Query;
}

getdatacommand::~getdatacommand()
{
}

void getdatacommand::run()
{
    FILE *fpipe;
    char* line = 0;
    size_t len = 0;
    QString string;

    if ( inRun != TRUE )
    {
        inRun = TRUE;

        fpipe = popen(theQuery.toUtf8(), "r");
        if(fpipe != NULL)
        {
            int findfile = 0;
            // delimiter is null character.
            while( getdelim(&line, &len, 0, fpipe) != -1 )
            {
                findfile++;
                string = QString::fromUtf8(line);
                theListBoxResults->addItem( string );
            }
            delete(line);
            pclose(fpipe);
            emit queryComplete(findfile);
        }

        inRun = FALSE;
    }
}


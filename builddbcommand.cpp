/***************************************************************************
                          builddbcommand.cpp  -  description
                             -------------------
    begin                : Sat Feb 16 2002
    copyright            : (C) 2002 by Steeve Beaudin
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

#include "builddbcommand.h"

builddbcommand::builddbcommand( QString command )
{
    inRun = FALSE;
    theCommand = command;
}

builddbcommand::~builddbcommand()
{
}

void builddbcommand::run()
{
    FILE* fpipe;
    char* line = 0;
    size_t len = 0;
    QString string;

    if ( inRun != TRUE )
    {
        inRun = TRUE;

        fpipe = popen( theCommand.toUtf8(), "r");
        while( getline(&line, &len, fpipe) != -1 )
        {
            fprintf(stdout,"%s", line);
        }
        delete(line);
        pclose(fpipe);

        inRun = FALSE;
    }
}

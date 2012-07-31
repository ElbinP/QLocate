/***************************************************************************
                          builddbcommand.h  -  description
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

#pragma once

#define QT_THREAD_SUPPORT

#include <qapplication.h>
#include <qobject.h>
#include <qthread.h>
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

class builddbcommand : public QThread
{

public:
    builddbcommand( QString command );
    ~builddbcommand();

private:
    bool inRun;
    QString theCommand;

protected:
    virtual void run();

};



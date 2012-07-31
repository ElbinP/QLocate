/***************************************************************************
                          filestat.h  -  description
                             -------------------
    begin                : Fri Oct 12 2001
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

#include <qapplication.h>
#include <qstring.h>
#include <qobject.h>
#include <qstring.h>
#include <qfileinfo.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include "utils.h"
#include "enviro.h"

class filestat : public QObject
{

public:
    filestat( enviro * env , const QString& filename);
    ~filestat();
    unsigned short getOwner();
    unsigned short getGroup();
    unsigned short getDevice();
    unsigned short getMode();
    unsigned long getSize();
    unsigned long getInode();
    QString getOwnerName();
    QString getGroupName();
    QString getAccessTime();
    QString getModifTime();
    QString getChangeTime();
    QString getFileType();

private:
    enviro * theenv;
    struct stat stat_p;
    QString thefile;

};



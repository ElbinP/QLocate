/***************************************************************************
                          rpmstat.h  -  description
                             -------------------
    begin                : Sat Oct 27 2001
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
#include <qobject.h>
#include <qstring.h>
#include <qregexp.h>
#include <libgen.h>
#include <limits.h>
#include "utils.h"
#include "enviro.h"

class rpmstat : public QObject
{

public:
    rpmstat( enviro * env, const QString& filename);
    ~rpmstat();

    QString getPackage();
    QString getMd5sum();
    QString getOwnerName();
    QString getGroup();
    QString getSymlink();
    unsigned short getMode();
    unsigned short getRDev();
    unsigned long getSize();
    QString getModifTime();
    bool getIsConfig();
    bool getIsDoc();
    bool isOwnedByPkg();

private:
    enviro *  theenv;
    QString thefile;
    void package();
    void retrieveDump();
    QString nopackage;

    struct fileinfo
    {
        QString packageName;
        QString md5sum;
        QString owner;
        QString group;
        QString symlink;
        unsigned short mode;
        unsigned short rdev;
        unsigned long size;
        unsigned long mtime;
        bool isconfig;
        bool isdoc;
        bool ownedbypkg;
    };
    fileinfo  rpminfo;

};



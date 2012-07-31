/***************************************************************************
                          filestat.cpp  -  description
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
#include "filestat.h"
#include "enviro.h"
#include <qregexp.h>

filestat::filestat( enviro * env, const QString& filename)
{
    thefile = filename;
    theenv = env;
    if ( -1 ==  lstat (thefile.toUtf8(), &stat_p))
    {
        fprintf(stderr, "%s", qPrintable(qApp->translate( "filestat", "Error occoured attempting to stat %1\n").arg(thefile)) );
    }
}

filestat::~filestat()
{
}

QString filestat::getOwnerName()
{
    struct passwd *passwdres;

    passwdres = getpwuid(stat_p.st_uid);
    if (passwdres == NULL)
    {
        fprintf( stderr, "%s", qPrintable(qApp->translate( "filestat", "This is a result of the user missing or a corrupted passwd file.\n")) );
        return QString( qApp->translate( "filestat", "No Name Define") );
    }
    return QString(passwdres->pw_name);
}

QString filestat::getGroupName()
{
    struct group *grpres;

    grpres = getgrgid(stat_p.st_gid);
    if (grpres == NULL)
    {
        fprintf( stderr, "%s", qPrintable(qApp->translate( "filestat", "This is a result of the group missing or a corrupted group file.\n")) );
        return QString( qApp->translate( "filestat", "No Name Define") );
    }
    return grpres->gr_name;
}


QString filestat::getFileType()
{

    FILE *fp;
    char* line = 0;			/* line of data from unix command*/
    size_t len = 0;
    QString strRep;
    QString strCommand;
    QString parsethefile;

    if ( QFileInfo(thefile).isReadable() )
    {
        if ( thefile.indexOf('$') != -1 || thefile.indexOf('"')  != -1 )
        {
            // Lock $ and " in filename before run command
            QString tmpStr = thefile;
            tmpStr.replace( QRegExp( QString("[$]") ) , QString("\\$") );   // $ => end of line
            tmpStr.replace( QRegExp( QString("\"") ) , QString("\\\"") );
            parsethefile= tmpStr;
        }
        else
        {
            parsethefile = thefile;
        }

        strCommand = theenv->getPathFile() + QString(" -b \"") + parsethefile + QString("\"");

        fp = popen( strCommand.toUtf8(), "r");		/* Issue the command.		*/
        while( getline(&line, &len, fp) != -1 )
        {
            strRep +=  QString::fromUtf8( line );
        }
        delete(line);
        pclose(fp);

        return strRep.left(strRep.length() -1 );
    }
    else
    {
        return QString( qApp->translate( "filestat", "have no right to read") );
    }
}

unsigned short filestat::getGroup()
{
    return stat_p.st_gid;
}

unsigned short filestat::getOwner()
{
    return stat_p.st_uid;
}

unsigned long filestat::getSize()
{
    return stat_p.st_size;
}

unsigned short filestat::getMode()
{
    return stat_p.st_mode;
}

QString filestat::getAccessTime()
{
    return QString(format_time(stat_p.st_atime));
}

QString filestat::getModifTime()
{
    return QString(format_time(stat_p.st_mtime));
}

QString filestat::getChangeTime()
{
    return QString(format_time(stat_p.st_ctime));
}


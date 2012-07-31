/***************************************************************************
                          rpmstat.cpp  -  description
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

#include "rpmstat.h"
#include <QStringListIterator>
#include <QVariant>

rpmstat::rpmstat(enviro *  env, const QString& filename )
{
    nopackage = qApp->translate( "rpmstat", "Not owned by any package." );
    thefile = filename;
    theenv = env;
    rpminfo.ownedbypkg = false;
    package();

    if ( rpminfo.packageName != nopackage )
    {
        retrieveDump();
    }
}

rpmstat::~rpmstat()
{}

void rpmstat::package()
{
    QString commandstr;
    QString parsethefile;
    const QString optioncommand = " -qf \"";
    const QString endcommand = "\" 2>&1";

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

    commandstr = theenv->getPathRpm() +  optioncommand + parsethefile + endcommand;

    FILE *fp;
    char* line = 0;                     /* line of data from unix command*/
    size_t len = 0;
    QString buff;

    fp = popen(commandstr.toUtf8(), "r");
    if( getline(&line, &len, fp) != -1 )
        buff =  QString::fromUtf8(line);
    delete(line);
    int exitStatus = pclose(fp);

    if( exitStatus == 0 )
    {
        rpminfo.packageName =  buff.left(buff.length() - 1 );  // package name - \n
    }
    else
    {
        rpminfo.packageName = nopackage;
    }
}

void rpmstat::retrieveDump()
{
    const int RPM_DUMP_COLUMNS = 11; // Number of columns returned from rpm -q --dump <packagename>

    // The column index of the various information in rpm dump output.
    int pathLastColumnIndex = 0; // If path has space(s) this variable will store the index of
                                 // the last column that contains the path.
    const int SIZE_COLUMN_OFFSET = 1;
    const int MTIME_COLUMN_OFFSET = 2;
    const int FILEDIGEST_COLUMN_OFFSET = 3;
    const int MODE_COLUMN_OFFSET = 4;
    const int OWNER_COLUMN_OFFSET = 5;
    const int GROUP_COLUMN_OFFSET = 6;
    const int ISCONFIG_COLUMN_OFFSET = 7;
    const int ISDOC_COLUMN_OFFSET = 8;
    const int RDEV_COLUMN_OFFSET = 9;
    const int SYMLINK_COLUMN_OFFSET = 10;

    FILE *fp;
    char* line = 0;			/* line of data from unix command*/
    QString qline;
    size_t len = 0;

    QString commandstr;
    const QString optioncommand = " -q --dump \"";
    const QString endcommand = "\"  2>/dev/null";

    commandstr = theenv->getPathRpm() + optioncommand + rpminfo.packageName + endcommand;

    fp = popen(commandstr.toUtf8(), "r");
    while( getline(&line, &len, fp) != -1 )
    {
        qline = QString::fromUtf8(line).trimmed();

        // Split the dump information to retrieve the individual columns
        QStringList stringList = qline.split(" ");
        // If we didn't get the required number of columns
        // then we didn't probably get a valid rpm dump output.
        if(stringList.count() < RPM_DUMP_COLUMNS)
            break;

        QString fullname;
        // If the pathname contains space(s) we get more columns
        // than RPM_DUMP_COLUMNS
        if(stringList.count() > RPM_DUMP_COLUMNS)
        {
            int numberOfPathNameColumns = (stringList.count() - RPM_DUMP_COLUMNS) + 1;
            for(int i=0; i<numberOfPathNameColumns; ++i)
            {
                fullname += (stringList[i] + " ");
            }
            fullname = fullname.left(fullname.length() - 1);
            pathLastColumnIndex = numberOfPathNameColumns - 1;
        }
        else
        {
            fullname = stringList[0];
            pathLastColumnIndex = 0;
        }

        if ( fullname == thefile )
        {
            rpminfo.ownedbypkg = true;

            rpminfo.rdev = stringList[pathLastColumnIndex + RDEV_COLUMN_OFFSET].toUShort();
            rpminfo.isdoc = stringList[pathLastColumnIndex + ISDOC_COLUMN_OFFSET] == "0" ? false : true;
            rpminfo.isconfig = stringList[pathLastColumnIndex + ISCONFIG_COLUMN_OFFSET] == "0" ? false : true;
            rpminfo.group = stringList[pathLastColumnIndex + GROUP_COLUMN_OFFSET];
            rpminfo.owner = stringList[pathLastColumnIndex + OWNER_COLUMN_OFFSET];
            rpminfo.mode = stringList[pathLastColumnIndex + MODE_COLUMN_OFFSET].toUShort(0, 8); // base 8
            rpminfo.md5sum = stringList[pathLastColumnIndex + FILEDIGEST_COLUMN_OFFSET];
            rpminfo.mtime = stringList[pathLastColumnIndex + MTIME_COLUMN_OFFSET].toULong();
            rpminfo.size = stringList[pathLastColumnIndex + SIZE_COLUMN_OFFSET].toULong();

            break ;
        }
    }
    delete(line);
    pclose(fp);
}

QString rpmstat::getPackage()
{
    return rpminfo.packageName;
}

QString rpmstat::getMd5sum()
{
    return rpminfo.md5sum;
}

QString rpmstat::getOwnerName()
{
    return rpminfo.owner;
}

QString rpmstat::getGroup()
{
    return rpminfo.group;
}

QString rpmstat::getSymlink()
{
    return rpminfo.symlink;
}

unsigned short rpmstat::getMode()
{
    return rpminfo.mode;
}

unsigned short rpmstat::getRDev()
{
    return rpminfo.rdev;
}

unsigned long rpmstat::getSize()
{
    return rpminfo.size;
}

QString rpmstat::getModifTime()
{
    return QString(format_time(rpminfo.mtime));
}

bool rpmstat::getIsConfig()
{
    return rpminfo.isconfig;
}

bool rpmstat::getIsDoc()
{
    return rpminfo.isdoc;
}

bool rpmstat::isOwnedByPkg()
{
    return rpminfo.ownedbypkg;
}

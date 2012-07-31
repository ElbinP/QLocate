/***************************************************************************
                          enviro.cpp  -  description
                             -------------------
    begin                : Thu Oct 25 2001
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

#include "enviro.h"
#include <QTextStream>

const QString enviro::updatedb("updatedb");
const QString enviro::locate("locate");
const QString enviro::file("file");
const QString enviro::rpm("rpm");
const QString enviro::applicationName("qlocate");
const QString enviro::organizationName("qlocate");

const QString enviro::updatedbpath = "updatedbpath";
const QString enviro::locatepath = "locatepath";
const QString enviro::rpmpath = "rpmpath";
const QString enviro::filepath = "filepath";
const QString enviro::startindexfrom = "startindexfrom";
const QString enviro::prunepaths = "prunepaths";
const QString enviro::security = "security";
const QString enviro::verbose = "verbose";
const QString enviro::database = "database";
const QString enviro::prunefs = "prunefs";
const QString enviro::usedefaultdb = "usedefaultdb";
const QString enviro::dbtoquery = "dbtoquery";
const QString enviro::queryusingregex = "queryusingregex";
const QString enviro::querycasesensitive = "querycasesensitive";
const QString enviro::queryquietmode = "queryquietmode";
const QString enviro::querybasenames = "querybasenames";
const QString enviro::queryresultstoshow = "queryresultstoshow";

enviro::enviro()
{
    qsettings = new QSettings(organizationName, applicationName, this);

    if ( getPathLocate().isEmpty()
        || ! QFileInfo(getPathLocate()).exists() )
    {
        fprintf( stderr, "%s", qPrintable(qApp->translate( "enviro", "Impossible to run ! locate command not find, add to PATH\n")) );
        exit(1);
    }

}

enviro::~enviro()
{
}


QString enviro::getFromPath(const QString& filename )
{
    // return full path and name , first occurence of filename in path.
    // return "" if filename not in path.
    QString returnValue;
    const char * Path="PATH";
    QString allPaths;

    if ( !(allPaths = QString(getenv(Path))).isEmpty())
    {
        QStringList pathList = allPaths.split(QRegExp(":"), QString::SkipEmptyParts);
        QString path;

        for (int i=0; i < pathList.size(); ++i)
        {
            path = pathList.at(i);
            path = QDir(path).absolutePath();

            if ( QFileInfo(path + QString("/") + filename).exists() )
            {
                returnValue = path + QString("/") + filename;
                break;
            }
        }
    }

    return returnValue;
}

QString enviro::getPathUpdateDB()
{
    if(qsettings->value(updatedbpath).toString().isEmpty())
        qsettings->setValue(updatedbpath, getFromPath(updatedb));

    return qsettings->value(updatedbpath).toString();
}

QString enviro::getPathFile()
{
    if(qsettings->value(filepath).toString().isEmpty())
        qsettings->setValue(filepath, getFromPath(file));
    
    return qsettings->value(filepath).toString();
}

QString enviro::getPathLocate()
{
    if(qsettings->value(locatepath).toString().isEmpty())
        qsettings->setValue(locatepath, getFromPath(locate));

    return qsettings->value(locatepath).toString();
}

QString enviro::getPathRpm()
{
    if(qsettings->value(rpmpath).toString().isEmpty())
        qsettings->setValue(rpmpath, getFromPath(rpm));

    return qsettings->value(rpmpath).toString();
}

QString enviro::getStartIndexFrom()
{
    if(qsettings->value(startindexfrom).toString().isEmpty())
        qsettings->setValue(startindexfrom, QDir::homePath() + QDir::separator());

    return qsettings->value(startindexfrom).toString();
}

void enviro::setStartIndexFrom(const QString& from)
{
    qsettings->setValue(this->startindexfrom, from);
}

QString enviro::getPrunepaths()
{
    if(qsettings->value(prunepaths).isNull())
        qsettings->setValue(prunepaths, "/afs,/mnt,/net,/proc,/tmp,/usr/tmp,/var/tmp");

    return qsettings->value(prunepaths).toString();
}

void enviro::setPrunepaths(const QString& prunePaths)
{
    qsettings->setValue(this->prunepaths, prunePaths);
}

QString enviro::getDatabase()
{
    if(qsettings->value(database).toString().isEmpty())
        qsettings->setValue(database, QDir::homePath() +
        QDir::separator() + ".qlocate" + QDir::separator() + "slocate.db");

    return qsettings->value(database).toString();
}

void enviro::setDatabase(const QString& database)
{
    qsettings->setValue(this->database, database);
}

QString enviro::getPrunefs()
{
    if(qsettings->value(prunefs).isNull())
        qsettings->setValue(prunefs, "devfs,devpts,iso9660,ncpfs,nfs,proc,smbfs,supermount,udf,usbdevfs,vfat");

    return qsettings->value(prunefs).toString();
}

void enviro::setPrunefs(const QString& pruneFs)
{
    qsettings->setValue(this->prunefs, pruneFs);
}

QString enviro::getDBToQuery()
{
    if(qsettings->value(dbtoquery).toString().isEmpty())
        qsettings->setValue(dbtoquery, QDir::homePath() +
        QDir::separator() + ".qlocate" + QDir::separator() + "slocate.db");

    return qsettings->value(dbtoquery).toString();
}

void enviro::setDBToQuery(const QString& dbToQuery)
{
    qsettings->setValue(this->dbtoquery, dbToQuery);
}

bool enviro::isSecurity()
{
    if(qsettings->value(security).isNull())
        qsettings->setValue(security, false);

    return qsettings->value(security).toBool();
}

void enviro::setSecurity(bool isSecurity)
{
    qsettings->setValue(this->security, isSecurity);
}

bool enviro::isVerbose()
{
    if(qsettings->value(verbose).isNull())
        qsettings->setValue(verbose, false);

    return qsettings->value(verbose).toBool();
}

void enviro::setVerbose(bool isVerbose)
{
    qsettings->setValue(this->verbose, isVerbose);
}

bool enviro::isUseDefaultDB()
{
    if(qsettings->value(usedefaultdb).isNull())
        qsettings->setValue(usedefaultdb, true);

    return qsettings->value(usedefaultdb).toBool();
}

void enviro::setUseDefaultDB(bool useDefaultDb)
{
    qsettings->setValue(this->usedefaultdb, useDefaultDb);
}

bool enviro::isQueryBaseNames()
{
    if(qsettings->value(querybasenames).isNull())
        qsettings->setValue(querybasenames, false);

    return qsettings->value(querybasenames).toBool();
}

void enviro::setQueryBaseNames(bool isQueryBaseNames)
{
    qsettings->setValue(querybasenames, isQueryBaseNames);
}

bool enviro::isQueryQuietMode()
{
    if(qsettings->value(queryquietmode).isNull())
        qsettings->setValue(queryquietmode, true);
    
    return qsettings->value(queryquietmode).toBool();
}

void enviro::setQueryQuietMode(bool isQueryQuietMode)
{
    qsettings->setValue(this->queryquietmode, isQueryQuietMode);
}

bool enviro::isQueryCaseSensitive()
{
    if(qsettings->value(querycasesensitive).isNull())
        qsettings->setValue(querycasesensitive, false);
    
    return qsettings->value(querycasesensitive).toBool();
}


void enviro::setQueryCaseSensitive(bool isQueryCaseSensitive)
{
    qsettings->setValue(this->querycasesensitive, isQueryCaseSensitive);
}

bool enviro::isQueryUsingRegex()
{
    if(qsettings->value(queryusingregex).isNull())
        qsettings->setValue(queryusingregex, false);
    
    return qsettings->value(queryusingregex).toBool();
}

void enviro::setQueryUsingRegex(bool isQueryUsingRegex)
{
    qsettings->setValue(this->queryusingregex, isQueryUsingRegex);
}

int enviro::getResultsToShow()
{
    if(qsettings->value(queryresultstoshow).isNull())
        qsettings->setValue(queryresultstoshow, 500);
    
    return qsettings->value(queryresultstoshow).toInt();
}

void enviro::setResultsToShow(int resultsToShow)
{
    qsettings->setValue(this->queryresultstoshow, resultsToShow);
}

QPoint enviro::getMainWindowPosition()
{
    qsettings->beginGroup("mainwindow");
    QVariant position = qsettings->value("position");
    qsettings->endGroup();

    if(position.isNull())
        throw("mainwindow/position not set");
    else
        return position.toPoint();
}

void enviro::setMainWindowPosition(QPoint point)
{
    qsettings->beginGroup("mainwindow");
    qsettings->setValue("position", point);
    qsettings->endGroup();
}

QSize enviro::getMainWindowSize()
{
    qsettings->beginGroup("mainwindow");
    QVariant size = qsettings->value("size");
    qsettings->endGroup();
    
    if(size.isNull())
        throw("mainwindow/size not set");
    else
        return size.toSize();
}

void enviro::setMainWindowSize(QSize size)
{
    qsettings->beginGroup("mainwindow");
    qsettings->setValue("size", size);
    qsettings->endGroup();
}

/***************************************************************************
                          enviro.h  -  description
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

#pragma once

#include <qapplication.h>
#include <qobject.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qdir.h>
#include <qregexp.h>
#include <stdio.h>
#include <stdlib.h>
#include <QSettings>
#include <QSize>
#include <QPoint>

class enviro : public QObject
{
    Q_OBJECT
public:
    enviro();
    ~enviro();

    QString getPathUpdateDB();
    QString getPathLocate();
    QString getPathRpm();
    QString getPathFile();
    QString getStartIndexFrom();
    QString getPrunepaths();
    QString getDatabase();
    QString getPrunefs();
    QString getDBToQuery();
    bool isSecurity();
    bool isVerbose();
    bool isUseDefaultDB();
    bool isQueryUsingRegex();
    bool isQueryCaseSensitive();
    bool isQueryQuietMode();
    bool isQueryBaseNames();
    int getResultsToShow();
    QSize getMainWindowSize();
    QPoint getMainWindowPosition();

    void setStartIndexFrom(const QString& startIndexFrom);
    void setPrunepaths(const QString& prunePaths);
    void setDatabase(const QString& database);
    void setPrunefs(const QString& pruneFs);
    void setDBToQuery(const QString& dbToQuery);
    void setSecurity(bool isSecurity);
    void setVerbose(bool isVerbose);
    void setUseDefaultDB(bool useDefaultDb);
    void setQueryUsingRegex(bool isQueryUsingRegex);
    void setQueryCaseSensitive(bool isQueryCaseSensitive);
    void setQueryQuietMode(bool isQueryQuietMode);
    void setQueryBaseNames(bool isQueryBaseNames);
    void setResultsToShow(int resultsToShow);
    void setMainWindowSize(QSize size);
    void setMainWindowPosition(QPoint point);

    static const QString applicationName;
    static const QString organizationName;

private:
    QString getFromPath(const QString& filename);
    
    static const QString updatedb;
    static const QString locate;
    static const QString file;
    static const QString rpm;

    // Config keys
    static const QString updatedbpath;
    static const QString locatepath;
    static const QString rpmpath;
    static const QString filepath;
    static const QString startindexfrom;
    static const QString prunepaths;
    static const QString security;
    static const QString verbose;
    static const QString database;
    static const QString prunefs;
    static const QString usedefaultdb;
    static const QString dbtoquery;
    static const QString queryusingregex;
    static const QString querycasesensitive;
    static const QString queryquietmode;
    static const QString querybasenames;
    static const QString queryresultstoshow;
    QSettings* qsettings;

private slots:

};


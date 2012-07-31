/***************************************************************************
                          getdatacommand.h  -  description
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

#pragma once

#define QT_THREAD_SUPPORT

#include <qapplication.h>
#include <qlineedit.h>
#include <qobject.h>
#include <qthread.h>
#include <QListWidget>
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>



class getdatacommand : public QThread
{
    Q_OBJECT
public:
    getdatacommand(QListWidget *ListBox, QString Query);
    ~getdatacommand();

signals:
    void queryComplete(int resultsFound);

private:
    bool inRun;

    QListWidget *theListBoxResults;
    QString theQuery;

protected:
    virtual void run();

};



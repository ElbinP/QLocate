/***************************************************************************
                          baseformimpl.h  -  description
                             -------------------
    begin                : Sat Oct 6 2001
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
#include <qobject.h>
#include <QResizeEvent>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qtimer.h>
#include <qstring.h>
#include <qclipboard.h>
#include <qcursor.h>
#include <qradiobutton.h>
#include <qpoint.h>
#include <qsplitter.h>
#include <qnamespace.h>
#include <qregexp.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qthread.h>
#include <limits.h>
#include <QDialog>
#include "themenu.h"
#include "enviro.h"
#include "getdatacommand.h"

namespace Ui
{
	class BaseForm;
}

class BaseForm : public QDialog
{
    Q_OBJECT
public:
    BaseForm();
    virtual ~BaseForm();

public slots:
    virtual void copy_slot();
    virtual void selectAll_slot();
    virtual void deselectAll_slot();
    virtual void setup_slot();
    virtual void openFile_slot();

protected:
    virtual void resizeEvent(QResizeEvent * event);
    virtual void moveEvent(QMoveEvent * event);

private slots:
    virtual void setQueryCaseSensitive();
    virtual void setQueryUsingRegex();
    virtual void setQueryQuietMode();
    virtual void setQueryBaseNames();
    virtual void setResultsToShow(int resultsToShow);
    virtual void sendOrder_slot();
    virtual void statusbar1Changed_slot();
    virtual void statusbar2Changed_slot(int findfile);
    virtual void regexChanged_slot();
    virtual void popEditMenu_slot();
    virtual void sortResultsAscending_slot();
    virtual void sortResultsDescending_slot();
    virtual void displayFileInfo_slot();

private:
    QString buildQueryString();
    themenu *menu;
    enviro *envinfo;
    getdatacommand *threadGetData;
    int resultsshow;
    bool quietmode;
    bool inRun;
    static const int XMARGE = 6;
    static const int SPACEMIN = 2;
    Ui::BaseForm* ui;

};



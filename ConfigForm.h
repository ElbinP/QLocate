/***************************************************************************
                          configformimpl.h  -  description
                             -------------------
    begin                : Wed Jan 30 2002
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


#include <qwidget.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qobject.h>
#include <qdialog.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qinputdialog.h>
#include <qtooltip.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <string.h>
#include "builddbcommand.h"
#include "enviro.h"

namespace Ui
{
	class ConfigForm;
}

class ConfigForm : public QDialog
{
    Q_OBJECT
public:
    ConfigForm(enviro* env, QWidget * parent);
    virtual ~ConfigForm();

public slots:
    virtual void changeSearchIn_slot();
    virtual void changeStartIndexFrom_slot();
    virtual void changeDBLocate_slot();
    virtual void addDirExclude_slot();
    virtual void deleteDirExclude_slot();
    virtual void addFsExclude_slot();
    virtual void deleteFsExclude_slot();
    virtual void save_slot();
    virtual void createDB_slot();
    virtual void setActiveSearchIn_slot();
    virtual void activeSaveButton_slot();

private:
    enviro* theenv;
    builddbcommand *threadBuildDB;
    bool validateDBLocate();
    bool validateStartIndexFrom();
    bool validateSearchIn();
    Ui::ConfigForm* ui;

private slots:

protected:

};



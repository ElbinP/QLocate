/***************************************************************************
                          themenu.h  -  description
                             -------------------
    begin                : Fri Oct 19 2001
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

#include <qwidget.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

class themenu : public QWidget
{
    Q_OBJECT
public:
    themenu(QWidget *parent=0, const char *name=0);
    ~themenu();

    QMenu *editMenu;
    QMenu *optionMenu;

private:
    QMenuBar   *mainMenu;
    QMenu *fileMenu;
    QMenu *helpMenu;

public slots:

private slots:
    void aboutQLocate_slot();
    void aboutQt_slot();
    void aboutLocate_slot();

};



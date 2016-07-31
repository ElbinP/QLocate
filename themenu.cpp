/***************************************************************************
                          themenu.cpp  -  description
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

#include "themenu.h"
#include "iconsdata.h"
#include <QLayout>
#include <QPixmap>
#include <QMenu>

themenu::themenu(QWidget *parent, const char *name ) : QWidget(parent)
{
    QAction* action;
    QPixmap tmpIcon;

    mainMenu = new QMenuBar(parent);

    fileMenu =  mainMenu->addMenu( tr("&File") );
    tmpIcon = QPixmap(quit_xpm);
    action = fileMenu->addAction( tmpIcon,tr("&Quit"),  parent, SLOT(reject()), Qt::CTRL+Qt::Key_Q );
    action->setWhatsThis( tr("Quit the application") );

    editMenu = mainMenu->addMenu( tr("&Edit") );
    tmpIcon = QPixmap(editcopy_xpm);
    action = editMenu->addAction( tmpIcon, tr("&Copy"), parent, SLOT(copy_slot()), Qt::CTRL+Qt::Key_C );
    action->setWhatsThis( tr("Copy data into clipboard") );
    tmpIcon = QPixmap(klipper_xpm);
    action = editMenu->addAction( tmpIcon, tr("Select &All"), parent, SLOT(selectAll_slot()), Qt::CTRL+Qt::Key_A );
    action->setWhatsThis( tr("Select all file from results") );
    action = editMenu->addAction( tr("&Deselect All"), parent, SLOT(deselectAll_slot()), Qt::CTRL+Qt::Key_D );
    action->setWhatsThis( tr("Deselect all files from the result") );
    action = editMenu->addAction( tr("&Open File"), parent, SLOT(openFile_slot()), Qt::CTRL+Qt::Key_O );
    action->setWhatsThis( tr("Open File") );


    optionMenu =  mainMenu->addMenu( tr("&Option") );
    tmpIcon = QPixmap(setup_xpm);
    action = optionMenu->addAction( tmpIcon, tr("&Setting"),  parent, SLOT(setup_slot()), Qt::CTRL+Qt::Key_P);
    action->setWhatsThis( tr("Set the database to search in and create slocate database.") );

    helpMenu =  mainMenu->addMenu( tr("&Help") );
    tmpIcon = QPixmap(info_xpm );
    action = helpMenu->addAction( tmpIcon,tr("About &Locate"),  this, SLOT(aboutLocate_slot()), Qt::CTRL+Qt::Key_L );
    action->setWhatsThis( tr("Info on locate") );
    action = helpMenu->addAction( tmpIcon,tr("About QL&ocate"),  this, SLOT(aboutQLocate_slot()), Qt::CTRL+Qt::Key_O );
    action->setWhatsThis( tr("Info on this application") );
    action = helpMenu->addAction( tmpIcon, tr("About Q&T"),  this, SLOT(aboutQt_slot()), Qt::CTRL+Qt::Key_T );
    action->setWhatsThis( tr("Info on QT Library") );

    parent->layout()->setMenuBar(mainMenu);
    mainMenu->show();
}

themenu::~themenu()
{}

void themenu::aboutQLocate_slot()
{
    QMessageBox::about( this, tr("QLocate info."),
                        tr("QLocate is free software; you can redistribute it and/or modify it \n"
                           "under the terms of the GNU General Public License.\n\n"
                           "See http://www.geocities.com/rebusnet/qlocate/index.html"));
}

void themenu::aboutQt_slot()
{
    QMessageBox::aboutQt( this, tr("Qt Application Example") );
}

void themenu::aboutLocate_slot()
{
    FILE *fp;
    char* line = 0;			/* line of data from unix command*/
    size_t len = 0;
    QString locateversion;

    fp = popen("locate -V", "r");		/* Issue the command.		*/
    while( getline(&line, &len, fp) != -1 )
    {
        locateversion +=  QString::fromUtf8( line );
    }
    delete(line);
    pclose(fp);

    QMessageBox::about( this, tr("Locate version."), locateversion );
}

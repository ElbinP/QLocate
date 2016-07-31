/***************************************************************************
                          BaseForm.cpp  -  description
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

#include "BaseForm.h"
#include <Qt/qdesktopservices.h>
#include <Qt/qdebug.h>
#include <Qt/qurl.h>
#include "qlicon.h"
#include "filestat.h"
#include "rpmstat.h"
#include "themenu.h"
#include "enviro.h"
#include "getdatacommand.h"
#include "ConfigForm.h"
#include <QResizeEvent>
#include <QPixmap>
#include "ui_BaseForm.h"

BaseForm::BaseForm() : ui(new Ui::BaseForm)
{
    ui->setupUi(this);
    setWindowIcon(QPixmap(qllogo_xpm));

    envinfo = new enviro();

    menu = new themenu(this);

    ui->LineEditSearch->setText("");
    ui->CheckBoxCaseSensitive->setChecked(envinfo->isQueryCaseSensitive());
    ui->CheckBoxQuietMode->setChecked(envinfo->isQueryQuietMode());
    ui->CheckBoxRegex->setChecked(envinfo->isQueryUsingRegex());
    ui->CheckBoxQueryFileNames->setChecked(envinfo->isQueryBaseNames());
    ui->ButtonGroupSort->setEnabled(FALSE);
    ui->LineEditStatusBar1->setText( tr("Ready"));
    ui->LineEditStatusBar2->setText("");
    ui->LineEditSearch->setFocus();
    ui->SpinBoxResultsShow->setValue(envinfo->getResultsToShow());

    ui->PushButtonSearch->setDefault(TRUE);

    inRun = FALSE;

    connect(ui->PushButtonSearch, SIGNAL(clicked()), SLOT(sendOrder_slot()) );
    connect(ui->LineEditSearch, SIGNAL(textChanged(QString)), SLOT(statusbar1Changed_slot()) );
    connect(ui->CheckBoxRegex, SIGNAL(stateChanged(int)), SLOT(statusbar1Changed_slot()) );
    connect(ui->CheckBoxRegex, SIGNAL(stateChanged(int)), SLOT(regexChanged_slot()) );
    connect(ui->CheckBoxRegex, SIGNAL(stateChanged(int)), SLOT(setQueryUsingRegex()) );
    connect(ui->CheckBoxQueryFileNames, SIGNAL(stateChanged(int)), SLOT(setQueryBaseNames()) );
    connect(ui->CheckBoxQuietMode, SIGNAL(stateChanged(int)), SLOT(setQueryQuietMode()) );
    connect(ui->SpinBoxResultsShow, SIGNAL(valueChanged(int)), SLOT(setResultsToShow(int)) );
    connect(ui->CheckBoxQueryFileNames, SIGNAL(stateChanged(int)), SLOT(statusbar1Changed_slot()) );
    connect(ui->CheckBoxCaseSensitive, SIGNAL(stateChanged(int)), SLOT(setQueryCaseSensitive()) );
    connect(ui->CheckBoxCaseSensitive, SIGNAL(stateChanged(int)), SLOT(statusbar1Changed_slot()) );
    connect(ui->CheckBoxQuietMode, SIGNAL(stateChanged(int)), SLOT(statusbar1Changed_slot()) );
    connect(ui->SpinBoxResultsShow, SIGNAL(valueChanged(int)), SLOT(statusbar1Changed_slot()) );
    connect(ui->ListBoxResults, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(displayFileInfo_slot()) );
    connect(ui->RadioButtonAscending, SIGNAL(toggled(bool)), SLOT(sortResultsAscending_slot()) );
    connect(ui->ListBoxResults, SIGNAL(customContextMenuRequested(QPoint)), SLOT(popEditMenu_slot()) );
    connect(ui->RadioButtonDescending, SIGNAL(toggled(bool)), SLOT(sortResultsDescending_slot()) );

    try
    {
        resize(envinfo->getMainWindowSize());
    }catch(...){}
    try
    {
        move(envinfo->getMainWindowPosition());
    }catch(...){}
}

BaseForm::~BaseForm()
{
    delete(menu);
    delete(envinfo);
    delete(ui);
}

void BaseForm::sendOrder_slot()
{
    if ( ui->LineEditSearch->text().trimmed() != "" && ! inRun )
    {
        inRun = TRUE;

        threadGetData = new getdatacommand( ui->ListBoxResults, buildQueryString() );
        connect(threadGetData, SIGNAL(queryComplete(int)), SLOT(statusbar2Changed_slot(int)));

        ui->ButtonGroupSort->setEnabled(FALSE);
        ui->ListBoxResults->clear();
        ui->FileInfoDisplay->clear();

        ui-> LineEditStatusBar1->setText( tr("Run : ") + buildQueryString() );
        statusbar2Changed_slot(0);

        qApp->processEvents();

        threadGetData->start();
        threadGetData->wait();

        ui->RadioButtonAscending->setAutoExclusive(false);
        ui->RadioButtonDescending->setAutoExclusive(false);
        ui->RadioButtonAscending->setChecked(FALSE);
        ui->RadioButtonDescending->setChecked(FALSE);
        ui->RadioButtonAscending->setAutoExclusive(true);
        ui->RadioButtonDescending->setAutoExclusive(true);

        if ( ui->ListBoxResults->count() > 1 )
        {
            ui->ButtonGroupSort->setEnabled(TRUE);
        }
        else
        {
            ui->ButtonGroupSort->setEnabled(FALSE);
        }

        ui->LineEditStatusBar1->setText(tr("Terminate : ") + buildQueryString() );
        delete(threadGetData);
        QApplication::beep();
        inRun =FALSE;
    }
}

void BaseForm::sortResultsAscending_slot()
{
    if ( ui->RadioButtonAscending->isChecked() )
    {
        ui->ListBoxResults->sortItems(Qt::AscendingOrder);
    }
}
void BaseForm::sortResultsDescending_slot()
{
    if ( ui->RadioButtonDescending->isChecked() )
    {
        ui->ListBoxResults->sortItems(Qt::DescendingOrder);
    }
}

void BaseForm::copy_slot()
{
    QClipboard *cb = QApplication::clipboard();
    QString selectedItems;

    QListIterator<QListWidgetItem*> iterator(ui->ListBoxResults->selectedItems());
    if( iterator.hasNext() )
    {
        while(iterator.hasNext())
            selectedItems.append(iterator.next()->text()).append("\n");
        cb->setText(selectedItems);
    }
}

void BaseForm::popEditMenu_slot()
{
    menu->editMenu->popup(QCursor::pos());
}

void BaseForm::selectAll_slot()
{
    ui->ListBoxResults->selectAll();
}

void BaseForm::deselectAll_slot()
{
    ui->ListBoxResults->clearSelection();
}

void BaseForm::openFile_slot()
{
    qDebug() << "Open file slot called";
    foreach (QListWidgetItem* ii, ui->ListBoxResults->selectedItems()){
        //QDesktopServices::openUrl(QUrl::fromLocalFile(ii->data(Qt::ToolTipRole).toString()));
        qDebug() << "Opening " << ii->text();
        QDesktopServices::openUrl(QUrl::fromLocalFile(ii->text()));        
    }
    
}

void BaseForm::setup_slot()
{
    ConfigForm *confform = new ConfigForm( envinfo, this);
    confform->exec();
    delete(confform);
}

void BaseForm::setQueryBaseNames()
{
    envinfo->setQueryBaseNames( ui->CheckBoxQueryFileNames->checkState() == Qt::Checked ? true : false );
}

void BaseForm::setQueryCaseSensitive()
{
    envinfo->setQueryCaseSensitive( ui->CheckBoxCaseSensitive->checkState() == Qt::Checked ? true : false );
}

void BaseForm::setQueryQuietMode()
{
    envinfo->setQueryQuietMode( ui->CheckBoxQuietMode->checkState() == Qt::Checked ? true : false );
}

void BaseForm::setQueryUsingRegex()
{
    envinfo->setQueryUsingRegex( ui->CheckBoxRegex->checkState() == Qt::Checked ? true : false );
}

void BaseForm::setResultsToShow(int resultsToShow)
{
    envinfo->setResultsToShow(resultsToShow);
}

QString BaseForm::buildQueryString()
{
    QString strQuery;
    QString parseFileName;

    // Parse LineEditSearch->text(), replace " by /"
    parseFileName=ui->LineEditSearch->text().replace( QRegExp("\""), "\\\"" );

    strQuery = envinfo->getPathLocate();

    if ( ui->CheckBoxRegex->isChecked() )
    {
        strQuery += (QString(" -r  \"") +  parseFileName + QString("\"") );
    }
    else
    {
        strQuery += (QString("  \"") +  parseFileName + QString("\"") );
    }

    if ( ui->CheckBoxQueryFileNames->isChecked() )
    {
        strQuery += QString(" -b ");
    }

    if ( ! ui->CheckBoxCaseSensitive->isChecked() )
    {
        strQuery +=  QString(" -i ");
    }

    if ( ui->CheckBoxQuietMode->isChecked() != 0 && ! ui->CheckBoxRegex->isChecked() )
    {
        strQuery += QString(" -q ");
    }

    if ( ui->SpinBoxResultsShow->value() != 0 && ! ui->CheckBoxRegex->isChecked() )
    {
        strQuery +=  QString(" -n ") + QString::number(ui->SpinBoxResultsShow->value());
    }

    if ( ! envinfo->isUseDefaultDB() )
    {
        strQuery += QString(" -d \"") + envinfo->getDBToQuery() + QString("\"");
    }

    strQuery += " -0 "; // delimit results by null character instead of newline
                        // this is to read output that has newline in filenames.

    return strQuery;
}

void BaseForm::statusbar1Changed_slot()
{
    if ( !ui->LineEditSearch->text().isEmpty() )
    {
        ui->LineEditStatusBar1->setText(buildQueryString());
    }
    else
    {
        ui->LineEditStatusBar1->setText("");
    }
}

void BaseForm::regexChanged_slot()
{
    if (ui->CheckBoxRegex->isChecked())
    {
        resultsshow = ui->SpinBoxResultsShow->value();
        ui->SpinBoxResultsShow->setEnabled(FALSE);
        quietmode = ui->CheckBoxQuietMode->isChecked();
        ui->CheckBoxQuietMode->setEnabled(FALSE);
    }
    else
    {
        ui->SpinBoxResultsShow->setEnabled(TRUE);
        ui->SpinBoxResultsShow->setValue(resultsshow);
        ui->CheckBoxQuietMode->setEnabled(TRUE);
        ui->CheckBoxQuietMode->setChecked(quietmode);
    }
}

void BaseForm::statusbar2Changed_slot( int findfile )
{
    ui->LineEditStatusBar2->setText( tr("Find : ") + QString::number(findfile) );
}

void BaseForm::moveEvent(QMoveEvent* event)
{
    QWidget::moveEvent(event);
    envinfo->setMainWindowPosition(event->pos());
}

void BaseForm::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
    envinfo->setMainWindowSize(event->size());
}

void BaseForm::displayFileInfo_slot()
{
    QString tmpStr;
    rpmstat * rpminfo;
    filestat * datainfo;

    ui->LineEditStatusBar1->setText( tr("Run get info for : ") + ui->ListBoxResults->currentItem()->text() );

    ui->FileInfoDisplay->clear();
    qApp->processEvents();

    datainfo = new filestat( envinfo, ui->ListBoxResults->currentItem()->text());

    tmpStr = QString( tr("Owner : ") + QString::number( datainfo->getOwner() ) + " ( " + datainfo->getOwnerName() + " )" );
    ui->FileInfoDisplay->append(tmpStr);

    tmpStr = QString( tr("Group : ") + QString::number( datainfo->getGroup() ) + " ( " + datainfo->getGroupName() + " )" );
    ui->FileInfoDisplay->append(tmpStr);

    tmpStr = QString( tr("Size : ") + QString::number( datainfo->getSize() ) + tr(" bytes") );
    ui->FileInfoDisplay->append(tmpStr);

    tmpStr = QString( tr("Mode : ") + QString::number( datainfo->getMode(), 8 ) );
    ui->FileInfoDisplay->append(tmpStr);

    tmpStr = QString( tr("Last access time : ") +  datainfo->getAccessTime() );
    ui->FileInfoDisplay->append(tmpStr);

    tmpStr = QString( tr("Last modification time : ") +  datainfo->getModifTime() );
    ui->FileInfoDisplay->append(tmpStr);

    tmpStr = QString( tr("Last change time : ") +  datainfo->getChangeTime()  );
    ui->FileInfoDisplay->append(tmpStr);

    if ( ! envinfo->getPathFile().trimmed().isEmpty() )
    {
        tmpStr = QString( tr("Type : ") +  datainfo->getFileType()  );
        ui->FileInfoDisplay->append(tmpStr);
    }

    if ( ! envinfo->getPathRpm().trimmed().isEmpty() )
    {
        rpminfo = new rpmstat( envinfo, ui->ListBoxResults->currentItem()->text() );

        if ( rpminfo->isOwnedByPkg() &&  datainfo->getOwnerName() != rpminfo->getOwnerName() )
        {
            tmpStr = QString( tr("#RPM Owner : ") +  rpminfo->getOwnerName() );
            ui->FileInfoDisplay->append(tmpStr);
        }

        if ( rpminfo->isOwnedByPkg() && datainfo->getGroupName() != rpminfo->getGroup() )
        {
            tmpStr = QString( tr("#RPM Group : ") +  rpminfo->getGroup() );
            ui->FileInfoDisplay->append(tmpStr);
        }

        if ( rpminfo->isOwnedByPkg() && datainfo->getSize() != rpminfo->getSize() )
        {
            tmpStr = QString( tr("#RPM Size : ") +  QString::number(rpminfo->getSize()) );
            ui->FileInfoDisplay->append(tmpStr);
        }

        if ( rpminfo->isOwnedByPkg() &&  datainfo->getMode() != rpminfo->getMode() )
        {
            tmpStr = QString( tr("#RPM mode : ") +  QString::number(rpminfo->getMode(), 8) );
            ui->FileInfoDisplay->append(tmpStr);
        }

        if ( rpminfo->isOwnedByPkg() &&  datainfo->getModifTime() != rpminfo->getModifTime() )
        {
            tmpStr = QString( tr("#RPM modification time : ") +  rpminfo->getModifTime() );
            ui->FileInfoDisplay->append(tmpStr);
        }

        tmpStr = QString( tr("Package : ") +  rpminfo->getPackage() );
        ui->FileInfoDisplay->append(tmpStr);
        delete (rpminfo);
    }

    delete (datainfo);

    ui->LineEditStatusBar1->setText( tr("Terminate get info for : ") + ui->ListBoxResults->currentItem()->text() );
}




/***************************************************************************
                          configformimpl.cpp  -  description
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

#include "ConfigForm.h"
#include "qlicon.h"
#include <QFileDialog>
#include <QPixmap>
#include "ui_ConfigForm.h"

ConfigForm::ConfigForm(enviro * env, QWidget * parent) : QDialog(parent), ui(new Ui::ConfigForm)
{
    ui->setupUi(this);

    theenv = env;

    setWindowIcon(QPixmap(qllogo_xpm));

    ui->PushButtonClose->setDefault(TRUE);

    if ( env->isUseDefaultDB() )
    {
        ui->CheckBoxUseDefaultDB->setChecked(TRUE);
        ui->LineEditSearchIn->setEnabled(FALSE);
        ui->PushButtonSerachIn->setEnabled(FALSE);
    }
    else
    {
        ui->CheckBoxUseDefaultDB->setChecked(FALSE);
        ui->LineEditSearchIn->setEnabled(TRUE);
        ui->PushButtonSerachIn->setEnabled(TRUE);
    }

    ui->LineEditSearchIn->setText( env->getDBToQuery() );

    ui->LineEditStartIndexFrom->setText(env->getStartIndexFrom() );

    ui->LineEditDBLocate->setText(env->getDatabase());

    ui->ListBoxDirExclude->clear();
    QStringListIterator stringIterator(env->getPrunepaths().split(","));
    while( stringIterator.hasNext() )
    {
        ui->ListBoxDirExclude->addItem(stringIterator.next().toUtf8());
    }
    if ( ui->ListBoxDirExclude->count() > 1 )
    {
        ui->ListBoxDirExclude->sortItems();
    }

    ui->ListBoxFsExclude->clear();
    stringIterator = QStringListIterator(env->getPrunefs().split(","));
    while( stringIterator.hasNext() )
    {
        ui->ListBoxFsExclude->addItem(stringIterator.next().toUtf8());
    }
    if ( ui->ListBoxFsExclude->count() > 1 )
    {
        ui->ListBoxFsExclude->sortItems();
    }

    if ( env->isSecurity() )
    {
        ui->CheckBoxSecurityLevel->setChecked(TRUE);
    }
    else
    {
        ui->CheckBoxSecurityLevel->setChecked(FALSE);
    }

    if ( env->isVerbose() )
    {
        ui->CheckBoxVerbose->setChecked(TRUE);
    }
    else
    {
        ui->CheckBoxVerbose->setChecked(FALSE);
    }

    ui->PushButtonConfSave->setEnabled(FALSE);

    connect(ui->PushButtonClose, SIGNAL(clicked(bool)), SLOT(accept()) );
    connect(ui->PushButtonSerachIn, SIGNAL(clicked(bool)), SLOT(changeSearchIn_slot()) );
    connect(ui->PushButtonStartIndexFrom, SIGNAL(clicked(bool)), SLOT(changeStartIndexFrom_slot()) );
    connect(ui->PushButtonDBLocate, SIGNAL(clicked(bool)), SLOT(changeDBLocate_slot()) );
    connect(ui->PushButtonAddDirExclude, SIGNAL(clicked(bool)), SLOT(addDirExclude_slot()) );
    connect(ui->PushButtonDeleteDirExclude, SIGNAL(clicked(bool)), SLOT(deleteDirExclude_slot()) );
    connect(ui->PushButtonAddFsExclude, SIGNAL(clicked(bool)), SLOT(addFsExclude_slot()) );
    connect(ui->PushButtonConfSave, SIGNAL(clicked(bool)), SLOT(save_slot()) );
    connect(ui->PushButtonCreateDB, SIGNAL(clicked(bool)), SLOT(createDB_slot()) );
    connect(ui->CheckBoxUseDefaultDB, SIGNAL(stateChanged(int)), SLOT(activeSaveButton_slot()) );
    connect(ui->CheckBoxUseDefaultDB, SIGNAL(stateChanged(int)), SLOT(setActiveSearchIn_slot()) );
    connect(ui->LineEditSearchIn, SIGNAL(textChanged(QString)), SLOT(activeSaveButton_slot()) );
    connect(ui->LineEditStartIndexFrom, SIGNAL(textChanged(QString)), SLOT(activeSaveButton_slot()) );
    connect(ui->CheckBoxSecurityLevel, SIGNAL(stateChanged(int)), SLOT(activeSaveButton_slot()) );
    connect(ui->CheckBoxVerbose, SIGNAL(stateChanged(int)), SLOT(activeSaveButton_slot()) );
    connect(ui->LineEditDBLocate, SIGNAL(textChanged(QString)), SLOT(activeSaveButton_slot()) );
    connect(ui->PushButtonDeleteFsExclude, SIGNAL(clicked(bool)), SLOT(deleteFsExclude_slot()) );

}

ConfigForm::~ConfigForm()
{
	delete ui;
}

void ConfigForm::setActiveSearchIn_slot()
{
    if ( ui->CheckBoxUseDefaultDB->isChecked() )
    {
        ui->LineEditSearchIn->setEnabled(FALSE);
        ui->PushButtonSerachIn->setEnabled(FALSE);
    }
    else
    {
        ui->LineEditSearchIn->setEnabled(TRUE);
        ui->PushButtonSerachIn->setEnabled(TRUE);
    }
}

void ConfigForm::activeSaveButton_slot()
{
    ui->PushButtonConfSave->setEnabled(TRUE);
}

void ConfigForm::changeSearchIn_slot()
{
    QFileDialog * myDiag;
    QStringList filterList;
    QString fileName;

    myDiag = new QFileDialog( this );

    myDiag->setWindowTitle( tr("Select slocate database for querying.") );
    myDiag->setFileMode( QFileDialog::ExistingFile );
    myDiag->setViewMode( QFileDialog::List );
    QStringList filters;
    filters << tr("db files (*.db *.DB)")
            << tr("All files (*)");
    myDiag->setNameFilters(filters);

    QFileInfo tmpFile = QFileInfo( ui->LineEditSearchIn->text() );
    if ( !ui->LineEditSearchIn->text().isEmpty() && tmpFile.dir().exists() )
    {
        myDiag->setDirectory( tmpFile.dir() );
    }
    else
    {
        myDiag->setDirectory( QDir::homePath() + QString("/.qlocate/") );
    }

    myDiag->exec();

    if( !myDiag->selectedFiles().isEmpty() )
        fileName = myDiag->selectedFiles().first();

    delete (myDiag);

    if ( !fileName.isEmpty() )
    {
        ui->LineEditSearchIn->setText(fileName);
    }
}

bool ConfigForm::validateSearchIn()
{
    QFileInfo theFile;
    QString warningStr = "";

    ui->LineEditSearchIn->setText( ui->LineEditSearchIn->text().trimmed() );
    theFile = QFileInfo(ui->LineEditSearchIn->text());
    if ( !theFile.exists() && !ui->CheckBoxUseDefaultDB->isChecked() )
    {
        warningStr += tr("The DB selected for query not exists !\n");
    }

    if ( !theFile.exists() && !ui->CheckBoxUseDefaultDB->isChecked() )
    {
        warningStr += tr("Invalide input:") + QString(" \"") + ui->LineEditSearchIn->text() + QString("\"\n\n") + tr("Operation is aborded !") ;
        QMessageBox::warning( this, tr("Invalide Input"), warningStr );
        ui->LineEditSearchIn->selectAll();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void ConfigForm::changeStartIndexFrom_slot()
{
    QString fileName;
    QString bakStartIndexFrom;
    QString openFrom;

    bakStartIndexFrom = ui->LineEditStartIndexFrom->text();

    QDir tmpDir = QDir( ui->LineEditStartIndexFrom->text() );

    if ( !ui->LineEditStartIndexFrom->text().isEmpty() && tmpDir.exists() )
    {
        openFrom = ui->LineEditStartIndexFrom->text();
    }
    else
    {
        openFrom = QDir::homePath() + QString("/") ;
    }

    fileName = QFileDialog::getExistingDirectory (  this, tr("Select the base directory to index."), openFrom );

    if ( !fileName.isEmpty() )
    {
        ui->LineEditStartIndexFrom->setText(fileName);
    }
    else
    {
        ui->LineEditStartIndexFrom->setText(bakStartIndexFrom);
    }
}

bool ConfigForm::validateStartIndexFrom()
{
    QDir theDir;
    QString warningStr = "";

    ui->LineEditStartIndexFrom->setText( ui->LineEditStartIndexFrom->text().trimmed() );
    theDir = QDir(ui->LineEditStartIndexFrom->text());
    if ( ui->LineEditStartIndexFrom->text().isEmpty() || !theDir.exists() )
    {
        warningStr += tr("The directory selected for indexing not exists !\n")
                      + tr("Invalide input:") + QString(" \"") + ui->LineEditStartIndexFrom->text() + QString("\"\n\n") + tr("Operation is aborded !");
        QMessageBox::warning( this, tr("Invalide Input"), warningStr );
        ui->LineEditStartIndexFrom->selectAll();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void ConfigForm::changeDBLocate_slot()
{
    QFileDialog * myDiag;
    QStringList filterList;
    QString fileName;

    myDiag = new QFileDialog( this );

    myDiag->setWindowTitle( tr("Select where slocate database is stored.") );
    myDiag->setFileMode( QFileDialog::AnyFile );
    myDiag->setViewMode( QFileDialog::List );
    QStringList filters;
    filters << tr("db files (*.db *.DB)")
            << tr("All files (*)");
    myDiag->setNameFilters(filters);

    QFileInfo tmpFile = QFileInfo(ui->LineEditDBLocate->text());
    if ( !ui->LineEditDBLocate->text().isEmpty() && tmpFile.dir().exists() )
    {
        myDiag->setDirectory( tmpFile.dir() );
    }
    else
    {
        myDiag->setDirectory( QDir::homePath() + QString("/.qlocate/") );
    }
    myDiag->exec();

    if ( !myDiag->selectedFiles().isEmpty() )
        fileName = myDiag->selectedFiles().first();

    delete (myDiag);

    if ( !fileName.isEmpty() )
    {
        ui->LineEditDBLocate->setText(fileName);
    }
}

bool ConfigForm::validateDBLocate()
{
    QFileInfo theFile;
    QDir theDir;
    QString warningStr = "";

    ui->LineEditDBLocate->setText( ui->LineEditDBLocate->text().trimmed() );
    theFile = QFileInfo(ui->LineEditDBLocate->text());
    theDir = theFile.dir();
    if ( !theDir.exists() )
    {
        warningStr += tr("The directory selected for create DB index not exists !\n");
    }

    if ( theFile.fileName().isEmpty() )
    {
        warningStr += tr("The file name selected for create DB index is empty !\n");
    }

    if ( !theDir.exists() || theFile.fileName().isEmpty() )
    {
        warningStr += tr("Invalide input:") + QString(" \"") + ui->LineEditDBLocate->text() + QString("\"\n\n") + tr("Operation is aborded !") ;
        QMessageBox::warning( this, tr("Invalide Input"), warningStr );
        ui->LineEditDBLocate->selectAll();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void ConfigForm::addDirExclude_slot()
{
    QString fileName;

    fileName = QFileDialog::getExistingDirectory ( this, tr("Select the directory to exclude."), QDir::homePath() );

    if ( !fileName.isEmpty() )
    {
        bool alreadyIn = FALSE;

        for ( int x = 0; x < ui->ListBoxDirExclude->count(); x++ )
        {
            if ( ui->ListBoxDirExclude->item(x)->text() == fileName ) alreadyIn = TRUE;
        }

        if ( ! alreadyIn )
        {
            ui->ListBoxDirExclude->addItem(fileName.trimmed());
            if ( ui->ListBoxDirExclude->count() > 1 )
            {
                ui->ListBoxDirExclude->sortItems();
            }
            ui->PushButtonConfSave->setEnabled(TRUE);
        }
    }
}

void ConfigForm::deleteDirExclude_slot()
{
    if ( ui->ListBoxDirExclude->currentRow() != -1 )
    {
        delete(ui->ListBoxDirExclude->takeItem(ui->ListBoxDirExclude->currentRow()));
        ui->PushButtonConfSave->setEnabled(TRUE);
    }
}

void ConfigForm::addFsExclude_slot()
{
    bool ok = FALSE;
    QString fs;
    QString fsList;
    fsList = QString("adfs\taffs\tbfs\tcoda\tcramfs\tdevfs\tdevpts\tefs\text2\text3\n"
                     "fat_cvf\tfreevxfs\thfs\thpfs\tiso9660\tisofs\tjfs\tminix\tmsdos\tncpfs\n"
                     "nfs\tntfs\tproc\tramfs\tromfs\tsmbfs\tsupermount\tsysv\tudf\n"
                     "ufs\tumsdos\tusbdevfs\tvfat\tvfs\txfs\n");

    //fs = QInputDialog::getText( tr( "Enter Filesystems to exclude." ), tr( "Some possible value are:\n\n") + fsList, QString::null, &ok, this );
    fs = QInputDialog::getText(
             this,
             tr( "Enter Filesystems to exclude." ),
             tr( "Some possible value are:\n\n") + fsList,
             QLineEdit::Normal,
             QString::null,
             &ok
         );
    if ( ok && !fs.isEmpty() )
    {
        bool alreadyIn = FALSE;

        for ( int x = 0; x < ui->ListBoxFsExclude->count(); x++ )
        {
            if ( ui->ListBoxFsExclude->item(x)->text() == fs ) alreadyIn = TRUE;
        }

        if ( ! alreadyIn )
        {
            ui->ListBoxFsExclude->addItem(fs.trimmed() );
            if ( ui->ListBoxFsExclude->count() > 1 )
            {
                ui->ListBoxFsExclude->sortItems();
            }
            ui->PushButtonConfSave->setEnabled(TRUE);
        }
    }
}

void ConfigForm::deleteFsExclude_slot()
{
    if ( ui->ListBoxFsExclude->currentRow() != -1 )
    {
        delete(ui->ListBoxFsExclude->takeItem(ui->ListBoxFsExclude->currentRow()));
        ui->PushButtonConfSave->setEnabled(TRUE);
    }
}

void ConfigForm::save_slot()
{    
    if ( validateSearchIn() &&  validateStartIndexFrom() && validateDBLocate() )
    {
        QString tmpStr;
        int x;
        
        theenv->setStartIndexFrom( ui->LineEditStartIndexFrom->text() );
        tmpStr = QString("");
        for ( x = 0; x < ui->ListBoxDirExclude->count(); x++ )
        {
            tmpStr += ui->ListBoxDirExclude->item(x)->text();
            if ( x != ui->ListBoxDirExclude->count() -1 ) tmpStr += QString(",");
        }
        theenv->setPrunepaths( tmpStr );
        
        tmpStr = QString("");
        for ( x = 0; x < ui->ListBoxFsExclude->count(); x++ )
        {
            tmpStr += ui->ListBoxFsExclude->item(x)->text();
            if ( x != ui->ListBoxFsExclude->count() -1 ) tmpStr += QString(",");
        }
        theenv->setPrunefs( tmpStr );
        
        theenv->setSecurity( ui->CheckBoxSecurityLevel->isChecked() );
        theenv->setVerbose( ui->CheckBoxVerbose->isChecked() );
        theenv->setDatabase( ui->LineEditDBLocate->text() );
        
        theenv->setUseDefaultDB( ui->CheckBoxUseDefaultDB->isChecked() );
        theenv->setDBToQuery( ui->LineEditSearchIn->text() );


        ui->PushButtonConfSave->setEnabled(FALSE);
    }
}

void ConfigForm::createDB_slot()
{
    if( !validateStartIndexFrom() || !validateDBLocate() )
        return;
    
    QString strSave;
    QString strCreate;
    QString strClose;
    QString buildCommand;
    bool saveState;

    strSave = ui->PushButtonConfSave->text();
    strCreate = ui->PushButtonCreateDB->text();
    strClose = ui->PushButtonClose->text();

    ui->PushButtonConfSave->setText( tr("Wait") );
    ui->PushButtonCreateDB->setText( tr("Wait") );
    ui->PushButtonClose->setText( tr("Wait") );

    saveState = ui->PushButtonConfSave->isEnabled();

    ui->PushButtonConfSave->setEnabled( FALSE );
    ui->PushButtonCreateDB->setEnabled( FALSE );
    ui->PushButtonClose->setEnabled( FALSE );

    ui->ButtonGroupSearchIn->setEnabled( FALSE );
    ui->GroupBoxUpdatedb->setEnabled( FALSE );

    qApp->processEvents();

    buildCommand = theenv->getPathUpdateDB();

    buildCommand += QString(" -U \"") + ui->LineEditStartIndexFrom->text() + QString("\"");

    QString prunepaths;
    for ( int x = 0; x < ui->ListBoxDirExclude->count(); x++ )
    {
        prunepaths += ui->ListBoxDirExclude->item(x)->text();
        if ( x != ui->ListBoxDirExclude->count() -1 ) prunepaths += QString(",");
    }
    if ( ! (prunepaths).isEmpty() )
    {
        buildCommand += QString(" -e \"") + prunepaths + QString("\"");
    }

    QString prunefs;
    for ( int x = 0; x < ui->ListBoxFsExclude->count(); x++ )
    {
        prunefs += ui->ListBoxFsExclude->item(x)->text();
        if ( x != ui->ListBoxFsExclude->count() -1 ) prunefs += QString(",");
    }
    if ( ! (prunefs).isEmpty() )
    {
        buildCommand += QString(" -f \"") + prunefs + QString("\"");
    }

    if ( ui->CheckBoxSecurityLevel->isChecked() )
    {
        buildCommand += QString(" -l 1");
    }
    else
    {
        buildCommand += QString(" -l 0");
    }

    if ( ui->CheckBoxVerbose->isChecked() ) buildCommand += QString(" -v");

    buildCommand += QString(" -o \"") + ui->LineEditDBLocate->text() + QString("\"");

    threadBuildDB = new builddbcommand( buildCommand );

    threadBuildDB->start();
    threadBuildDB->wait();

    delete ( threadBuildDB );

    ui->PushButtonConfSave->setText(strSave);
    ui->PushButtonCreateDB->setText(strCreate);
    ui->PushButtonClose->setText(strClose);

    ui->PushButtonConfSave->setEnabled( saveState );
    ui->PushButtonCreateDB->setEnabled( TRUE );
    ui->PushButtonClose->setEnabled( TRUE );

    ui->ButtonGroupSearchIn->setEnabled( TRUE );
    ui->GroupBoxUpdatedb->setEnabled( TRUE );
}

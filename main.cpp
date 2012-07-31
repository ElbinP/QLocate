/***************************************************************************
                          main.cpp  -  description
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

#include <qapplication.h>
#include <qtextcodec.h>
#include <QTranslator>
#include "BaseForm.h"
#include <QDir>

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );

    bool option_e = FALSE;
    bool option_f = FALSE;

    for ( unsigned short i = 0; i < app.argc(); i++ )
    {
        if ( QString(app.argv()[i]) == QString("-e") ) option_e = TRUE;
        else if ( QString(app.argv()[i]) == QString("-f") ) option_f = TRUE;
    }

    //Option -e predominate on option -f.
    if ( option_e ) option_f = FALSE;

    QTranslator translator;
    if ( option_f )
    {
        translator.load( "qlocate.fr", QDir::homePath() + QDir::separator() + ".qlocate" + QDir::separator() + "i18n" );
        app.installTranslator( &translator );
    }
    else if ( !option_e ) 
    {
    	// Do not load any translation file if -e is given in parameter list. 
    	// Instead we display the native strings that are in English.
    	QString locale = QLocale::system().name();
        translator.load( QString("qlocate.") + locale, QDir::homePath() + QDir::separator() + ".qlocate" + QDir::separator() + "i18n" );
	    app.installTranslator( &translator );
    }

    BaseForm myForm;
    myForm.show();

    return app.exec();
}

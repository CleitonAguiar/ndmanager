/***************************************************************************
 *   Copyright (C) 2006 by Michael Zugaro                                  *
 *   michael.zugaro@college-de-france.fr                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef QUERYINPUTDIALOG_H
#define QUERYINPUTDIALOG_H

// #include <queryinputdialogbase.h>
#include "qlayout.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "kdialogbase.h"

/**
Dialog used to get information for query.

	@author Michaël Zugaro <michael.zugaro@college-de-france.fr>
*/
class QueryInputDialog : public KDialogBase
{
	Q_OBJECT
	public:
		QueryInputDialog(QWidget *parent = 0,const QString& caption = "Query",const QString& urltext = "");
		virtual ~QueryInputDialog();
        inline QString getQuery() { return query->text(); }
        inline QString getPath() { return path->text(); }
			

	private slots:
		void pathChanged(const QString & newPath);
		
	private:		
		QWidget			*page;
        QLineEdit	*path;
		QLineEdit		*query;
		QVBoxLayout		*layout;
};

#endif

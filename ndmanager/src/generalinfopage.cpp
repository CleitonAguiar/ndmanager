/***************************************************************************
 *   Copyright (C) 2004 by Lynn Hazan                                      *
 *   lynn.hazan@myrealbox.com                                              *
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
#include "generalinfopage.h"

GeneralInfoPage::GeneralInfoPage(QWidget *parent)
    : GeneralInfoLayout(parent),modified(false),isInit(true){
    descriptionTextEdit->setAcceptRichText(false);
    notesTextEdit->setAcceptRichText(false);

    connect(experimentersLineEdit,SIGNAL(textChanged(QString)),this,SLOT(propertyModified()));
    connect(descriptionTextEdit,SIGNAL(textChanged()),this,SLOT(propertyModified()));
    connect(notesTextEdit,SIGNAL(textChanged()),this,SLOT(propertyModified()));
    connect(kDatePicker,SIGNAL(clicked(QDate)),this,SLOT(propertyModified()));
}


GeneralInfoPage::~GeneralInfoPage()
{
}



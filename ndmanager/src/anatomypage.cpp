/***************************************************************************
 *   Copyright (C) 2004 by Lynn Hazan                                      *
 *   lynn.hazan@myrealbox.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                 void addAttributes(QMap<QString QMap<int,QString> > attributes);  *
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
//include files for the application
#include "anatomypage.h"

// include files for QT
#include <QList>
#include <qstringlist.h>
//Added by qt3to4:
#include <QEvent>
#include <QVector>

AnatomyPage::AnatomyPage(QWidget* parent)
    : AnatomyLayout(parent),
     isIncorrectRow(false),
      incorrectRow(0),
      modified(false)
{
#ifdef KDAB_PENDING
    for(int i = 0;i<attributesTable->numCols();++i)
        attributesTable->setColumnStretchable(i,true);
    for(int i = 0;i<groupTable->numCols();++i)
        groupTable->setColumnStretchable(i,true);
#endif
    //install a filter on the groupTable in order to validate the entries
    groupTable->installEventFilter(this);

    connect(addGroupButton,SIGNAL(clicked()),this,SLOT(addGroup()));
    connect(removeGroupButton,SIGNAL(clicked()),this,SLOT(removeGroup()));
    connect(groupTable, SIGNAL(currentChanged(int,int)),this, SLOT(slotValidate()));
    connect(groupTable, SIGNAL(valueChanged(int,int)),this, SLOT(groupChanged(int,int)));
    connect(groupTable, SIGNAL(cellPressed(int,int)),this, SLOT(slotValidate()));
    connect(groupTable, SIGNAL(cellClicked(int,int)),this,SLOT(slotValidate()));
    connect(groupTable, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slotValidate()));

    connect(attributesTable, SIGNAL(valueChanged(int,int)),this, SLOT(attributeChanged(int,int)));
}

AnatomyPage::~AnatomyPage(){}

bool AnatomyPage::eventFilter(QObject* object,QEvent* event){
    QString name = object->objectName();
    if(name.indexOf("groupTable") != -1 && isIncorrectRow){
        groupTable->selectRow(incorrectRow);
        groupTable->setCurrentCell(incorrectRow,0);
        return true;
    }
    else if(name.indexOf("groupTable") != -1 && event->type() == QEvent::Leave){
        if(groupTable->currentRow() != -1){
            int row = groupTable->currentRow();
            int column = groupTable->currentColumn();
            QWidget* widget = groupTable->cellWidget(row,column);
            if(widget != 0 && widget->metaObject()->className() == ("QLineEdit")){
                groupTable->editItem(groupTable->item(row,column));
                return true;
            }
            else return QWidget::eventFilter(object,event);
        }
        else return QWidget::eventFilter(object,event);
    }
    else return QWidget::eventFilter(object,event);
}

void AnatomyPage::setAttributes(const QMap<QString, QMap<int,QString> >& attributes){
    for(int i =0; i<attributesTable->columnCount();++i){
        QMap<int,QString> values = attributes[attributesTable->horizontalHeaderItem(i)->text()];//the headers have been set in the ui file and the corresponding entries in the map in the xmlreader
        //insert the values in the table and set the line headers
        for(int j = 0;j<attributesTable->rowCount();++j){
            attributesTable->setItem(j,i,new QTableWidgetItem(values[j]));
            attributesTable->setHorizontalHeaderItem(j,new QTableWidgetItem(QString::number(j)));
        }
        //attributesTable->adjustColumn(i);
    }
}

void AnatomyPage::getAttributes(QMap<QString, QMap<int,QString> >& attributesMap)const{
    for(int i =0; i<attributesTable->columnCount();++i){
        QMap<int,QString> values;
        for(int j = 0;j<attributesTable->rowCount();++j){
            QString attribut = attributesTable->item(j,i)->text().simplified();
            if(attribut != " ")
                values.insert(j,attribut);
        }
        attributesMap.insert(attributesTable->horizontalHeaderItem(i)->text(),values);
    }
}

void AnatomyPage::setGroups(const QMap<int, QList<int> >& groups){
    groupTable->clearContents();
    groupTable->setRowCount(groups.count());

    QMap<int,QList<int> >::const_iterator iterator;
    //The iterator gives the keys sorted.
    for(iterator = groups.begin(); iterator != groups.end(); ++iterator){
        QList<int> channelIds = iterator.value();
        QList<int>::iterator channelIterator;

        //create the string containing the channel ids
        QString group;
        for(channelIterator = channelIds.begin(); channelIterator != channelIds.end(); ++channelIterator){
            group.append(QString::number(*channelIterator));
            group.append(" ");
        }

        groupTable->setItem(iterator.key() - 1,0,new QTableWidgetItem(group));

        //groupTable->adjustRow(iterator.key() - 1);
    }//end of groups loop
}

void AnatomyPage::getGroups(QMap<int, QList<int> >& groups)const{

    int groupId = 1;
    for(int i =0; i<groupTable->rowCount();++i){
        QList<int> channels;
        QString item = groupTable->item(i,0)->text();
        QString channelList = item.simplified();
        if(channelList == " ") continue;
        QStringList channelParts = channelList.split(" ", QString::SkipEmptyParts);
        for(uint j = 0;j < channelParts.count(); ++j)
            channels.append(channelParts[j].toInt());
        groups.insert(groupId,channels);
        groupId++;
    }
}

void AnatomyPage::removeGroup(){
    if(isIncorrectRow) return;
    #ifdef KDAB_PENDING
    modified = true;
    int nbSelections = groupTable->numSelections();
    
    if(nbSelections > 0){
        QList< QVector<int> > rowsToRemove;
        //Look up the rows to be removed
        for(int j = 0; j < nbSelections;++j){
            Q3TableSelection selection = groupTable->selection(j);
            bool active = selection.isActive();
            if(active){
                int nbRows = selection.bottomRow() - selection.topRow() + 1;
                QVector<int> rows(nbRows);
                for(int i = 0; i < nbRows;++i){
                    rows[i] = selection.topRow() + i;
                }
                rowsToRemove.append(rows);
            }
        }
        //Actually remove the rows
        QList< QVector<int> >::iterator iterator;
        for(iterator = rowsToRemove.begin(); iterator != rowsToRemove.end(); ++iterator)
            groupTable->removeRows(*iterator);
    }
#endif
}

void AnatomyPage::addGroup(){
    if(isIncorrectRow) return;
    modified = true;
    groupTable->insertRow(groupTable->rowCount());
    groupTable->setItem(groupTable->rowCount() - 1,0,new QTableWidgetItem());
}

void AnatomyPage::groupChanged(int row,int column){
    #ifdef KDAB_PENDING
    modified = true;
    QString group = groupTable->text(row,column);

    if(isIncorrectRow){
        QWidget* widget = groupTable->cellWidget(incorrectRow,0);
        QString incorrectGroup;
        if(widget != 0 && widget->metaObject()->className() == ("QLineEdit"))
            incorrectGroup = static_cast<QLineEdit*>(widget)->text();
        else if(widget == 0) incorrectGroup = groupTable->item(incorrectRow,0)->text();
        if(incorrectGroup.contains(QRegExp("[^\\d\\s]")) != 0){
            groupTable->selectRow(incorrectRow);
            groupTable->setCurrentCell(incorrectRow,0);
            return;
        }
    }
    isIncorrectRow = false;
    incorrectRow = 0;
    groupTable->adjustRow(row);

    //the group entry should only contain digits and whitespaces
    if(group.contains(QRegExp("[^\\d\\s]")) != 0){
        isIncorrectRow = true;
        incorrectRow = row;
        groupTable->selectRow(incorrectRow);
        groupTable->setCurrentCell(incorrectRow,0);
    }
#endif

}

void AnatomyPage::attributeChanged(int row,int column){
    modified = true;
    //hard coded that the skip attribut is an int, later it can be made more dynamic
    QTableWidgetItem * 	header = attributesTable->horizontalHeaderItem ( column );
    QString attributName = header->text();
    if(attributName == "Skip"){
        bool ok;
	QTableWidgetItem * item = attributesTable->item ( row, column );
        QString attribut = item->text();
        attribut.toInt(&ok);
        if(ok) { 
	   //KDAB_PENDING attributesTable->adjustRow(row);
	} else {
            attributesTable->selectRow(row);
            attributesTable->setCurrentCell(row,column);
        }
    } else { 
      //KDAB_PENDING attributesTable->adjustRow(row);
    }
}

void AnatomyPage::slotValidate(){
    modified = true;
    if(isIncorrectRow){
        groupTable->selectRow(incorrectRow);
        groupTable->setCurrentCell(incorrectRow,0);
    }
}

void AnatomyPage::setNbChannels(int nbChannels){
    for(int i =0; i<attributesTable->rowCount();++i)
        attributesTable->removeRow(i);
    attributesTable->setRowCount(nbChannels);
}



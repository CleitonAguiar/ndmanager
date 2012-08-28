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
#include "managerview.h"

//Include files for KDE



// include files for Qt
#include <q3textedit.h> 
#include <qwidget.h> 
#include <qdir.h> 
#include <qstringlist.h> 
#include <qlayout.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <QList>
#include <QFrame>
#include <QProcess>
#include <QDebug>
#include <QAction>

//General C++ include files
#include <iostream>


using namespace std;

ManagerView::ManagerView(QWidget *parent, const char *name)
 : /*QSplitter(parent, name),*/QFrame(parent, name),konsole(0L),isUptoDate(true){
 // setOrientation(Qt::Vertical);
 frameLayout = new Q3VBoxLayout(this,0,0);
}


ManagerView::~ManagerView(){
}

ManagerView::returnMessage ManagerView::addKonsole(const QString url,int nbSpikeGroups,QList<QString> fileExtensions,const QList<QString>& scriptNames){
 if(konsole == 0L){ 
  parameterUrl = QString(url);

  konsole = new NdKonsole(this,"NdKonsole");
  frameLayout->addWidget(konsole);
      
  toolbar = new QToolBar(this);
  

  toolbar->addAction(QIcon(":icons/neuroscope-22")),tr("NeuroScope"),this,SLOT(launchNeuroscope());

   //qDebug()<<"url "<<url.directory()<<" url.fileName() "<<url.fileName()<<" parameterUrl.fileName() "<<parameterUrl.fileName();
  
  QStringList neuroscopeFiles;
  neuroscopeFiles<<".dat"<<".eeg";
  
  QList<QString>::iterator iterator;
  for(iterator = fileExtensions.begin(); iterator != fileExtensions.end(); ++iterator){
   neuroscopeFiles<<QString(".%1").arg(*iterator);   
  }

  neuroscopeComboBox = new QComboBox(toolbar);
  neuroscopeComboBox->setGeometry(QRect(13,53,150,20));
  neuroscopeComboBox->setMinimumSize(QSize(90,20));
  neuroscopeComboBox->setMaximumSize(QSize(150,20));

  neuroscopeComboBox->insertStringList(neuroscopeFiles);
  connect(neuroscopeComboBox,SIGNAL(activated(int)),this,SLOT(neuroscopeFileChange(int)));
  toolbar->addWidget(neuroscopeComboBox);
 // toolbar->insertCombo(neuroscopeFiles,1,false,SIGNAL(activated(int)),this,SLOT(neuroscopeFileChange(int))); 
  
  toolbar->addSeparator();
  toolbar->addSeparator();
  toolbar->addSeparator();
  toolbar->insertLineSeparator();
  toolbar->addSeparator();
  toolbar->addSeparator();
  toolbar->addAction(QIcon(":icon/scripts")),tr("Start"),this,SLOT(launchScript());
  scriptsComboBox = new QComboBox(toolbar);
  scriptsComboBox->setGeometry(QRect(13,53,150,20));
  scriptsComboBox->setMinimumSize(QSize(90,20));
  scriptsComboBox->setMaximumSize(QSize(150,20));

  QStringList scripts;
  QList<QString>::const_iterator iterator2;
  for(iterator2 = scriptNames.begin(); iterator2 != scriptNames.end(); ++iterator2){
    scripts<<static_cast<QString>(*iterator2);  
  }
  scriptsComboBox->insertStringList(scripts);
  toolbar->addWidget(scriptsComboBox);
  toolbar->addAction(QIcon(":icon/stop")),tr("Stop"),this,SLOT(stopScript());

  toolbar->addSeparator();
  toolbar->addSeparator();
  toolbar->addSeparator();
  toolbar->insertLineSeparator();
  toolbar->addSeparator();
  toolbar->addSeparator();
  
  toolbar->addAction(QIcon(":icon/klusters-22")),tr("KLusters"),this,SLOT(launchKlusters());

  klustersComboBox = new QComboBox(toolbar);
  klustersComboBox->setGeometry(QRect(13,53,150,20));
  klustersComboBox->setMinimumSize(QSize(90,20));
  klustersComboBox->setMaximumSize(QSize(150,20));
  QStringList klustersFiles;
  for (int i = 0; i < nbSpikeGroups;++i){
   klustersFiles<<QString("%1").arg(i+1);
  }

  klustersComboBox->insertStringList(klustersFiles);
  toolbar->addWidget(klustersComboBox);
  frameLayout->addWidget(toolbar);


  
  //setOpaqueResize(true);   
  connect(konsole, SIGNAL(beingDestroyed()), this, SLOT(konsoleBeingDestroyed()));
 
  return static_cast<returnMessage>(konsole->loadConsoleIfNeeded(url));
 }
 else{
  return static_cast<returnMessage>(konsole->loadConsoleIfNeeded(url));
 }
 return OK;
}

void ManagerView::neuroscopeFileChange(int){

}

void ManagerView::updateSpikeGroupList(int nbGroups){
 klustersComboBox->clear();
 QStringList klustersFiles;
  for (int i = 0; i < nbGroups;++i){
   klustersFiles<<QString("%1").arg(i+1);
  }

 klustersComboBox->insertStringList(klustersFiles);
}

void ManagerView::updateFileList(QList<QString> extensions){
  neuroscopeComboBox->clear();
  QStringList neuroscopeFiles;
  neuroscopeFiles<<".dat"<<".eeg";
  
  QList<QString>::iterator iterator;
  for(iterator = extensions.begin(); iterator != extensions.end(); ++iterator){
   QString extension = QString(".%1").arg(static_cast<QString>(*iterator));
   neuroscopeFiles<<extension;
  }
  neuroscopeComboBox->insertStringList(neuroscopeFiles);
}

void ManagerView::updateScriptList(const QList<QString>& scriptNames){
  scriptsComboBox->clear();
  QStringList scripts;
  
  QList<QString>::const_iterator iterator;
  for(iterator = scriptNames.begin(); iterator != scriptNames.end(); ++iterator){
    scripts<<static_cast<QString>(*iterator);  
  }
  scriptsComboBox->insertStringList(scriptNames);
}

void ManagerView::launchNeuroscope(){
 emit checkBeforeLaunchingPrograms();
 //The parameter file is new or has been imported from an existing file.
 if(parameterUrl.fileName() == "Untitled"){
  QMessageBox::critical (this,tr("Unsaved file!"),tr("In order to launch NeuroScope, the parameter file has to be saved first.") );
  return;
 }
 else{
  if(!isUptoDate){
   QMessageBox::critical (this, tr("Unsaved file!"),tr("The parameter file contains unsaved data, please save before launching NeuroScope."));
   return;   
  }
  //Launch NeuroScope.
  else{
  QString fileName = parameterUrl.fileName();
  QString baseName = fileName.left(fileName.length()-4);
  QString fileToUse = baseName.append(neuroscopeComboBox->currentText());
  QProcess::startDetached("neuroscope", QStringList()<<QString(parameterUrl.directory(false) + fileToUse));
  }
 }
}

void ManagerView::launchKlusters(){
 if(klustersComboBox->count() == 0){
  QMessageBox::critical (this, tr("Undefined spike groups"),tr("No spike groups have been defined, please define at least one spike group before launching Klusters."));
  return;  
 }

 emit checkBeforeLaunchingPrograms();
 //The parameter file is new or has been imported from an existing file.
 if(parameterUrl.fileName() == "Untitled"){
  QMessageBox::critical (this, tr("Unsaved file!"),tr("In order to launch Klusters, the parameter file has to be saved first."));
  return;
 }
 else{
  if(!isUptoDate){
   QMessageBox::critical (this, tr("Unsaved file!"),tr("The parameter file contains unsaved data, please save before launching Klusters."));
   return;   
  }
  //Launch Klusters.
  else{
  QString fileName = parameterUrl.fileName();
  QString baseName = fileName.left(fileName.length()-4);
  QString fileToUse = baseName.append(".spk.").append(klustersComboBox->currentText());
  QProcess::startDetached("klusters",QStringList()<<parameterUrl.directory(false) + fileToUse);
  }
 }
}

void ManagerView::launchScript(){
 emit checkBeforeLaunchingScripts();
 QString script = scriptsComboBox->currentText();
 //The parameter file is new or has been imported from an existing file.
 if(parameterUrl.fileName() == "Untitled"){
  QString message = tr("In order to launch %1, the parameter file has to be saved first.").arg(script);
  QMessageBox::critical (this, tr("Unsaved file!"),message);
  return;
 }
 else{
  if(!isUptoDate){
   QString message = tr("The parameter file contains unsaved data or script, please save before launching %1.").arg(script);
   QMessageBox::critical (this, tr("Unsaved data!"),message);
   return;   
  }
  //Launch the script.
  else{
	  QString fileName = parameterUrl.fileName();
	  QString baseName = fileName.left(fileName.length()-4);
	  konsole->runCommand(script+" "+baseName);
  }
 }
}


void ManagerView::stopScript(){
}


void ManagerView::updateDocUrl(const QString url){
 parameterUrl = QString(url);
}

void ManagerView::updateDocumentInformation(const QString url,bool isUptoDate){
 parameterUrl = QString(url);
 this->isUptoDate = isUptoDate;
}

#include "managerview.moc"

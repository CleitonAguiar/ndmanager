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
#ifndef MANAGERVIEW_H
#define MANAGERVIEW_H

// include files for Qt
#include <qsplitter.h>
#include <qstring.h>
#include <qcombobox.h>
#include <qspinbox.h> 
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <Q3ValueList>

//Include for the application
#include "launcherpage.h"
#include "ndkonsole.h"

//Include files for KDE
#include <QToolBar>
#include <QDockWidget>

/**
@author Lynn Hazan
*/
class ManagerView : public Q3Frame/*QSplitter*/
{
Q_OBJECT
public:
    ManagerView(QWidget *parent = 0, const char *name = 0);
    ~ManagerView();

 /**Information retun after a call to addKonsole*/
  enum returnMessage {OK=0,NO_KPART=1,PART_LOADING_ERROR=2};
    
  /** Adds a Konsole and a LauncherPage.
  * @param url url of the currently open parameter file.
  * @param nbSpikeGroups number of current spike groups.
  * @param fileExtensions list containing the extensions of the all specific files (other than the .dat and .eeg files).
  * @param scriptNames list containing the script names.
  * @return message indicating if the konsole could be added.
  */
  returnMessage addKonsole(const QString url,int nbSpikeGroups,Q3ValueList<QString> fileExtensions,
                          const Q3ValueList<QString>& scriptNames); 

  /** Updates the dropdown list containing the spike groups used to launch Klusters.
  * @param nbGroups the new number of spike groups.
  */
  void updateSpikeGroupList(int nbGroups);
  
  /**Triggers the update of the dropdown list containing the file extensions.
  * @param extensions list containing the extensions of all the specific files.
  */
  void updateFileList(Q3ValueList<QString> extensions);

  /**Triggers the update of the dropdown list containing the script names.
  * @param scriptNames list containing the script names.
  */
  void updateScriptList(const Q3ValueList<QString>& scriptNames);

  /** Updates the parameter file url.
  * @param url url of the currently open parameter file.
  */  
  void updateDocUrl(const QString url);

  /** Updates the parameter file information.
  * @param url url of the currently open parameter file.
  * @param isUptoDate true if the parameter file is up to date, false if there are unsaved changes.
  */
  void updateDocumentInformation(const QString url,bool isUptoDate);

signals:
  void beingDestroyed();
  void checkBeforeLaunchingPrograms();
  void checkBeforeLaunchingScripts();
   
public slots:  
 inline void konsoleBeingDestroyed(){emit beingDestroyed();};
 void neuroscopeFileChange(int index);

private slots: 
 /**Launches NeuroScope using, as a parameter, the file selected in the dropdown list containing the file extensions 
 */
 void launchNeuroscope();
 
 /**Launches Klusters using, as a parameter, the electrode group selected in the dropdown list containing the electrode ids 
 */
 void launchKlusters();

 /**Launches the script selected in the dropdown list containing the existing scripts. 
 */
 void launchScript();

 /**Stops the currently running script. 
 */
 void stopScript();

private:
 NdKonsole* konsole;  
 LauncherPage* launcherpage;
 QToolBar* toolbar;
 Q3VBoxLayout* frameLayout;
 QComboBox* neuroscopeComboBox;
 QComboBox* klustersComboBox;
 QComboBox* scriptsComboBox;
 QString parameterUrl;
 bool isUptoDate;
};

#endif

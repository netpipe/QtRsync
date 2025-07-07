/***************************************************************************
 *   Copyright (C) 2007 by Marco Brescianini   *
 *   marco454_36@hotmail.com   *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QLabel>

class QAction;
class QMenu;
class QPushButton;
class QCheckBox;
class QTabWidget;
class QLabel;
class QLineEdit;
class QHBoxLayout;
class QGridLayout;
class RSyncWidget;
class QComboBox;

class MainWindow : public QMainWindow
{

    Q_OBJECT

    public:

	MainWindow();
	~MainWindow();

    protected:
	void closeEvent(QCloseEvent* event);

    private slots:

	void about();
	void aboutQt();
	void slotExecute();
	void slotDryRun();
	void slotArchive();
	void sourceFileDialog();
	void destFileDialog();

    private:

	void createMenus();
	void createActions();
	void createButtons();
	void createTabs();
	void createCheckBoxes();
	void createSourceDest();
	void createSession();

	QStringList optionsChecked();

	//Menus
	QMenu* fileMenu;
	QMenu* helpMenu;
	
	//Actions
	QAction* quitAct;
	QAction* aboutAct;
	QAction* aboutQtAct;
	QAction* executeAct;
	QAction* dryRunAct;

	//Buttons
	QPushButton* quitButton;
	QPushButton* runButton;
	QPushButton* dry_runButton;
	
	//CheckBoxes
	QCheckBox* checkPresPerms;
	QCheckBox* checkPresHardLinks;
	QCheckBox* checkPresDate;
	QCheckBox* checkArchiveMode;
	QCheckBox* checkPresGroup;
	QCheckBox* checkRelativePaths;
	QCheckBox* checkPresDev;
	QCheckBox* checkCompressData;
	QCheckBox* checkBackup;
	QCheckBox* checkUpdate;
	QCheckBox* checkMapUidGid;
	QCheckBox* checkVerbose;
	QCheckBox* checkSymLinks;
	QCheckBox* checkRecursive;
	QCheckBox* checkDeleteOnReceiver;
	QCheckBox* checkPresSpecial;
	QCheckBox* checkFilesystemBoundaries;
	QCheckBox* checkChecksum;
	QCheckBox* checkSizeOnly;
	QCheckBox* checkPartially;	
	QCheckBox* checkPresOwner;
	QCheckBox* checkDirs;
	QCheckBox* checkStats;
	QCheckBox* checkHuman;
	QCheckBox* checkExisting;
	QCheckBox* checkList;
	QCheckBox* checkProgress;

	//Source & Dest visuals
	QLabel* sourceLabel;
	QLabel* destLabel;
	QLineEdit* sourceLine;
	QLineEdit* destLine;	
	QPushButton* sourceBrowse;
	QPushButton* destBrowse;
	QPushButton* clearSource;
	QPushButton* clearDest;
	QWidget* sourceWid;
	QWidget* destWid;

	//Session
	QLabel* sessionLabel;
	QComboBox* comboSession;

	//Layouts
	QGridLayout* urlLayout;
	QHBoxLayout* buttonLayout;
	QHBoxLayout* sessionLayout;

	//Tabs
	QTabWidget* tabWidget;
	QWidget *pageBaseOption;
	QWidget *pageAdvancedOption;
	QWidget* pageExtraOption;

	QWidget* centralWidget;
		
	//Source & Dest
	QUrl sourceUrl;
	QUrl destUrl;

	RSyncWidget* run;
	
	bool dryRun;	
};


#endif //multiple inclusion guard


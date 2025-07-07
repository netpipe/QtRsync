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

#include <QtGui>
#include <QMessageBox>

#include "rsyncwidget.h"
#include "mainwindow.h"


static QString programName = "QtRsync";
static QString version = "0.1";

MainWindow::MainWindow ()
		: QMainWindow(),
		sourceUrl ( 0 ),
		destUrl ( 0 ),
		dryRun(false)
{
	centralWidget = new QWidget ( this );

	createActions();
	createMenus();
	createButtons();
	createSourceDest();
	createTabs();
	//createSession();

	QVBoxLayout* layout = new QVBoxLayout;
	
	//layout->addLayout (sessionLayout);
	//layout->addLayout ( urlLayout );
	layout->addWidget(sourceLabel);
	layout->addWidget(sourceWid);
	layout->addWidget(destLabel);
	layout->addWidget(destWid);
	layout->addWidget ( tabWidget );
	layout->addLayout ( buttonLayout);
	setWindowTitle ( programName );

	centralWidget->setLayout ( layout );
	setCentralWidget ( centralWidget );

	run = new RSyncWidget(0, Qt::SubWindow);
	run->hide();
}

//Do not implement
MainWindow::~MainWindow()
{
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu ( tr ( "&File" ) );
	fileMenu -> addAction ( executeAct );
	fileMenu -> addAction ( dryRunAct );
	fileMenu->addSeparator();
	fileMenu -> addAction ( quitAct );

	helpMenu = menuBar()->addMenu ( tr ( "&Help" ) );
	helpMenu -> addAction ( aboutAct );
	helpMenu -> addAction ( aboutQtAct );

}


void MainWindow::createActions()
{
	quitAct = new QAction ( tr ( "&Quit" ), this );
	connect ( quitAct, SIGNAL ( activated() ), this, SLOT ( close() ) );

	aboutAct = new QAction ( tr ( "&About" ), this );
	connect ( aboutAct, SIGNAL ( activated() ), this, SLOT ( about() ) );

	aboutQtAct = new QAction ( tr ( "About &Qt" ), this );
	connect ( aboutQtAct, SIGNAL ( activated() ), this, SLOT ( aboutQt() ) );

	executeAct = new QAction ( tr ( "Execute" ), this );
	connect ( executeAct, SIGNAL ( activated() ), this, SLOT ( slotExecute() ) );

	dryRunAct = new QAction ( tr ( "Dry Run" ), this );
	connect ( dryRunAct, SIGNAL ( activated() ), this, SLOT ( slotDryRun() ) );
}


void MainWindow::createButtons()
{
	quitButton = new QPushButton ( tr ( "&Quit" ), centralWidget );
	QIcon quitIcon(":/icons/application-exit.png");
	quitButton->setIcon(quitIcon);
	quitButton->setMinimumWidth(120);
	connect ( quitButton, SIGNAL ( clicked() ), this, SLOT ( close() ) );

	runButton = new QPushButton ( tr ( "&Run" ), centralWidget );
	QIcon runIcon(":/icons/system-run.png");
	runButton->setIcon(runIcon);
	runButton->setMinimumWidth(120);
	connect ( runButton, SIGNAL ( clicked() ), this, SLOT ( slotExecute() ) );

	dry_runButton = new QPushButton ( tr ( "&Dry Run" ), centralWidget );
	QIcon dryIcon(":/icons/help-about.png");
	dry_runButton->setIcon(dryIcon);
	dry_runButton->setMinimumWidth(120);
	connect ( dry_runButton, SIGNAL ( clicked() ), this, SLOT ( slotDryRun() ) );

	buttonLayout = new QHBoxLayout();
	buttonLayout->setMargin(5);
	buttonLayout->setAlignment(Qt::AlignRight);
	buttonLayout->addWidget ( runButton );
	buttonLayout->addWidget ( dry_runButton );
	buttonLayout->addWidget ( quitButton );
}


void MainWindow::slotExecute()
{
	sourceUrl.setUrl(sourceLine->text());
	destUrl.setUrl(destLine->text());

	qDebug() << sourceUrl;
	qDebug() << destUrl;

	if ( sourceUrl.isEmpty() || destUrl.isEmpty() )
	{
		QMessageBox::critical ( this, tr ( "Error" ), tr ( "Please insert a source and a destination." ) );
	}
	else
	{
		run->show();
		
		QStringList *options = new QStringList ();
		
		if(dryRun)
		{
		*options << "-n" << optionsChecked();
		}else{
		*options << optionsChecked();
		}

		qDebug() << "Args List: " << *options;

		*options << sourceUrl.toString() << destUrl.toString();

		run->startRsync(*options);
		sourceLine->clear();
		destLine->clear();

		delete options;
		options=0;
	}
}

void MainWindow::slotDryRun()
{
	dryRun = true;
	slotExecute();
	dryRun = false;
}

void MainWindow::about()
{
	QString msg("");
	msg += tr ( "<b>QtRsync</b> is a simple GUI for the rsync tool");
	msg += "<p>";
	msg += "This program is free software; you can redistribute it and/or "
            	"modify it under the terms of the GNU General Public License "
            	"as published by the Free Software Foundation; either version 2 "
            	"of the License, or (at your option) any later version" ;
	msg += "</p>";
	msg += "<p>";
  	msg += "This program is distributed in the hope that it will be useful, "
        	"but WITHOUT ANY WARRANTY; without even the implied warranty of "
            	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
            	"GNU General Public License for more details.";
  	msg += "</p>";

	msg += "<p>";
	msg += "All icons are part of KDE 4 main icon theme.                    "
	       "See <a href=\"http://www.oxygen-icons.org\"> www.oxygen-icons.org </a> for legal and licenses ";
	msg += "</p>";	

	msg += "<b>Copyright (C) 2007 Marco Brescianini</b>";
	msg += "\n";
	msg += "<p>";
	msg += "All other trademarks are property of their respective owners";
	msg += "</p>";
	msg += "<p>";
	msg += "<b>Version: </b>" + version;
	msg += "</p>";

	QMessageBox::about ( this, tr ( "About QtRsync"), msg );
}

void MainWindow::aboutQt()
{
	QMessageBox::aboutQt ( this, tr ( "About Qt" ) );
}

void MainWindow::createTabs()
{
	tabWidget = new QTabWidget();
	pageBaseOption = new QWidget();
	pageAdvancedOption = new QWidget();
	pageExtraOption = new QWidget();

	createCheckBoxes();

	QGridLayout *gridLayout = new QGridLayout ( pageBaseOption );
	gridLayout->addWidget ( checkPresDate, 0, 1 );
    gridLayout->addWidget( checkPresPerms, 0, 2 );
    gridLayout->addWidget ( checkPresOwner, 1, 1 );
	gridLayout->addWidget ( checkPresGroup, 1, 2 );
	gridLayout->addWidget ( checkRecursive, 2, 1 );
	gridLayout->addWidget ( checkVerbose, 2, 2 );
	gridLayout->addWidget ( checkDeleteOnReceiver, 3, 1 );
	gridLayout->addWidget ( checkUpdate, 3, 2 );
	gridLayout->addWidget ( checkSizeOnly, 4, 1 );
	gridLayout->addWidget ( checkArchiveMode, 4, 2 );
//	pageBaseOption->setLayout ( gridLayout );

	QGridLayout* gridAdvancedLayout = new QGridLayout ( pageAdvancedOption );
	
	gridAdvancedLayout->addWidget ( checkSymLinks, 0, 1 );
	gridAdvancedLayout->addWidget ( checkMapUidGid, 0, 2 );
	gridAdvancedLayout->addWidget ( checkPresHardLinks, 1, 1 );
	gridAdvancedLayout->addWidget ( checkFilesystemBoundaries, 1, 2 );
	gridAdvancedLayout->addWidget ( checkChecksum, 2, 1 );
	gridAdvancedLayout->addWidget ( checkPresDev, 2, 2 );
	gridAdvancedLayout->addWidget ( checkCompressData, 3, 1 );
	gridAdvancedLayout->addWidget ( checkPartially, 3, 2 );
	gridAdvancedLayout->addWidget ( checkRelativePaths, 4, 1 );
	
//	pageAdvancedOption->setLayout( gridAdvancedLayout );

	QGridLayout* gridExtraLayout = new QGridLayout ( pageExtraOption );
	gridExtraLayout->addWidget ( checkBackup, 0, 1 );	
	gridExtraLayout->addWidget(checkHuman, 0, 2);
	gridExtraLayout->addWidget(checkList, 1, 1);
	gridExtraLayout->addWidget(checkExisting, 1, 2);
	gridExtraLayout->addWidget ( checkStats, 2, 1);
	gridExtraLayout->addWidget ( checkPresSpecial, 2, 2 );
	gridExtraLayout->addWidget(checkProgress, 3, 1);
	gridExtraLayout->addWidget(checkDirs, 3, 2);

	tabWidget->addTab ( pageBaseOption, tr ( "Options" ) );
	tabWidget->addTab ( pageAdvancedOption, tr ( "Advanced Options" ) );
	tabWidget->addTab ( pageExtraOption, tr ( "Extra Options"));
}


void MainWindow::createCheckBoxes()
{
	//creating checkboxes

	checkPresPerms = new QCheckBox ( tr ( "Preserve Permissions" ) );//, pageBaseOption);
	checkPresPerms->setToolTip(tr("--perms - preserve permissions"));
	checkPresDate = new QCheckBox ( tr ( "Preserve Date" ) );//, pageBaseOption);
	checkPresDate->setToolTip(tr("--times - preserve times"));
	checkPresDev = new QCheckBox ( tr ( "Preserve Device" ) );//, pageBaseOption);
	checkPresDev->setToolTip(tr("--devices - preserve device files"));
	checkSymLinks = new QCheckBox ( tr ( "Preserve Symlinks" ) );//, pageBaseOption);
	checkSymLinks->setToolTip(tr("-l, --links - copy symlinks as symlinks"));
	checkUpdate = new QCheckBox ( tr ( "Update" ) );//, pageBaseOption);
	checkUpdate->setToolTip(tr("-u, --update - skip files that are newer on the receiver"));
	checkChecksum = new QCheckBox ( tr ( "Checksum" ) );//, pageBaseOption);
	checkChecksum->setToolTip(tr("-c, --checksum - skip based on checksum, not mod-time & size"));
	checkCompressData = new QCheckBox ( tr ( "Compress Data" ) );//, pageBaseOption);
	checkCompressData->setToolTip(tr("-z, --compress - compress file data during the transfer"));
	checkArchiveMode = new QCheckBox ( tr ( "Archive Mode" ) );//, pageBaseOption);
	checkArchiveMode->setToolTip(tr("-a, --archive - archive mode; same as -rlptgoD (no -H, -A)"));
	checkDeleteOnReceiver = new QCheckBox ( tr ( "Delete on receiver" ) );//, pageBaseOption);
	checkDeleteOnReceiver->setToolTip(tr("--delete - delete extraneous files from dest dirs"));
	checkVerbose = new QCheckBox ( tr ( "Increase Verbosity" ) );//, pageBaseOption);
	checkVerbose->setToolTip(tr("-v, --verbose - increase verbosity"));
	checkPresHardLinks = new QCheckBox ( tr ( "Preserve HardLinks" ) );//, pageBaseOption);
	checkPresHardLinks->setToolTip(tr("-H, --hard-links - preserve hard links"));
	checkPresSpecial = new QCheckBox ( tr ( "Preserve special files" ) );//, pageBaseOption);
	checkPresSpecial->setToolTip(tr("--specials - preserve special files"));
	checkMapUidGid = new QCheckBox ( tr ( "Do not map uid/gid values" ) );//, pageBaseOption);
	checkMapUidGid->setToolTip(tr("--numeric-ids - don't map uid/gid values by user/group name"));
	checkPresGroup = new QCheckBox ( tr ( "Preserve Group" ) );//, pageBaseOption);
	checkPresGroup->setToolTip(tr("-g, --group - preserve group"));
	checkBackup = new QCheckBox ( tr ( "Backup" ) );//, pageBaseOption);
	checkBackup->setToolTip(tr("-b, --backup - make backups"));
	checkRelativePaths = new QCheckBox ( tr ( "Relative Pathnames" ) );//, pageBaseOption);
	checkRelativePaths->setToolTip(tr("-R, --relative - use relative path names"));
	checkFilesystemBoundaries = new QCheckBox ( tr ( "Do not cross filesystem boundaries" ) );//, pageBaseOption);
	checkFilesystemBoundaries->setToolTip(tr("-x, --one-file-system - don't cross filesystem boundaries"));
	checkRecursive = new QCheckBox ( tr ( "Recursive" ) );//, pageBaseOption);
	checkRecursive->setToolTip(tr("-r, --recursive - recurse into directories"));
	checkPartially = new QCheckBox ( tr ( "Keep Partially transferred files" ) );//, pageBaseOption);
	checkPartially->setToolTip(tr("--partial - keep partially transferred files"));
	checkSizeOnly = new QCheckBox ( tr ( "Size Only" ) );//, pageBaseOption);
	checkSizeOnly->setToolTip(tr("--size-only - skip files that match in size"));
	checkPresOwner = new QCheckBox ( tr ( "Preserve Owner" ) );//, pageBaseOption);
	checkPresOwner->setToolTip(tr("-o, --owner - preserve owner"));
	checkDirs = new QCheckBox (tr("Directories"));
	checkDirs->setToolTip(tr("-d, --dirs - transfer directories without recursing"));
	checkStats = new QCheckBox(tr("Statistics"));
	checkStats->setToolTip(tr("--stats - give some file-transfer stats"));
	checkHuman = new QCheckBox(tr("Human readable"));
	checkHuman->setToolTip(tr("-h, --human-readable - output numbers in a human-readable format"));
	checkExisting = new QCheckBox(tr("Skip existing files"));
	checkExisting->setToolTip(tr("--existing - skip creating new files on receiver"));
	checkList = new QCheckBox(tr("List files"));
	checkList->setToolTip(tr("--list-only - list the files instead of copying them"));
	checkProgress = new QCheckBox(tr("Show Progress"));
	checkProgress->setToolTip(tr("--progress - show progress during transfer"));

	checkProgress->setChecked(true);
	checkHuman->setChecked(true);
	checkRecursive->setChecked(true);
	
	//If archive mode is checked we disable all options that will be used by this one.
	connect ( checkArchiveMode, SIGNAL ( stateChanged ( int ) ), this, SLOT ( slotArchive() ) );
}


void MainWindow::slotArchive()
{
//TODO: Be sure that hardlinks and preserve ACLS are not checked
	if ( checkArchiveMode->checkState() == Qt::Checked )
	{
		checkRecursive->setCheckState ( Qt::Checked );
		checkRecursive->setCheckable ( false );
		checkRecursive->setEnabled ( false );
		checkSymLinks->setCheckState ( Qt::Checked );
		checkSymLinks->setCheckable ( false );
		checkSymLinks->setEnabled ( false );
		checkPresPerms->setCheckState ( Qt::Checked );
		checkPresPerms->setCheckable ( false );
		checkPresPerms->setEnabled ( false );
		checkPresDate->setCheckState ( Qt::Checked );
		checkPresDate->setCheckable ( false );
		checkPresDate->setEnabled ( false );
		checkPresGroup->setCheckState ( Qt::Checked );
		checkPresGroup->setCheckable ( false );
		checkPresGroup->setEnabled ( false );
		checkPresOwner->setCheckState ( Qt::Checked );
		checkPresOwner->setCheckable ( false );
		checkPresOwner->setEnabled ( false );
		checkPresDev->setCheckState ( Qt::Checked );
		checkPresDev->setCheckable ( false );
		checkPresDev->setEnabled ( false );
		checkPresSpecial->setCheckState ( Qt::Checked );
		checkPresSpecial->setCheckable ( false );
		checkPresSpecial->setEnabled ( false );
	}
	else
	{
		checkRecursive->setCheckState ( Qt::Unchecked );
		checkRecursive->setCheckable ( true );
		checkRecursive->setEnabled ( true );
		checkSymLinks->setCheckState ( Qt::Unchecked );
		checkSymLinks->setCheckable ( true );
		checkSymLinks->setEnabled ( true );
		checkPresPerms->setCheckState ( Qt::Unchecked );
		checkPresPerms->setCheckable ( true );
		checkPresPerms->setEnabled ( true );
		checkPresDate->setCheckState ( Qt::Unchecked );
		checkPresDate->setCheckable ( true );
		checkPresDate->setEnabled ( true );
		checkPresGroup->setCheckState ( Qt::Unchecked );
		checkPresGroup->setCheckable ( true );
		checkPresGroup->setEnabled ( true );
		checkPresOwner->setCheckState ( Qt::Unchecked );
		checkPresOwner->setCheckable ( true );
		checkPresOwner->setEnabled ( true );
		checkPresDev->setCheckState ( Qt::Unchecked );
		checkPresDev->setCheckable ( true );
		checkPresDev->setEnabled ( true );
		checkPresSpecial->setCheckState ( Qt::Unchecked );
		checkPresSpecial->setCheckable ( true );
		checkPresSpecial->setEnabled ( true );
	}

}


void MainWindow::createSourceDest()
{
	sourceLabel = new QLabel ( tr ( "<b> Source: <b/>" ) );
	destLabel = new QLabel ( tr ( "<b> Destination: <b/> " ) );

	sourceLine = new QLineEdit();
	destLine = new QLineEdit();
	
	QIcon broIcon(":/icons/folder.png"); 
	sourceBrowse = new QPushButton( tr ( "Browse") );	
	destBrowse = new QPushButton( tr ( "Browse") );
	sourceBrowse->setIcon(broIcon);
	destBrowse->setIcon(broIcon);

	clearSource = new QPushButton();
	clearDest = new QPushButton();

	QIcon sourceIcon(":/icons/edit-clear-locationbar.png");
	clearSource->setIcon(sourceIcon);
	clearSource->setFlat(true);
	clearDest->setIcon(sourceIcon);
	clearDest->setFlat(true);
	
	sourceWid = new QWidget;
	QHBoxLayout* lay = new QHBoxLayout;
	lay->addWidget(clearSource);
	lay->addWidget(sourceLine);
	lay->addWidget(sourceBrowse);
	sourceWid->setLayout(lay);

	destWid = new QWidget;
	QHBoxLayout* lay2 = new QHBoxLayout;
	lay2->addWidget(clearDest);
	lay2->addWidget(destLine);
	lay2->addWidget(destBrowse);
	destWid->setLayout(lay2);

	/*
	urlLayout = new QGridLayout;	
	urlLayout->setMargin(2);
	urlLayout->addWidget ( sourceLabel );
	urlLayout->addWidget (clearSource);
	urlLayout->addWidget ( sourceLine );
	urlLayout->addWidget ( sourceBrowse);
	urlLayout->addWidget ( destLabel);
	urlLayout->addWidget (clearDest);
	urlLayout->addWidget ( destLine );
	urlLayout->addWidget ( destBrowse);
	*/
	
	connect (sourceBrowse, SIGNAL(clicked()), this, SLOT(sourceFileDialog()));
	connect (destBrowse, SIGNAL(clicked()), this, SLOT(destFileDialog()));
	connect (clearSource, SIGNAL(clicked()), sourceLine, SLOT(clear()));
	connect (clearDest, SIGNAL(clicked()), destLine, SLOT(clear()));
}

void MainWindow::sourceFileDialog()
{
	//QString sourceFile = QFileDialog::getOpenFileName(0, "Select a source", "/home");

	QFileDialog sourceDialog(this);
	sourceDialog.setDirectory(QDir::home());
	sourceDialog.setFileMode(QFileDialog::Directory);
	sourceDialog.setWindowTitle(tr("Select the Source Directory"));
	
	QStringList source(0);	

	if(sourceDialog.exec())
	source = sourceDialog.selectedFiles();

	if(!source.empty()){
	sourceLine->clear();
	sourceLine->insert(source.first());
	}
	else
	qDebug() << "Source List is empty";
	
}


void MainWindow::destFileDialog()
{

	QFileDialog destDialog(this);
	destDialog.setDirectory(QDir::home());
	destDialog.setFileMode(QFileDialog::Directory);
	destDialog.setWindowTitle(tr("Select the Destination directory"));
	QStringList dest(0);
	
	if(destDialog.exec())
	dest = destDialog.selectedFiles();

	if(!dest.empty()){
	destLine->clear();
	destLine->insert(dest.first());
	}
	else
	qDebug() << "Destination List is empty";
}



QStringList MainWindow::optionsChecked()
{
	QStringList args(0);
	
	if(checkArchiveMode->isChecked()) args<< "-a";
	if(checkPresOwner->isChecked()) args << "--owner";
	if(checkPresPerms->isChecked()) args << "--perms";
	if(checkPresDate->isChecked()) args << "--times";
	if(checkPresGroup->isChecked()) args << "--group";
	if(checkRelativePaths->isChecked()) args << "--relative";
	if(checkPresDev->isChecked()) args << "--devices";
	if(checkCompressData->isChecked()) args <<"--compress";
	if(checkBackup->isChecked()) args << "--backup";
	if(checkUpdate->isChecked()) args << "--update";
	if(checkMapUidGid->isChecked()) args << "--numeric-ids";
	if(checkSymLinks->isChecked()) args << "--links";
	if(checkPresHardLinks->isChecked()) args << "--hard-links";
	if(checkRecursive->isChecked()) args << "-r";
	if(checkDeleteOnReceiver->isChecked()) args << "--delete";
	if(checkPresSpecial->isChecked()) args << "--specials";
	if(checkFilesystemBoundaries->isChecked()) args << "--one-file-system";
	if(checkChecksum->isChecked()) args << "--checksum";
	if(checkSizeOnly->isChecked()) args << "--size-only";
	if(checkPartially->isChecked()) args << "--partial";
	if(checkDirs->isChecked()) args << "--dirs";
	if(checkStats->isChecked()) args << "--stats";
	if(checkHuman->isChecked()) args << "--human-readable";
	if(checkExisting->isChecked()) args << "--existing";
	if(checkList->isChecked()) args << "--list-only";
	if(checkProgress->isChecked()) args << "--progress";
	if(checkVerbose->isChecked()) args << "-v";
	
	args.removeFirst(); //This function needs to be called in order to remove the empty space created in initialization
	return args;
}

/*
void MainWindow::createSession()
{
	sessionLabel = new QLabel(tr("<b>Session:<b/>"));
	comboSession = new QComboBox();
	sessionLayout = new QHBoxLayout();
	sessionLayout->setMargin(5);
	sessionLayout->addWidget(sessionLabel);
	sessionLayout->addWidget(comboSession);
	
}*/

void MainWindow::closeEvent(QCloseEvent* event)
{
	if(run->canClose())
	{
		event->accept();
	}else{
		QMessageBox::warning(this, tr("Warning"), tr("Rsync is still running"));
		event->ignore();
	}

}

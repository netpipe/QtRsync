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
#include "rsyncwidget.h"
#include "rsyncwrapper.h"

RSyncWidget::RSyncWidget(QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	rsyncOutput = new QTextEdit();
	//rsyncOutput->setMaximumSize(640,480);
	rsyncOutput->setMinimumSize(400,220);
	rsyncOutput->setReadOnly(true);
	
	closeButton = new QPushButton(tr("&Close"));
	closeButton->setMinimumWidth(100);
	QIcon closeIcon("icons/window-close.png");
	closeButton->setIcon(closeIcon);
	connect (closeButton, SIGNAL(clicked()), this, SLOT(close()));
	
	QVBoxLayout* lay = new QVBoxLayout;
	lay->addWidget(rsyncOutput);
	lay->addWidget(closeButton);
	lay->setAlignment(closeButton, Qt::AlignRight);
	setLayout(lay);
	
	rsync = new RsyncWrapper;
	
	connect(rsync, SIGNAL(dataReady(const QString &)), rsyncOutput, SLOT(append(const QString &)));
	//connect(rsync, SIGNAL(dataReady(const QString & data)), rsyncOutput, SLOT(scrollToAnchor(const QString & data)));
}


RSyncWidget::~RSyncWidget()
{
	delete rsync;
	delete closeButton;
	delete rsyncOutput;
	rsync=0;
	closeButton=0;
	rsyncOutput=0;
}


void RSyncWidget::startRsync(QStringList & args)
{
	if(rsync->isRunning())
	{
		QMessageBox::critical(this, tr("Error"), tr("RSync is already running."));
	}else{
		QString msg("");
		msg+=tr("*** Launching Rsync... ***");
		msg+="\n";

		QStringList::Iterator iter;

		msg+=program + " ";

		for(iter=args.begin(); iter!=args.end(); ++iter)
		{
		msg+=*iter;
		msg+=" ";	
		}
		msg+="\n";
	
		rsyncOutput->append(msg);

		rsync->setArgs(args);
		rsync->start();
	}
}


void RSyncWidget::closeEvent(QCloseEvent* event)
{
	if(canClose())
	{
		rsyncOutput->clear();
		event->accept();
	}else{
		QMessageBox::warning(this,tr("Warning"), tr("RSync is still running"));
		event->ignore();
	}
}


bool RSyncWidget::canClose()
{
	if(rsync->isRunning())
	{
	return false;
	}else{
	return true;
	}
}


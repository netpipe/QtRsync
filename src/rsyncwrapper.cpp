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

#include <QtCore>
#include <QProcess>
#include "rsyncwrapper.h"
#include "rsyncwidget.h"


RsyncWrapper::RsyncWrapper() : QThread()
{
    args = new QStringList();

}

RsyncWrapper::~RsyncWrapper()
{
	delete args;
	args=0;
}

void RsyncWrapper::run()
{
/*	QProcess process;
	process.setProcessChannelMode(QProcess::MergedChannels);

	qDebug() << *args;
	
	process.start(program, *args);
	if(!process.waitForStarted())
	return;
*/
	QProcess* process = new QProcess();
	process->setProcessChannelMode(QProcess::MergedChannels);
	
	process->start(program, *args);
	if(!process->waitForStarted())
	qDebug() << "Process did not start";
/*

	//qDebug() << process->execute(program, args);
	if(!process.waitForFinished())
	return;
	else
	{
	QString out("");
	out += process.readAll();	
	//qDebug() << process.readAll();	
	//output += process.readAll();
	qDebug() << out;
	output+=out;
	}	
*/
	//FIXME: Sometimes this function skips some words, this should be fixed. I thought that a possible solution 
	//is to implement the QProcess has a member of rsyncwrapper and link some signals and slots. But there's
	// a problem, if the process is a member of the class it is running on a different thread.
	while(process->waitForReadyRead())
	{
	QString line = process->readLine();
	
	emit dataReady(line.simplified());
	qDebug() << line << " " << line.size();//process->readLine();

	
	}
	
	//NOTE: This solution sucks. Find a better one
	if(process->pid()==0)
	{
		delete process;
		qDebug() << "Process deleted";
		clearArgs();
	}else{
		qDebug() << "We have a problem " << "Process did not finished";
		process->kill();
		delete process;
	}
}

void RsyncWrapper::setArgs(const QStringList& argsList)
{
	clearArgs();
	*args << argsList;
}

void RsyncWrapper::clearArgs()
{
	args->clear();
}

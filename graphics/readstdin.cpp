/***************************************************************************
 *   Copyright (C) 2009 by Volodymyr Shevchyk                              *
 *   volder@users.sourceforge.net                                          *
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

#include <QDebug>
#include <stdio.h>
#include <QCoreApplication>

#include "readstdin.h"

ReadStdIn::ReadStdIn(QObject *parent)
 : QThread(parent)
{
}


ReadStdIn::~ReadStdIn()
{
}

void ReadStdIn::run()
{
	QString str;
	QTextStream in(stdin);
        do
	{
            QCoreApplication::processEvents();
            str = in.readLine();
            if (!str.isEmpty())
                emit commandAppeared(str);
        }
        while (("closegraph();" != str) || ("close();" != str));
}

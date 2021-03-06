/***************************************************************************
 *   Copyright (C) 2008 by Volodymyr Shevchyk                              *
 *   i'mnotageycom.ua                                                      *
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
#include <Qsci/qsciscintilla.h>
#include <QMessageBox>

#include "QDebug"

#include "replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent)
 : QDialog(parent)//, Ui::replaceForm()
{
        setupUi(this);
	textEditor=(QsciScintilla*)parentWidget();
	connect(closeBtn,SIGNAL(clicked()),this,SLOT(close()));
	connect(replaceBtn,SIGNAL(clicked()),this,SLOT(slotReplace()));
}


ReplaceDialog::~ReplaceDialog()
{
}
/**
*********************************************************************
*********translates program code to english***
**/
void ReplaceDialog::replaceCode(QString key, QString translation)
{
    while(textEditor->findFirst(key,false,caseSensCB->isChecked(), wWOnlyCB->isChecked(),false,true,-1,-1,true))
    {
        textEditor->replace(translation);
    }
}

void ReplaceDialog::retranslate(void)
{
	retranslateUi(this);
}

void ReplaceDialog::slotReplaceDialog(void)
{
	show();
	if(findCombo->findText(textEditor->selectedText())==-1)
	{
		if(findCombo->count()>5) findCombo->clear();
		findCombo->insertItem(0,textEditor->selectedText());
		findCombo->setCurrentIndex(0);
	}
}
void ReplaceDialog::slotReplace(void)
{
        int line,index;
	bool startpos=false;
	QVariant v;
	if(!fromcurCB->isChecked())
	{
		textEditor->setCursorPosition(0,0);
	}
	textEditor->getCursorPosition(&line,&index);
	if((line==0)&&(index==0)) startpos=true;
	
	if(promptChB->isChecked())
	{
		if(textEditor->findFirst(	findCombo->currentText(),false,caseSensCB->isChecked(),
						wWOnlyCB->isChecked(),false,true,-1,-1,true))
		{
			close();
			promptMessage();
		}
	}
	else  
	{
		slotReplaceAll();
	}
}
void ReplaceDialog::promptMessage(void)
{
	promptMessageDialog = new QWidget();
	promptMessageDialog->setWindowModality(Qt::ApplicationModal);
	QPushButton *replcBtn = new QPushButton(tr("Replace"));
	QPushButton *repl_closeBtn = new QPushButton(tr("Replace & Close"));
	QPushButton *replaceAllBtn = new QPushButton(tr("Replace All"));
	QPushButton *findNextBtn = new QPushButton(tr("Find Next"));
	QPushButton *cancelBtn = new QPushButton(tr("Cancel"));

	connect(replcBtn,SIGNAL(clicked()),this,SLOT(slotReplaceOnce()));
	connect(repl_closeBtn,SIGNAL(clicked()),this,SLOT(slotReplace_Close()));
	connect(replaceAllBtn,SIGNAL(clicked()),this,SLOT(slotReplaceAll()));
	connect(findNextBtn,SIGNAL(clicked()),this,SLOT(slotFindNext()));
        connect(cancelBtn,SIGNAL(clicked()),this,SLOT(slotCancel()));
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(replcBtn);
	layout->addWidget(repl_closeBtn);
	layout->addWidget(replaceAllBtn);
	layout->addWidget(findNextBtn);
	layout->addWidget(cancelBtn);
	promptMessageDialog->setLayout(layout);
	promptMessageDialog->show();
}
void ReplaceDialog::slotReplaceOnce( void )
{
	textEditor->replace(replaceCombo->currentText());
}

void ReplaceDialog::slotReplaceAll()
{
        int count=0;
	bool startpos=false;
	QVariant v;
label3:	while(textEditor->findFirst(    findCombo->currentText(),false,caseSensCB->isChecked(),
                                        wWOnlyCB->isChecked(),false,true,-1,-1,true))
		{
                        textEditor->replace(replaceCombo->currentText());
			count++;
		}
		close();
		v.setValue(count);
		QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Replace"));
		msgBox.setText(v.toString()+" "+tr("replacements made"));
		msgBox.exec();	
		
		if(fromcurCB->isChecked()&&(startpos==false))
		{
			v.setValue(count);
			QMessageBox msgBox;
                        msgBox.setIcon(QMessageBox::Information);
                        msgBox.setWindowTitle(tr("Replace"));
			msgBox.setText("End of document reached.");
                        msgBox.setInformativeText(tr("Continue from the begining?"));
			QAbstractButton *continueBtn = msgBox.addButton(tr("Continue"),QMessageBox::ActionRole);
			QAbstractButton *stopBtn = msgBox.addButton(tr("Stop"),QMessageBox::ActionRole);
			msgBox.exec();
			if (msgBox.clickedButton()==continueBtn)
			{
				textEditor->setCursorPosition(0,0);
				startpos=true;
				count=0;	
				goto label3;	
			}
                        if (msgBox.clickedButton()==stopBtn)
                        {
                            msgBox.close();
                            promptMessageDialog->close();
                        }
		}
	
}
void ReplaceDialog::slotReplace_Close(void)
{
	textEditor->replace(replaceCombo->currentText());
	promptMessageDialog->close();
}
void ReplaceDialog::slotFindNext(void)
{
	textEditor->findNext();
}
void ReplaceDialog::slotCancel()
{
         promptMessageDialog->close();
}

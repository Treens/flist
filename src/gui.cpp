/* Copyright 2017 Pierre Franco
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0

*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
#include <QApplication>
#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLineEdit>
#include <QLabel>
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>

#include "gui.h"
#include "filelist.h"

MyWindow::MyWindow()
{
    this->setFixedSize(800, 220);

    // Build UI
    build_ui();
}

MyWindow::~MyWindow()
{
    // Apply configuration changes on exit
    config.storeFolder(m_dir->text());
    config.storeResult(m_log->text());
    config.storeDuplicateResult(m_dup->text());
    config.storeFolderList(m_cb_dir->checkState() == Qt::Checked);
    config.storeDuplicateList(m_cb_dup->checkState() == Qt::Checked);

    config.writeChanges();

    delete m_ml;
}

void MyWindow::openDirBrowse()
{
    QString dir = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Select the folder to be listed"), QDir::rootPath());

    if(!dir.isEmpty())
        m_dir->setText(dir);
}

void MyWindow::openLogBrowse()
{
    QString log = QFileDialog::getSaveFileName(this, QString::fromUtf8("Save the folder listing result"), QDir::homePath(), "Fichier CSV (*.csv)");

    if(!log.isEmpty())
        m_log->setText(log);
}

void MyWindow::openDuplicateBrowse()
{
    QString log = QFileDialog::getSaveFileName(this, QString::fromUtf8("Save the duplicate parsing result"), QDir::homePath(), "Fichier CSV (*.csv)");

    if(!log.isEmpty())
        m_dup->setText(log);
}

void MyWindow::dblState(int state)
{
    if(state == Qt::Unchecked)
    {
        m_dup->setDisabled(true);
        m_browsedup->setDisabled(true);
    }
    else if(state == Qt::Checked)
    {
        m_dup->setEnabled(true);
        m_browsedup->setEnabled(true);
    }
}

void MyWindow::startAnalysis()
{
    if(!QDir::isAbsolutePath(m_dir->text()) || !QDir(m_dir->text()).exists())
    {
        QMessageBox::critical(this, "Error", QString::fromUtf8("The selected folder does not exist or the path is invalid."));
        return;
    }
    else if(!QDir::isAbsolutePath(m_log->text()))
    {
        QMessageBox::critical(this, "Error", QString::fromUtf8("The file path for the listing result is invalid."));
        return;
    }

    disable_ui(true);

    QFileInfoList* ret;

    if( (ret = createFileList(m_dir->text(), m_log->text(), m_cb_dir->isChecked())) )
    {
        if(!m_cb_dup->isChecked())
        {
            m_bar->setValue(100);
            QMessageBox::information(this, QString::fromUtf8("Success"), QString::fromUtf8("Listing finished"));
            m_bar->setValue(0);
            disable_ui(false);
            return;
        }

        m_bar->setValue(50);

        if(!QDir::isAbsolutePath(m_dup->text()))
        {
            QMessageBox::critical(this, "Error", QString::fromUtf8("The file path for the duplicate parsing result is invalid."));
            m_bar->setValue(0);
            disable_ui(false);
            return;
        }
        else if(createDuplicateList(ret, m_dup->text(), m_bar))
        {
            m_bar->setValue(100);
            QMessageBox::information(this, QString::fromUtf8("Success"), QString::fromUtf8("Duplicate parsing finished!"));
            m_bar->setValue(0);
        }
        else
        {
            QMessageBox::critical(this, "Error", QString::fromUtf8("An error occured while parsing for duplicates."));
            m_bar->setValue(0);
            disable_ui(false);
            return;
        }

        delete ret;
        disable_ui(false);
    }
    else
    {
        QMessageBox::critical(this, "Erreur", QString::fromUtf8("An error occured while parsing for listing the folder's content."));
        disable_ui(false);
        return;
    }
}

void MyWindow::build_ui()
{
    m_tab = new QTabWidget;
    m_tab1 = new QWidget;
    m_tab2 = new QWidget;
    m_tab3 = new QLabel;

    m_ml = new QVBoxLayout;
    m_tab1l = new QGridLayout;
    m_tab2l = new QVBoxLayout;

    m_cb_dir = new QCheckBox("List folders");
    m_cb_dup = new QCheckBox("Parse duplicates");
    m_bar = new QProgressBar;
    m_lbl_dir = new QLabel(QString::fromUtf8("Folder to be parsed:"));
    m_lbl_log = new QLabel(QString::fromUtf8("Listing result:"));
    m_lbl_dup = new QLabel(QString::fromUtf8("Duplicate parsing result:"));
    m_dir = new QLineEdit();
    m_log = new QLineEdit();
    m_dup = new QLineEdit();
    m_exec = new QPushButton("Start");
    m_browsedir = new QPushButton("Browse");
    m_browselog = new QPushButton("Browse");
    m_browsedup = new QPushButton("Browse");

    this->setLayout(m_ml);

    m_ml->addWidget(m_tab);
    m_ml->addWidget(m_exec);
    m_ml->addWidget(m_bar);

    m_tab1->setLayout(m_tab1l);
    m_tab2->setLayout(m_tab2l);
    m_tab3->setText("Flist version " + QString(FLISTVERSION) + " from " + QString(BUILDDATE));

    m_tab->addTab(m_tab1, QString::fromUtf8("Parsing and Listing"));
    m_tab->addTab(m_tab2, "Options");
    m_tab->addTab(m_tab3, "About");

    m_tab1l->addWidget(m_lbl_dir,0,0);
    m_tab1l->addWidget(m_lbl_log,1,0);
    m_tab1l->addWidget(m_lbl_dup,2,0);
    m_tab1l->addWidget(m_dir,0,1);
    m_tab1l->addWidget(m_log,1,1);
    m_tab1l->addWidget(m_dup,2,1);
    m_tab1l->addWidget(m_browsedir,0,2);
    m_tab1l->addWidget(m_browselog,1,2);
    m_tab1l->addWidget(m_browsedup,2,2);

    m_tab2l->addWidget(m_cb_dir);
    m_tab2l->addWidget(m_cb_dup);

    m_bar->setValue(0);
    m_dup->setEnabled(false);
    m_browsedup->setEnabled(false);

    m_dir->setPlaceholderText(QString::fromUtf8("Click on \"Browse\" to select the folder to be parsed"));
    m_log->setPlaceholderText(QString::fromUtf8("Click on \"Browse\" to select the file path for the listing result"));
    m_dup->setPlaceholderText(QString::fromUtf8("Click on \"Browse\" to select the file path for the duplicate parsing result"));

    QObject::connect(m_browsedir, SIGNAL(pressed()), SLOT(openDirBrowse()));
    QObject::connect(m_browselog, SIGNAL(pressed()), SLOT(openLogBrowse()));
    QObject::connect(m_browsedup, SIGNAL(pressed()), SLOT(openDuplicateBrowse()));
    QObject::connect(m_exec, SIGNAL(pressed()), SLOT(startAnalysis()));
    QObject::connect(m_cb_dup, SIGNAL(stateChanged(int)), SLOT(dblState(int)));

    // Load on-disk configuration
    bool* r;

    r = config.listFolders();
    if(r)
    {
        m_cb_dir->setChecked(*r);
        delete r;
    }

    r = config.listDuplicates();
    if(r)
    {
        m_cb_dup->setChecked(*r);
        delete r;
    }

    QString* s;

    s = config.getFolder();
    if(s)
    {
        m_dir->setText(*s);
        delete s;
    }

    s = config.getResult();
    if(s)
    {
        m_log->setText(*s);
        delete s;
    }

    s = config.getDuplicateResult();
    if(s)
    {
        m_dup->setText(*s);
        delete s;
    }
}

void MyWindow::disable_ui(bool disable)
{
    this->setDisabled(disable);

    m_exec->setText(disable ? "Analyzing" : "Start");
}

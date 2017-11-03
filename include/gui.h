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
#ifndef GUI_H
#define GUI_H

#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "config.h"

#define BUILDDATE	__DATE__

class MyWindow : public QWidget
{
    Q_OBJECT

    public:
        MyWindow();
        ~MyWindow();

    public slots:
        void dblState(int);

        void openDirBrowse();
        void openLogBrowse();
        void openDuplicateBrowse();

        void startAnalysis();

    private:
        QTabWidget* m_tab;
        QWidget* m_tab1, *m_tab2;
        QLabel* m_tab3;

        QGridLayout* m_tab1l;
        QVBoxLayout* m_ml, *m_tab2l;

        QCheckBox* m_cb_dir, *m_cb_dup;
        QProgressBar* m_bar;
        QLabel* m_lbl_dir, *m_lbl_log, *m_lbl_dup;
        QLineEdit* m_dir, *m_log, *m_dup;
        QPushButton* m_exec, *m_browsedir, *m_browselog, *m_browsedup;

        FlistConfig config;

        void build_ui();
        void disable_ui(bool);
};

#endif // GUI_H

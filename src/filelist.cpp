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
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QList>
#include <QPair>
#include <QProgressBar>
#include <QString>
#include <QStringList>

#include <fstream>
#include <cstring>

#include "filelist.h"

#define BLOCKSIZE (4)
#define min(a, b) ((a) < (b) ? (a) : (b))

QFileInfoList* createFileList(QString path, QString logpath, bool listdir_b)
{
    QFileInfoList* rlist = new QFileInfoList();
    QStringList paths;
    std::ofstream log(logpath.toStdString().c_str());

    if(!log)
        return nullptr;

    log << "Type;Path;Name;Extension;Size (KiB);Modification Date\n;;;;;\n" << std::endl;
    rlist->clear();
    paths.append(path);

    do
    {
        QDir cur_dir(paths.first());
        cur_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QFileInfoList cur_dircontent(cur_dir.entryInfoList());

        for(int i = 0; i < cur_dircontent.size(); i++)
        {
            QFileInfo cur_object = cur_dircontent.at(i);

            if(cur_object.isDir())
            {
                paths.append(cur_object.absoluteFilePath());
                if(listdir_b)
                {
                    log << "Folder;";
                    log << cur_object.absoluteFilePath().toUtf8().data() << ";";
                    log << ";";
                    log << ";";
                    log << "0;";
                    log << cur_object.lastModified().toString("MM/dd/yyyy").toUtf8().data();
                    log << "\n";
                }
            }
            else if(cur_object.isFile())
            {
                log << "File;";
                log << cur_object.dir().absolutePath().toUtf8().data() << ";";
                log << cur_object.fileName().toUtf8().data() << ";";
                log << cur_object.completeSuffix().toUtf8().data() << ";";
                log << (cur_object.size()/1024) << ";";
                log << cur_object.lastModified().toString("MM/dd/yyyy").toUtf8().data();
                log << "\n";
                rlist->append(cur_object);
            }
        }

        paths.removeFirst();
    }while(!paths.isEmpty());

    return rlist;
}

bool createDuplicateList(QFileInfoList* filelist, QString logpath, QProgressBar* bar)
{
    std::ofstream log(logpath.toStdString().c_str());
    uint64_t buf1, buf2;
    int progress = 0;
    bool dbl;

    if(!log)
        return false;

    log << "Path 1;File 1;Path 2;File 2;Size (KiB)\n;;;;\n";

    for(QFileInfoList::iterator i = filelist->begin(); i != filelist->end(); ++i)
    {
        for(QFileInfoList::iterator j = i+1; j != filelist->end(); ++j)
        {
            if(i->size() == j->size())
            {
                std::ifstream f1(i->absoluteFilePath().toUtf8().data(), std::ios::binary), f2(j->absoluteFilePath().toUtf8().data(), std::ios::binary);
                dbl = true;

                if(f1 && f2)
                {
                    do
                    {
                        buf1 = buf2 = 0;

                        f1.read((char*)&buf1, BLOCKSIZE);
                        f2.read((char*)&buf2, BLOCKSIZE);

                        if(buf1 != buf2)
                        {
                            dbl = false;
                            break;
                        }
                    }while(!f1.eof() && !f2.eof());

                    if(dbl)
                    {
                        log << i->absolutePath().toUtf8().data() << ";";
                        log << i->fileName().toUtf8().data() << ";";
                        log << j->absolutePath().toUtf8().data() << ";";
                        log << j->fileName().toUtf8().data() << ";";
                        log << j->size() << "\n";
                    }
                }
                else
                    return false;
            }
        }

        bar->setValue((int)(50. + ((float)(++progress)+1.)*50./(float)filelist->size()));
    }

    bar->setValue(100);

    return true;
}

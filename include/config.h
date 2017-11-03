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
#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QJsonObject>
#include <QString>

#define CONFIG_FILE "flist.conf"

// Persistent configuration management
class FlistConfig
{
    public:
        FlistConfig();
        ~FlistConfig();

        // Analyzed folder
        QString* getFolder() const;
        // Listing output path
        QString* getResult() const;
        // Duplicate output path
        QString* getDuplicateResult() const;
        // List folders or not
        bool* listFolders() const;
        // Do duplicate analysis or not
        bool* listDuplicates() const;

        void storeFolder(QString);
        void storeResult(QString);
        void storeDuplicateResult(QString);
        void storeFolderList(bool);
        void storeDuplicateList(bool);

        // Write configuration changes on disk
        void writeChanges();

    private:
        QFile configFile;
        QJsonObject configJson;
        bool ok;
};

#endif // CONFIG_H

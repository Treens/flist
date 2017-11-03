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
#include <QDir>
#include <QIODevice>
#include <QJsonDocument>
#include <QStandardPaths>

#include "config.h"

FlistConfig::FlistConfig() :
    configFile(QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)).
               filePath(CONFIG_FILE))
{
    ok = configFile.open(QIODevice::ReadOnly);

    if(ok)
    {
        configJson = QJsonDocument::fromJson(configFile.readAll()).object();
        configFile.close();
    }
}

FlistConfig::~FlistConfig()
{
    if(ok)
        configFile.close();
}

QString* FlistConfig::getFolder() const
{
    if(!ok)
        return nullptr;

    return new QString(configJson["OldFolder"].toString());
}

QString* FlistConfig::getResult() const
{
    if(!ok)
        return nullptr;

    return new QString(configJson["OldResult"].toString());
}

QString* FlistConfig::getDuplicateResult() const
{
    if(!ok)
        return nullptr;

    return new QString(configJson["OldDupResult"].toString());
}

bool* FlistConfig::listFolders() const
{
    if(!ok)
        return nullptr;

    return new bool(configJson["ListFolders"].toBool());
}

bool* FlistConfig::listDuplicates() const
{
    if(!ok)
        return nullptr;

    return new bool(configJson["ListDup"].toBool());
}

void FlistConfig::storeFolder(QString folder)
{
    configJson["OldFolder"] = folder;
}

void FlistConfig::storeResult(QString result)
{
    configJson["OldResult"] = result;
}

void FlistConfig::storeDuplicateResult(QString result)
{
    configJson["OldDupResult"] = result;
}

void FlistConfig::storeFolderList(bool l)
{
    configJson["ListFolders"] = l;
}

void FlistConfig::storeDuplicateList(bool l)
{
    configJson["ListDup"] = l;
}

void FlistConfig::writeChanges()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(path);
    ok = configFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    if(ok)
    {
        configFile.write(QJsonDocument(configJson).toJson(QJsonDocument::Indented));
        configFile.close();
    }
}

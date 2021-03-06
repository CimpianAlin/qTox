/*
    Copyright © 2015-2017 by The qTox Project Contributors

    This file is part of qTox, a Qt-based graphical interface for Tox.

    qTox is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qTox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qTox.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PROFILE_H
#define PROFILE_H

#include "src/core/toxencrypt.h"
#include "src/core/toxid.h"

#include "src/persistence/history.h"

#include <QByteArray>
#include <QObject>
#include <QPixmap>
#include <QString>
#include <QVector>
#include <memory>

class Core;
class QThread;

class Profile : public QObject
{
    Q_OBJECT

public:
    static Profile* loadProfile(QString name, const QString& password = QString());
    static Profile* createProfile(QString name, QString password);
    ~Profile();

    Core* getCore();
    QString getName() const;

    void startCore();
    void restartCore();
    bool isNewProfile();
    bool isEncrypted() const;
    QString setPassword(const QString& newPassword);
    const ToxEncrypt* getPasskey() const;

    void saveToxSave();
    void saveToxSave(QByteArray data);

    QPixmap loadAvatar();
    QPixmap loadAvatar(const QString& ownerId);
    QByteArray loadAvatarData(const QString& ownerId);
    void saveAvatar(QByteArray pic, const QString& ownerId);
    QByteArray getAvatarHash(const QString& ownerId);
    void removeAvatar(const QString& ownerId);
    void removeAvatar();

    bool isHistoryEnabled();
    History* getHistory();

    QVector<QString> remove();

    bool rename(QString newName);

    static void scanProfiles();
    static QVector<QString> getProfiles();

    static bool exists(QString name);
    static bool isEncrypted(QString name);
    static QString getDbPath(const QString& profileName);

public slots:
    void onRequestSent(const ToxPk& friendPk, const QString& message);

private slots:
    void loadDatabase(const ToxId& id, QString password);

private:
    Profile(QString name, const QString& password, bool newProfile, const QByteArray& toxsave);
    static QVector<QString> getFilesByExt(QString extension);
    QString avatarPath(const QString& ownerId, bool forceUnencrypted = false);

private:
    Core* core;
    QThread* coreThread;
    QString name;
    std::unique_ptr<ToxEncrypt> passkey = nullptr;
    std::shared_ptr<RawDatabase> database;
    std::unique_ptr<History> history;
    bool newProfile;
    bool isRemoved;
    bool encrypted = false;
    static QVector<QString> profiles;
};

#endif // PROFILE_H

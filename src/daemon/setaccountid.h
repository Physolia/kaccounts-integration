/*************************************************************************************
 *  Copyright (C) 2013 by Alejandro Fiestas Olivares <afiestas@kde.org>              *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#ifndef SETACCOUNTID_RESOURCE_H
#define SETACCOUNTID_RESOURCE_H

#include <QDBusMessage>

#include <KJob>
#include <Akonadi/AgentInstance>
#include <Accounts/Account>

class QDBusPendingCallWatcher;
class SetAccountId : public KJob
{
    Q_OBJECT
    public:
        explicit SetAccountId(QObject* parent = 0);
        virtual ~SetAccountId();

        virtual void start();

        void setAccountId(const Accounts::AccountId &accId);
        void setAgentInstance(const Akonadi::AgentInstance &agent);

    private Q_SLOTS:
        void getMethodName();
        void introspectDo(QDBusPendingCallWatcher* watcher);
        void accountSet(QDBusPendingCallWatcher* watcher);
        void configWritten(QDBusPendingCallWatcher* watcher);

    private:
        QDBusMessage createCall(const QString &method);
        void setAccountId();
        void writeConfig();

        QString m_resource;
        Akonadi::AgentInstance m_agent;
        Accounts::AccountId m_accountId;
};

#endif //SETACCOUNTID
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

#include "enableservicejob.h"
#include "fetchsettingsjob.h"
#include "changesettingsjob.h"

#include <QDebug>

EnableServiceJob::EnableServiceJob(QObject* parent): AbstractAkonadiJob(parent)
{

}

void EnableServiceJob::start()
{
    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
}

void EnableServiceJob::init()
{
    FetchSettingsJob *job = new FetchSettingsJob(this);
    connect(job, SIGNAL(finished(KJob*)), SLOT(fetchSettingsJobDone(KJob*)));

    if (!m_interface.isEmpty()) {
        job->setInterface(m_interface);
    }
    job->setResourceId(m_resourceId);
    job->setKey("accountServices");
    job->start();
}

void EnableServiceJob::fetchSettingsJobDone(KJob* job)
{
    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
        emitResult();
        return;
    }

    FetchSettingsJob *fetchJob = qobject_cast<FetchSettingsJob*>(job);
    QStringList services = fetchJob->value<QStringList>();
    if (services.contains(m_serviceType) && m_serviceStatus == Enable) {
        qDebug() << "Already enabled service: " << m_serviceType;
        emitResult();
        return;
    }
    if (!services.contains(m_serviceType) && m_serviceStatus == Disable) {
        qDebug() << "Trying to disable a not enabled service: " << m_serviceType;
        emitResult();
        return;
    }

    if (m_serviceStatus == Enable) {
        services.append(m_serviceType);
    } else {
        services.removeAll(m_serviceType);
    }

    ChangeSettingsJob *changeJob = new ChangeSettingsJob(this);
    connect(changeJob, SIGNAL(finished(KJob*)), SLOT(changeSettingsDone(KJob*)));
    if (!m_interface.isEmpty()) {
        changeJob->setInterface(m_interface);
    }
    changeJob->setResourceId(m_resourceId);

    changeJob->setSetting("setAccountServices", services);
    changeJob->start();
}

void EnableServiceJob::changeSettingsDone(KJob* job)
{
    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
    }

    emitResult();
}

void EnableServiceJob::setServiceType(const QString& serviceType, EnableServiceJob::Status status)
{
    m_serviceType = serviceType;
    m_serviceStatus = status;
}
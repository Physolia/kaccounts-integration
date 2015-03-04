/*************************************************************************************
 *  Copyright (C) 2012 by Alejandro Fiestas Olivares <afiestas@kde.org>              *
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

#include "create.h"
#include "jobs/createaccount.h"
#include <core.h>

#include "ui_types.h"

#include <QDebug>

#include <QWidget>
#include <QCommandLinkButton>
#include <QMessageBox>

#include <Accounts/Manager>
#include <Accounts/Provider>

#include <KLocalizedString>

Create::Create(QWidget *parent)
    : QObject(parent)
    , m_form(0)
    , m_manager(KAccounts::accountsManager())
{
    m_parent = parent;
}

Create::~Create()
{
}

QWidget* Create::widget()
{
    if (!m_form) {
        m_form = new Ui_createForm();
    }

    QWidget *widget = new QWidget(m_parent);
    m_form->setupUi(widget);

    QMetaObject::invokeMethod(this, "fillInterface", Qt::QueuedConnection);

    return widget;
}

void Create::fillInterface()
{
    Accounts::ProviderList providerList = m_manager->providerList();

    // sort accounts alphabetically
    std::sort(providerList.begin(), providerList.end(), [](const Accounts::Provider &a, const Accounts::Provider &b) {
        return QString::localeAwareCompare(b.displayName(), a.displayName()) < 0;
    });

    QCommandLinkButton *button;
    Q_FOREACH(const Accounts::Provider &provider, providerList) {
        if (provider.name() == "ktp-generic") {
            continue;
        }
        button = new QCommandLinkButton(i18nd(provider.trCatalog().toLatin1().constData(), provider.displayName().toUtf8().constData()));
        button->setIcon(QIcon::fromTheme(provider.iconName()));
        button->setProperty("providerName", provider.name());
        button->setToolTip(i18nd(provider.trCatalog().toLatin1().constData(), provider.description().toUtf8().constData()));

        connect(button, SIGNAL(clicked(bool)), SLOT(createAccount()));
        m_form->verticalLayout->insertWidget(0, button);
    }
}

void Create::createAccount()
{
    QString providerName = sender()->property("providerName").toString();
    CreateAccount *acc = new CreateAccount(providerName, this);

    connect(acc, &CreateAccount::finished, [=](KJob *job) {
        if (job->error() == KJob::UserDefinedError) {
            QMessageBox::critical(m_parent, i18nc("Messagebox title; meaning 'Unable to finish the action you started'", "Unable to finish"), job->errorText());
        }
    });

    acc->start();
}

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

#include "webaccounts.h"
#include "create.h"
#include "accountwidget.h"
#include "ui_kcm.h"

#include <QDebug>

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QStackedLayout>

#include <kpluginfactory.h>

K_PLUGIN_FACTORY(WebAccountsFactory, registerPlugin<WebAccounts>();)
K_EXPORT_PLUGIN(WebAccountsFactory("webaccounts", "webaccounts"))

WebAccounts::WebAccounts(QWidget *parent, const QVariantList&)
: KCModule(WebAccountsFactory::componentData(), parent)
, m_create(0)
, m_layout(new QStackedLayout)
{

    m_ui = new Ui::KCMWebAccounts();
    m_ui->setupUi(this);

    m_ui->accountInfo->setLayout(m_layout);
    m_ui->accList->setIconSize(QSize(64, 64));

    connect(m_ui->remoteBtn, SIGNAL(clicked(bool)), this, SLOT(rmBtnClicked()));
    connect(m_ui->addBtn, SIGNAL(clicked(bool)), this, SLOT(addBtnClicked()));
    connect(m_ui->accList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));

    QMetaObject::invokeMethod(this, "addExistingAccounts", Qt::QueuedConnection);
}

WebAccounts::~WebAccounts()
{
    delete m_ui;
}

void WebAccounts::addExistingAccounts()
{
    KSharedConfigPtr config = KSharedConfig::openConfig("webaccounts");
    QStringList groups = config->groupList();

    Q_FOREACH(const QString &group, groups) {
        QStringList accounts = config->group(group).groupList();
        Q_FOREACH(const QString &account, accounts) {
            addAccount(account, account);
        }
    }

    m_create = new Create(this);
    connect(m_create, SIGNAL(newAccount(QString,QString)), this, SLOT(newAccount(QString,QString)));

    QWidget *widget = m_create->widget();
    m_newAccountItem = createQListWidgetItem(i18n("New Account"), "applications-education-miscellaneous", i18n("Select a supported Web Account"), widget);

    m_layout->addWidget(widget);

    m_ui->accList->addItem(m_newAccountItem);

    if (m_ui->accList->count() == 0) {
        m_ui->accList->setCurrentItem(m_newAccountItem);
        return;
    }

    m_ui->accList->setCurrentItem(m_ui->accList->item(0));
}

void WebAccounts::addAccount(const QString& name, const QString& account)
{
    AccountWidget *accountWidget = new AccountWidget(account, this);
    m_layout->addWidget(accountWidget);

    QListWidgetItem *newItem = createQListWidgetItem(name, "gmail", name, accountWidget);
    m_ui->accList->addItem(newItem);
}

void WebAccounts::addBtnClicked()
{
    m_ui->accList->setCurrentItem(m_newAccountItem, QItemSelectionModel::SelectCurrent);
}

void WebAccounts::rmBtnClicked()
{
    QListWidgetItem *item = m_ui->accList->currentItem();
    if (item == m_newAccountItem) {
        return;
    }

    QString accName = item->data(Qt::UserRole).toString();

    KSharedConfig::openConfig("webaccounts")->group("google").deleteGroup(accName);
    m_ui->accList->takeItem(m_ui->accList->row(item));

    delete item->data(Qt::UserRole + 1).value<QWidget *>();
}

void WebAccounts::currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) {
        return;
    }

    m_ui->accountInfo->setTitle(current->data(Qt::UserRole).toString());
    m_layout->setCurrentWidget(current->data(Qt::UserRole + 1).value<QWidget *>());
}

void WebAccounts::newAccount(const QString& type, const QString& name)
{
    QLabel *lb = new QLabel("Meh");
    m_layout->addWidget(lb);

    QListWidgetItem *newItem = createQListWidgetItem(name, "gmail", name, lb);
    int row = m_ui->accList->row(m_newAccountItem);

    m_ui->accList->takeItem(row);
    m_ui->accList->insertItem(row, newItem);
    m_ui->accList->addItem(m_newAccountItem);
}

QListWidgetItem* WebAccounts::createQListWidgetItem(const QString& name, const QString& icon, const QString& title, QWidget *widget)
{
    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setIcon(QIcon::fromTheme(icon));
    newItem->setText(name);
    newItem->setData(Qt::UserRole, QVariant(title));
    newItem->setData(Qt::UserRole + 1, QVariant::fromValue<QWidget *>(widget));

    return newItem;
}

#include "webaccounts.moc"

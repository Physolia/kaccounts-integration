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

#ifndef webaccounts_H
#define webaccounts_H

#include <kcmodule.h>

class Create;
class QStackedLayout;
class QListWidgetItem;
class KConfigGroup;
namespace Ui {
    class KCMWebAccounts;
}

class WebAccounts : public KCModule
{
Q_OBJECT
public:
    WebAccounts(QWidget *parent, const QVariantList&);
    virtual ~WebAccounts();

private Q_SLOTS:
    void addBtnClicked();
    void rmBtnClicked();
    void currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void newAccount(const QString &type, const QString &name);
    void addExistingAccounts();

private:
    void addAccount(const QString &name, const QString &account);
    QListWidgetItem* createQListWidgetItem(const QString &name, const QString &icon, const QString &title, QWidget *widget);

private:
    Create *m_create;
    QStackedLayout *m_layout;
    QListWidgetItem *m_newAccountItem;
    Ui::KCMWebAccounts *m_ui;
};

#endif // webaccounts_H

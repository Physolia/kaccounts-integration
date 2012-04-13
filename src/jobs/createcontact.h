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

#ifndef CREATECONTACT_H
#define CREATECONTACT_H

#include <kconfiggroup.h>
#include <KDE/KJob>

class CreateContact : public KJob
{
    Q_OBJECT
    public:
        explicit CreateContact(KConfigGroup &group, QObject* parent = 0);
        virtual ~CreateContact();

        virtual void start();

    private Q_SLOTS:
        void createResource();
        void resourceCreated(KJob *job);

    private:
        KConfigGroup m_config;
};

#endif //CREATECONTACT_H
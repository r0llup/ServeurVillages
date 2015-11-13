/*
    ServeurVillages

    Copyright (C) 2012 Sh1fT

    This file is part of ServeurVillages.

    ServeurVillages is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    ServeurVillages is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ServeurVillages; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
    USA
*/

#ifndef SERVEURVILLAGES_H
#define SERVEURVILLAGES_H

const std::string serveurVillagesLog("ServeurVillages.log");
#define PORT_VILLAGE 32010
#define MAXSTRING 125
#define EOC "EXIT"
#define SGI "SIGINT"
#define MAXCLIENT 5

int createSocket();
void *fctThread(void *param);

#endif

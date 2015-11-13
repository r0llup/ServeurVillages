/*
    Main

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

#include <iostream>
#include "LibrairieVillages.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "login: " << login("F_AGENTS.csv", "Sculpturale", "Johnny69") << endl;
    cout << "login: " << login("F_AGENTS.csv", "Sculpturddddale", "Johnny694444") << endl;
    cout << "bookingMaterial: " << bookingMaterial("F_MATERIALS.csv", "F_BOOKINGS.csv", "PEDALO1422", "livraison", "25/11/2012") << endl;
    cout << "bookingMaterial: " << bookingMaterial("F_MATERIALS.csv", "F_BOOKINGS.csv", "PEDAddLO1422", "livradddison", "25/11/2012") << endl;
    cout << "cancelMaterial: " << cancelMaterial("F_BOOKINGS.csv", "PEDALO1422", "livraison", "25/11/2012") << endl;
    cout << "cancelMaterial: " << cancelMaterial("F_BOOKINGS.csv", "PEDAddLO1422", "livradddison", "25/11/2012") << endl;
    cout << "askMaterial: " << askMaterial("F_ORDERS.csv", "caramel", "Camel mou", "Camel", "15.55€") << endl;
    return 0;
}

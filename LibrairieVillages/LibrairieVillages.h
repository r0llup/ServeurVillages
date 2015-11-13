/*
    LibrairieVillages

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

#ifndef LIBRAIRIEVILLAGES_H
#define LIBRAIRIEVILLAGES_H

const std::string librairieVillagesLog("LibrairieVillages.log");
const std::string whiteSpaces(" \f\n\r\t\v");

void trimRight(std::string& str, const std::string& trimChars);
void trimLeft(std::string& str, const std::string& trimChars);
void trim(std::string& str, const std::string& trimChars);
void createTraceFile(std::string filename);
bool login(const std::string loginsFilename, const std::string clientName,
    const std::string clientPassword);
bool bookingMaterial(const std::string materialsFilename,
    const std::string bookingsFilename, const std::string idMaterial,
    const std::string actionMaterial, const std::string dateMaterial);
bool cancelMaterial(const std::string bookingsFilename, const std::string idMaterial,
    const std::string actionMaterial, const std::string dateMaterial);
bool askMaterial(const std::string ordersFilename, const std::string typeMaterial,
    const std::string labelMaterial, const std::string brandMaterial,
    const std::string priceMaterial);

#endif

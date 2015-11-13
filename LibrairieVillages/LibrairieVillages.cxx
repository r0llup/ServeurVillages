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

#include <string>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include "LibrairieVillages.h"

using namespace std;

void trimRight(std::string& str, const std::string& trimChars = whiteSpaces)
{
    std::string::size_type pos = str.find_last_not_of(trimChars);
    str.erase(pos + 1);
}

void trimLeft(std::string& str, const std::string& trimChars = whiteSpaces)
{
    std::string::size_type pos = str.find_first_not_of(trimChars);
    str.erase(0, pos);
}

void trim(std::string& str, const std::string& trimChars = whiteSpaces)
{
    trimRight(str, trimChars);
    trimLeft(str, trimChars);
}

void createTraceFile(string filename)
{
    int traceFile = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    dup2(traceFile, fileno(stderr));
}

bool login(const std::string loginsFilename, const std::string clientName,
    const std::string clientPassword)
{
    createTraceFile(librairieVillagesLog);
    ifstream loginsFile(loginsFilename.c_str(), ios::in);

    if(!loginsFile) // trying to open the 'loginsFile' file
    {
        fprintf(stderr, "Ko. file '%s' not found\n", loginsFilename.c_str());
        return false;
    }

    else
    {
        fprintf(stderr, "Ok. file '%s' loaded successfully\n", loginsFilename.c_str());
        char line[25];
        string clientNameFile;
        string clientPasswordFile;
        int counter = 0;

        while(loginsFile.getline(line, 99, ';'))
        {
            if(counter == 4) // eol
                counter = 0;

            counter++;

            switch(counter)
            {
                case 1: // clientName
                    clientNameFile = line;
                    trim(clientNameFile, whiteSpaces);
                    fprintf(stderr, "clientNameFile: %s\n", clientNameFile.c_str());
                    break;
                case 4: // clientPassword
                    clientPasswordFile = line;
                    trim(clientPasswordFile, whiteSpaces);
                    fprintf(stderr, "clientPasswordFile: %s\n", clientPasswordFile.c_str());
                    break;
            }
            if ((clientName.compare(clientNameFile) == 0) &&
                (clientPassword.compare(clientPasswordFile) == 0))
                return true;
        }

        loginsFile.close();
    }
    return false;
}

bool bookingMaterial(const std::string materialsFilename, const std::string bookingsFilename,
    const std::string idMaterial, const std::string actionMaterial, const std::string dateMaterial)
{
    createTraceFile(librairieVillagesLog);
    ifstream materialsFile(materialsFilename.c_str(), ios::in);
    ofstream bookingsFile(bookingsFilename.c_str(), ios::app);

    if(!materialsFile) // trying to open the 'materialsFile' file
    {
        fprintf(stderr, "Ko. file '%s' not found\n", materialsFilename.c_str());
        return false;
    }

    fprintf(stderr, "Ok. file '%s' loaded successfully\n", materialsFilename.c_str());

    if(!bookingsFile) // trying to open the 'bookingsFile' file
    {
        fprintf(stderr, "Ko. file '%s' not found\n", bookingsFilename.c_str());
        return false;
    }

    else
    {
        fprintf(stderr, "Ok. file '%s' loaded successfully\n", bookingsFilename.c_str());
        char line[25];
        string idMaterialFile;
        int counter = 0;

        while(materialsFile.getline(line, 99, ';'))
        {
            if(counter == 2) // eol
                counter = 0;

            counter++;

            switch(counter)
            {
                 case 1: // idMaterial
                     idMaterialFile = line;
                     trim(idMaterialFile, whiteSpaces);
                     fprintf(stderr, "idMaterialFile: %s\n", idMaterialFile.c_str());
                     break;
            }
            if(idMaterial.compare(idMaterialFile) == 0)
            {
                bookingsFile << idMaterial << ";" << actionMaterial << ";" << dateMaterial << ";" << endl;
                return true;
            }
        }

        materialsFile.close();
        bookingsFile.close();
    }
    return false;
}

bool cancelMaterial(const std::string bookingsFilename, const std::string idMaterial,
    const std::string actionMaterial, const std::string dateMaterial)
{
    createTraceFile(librairieVillagesLog);
    ifstream bookingsFile(bookingsFilename.c_str(), ios::in);
    ofstream bookings2File("F_BOOKINGS2.csv", ios::out);

    if(!bookingsFile) // trying to open the 'bookingsFile' file
    {
        fprintf(stderr, "Ko. file '%s' not found\n", bookingsFilename.c_str());
        return false;
    }

    if(!bookings2File) // trying to open the 'bookings2File' file
    {
        fprintf(stderr, "Ko. file 'F_BOOKINGS2.csv' not found\n");
        return false;
    }

    else
    {
        fprintf(stderr, "Ok. file '%s' loaded successfully\n", bookingsFilename.c_str());
        fprintf(stderr, "Ok. file 'F_BOOKINGS2.csv' loaded successfully\n");
        char line[25];
        string idMaterialFile;
        string actionMaterialFile;
        string dateMaterialFile;
        int counter = 0;
        int result;

        while(bookingsFile.getline(line, 99, ';'))
        {
            if(counter == 3) //eol
                counter = 0;

            counter++;

            switch(counter)
            {
              case 1: // idMaterial
                  idMaterialFile = line;
                  trim(idMaterialFile, whiteSpaces);
                  fprintf(stderr, "idMaterialFile: %s\n", idMaterialFile.c_str());
                  break;
              case 2: // actionMaterial
                  actionMaterialFile = line;
                  trim(actionMaterialFile, whiteSpaces);
                  fprintf(stderr, "actionMaterialFile: %s\n", actionMaterialFile.c_str());
                  break;
              case 3: // dateMaterial
                  dateMaterialFile = line;
                  trim(dateMaterialFile, whiteSpaces);
                  fprintf(stderr, "dateMaterialFile: %s\n", dateMaterialFile.c_str());
                  break;
            }
            if((idMaterial.compare(idMaterialFile) != 0) &&
                (actionMaterial.compare(actionMaterialFile) != 0) &&
                (dateMaterial.compare(dateMaterialFile) != 0))
            {
                bookings2File << idMaterialFile << ";" << actionMaterialFile << ";"
                    << dateMaterialFile << ";" << endl;
            }
        }

        bookingsFile.close();
        bookings2File.close();
        result = rename("F_BOOKINGS2.csv", bookingsFilename.c_str());

        if(result == 0)
        {
            fprintf(stderr, "Ok. file '%s' successfully renamed\n", bookingsFilename.c_str());
            return true;
        }

        fprintf(stderr, "Ko. file '%s' not successfully renamed\n", bookingsFilename.c_str());
    }
    return false;
}

bool askMaterial(const std::string ordersFilename, const std::string typeMaterial,
    const std::string labelMaterial, const std::string brandMaterial,
    const std::string priceMaterial)
{
    createTraceFile(librairieVillagesLog);
    ofstream ordersFile(ordersFilename.c_str(), ios::app);

    if(!ordersFile) // trying to open the 'ordersFile' file
    {
        fprintf(stderr, "Ko. file '%s' not found\n", ordersFilename.c_str());
        return false;
    }

    else
    {
        fprintf(stderr, "Ok. file '%s' loaded successfully\n", ordersFilename.c_str());
        ordersFile << typeMaterial << ";" << labelMaterial << ";" << brandMaterial
            << ";" << priceMaterial << ";" << endl;
    }

    ordersFile.close();
    return true;
}

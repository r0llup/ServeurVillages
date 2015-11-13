/*
    ApplicMateriel

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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include "ApplicMateriel.h"
#include "LibrairieVillages.h"

using namespace std;

int hSocket;

int main(int argc, char *argv[])
{
    int tailleSocketaddr_in = sizeof(struct sockaddr_in);
    struct hostent *infosHost;
    struct in_addr adresseIP;
    struct sockaddr_in adresseSocket;
    struct sigaction act;
    char msgClient[MAXSTRING];
    char msgServeur[MAXSTRING];
    int login = 0;
    char clientOrder[25];
    char clientOrderTemp[25];
    char clientName[25];
    char clientPassword[25];
    char idMaterial[25];
    char actionMaterial[25];
    char dateMaterial[25];
    char typeMaterial[25];
    char labelMaterial[25];
    char brandMaterial[25];
    char priceMaterial[25];

    /* Création du fichier de trace */
    createTraceFile(applicMaterielLog);

    /* 1. Création de la socket */
    hSocket = createSocket();

    if(hSocket == -1)
    {
        fprintf(stderr, "Ko. createSocket\n");
        return -1;
    }
    fprintf(stderr, "Ok. createSocket\n");

    /* 2. Acquisition des informations sur l'ordinateur local */
    if((infosHost = gethostbyname("localhost")) == 0)
    {
        fprintf(stderr, "Ko. gethostbyname\n");
        close(hSocket);
        return -1;
    }
    fprintf(stderr, "Ok. gethostbyname\n");

    /* 3. Préparation de la structure sockaddr_in */
    memset(&adresseSocket, NULL, tailleSocketaddr_in);
    adresseSocket.sin_family = AF_INET; // Domaine
    adresseSocket.sin_port = htons(PORT_VILLAGE);

    /* 4. Tentative de connexion */
    if((connect(hSocket, (struct sockaddr *) &adresseSocket, tailleSocketaddr_in)) == -1)
    {
        fprintf(stderr, "Ko. connect\n");
        close(hSocket);
        return -1;
    }
    fprintf(stderr, "Ok. connect\n");

    /* Armement du signal SIGINT */
    act.sa_handler = handlerSigInt;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    strcpy(clientOrder, "");

    do
    {
        strcpy(msgClient, "");

        if(strcmp(clientOrder, "") == 0)
        {
            cout << "Veuillez encoder une commande: ";
            gets(clientOrder);
        }

        if(!login)
        {
            strcpy(clientOrderTemp, clientOrder);
            strcpy(clientOrder, "LOGIN");
        }

        if(strcmp(clientOrder, "LOGIN") == 0)
        {
            strcat(msgClient, "1:");
            cout << "~ Identification ~" << endl;
            cout << "Name: ";
            gets(clientName);
            strcat(msgClient, clientName);
            strcat(msgClient, ";");
            cout << "Password: ";
            gets(clientPassword);
            strcat(msgClient, clientPassword);
            strcat(msgClient, ";");
        }

        else if(strcmp(clientOrder, "BMAT") == 0)
        {
            strcat(msgClient, "2:");
            cout << "~ Booking Material ~" << endl;
            cout << "Id: ";
            gets(idMaterial);
            strcat(msgClient, idMaterial);
            strcat(msgClient, ";");
            cout << "Action: ";
            gets(actionMaterial);
            strcat(msgClient, actionMaterial);
            strcat(msgClient, ";");
            cout << "Date: ";
            gets(dateMaterial);
            strcat(msgClient, dateMaterial);
            strcat(msgClient, ";");
        }

        else if(strcmp(clientOrder, "CMAT") == 0)
        {
            strcat(msgClient, "3:");
            cout << "~Cancel Material ~" << endl;
            cout << "Id: ";
            gets(idMaterial);
            strcat(msgClient, idMaterial);
            strcat(msgClient, ";");
            cout << "Action: ";
            gets(actionMaterial);
            strcat(msgClient, actionMaterial);
            strcat(msgClient, ";");
            cout << "Date: ";
            gets(dateMaterial);
            strcat(msgClient, dateMaterial);
            strcat(msgClient, ";");
        }

        else if(strcmp(clientOrder, "ASKMAT") == 0)
        {
            strcat(msgClient, "4:");
            cout << "~ Ask Material ~" << endl;
            cout << "Type: ";
            gets(typeMaterial);
            strcat(msgClient, typeMaterial);
            strcat(msgClient, ";");
            cout << "Label: ";
            gets(labelMaterial);
            strcat(msgClient, labelMaterial);
            strcat(msgClient, ";");
            cout << "Brand: ";
            gets(brandMaterial);
            strcat(msgClient, brandMaterial);
            strcat(msgClient, ";");
            cout << "Price: ";
            gets(priceMaterial);
            strcat(msgClient, priceMaterial);
            strcat(msgClient, ";");
        }

        /* 5. Envoi d'un message client */
        if(send(hSocket, msgClient, MAXSTRING, NULL) == -1)
        {
            fprintf(stderr, "Ko. send\n");
            close(hSocket);
            return -1;
        }
        fprintf(stderr, "Ok. send\n");

        /* 6. Reception de l'ACK du serveur au client */
        if(recv(hSocket, msgServeur, MAXSTRING, NULL) == -1)
        {
            fprintf(stderr, "Ko. recv\n");
            close(hSocket);
            return -1;
        }
        fprintf(stderr, "Ok. recv\n");

        if(strcmp(msgServeur, "OK") == 0)
        {
            if(strcmp(clientOrder, "LOGIN") == 0)
            {
                fprintf(stderr, "Ok. login\n");
                cout << "Login successfully executed :)" << endl;
                login = 1;
                strcpy(clientOrder, clientOrderTemp);
            }

            else if(strcmp(clientOrder, "BMAT") == 0)
            {
                fprintf(stderr, "Ok. bmat\n");
                cout << "Action successfully executed :)" << endl;
            }

            else if(strcmp(clientOrder, "CMAT") == 0)
            {
                fprintf(stderr, "Ok. cmat\n");
                cout << "Canceling successfully executed :)" << endl;
            }

            else if(strcmp(clientOrder, "ASKMAT") == 0)
            {
                fprintf(stderr, "Ok. askmat\n");
                cout << "Order successfully executed :)" << endl;
            }
        }
        else
        {
            if(strcmp(clientOrder, "LOGIN") == 0)
            {
                fprintf(stderr, "Ko. login\n");
                cout << "Login not successfully executed :(" << endl;
            }

            else if(strcmp(clientOrder, "BMAT") == 0)
            {
                fprintf(stderr, "Ko. bmat\n");
                cout << "Action not successfully executed :(" << endl;
            }

            else if(strcmp(clientOrder, "CMAT") == 0)
            {
                fprintf(stderr, "Ko. cmat\n");
                cout << "Canceling not successfully executed :)" << endl;
            }

            else if(strcmp(clientOrder, "ASKMAT") == 0)
            {
                fprintf(stderr, "Ko. askmat\n");
                cout << "Order not successfully executed :(" << endl;
            }
        }
    } while(strcmp(msgClient, EOC) != 0);

    /* 7. Fermeture de la socket */
    close(hSocket);

    return 0;
}

int createSocket()
{
    return socket(AF_INET, SOCK_STREAM, NULL);
}

void handlerSigInt(int sig)
{
    char msgClient[MAXSTRING];
    sprintf(msgClient, "%s:", SGI);

    /* Envoi d'un message client */
    if(send(hSocket, msgClient, MAXSTRING, NULL) == -1)
    {
        fprintf(stderr, "Ko. send\n");
        close(hSocket);
        exit(1);
    }

    fprintf(stderr, "Ok. send\n");
    /* Fermeture de la socket */
    close(hSocket);
    exit(0);
}

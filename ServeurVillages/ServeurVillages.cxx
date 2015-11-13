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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include "ServeurVillages.h"
#include "LibrairieVillages.h"

using namespace std;

int hSocketService[MAXCLIENT];
int cptClient = 0;
int posClient = -1;
pthread_mutex_t mutexCompteurClient;
pthread_mutex_t mutexPositionClient;
pthread_cond_t condPositionClient;

int main(int argc, char *argv[])
{
    int hSocketEcoute;
    int hSocketDupliquee;
    int tailleSockaddr_in = sizeof(struct sockaddr_in);
    socklen_t tailleSocklen_t;
    int posLibre = -1;
    int i = 0;
    int fakeCpt = 0;
    char msgServeur[MAXSTRING];
    struct hostent *infosHost;
    struct in_addr adresseIP;
    struct sockaddr_in adresseSocket;
    pthread_t threadHandle;

    /* Création du fichier de trace */
    createTraceFile(serveurVillagesLog);

    /* Initialisation des mutex */
    pthread_mutex_init(&mutexCompteurClient, NULL);
    pthread_mutex_init(&mutexPositionClient, NULL);

    /* Initialisation des variables de condition */
    pthread_cond_init(&condPositionClient, NULL);

    /* Lançement des threads */
    for(i=0 ; i < MAXCLIENT ; i++)
    {
        pthread_create(&threadHandle, NULL, fctThread, (void *) i);
        pthread_detach(threadHandle);
    }

    /* 1. Création de la socket d'ecoute */
    hSocketEcoute = createSocket();

    if(hSocketEcoute == -1)
    {
        fprintf(stderr, "(MAIN) Ko. createSocket\n");
        return -1;
    }
    fprintf(stderr, "(MAIN) Ok. createSocket\n");

    /* 2. Acquisition des informations sur l'ordinateur local */
    if((infosHost = gethostbyname("localhost")) == 0)
    {
        fprintf(stderr, "(MAIN) Ko. gethostbyname\n");
        close(hSocketEcoute);
        return -1;
    }
    fprintf(stderr, "(MAIN) Ok. gethostbyname\n");

    /* 3. Préparation de la structure sockaddr_in */
    memset(&adresseSocket, NULL, tailleSockaddr_in);
    memset(&adresseSocket, NULL, tailleSocklen_t);
    adresseSocket.sin_family = AF_INET; // Domaine
    adresseSocket.sin_port = htons(PORT_VILLAGE);

    /* 4. Le système prend connaissance de l'adresse et du port de la socket */
    if(bind(hSocketEcoute, (struct sockaddr *) &adresseSocket, tailleSockaddr_in) == -1)
    {
        fprintf(stderr, "(MAIN) Ko. bind\n");
        close(hSocketEcoute);
        return -1;
    }
    fprintf(stderr, "(MAIN) Ok. bind\n");

    for(i=0 ; i < MAXCLIENT ; i++, hSocketService[i] = -1); // init

    do
    {
        /* 5. Mise à l'écoute d'une requête de connexion */
        if(listen(hSocketEcoute, SOMAXCONN) == -1)
        {
            fprintf(stderr, "(MAIN) Ko. listen\n");
            close(hSocketEcoute);
            return -1;
        }
        fprintf(stderr, "(MAIN) Ok. listen\n");

        /* 6. Acceptation d'une connexion */
        if((hSocketDupliquee =
            accept(hSocketEcoute, (struct sockaddr *) &adresseSocket, &tailleSocklen_t)) == -1)
        {
            fprintf(stderr, "(MAIN) Ko. accept\n");
            close(hSocketEcoute);
            return -1;
        }
        fprintf(stderr, "(MAIN) Ok. accept\n");

        pthread_mutex_lock(&mutexCompteurClient);
        if(cptClient >= MAXCLIENT)
        {
            strcpy(msgServeur, "Err. too much customers");
            if(send(hSocketDupliquee, msgServeur, MAXSTRING, NULL) == -1)
            {
                fprintf(stderr, "(MAIN) Ko. send\n");
                close(hSocketDupliquee);
                pthread_mutex_unlock(&mutexCompteurClient);
                return -1;
            }
            fprintf(stderr, "(MAIN) Ok. send\n");
            pthread_mutex_unlock(&mutexCompteurClient);
            continue;
        }
        else
        {
            pthread_mutex_unlock(&mutexCompteurClient);
            for(i=0 ; i < MAXCLIENT ; i++)
            {
                if(hSocketService[i] == -1)
                    posLibre = i;
            }
            hSocketService[posLibre] = hSocketDupliquee;
            fakeCpt++;
            pthread_mutex_lock(&mutexCompteurClient);
            cptClient = fakeCpt;
            pthread_mutex_unlock(&mutexCompteurClient);
            pthread_mutex_lock(&mutexPositionClient);
            posClient = posLibre;
            pthread_cond_broadcast(&condPositionClient);
            pthread_mutex_unlock(&mutexPositionClient);
        }
    } while(1);

    /* 9. Fermeture des sockets */
    pthread_mutex_lock(&mutexCompteurClient);
    for(i=0 ; i < MAXCLIENT ; i++)
    {
        if(hSocketService[i] != -1)
        {
            close(hSocketService[i]);
            fprintf(stderr, "(MAIN) Ok. close(hSocketService)\n");
        }
    }
    pthread_mutex_unlock(&mutexCompteurClient);
    close(hSocketEcoute);
    fprintf(stderr, "(MAIN) Ok. close(hSocketEcoute)\n");

    return 0;
}

int createSocket()
{
    return socket(AF_INET, SOCK_STREAM, NULL);
}

void *fctThread(void *param)
{
    int nbreCarLu = 0, i = 0;
    char msgClient[MAXSTRING];
    char msgServeur[MAXSTRING];
    char *clientOrder, *clientName, *clientPassword, *idMaterial,
        *actionMaterial, *dateMaterial, *typeMaterial, *labelMaterial,
        *brandMaterial, *priceMaterial;

    while(1)
    {
        pthread_mutex_lock(&mutexPositionClient);
        while(posClient != (size_t) param)
            pthread_cond_wait(&condPositionClient, &mutexPositionClient);
        pthread_mutex_unlock(&mutexPositionClient);
        do
        {
            strcpy(msgClient, "");

            /* 7. Reception d'un message client */
            if((nbreCarLu = recv(hSocketService[(size_t) param], msgClient, MAXSTRING, NULL)) == -1)
            {
                fprintf(stderr, "(THREAD_CLIENT) Ko. recv\n");
                close(hSocketService[(size_t) param]);
                exit(1);
            }
            fprintf(stderr, "(THREAD_CLIENT) Ok. recv\n");
            msgClient[nbreCarLu] = '\0';
            fprintf(stderr, "Received message: %s\n", msgClient);
            clientOrder = strtok(msgClient, ":");
            if(atoi(clientOrder) == 1)
            {
                clientName = strtok(NULL, ";");
                clientPassword = strtok(NULL, ";");
                fprintf(stderr, "clientOrder: %s\n", clientOrder);
                fprintf(stderr, "clientName: %s\n", clientName);
                fprintf(stderr, "clientPassword: %s\n", clientPassword);
                if(login("F_AGENTS.csv", clientName, clientPassword))
                    strcpy(msgServeur, "OK");
                else
                    strcpy(msgServeur, "KO");
            }
            else if(atoi(clientOrder) == 2)
            {
                idMaterial = strtok(NULL, ";");
                actionMaterial = strtok(NULL, ";");
                dateMaterial = strtok(NULL, ";");
                fprintf(stderr, "idMaterial: %s\n", idMaterial);
                fprintf(stderr, "actionMaterial: %s\n", actionMaterial);
                fprintf(stderr, "dateMaterial: %s\n", dateMaterial);
                if(bookingMaterial("F_MATERIALS.csv", "F_BOOKINGS.csv", idMaterial,
                    actionMaterial, dateMaterial))
                    strcpy(msgServeur, "OK");
                else
                    strcpy(msgServeur, "KO");
            }
            else if(atoi(clientOrder) == 3)
            {
                idMaterial = strtok(NULL, ";");
                actionMaterial = strtok(NULL, ";");
                dateMaterial = strtok(NULL, ";");
                fprintf(stderr, "idMaterial: %s\n", idMaterial);
                fprintf(stderr, "actionMaterial: %s\n", actionMaterial);
                fprintf(stderr, "dateMaterial: %s\n", dateMaterial);
                if(cancelMaterial("F_BOOKINGS.csv", idMaterial, actionMaterial,
                    dateMaterial))
                    strcpy(msgServeur, "OK");
                else
                    strcpy(msgServeur, "KO");
            }
            else if(atoi(clientOrder) == 4)
            {
                typeMaterial = strtok(NULL, ";");
                labelMaterial = strtok(NULL, ";");
                brandMaterial = strtok(NULL, ";");
                priceMaterial = strtok(NULL, ";");
                fprintf(stderr, "typeMaterial: %s\n", typeMaterial);
                fprintf(stderr, "labelMaterial: %s\n", labelMaterial);
                fprintf(stderr, "brandMaterial: %s\n", brandMaterial);
                fprintf(stderr, "priceMaterial: %s\n", priceMaterial);
                if(askMaterial("F_ORDERS.csv", typeMaterial, labelMaterial,
                    brandMaterial, priceMaterial))
                    strcpy(msgServeur, "OK");
                else
                    strcpy(msgServeur, "KO");
            }

            /* 8. Envoi d'un message serveur */
            if(send(hSocketService[(size_t) param], msgServeur, MAXSTRING, NULL) == -1)
            {
                fprintf(stderr, "(THREAD_CLIENT) Ko. send\n");
                close(hSocketService[(size_t) param]);
                exit(1);
            }
            fprintf(stderr, "(THREAD_CLIENT) Ok. send\n");
        } while((strcmp(clientOrder, EOC) != 0) && (strcmp(clientOrder, SGI) != 0));

        hSocketService[(size_t) param] = -1;
        pthread_mutex_lock(&mutexCompteurClient);
        cptClient--;
        pthread_mutex_unlock(&mutexCompteurClient);
        pthread_mutex_lock(&mutexPositionClient);
        posClient=-1;
        pthread_mutex_unlock(&mutexPositionClient);
    }
    pthread_exit(NULL);
}

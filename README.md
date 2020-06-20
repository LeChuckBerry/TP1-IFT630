#TP1-IFT630

### Notes 
- Tous les problèmes réglés à l'aide de moniteurs utilisent des sémaphores implémentées à l'aide d'un moniteur 
- Pour le moment, le problème du parc d'attraction n'a pas été réglé avec les canaux de communication 

### Compilation et exécution 
- Le projet peut être compilé à l'aide de CMake (requiert la version 3.15 ou plus récente)
- Chaque problème à régler possède son propre exécutable du même nom que le fichier cpp associé 
- Tous les exécutables seront générés à la racine du dossier de compilation 
- Les programmes ont été écris pour fonctionner dans un environnement Unix, mais peuvent fonctionner également avec Windows


### Les primitives de synchronisation (dossier sync_primitives) 
- Canal de communication : comm_channel.h
- Semaphore implémentée à l'aide d'un moniteur : monitor.h
- Semaphore (crédit Daniel-Junior-Dubé) : semaphore.h


### Le problème des philosophes (dossier philosophers)

- La version avec sémaphores se trouve dans philo_sem.cpp
- La version avec moniteur se trouve dans philo_monitor.cpp

###  Le problème du parc d'attarction (dossier roller_coaster)
- Avec moniteurs : roller_coaster_monitor.cpp
- Avec semaphore : roller_coaster_sem.cpp 

### Le problème du babrbier (dossier barber)
- Avec sémaphores : barber_sem.cpp

### Autre 
- Démo pour le canal de messages : mailboxDemo.cpp
# **8-Philosophers**

Debut le 19 juin 2023 - Validé le 8 aout 2023 => 50 jours, BH gagné 38 jours

## **INTRODUCTION :**

Ce projet est une introduction au threading et aux processus, et sur comment travailler
sur le même espace mémoire.
Vous apprendrez à manipuler des threads.
Vous découvrirez les mutex, les sémaphores et la mémoire partagée.


## **LIENS UTILS :**

Tips : 
- https://private-rooster-996.notion.site/philosophers-VM-c60be9c836084edfbcd9c07e29b429c4
- https://www.codequoi.com/threads-mutex-et-programmation-concurrente-en-c/
- https://www.notion.so/Philosophers-2b872948598e4f0cba91c66d8b5ba821

Visualizer : https://nafuka11.github.io/philosophers-visualizer/

Tester : 
- https://github.com/cacharle/philosophers_test
- https://github.com/nesvoboda/socrates
- https://github.com/newlinuxbot/Philosphers-42Project-Tester

## **COMMANDES :**

[... a developper ...]


## **La programmation concurrente :**

La programmation concurrente en opposition à la programmation séquentielle.

Permet à un programme d’effectuer plusieurs tâches simultanément au lieu de devoir attendre la fin d’une opération pour commencer la suivante.

## **Threads :**

Un thread (ou un fil d’exécution en français), c’est une suite logique d’instructions à l’intérieur d’un processus qui est automatiquement gérée par le noyau du système d’exploitation.

En resumé, il s'agit d'une instruction envoyé au processus. 

En C, l’interface standard pour manipuler les threads est POSIX avec la bibliothèque <pthread.h>.

phthread_t      ->  1 - creer une structure
pthread_create  ->  2 - creer le thread
pthread_join    ->  3 - wait execution

pthread_create est utilisé pour lancer un nouveau thread, tandis que pthread_join est utilisé pour attendre qu'un thread se termine.

## **Mutex :**

Un mutex est l'abrégé de « mutual exclusion ».
On peut penser à un mutex comme au verrou de la porte des toilettes. Un thread vient le verrouiller pour indiquer que les toilettes sont occupées. Les autres threads devront alors attendre patiemment que la porte soit déverrouillée avant de pouvoir accéder aux toilettes à leur tour.
Les mutex permettent notamment d'éviter les data race. 

## **Data race :**

Une Data Race survient quand (a) une donnée partagée (b) est accédée par au moins deux threads (c) dont au moins un en écriture (d) et ce, sans synchronisation.

## **Semaphores :**

[UTILS FOR BONUS PART]

Les **semaphores** representent les "fourchettes" exemples : semaphores fork[nb of philo].
Lorsqu un philo relachera sa fourcette il executera un signal.

Un semaphore est une variable de structure utilisée pour contrôler l'accès à une ressource partagée par plusieurs processus (ou threads).

Il évite les problèmes de section critique dans un système concurrent tel qu'un système d'exploitation multitâche.

**Sémaphores binaires** : ils sont limités aux valeurs 0 et 1 (ou verrouillé/déverrouillé, indisponible/disponible) et sont utilisés pour implémenter les verrous.

**Les sémaphores de comptage** : permettent un comptage arbitraire/aléatoire des ressources.

##
##

**=> TEST :**

Le drapeau -fsanitize=thread -g qu’on ajoute au moment de la compilation. L’option -g permet d’afficher les numéros de ligne qui ont produit l’erreur.

L’outil de détection d’erreurs de thread Helgrind avec lequel on peut exécuter notre programme, comme ceci : 
valgrind --tool=helgrind ./programme.

L’outil de détection d’erreurs de threads DRD, qu’on lance aussi au moment de l’exécution comme ceci : 
valgrind --tool=drd ./programme.

Attention, valgrind et -fsanitize=thread ne s’entendent pas du tout et ne doivent pas s’utiliser ensemble !


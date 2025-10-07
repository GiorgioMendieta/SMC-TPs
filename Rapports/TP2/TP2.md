---
title: 'SMC- TP2 : Prototypage Virtuel avec SoCLib'

---

# SMC- TP2 : Protocole de communication VCI/OCP

Encadrant : Franck Wajsbürt 
Wilfrid LYAUTEY - 21310245 - wilfrid.lyautey@etu.sorbonne-universite.fr
Jorge MENDIETA - 21304469 - jorge.mendieta_orozco@etu.sorbonne-universite.fr

## 1)
Question 1 : à quoi sert le paquet réponse dans le cas d'une transaction d'écriture ?

    Le paquet réponse sert à indiquer la fin de la transaction du premier paquet pour que le paquet (n+1) soit traité. 
    Le paquet réponse sert plus généralement à : 
    - émettre la fin du paquet pour que le paquet (n+1) soit executé (REOP) 
    - indiquer une erreur (RERROR) 
    - aiguiller le paquet réponse vers l'initiateur et la transaction de l'initiateur (SRCID et STRDID).
    
Question 2 : Pourquoi les différents types de sous-sytèmes d'interconnexion (bus, cross-bar, micro-réseaux, etc.) sont-ils conçus de telle sorte qu'ils utilisent des ressources matérielles séparées pour aiguiller les commandes et les réponses ?

    Les sous-systèles d'interconnexion sont conçus à utiliser des ressources matérielles séparées pour potentiellement paralléliser les transactions.


## 5 Travail à réaliser

### vci_gcd_master.cpp

### vci_gcd_coprocessor.cpp
    
### tp2_simple_top.cpp

## 5.1) Composant VciGcdCoprocessor

Question 3 : Pourquoi faut-t-il deux automates séparés pour contrôler l'interface VCI et pour contrôler le calcul du PGCD proprement dit ?


    Séparer les 2 automates permet tout d'abord une meilleure lisibilité, l'un s'occupe du PGCD pendant que l'autre s'occupe de la communication. Cela permet également de ne pas alourdir le matériel et permet à chacun de fonctionner indépendamment sans attendre la fin de l'autre. 
 

Question 4 : Dans la fonction de transition, on a écrit le code définissant les transitions de l'automate vci_fsm après le code définissant les transitions de l'automate exe_fsm, bien que les transitions de l'automate exe_fsm dépendent de l'état de l'automate vci_fsm. On aurait aussi bien pu décrire ces deux automates dans l'ordre inverse. Quelle mécanisme du langage SystemC utilise-t-on pour rendre l'ordre de description des automates indifférent?

    On utilise l'algorithme de relaxation, ainsi le code est réexecuté jusqu'à que les valeurs soient disponibles. 
    

Question 5 : comment sont traitées les erreurs dans ce modèle de simulation? à quoi servent ces vérifications ?


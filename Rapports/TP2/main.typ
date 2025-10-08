#import "template.typ": *
#show: template

#print_title([SMC - TP2 : Protocole de communication VCI/OCP])

Encadrant : Franck Wajsbürt

- Wilfrid LYAUTEY - 21310245 - #link("mailto:wilfrid.lyautey@etu.sorbonne-universite.fr")
- Jorge MENDIETA - 21304469 - #link("mailto:jorge.mendieta_orozco@etu.sorbonne-universite.fr")

= Objectif
L'objectif de ce second TP est d'introduire la modélisation SystemC d'architectures utilisant le *protocole de communication VCI/OCP*. On va re-écrire les modèles de simulation des deux composants matériels du TP1, pour qu'ils utilisent des ports de communication VCI plutôt que des ports FIFO. Ceci va permettre d'interconnecter plusieurs composants initiateurs et plusieurs composants cibles à travers un bus système.

= Architecture matérielle cible

L'architecture matérielle instancie trois composants matériels : Les deux composants `VciGcdCoprocessor` et `VciGcdMaster` sont instanciés trois fois chacun. Ces composants possèdent des ports de communication qui respectent le protocole VCI/OCP.

A la différence du protocole FIFO, le protocole VCI introduit une dissymétrie entre les initiateurs et les cibles : Le composant `VciGcdMaster` se comporte comme un _initiateur_, et contrôle non seulement le chargement des données vers le le composant `VciGcdCoprocesseur` _cible_, mais contrôle également la récupération du résultat.

Le troisième composant `VciVgsb` (Virtual Generic System Bus) est un composant matériel modélisant un bus multi-maîtres, multi-cibles respectant le protocole VCI/OCP.

#figure(
  image("soclib_tp2_archi.png", width: 80%),
  caption: [Architecture Matérielle],
) <archi_mat>

Le composant `VciVgsb` se comporte comme un bus, car il ne traite qu'une seule transaction à la fois. Si plusieurs initiateurs cherchent à envoyer un paquet commande, le contrôleur du bus sélectionne un initiateur (en respectant une priorité tournante de type _round-robin_), il aiguille la commande vers la bonne cible en décodant les bits de poids fort de l'adresse, attend la réponse de la cible, et aiguille celle-ci vers le bon initiateur. La transaction (n+1) n'est traitée que lorsque la transaction (n) est entièrement terminée.

= Protocole VCI/OCP

Le protocol de communication VCI permet de construire des architectures matérielles multi-processeurs à memoire partagée. Dans ce type d'architecture, les différents composants matériels utilisent des transactions pour communiquer entre eux.

Terminologie :

- Un *flit* est l'information qui peut être transférée en un seul cycle sur un interface VCI.
- Un *paquet* est une séquence de flits qui sont transmis de façon atomique à travers le réseau d'interconnexion.
- Une *transaction* est un couple (paquet commande / paquet réponse)
Une transaction est initiée par un composant initiateur, qui envoie un paquet commande, et est terminée par un composant cible, qui répond à la commande en renvoyant un paquet réponse.

== Interconnection VCI

Dans la spécification VCI advanced, il y a principalement deux types de commandes :

- Transaction `CMD_WRITE` : le paquet commande contient un ou plusieurs flits (à des adresses constantes ou consécutives). Le paquet réponse contient un seul flit pour acquitter la transaction.
- Transaction `CMD_READ` : le paquet commande contient un seul flit (à l'adresse du premier octet) et le nombre d'octets à lire est défini par le champs `PLEN`. Le paquet réponse contient un ou plusieurs flits suivant la longueur de la rafale.

// TODO:
_*Question 1* : à quoi sert le paquet réponse dans le cas d'une transaction d'écriture ?_

Le paquet réponse sert à indiquer la fin de la transaction du premier paquet pour que le paquet (n+1) soit traité.
Le paquet réponse sert plus généralement à :
- émettre la fin du paquet pour que le paquet (n+1) soit executé (`REOP`)
- indiquer une erreur (`RERROR`)
- aiguiller le paquet réponse vers l'initiateur et la transaction de l'initiateur (`SRCID` et `STRDID`).

Dans une architecture à espace d'adressage partagé, n'importe quel initiateur est capable de communiquer avec n'importe quelle cible. La cible est désignée par les bits de poids fort de l'adresse. Le champs `VCI ADDRESS` doit donc être décodé par le (ou les) composant(s) matériel(s) qui réalise(nt) le sous-sytème d'interconnexion, pour aiguiller le paquet commande vers sa destination. De façon symétrique, le sous-système d'interconnexion doit décoder le champs `VCI RSRCID` pour aiguiller le paquet réponse vers l'initiateur concerné.

// TODO:
_*Question 2* : Pourquoi les différents types de sous-sytèmes
  d'interconnexion (bus, cross-bar, micro-réseaux, etc.) sont-ils conçus de telle sorte qu'ils utilisent des ressources matérielles séparées pour aiguiller les commandes et les réponses ?_

Les sous-systèmes d'interconnexion sont conçus à utiliser des ressources matérielles séparées pour potentiellement paralléliser les transactions.

#figure(
  image("soclib_tp2_vci_protocol.png", width: 80%),
  caption: [Protocole VCI],
) <vci_protocol>

Un canal de communication VCI utilise donc deux sous-canaux : un sous-canal dans le sens (initiateur => cible) pour la commande et un sous-canal dans le sens (cible=>initiateur) pour la réponse. Il est intéressant de noter que chacun de ces deux sous-canaux respecte le protocole FIFO. Les signaux `CMDVAL` et `CMDACK` (resp. `RSPVAL` et `RSPACK`) correspondent aux signaux `WOK` et `ROK` du protocole FIFO pour le sous-canal commande (resp. réponse).

== Les signaux VCI

La figure #ref(<vci_signals>) ci-dessous détaille les signaux utilisés par le protocole VCI.

#figure(
  image("soclib_tp2_vci_signals.png", width: 80%),
  caption: [Signaux VCI],
) <vci_signals>

La plupart des champs VCI on des largeurs paramètrables (en nombre de bits) :

le paramètre N définit le nombre de bits du champs `ADDRESS`. Les adresses VCI sont des adresses octets, mais elles doivent être alignées sur des frontières de mot.
le paramètre B définit le nombre d'octets d'un mot de donnée VCI. Ce paramètre définit le largeur des trois champs `WDATA`, `RDATA` et `BE`.
le paramètre K définit le nombres de bits termettant de coder la longueur `PLEN` d'un paquet (en nombre d'octets). Nous n'utiliserons que des valeurs de `PLEN` multiples du paramètre B.
le paramètre S définit le nombre de bits du champs `SRCID`, qui permet de coder le numéro de l'initiateur VCI qui a démarré la transaction. Ce paramètre définit donc le nombre maximum d'initiateurs dans le système.
Le paramètre E définit le nombre de bits permettant de coder le type d'erreur dans le champs `RERROR` du paquet commande. `RERROR = 0` signifie "pas d'erreur".
Les deux paramètres T et P définissent le nombre de bits des deux champs `TRDID` et `PKTID`. Ces deux champs permettent d'étiqueter une commande VCI par une numéro de thread et/ou par un numéro de transaction. Ils peuvent être utilisés par un initiateur pour envoyer une commande (n+1) sans attendre d'avoir reçu la réponse à la commande (n).
Bien sûr les valeurs de ces paramètres VCI doivent être les mêmes pour tous les composants matériels d'une même architecture, et doivent être définis dans la top-cell décrivant l'architecture générale du système.


== Modélisation des interfaces VCI

La généricité des interfaces de communication VCI est évidemment une souplesse très utile, puisqu'elle permet d'adapter le protocole aux besoins particuliers de chaque application embarquée (on n'a pas toujours besoin de 4 Goctets d'espace adressable). Mais elle crée une difficulté pour la modélisation des composants matériels, puisqu'il faut écrire des modèles de simulation génériques, capable de s'adapter à différentes largeurs des champs adresse ou donnée. On utilise pour cela la techique des templates du langage C++ : on regroupe toutes les valeurs de ces paramètres dans un objet C++ de type `VciParams`, qui est utilisé comme paramètre template par tous les composants matériels qui possèdent des ports de communication VCI. (voir fichier `vci_param.h`).

Comme on l'a fait pour le canal de communication FIFO, on définit trois objets, qui facilitent l'écriture des modèles de simulation CABA :

l'objet `VciSignals` regroupe tous les signaux (commande et réponse) d'un canal VCI. (voir fichier `vci_signals.h`)
l'objet `VciInitiator` regroupe tous les ports utilisés par un initiateur VCI pour émettre une commande, et recevoir la réponse. (voir fichier `vci_initiator.h`)
l'objet `VciTarget` regroupe tous les ports utilisés par une cible VCI pour émettre une réponse, et recevoir une commande. (voir fichier `vci_target.h`)
Ces trois objets possèdent évidemment un paramètre template de type `VciParams`.

= Outillage logiciel

Dans cette section, on présente différentes classes C++ définies par la plate-forme de prototypage virtuel SoCLib pour faciliter et simplifier l'écriture des modèles de simulation.

== Indexation des composants VCI

On assigne à tout composant matériel possédant un port VCI (composant cible ou composant initiateur) un index permettant de l'identifier de façon unique. Cet index peut être éventuellement structuré (on parle d'index composite) si l'architecture est structurée en clusters. Un index composite est la concaténation d'un index global (le numéro de cluster) et d'un index local (à l'intérieur d'un cluster).

// Note: Important
La plate-forme SoCLib définit la classe C++ `IntTab` pour représenter ces index composites. (voir fichier `int_tab.h`). Dans ce TP, on utilisera un seul niveau d'indexation, mais il faut utiliser l'objet `IntTab` pour indexer les composants VCI.

== Segmentation de l'espace addressable

Dans une architecture à mémoire partagée, on assigne à tout composant cible un (ou plusieurs) segment(s) dans l'espace adressable. Un segment est une tranche de l'espace adressable. Il possède donc une adresse de base, et une longueur (en nombre d'octets). La longueur d'un segment peut être très variable : de quelques dizaines d'octets pour un périphériques adressable, à quelques Moctets pour un contrôleur mémoire. C'est en analysant les bits de poids fort de l'adresse que le sous-système d'interconnexion détermine à quel segment appartient l'adresse, et donc vers quelle cible un paquet commande doit être aiguillé. Le découpage de l'espace adressable en segments et l'assignation de ces segments aux différentes cibles est donc un caractéristique globale de l'architecture, et doit donc être définie dans la top-cell décrivant l'architecture générale du système.

La plate-forme SoCLib définit la classe C++ `MappingTable` permettant de centraliser dans un même objet logiciel toutes les informations concernant la segmentation de l'espace addressable, et la correspondance entre une cible VCI (identifiées par son index), et un segment. (voir fichiers `mapping_table.h` et `segment.h`).

Un segment est donc un objet caractérisé par 5 informations :

+ un nom
+ une adresse de base
+ une taille (en octets)
+ l'index de la cible VCI à laquelle il est assigné
+ un attribut de cachabilité

Pour plus de détails, vous pouvez consulter le site WEB du projet SoCLib :
#link("https://www.soclib.fr/trac/dev/wiki/Component/MappingTable")


== Décodage de l'adresse
Le champs `ADDRESS` de la commande VCI est décodé à deux endroits :

les bits de poids faibles sont décodés par les cibles VCI pour déterminer sur quel octet porte la commande.
les bits de poids fort sont décodés par le sous-système d'interconnexion pour déterminer l'index de la cible et router le paquet commande vers la cible concernée.
Pour faciliter ce décodage, la plate-forme SoCLib définit les classe C++ `AddressDecodingTable` (voir fichier `address_decoding_table.h`) et `AddressMaskingTable` (voir fichier `address_masking_table.h`)

== Allocation de tableaux

On a parfois besoin d'utiliser des tableaux d'objets complexes. Par exemple, le composant générique `VciVgsb` possède un nombre variable de ports VCI initiateur, et un nombre variable de ports VCI cibles. Ces ports sont donc définis comme des tableaux de ports. Pour pouvoir nommer chacun des éléments d'un tableau, la plate-forme SoCLib définit un mécanisme générique d'allocation et de désallocation de tableaux (voir fichier `alloc_elems.h`).

// TODO:
= Travail à réaliser

== Composant VciGcdCoprocessor

#figure(
  image("soclib_tp2_coprocessor.png", width: 80%),
  caption: [Diagramme du composant VciGcdCoprocessor],
) <vci_gcd_coprocessor>


Le composant VciGcdCoprocessor est un périphérique adressable, et doit donc être modélisé comme une cible VCI. Il possède un seul port de type VciTarget, et 4 registres (ou pseudo-registres) implantés dans l'espace addressable, qui peuvent donc - en principe - être lus ou écrits par n'importe quel initiateur du sytème.
*Chacun de ces registres a une largeur de 4 octets. Par conséquent, le segment occupé par ce périphérique dans l'espace adressable a une longueur de 4*4 = 16 octets.*

// TODO:
_*Question 3* : Pourquoi faut-t-il deux automates séparés pour contrôler l'interface VCI et pour contrôler le calcul du PGCD proprement dit ?_


Séparer les 2 automates permet tout d'abord une meilleure lisibilité, l'un s'occupe du PGCD pendant que l'autre s'occupe de la communication. Cela permet également de ne pas alourdir le matériel et permet à chacun de fonctionner indépendamment sans attendre la fin de l'autre.

// TODO:
_*Question 4* : Dans la fonction de transition, on a écrit le code
  définissant les transitions de l'automate vci\_fsm après le code
  définissant les transitions de l'automate exe\_fsm, bien que les
  transitions de l'automate exe\_fsm dépendent de l'état de l'automate
  vci\_fsm. On aurait aussi bien pu décrire ces deux automates dans
  l'ordre inverse. Quelle mécanisme du langage SystemC utilise-t-on pour
  rendre l'ordre de description des automates indifférent?_


On utilise l'algorithme de relaxation, ainsi le code est réexecuté jusqu'à que les valeurs soient disponibles.

// TODO:
_*Question 5* : comment sont traitées les erreurs dans ce modèle de simulation? à quoi servent ces vérifications ?_

== Composant VciGcdMaster

#figure(
  image("soclib_tp2_master.png", width: 80%),
  caption: [Diagramme du composant VciGcdMaster],
) <vci_gcd_master>


== Architecture minimale

#figure(
  image("soclib_tp2_simple_archi.png", width: 80%),
  caption: [Architecture minimale du système],
) <simple_archi>

== Compilation et génération du simulateur

== Architecture multi-maitres / multi-cibles

#figure(
  image("soclib_tp2_bus.png", width: 80%),
  caption: [Architecture multi-maîtres / multi-cibles avec bus VCI],
) <multi_bus_archi>


= VciGcdCoprocessor

Enfin, comme tous les modèles CABA de SoCLib, le coprocesseur GCD possède des fonctions membres définissant le comportement du composant, qui sont de trois types :

la fonction transition() est sensible au *front montant* du port d'entrée CK, et permet de calculer la valeur future des registres en fonction de la valeur courante des registres et des valeurs présentes sur les autres ports d'entrée.

la fonction genMoore() est sensible au *front descendant* du port d'entrée CK, et permet de calculer la valeur des ports de sortie qui ne dépendent que des valeurs stockées dans les registres.

les fonctions genMealy() (une ou plusieurs fonctions) sont sensibles au *front descendant* du port CK. De plus chaque fonction est sensible à un ensemble particulier de port d'entrée. Elle permettent de calculer la valeur des ports de sorties qui dépendent de façon combinatoire d'un ou plusieurs ports d'entrée.


```cpp
SC_METHOD(transition);
dont_initialize();
sensitive << p_clk.pos();

SC_METHOD(genMoore);
dont_initialize();
sensitive << p_clk.neg();
```

Le coprocesseur a deux automates : un automate `vci_fsm` qui contrôle l'interface VCI, et un automate `exe_fsm` qui contrôle le calcul du PGCD proprement dit.

Au reset, les automates sont initialisés dans l'état `VCI_GET_CMD` (attente d'une commande VCI) et `EXE_IDLE` (attente d'un calcul).

```cpp
if (!p_resetn.read())
{
  r_vci_fsm = VCI_GET_CMD;
  r_exe_fsm = EXE_IDLE;
  return;
}
```

Après, nous avons complété les transitions des deux automates d'après le diagramme d'état fourni dans l'énoncé.

== MAE EXE

Pour les états EXE_DECA et EXE_DECB, nous avons complété les opérations de décrémentation des registres opa et opb, ainsi que les transitions vers l'état EXE_COMPARE.

== MAE VCI

Cette machine á ètats est un peu plus complèxe, car elle doit gérer les deux types de transactions VCI (lecture et écriture). Nous avons complété les transitions d'états en fonction du diagramme d'état fourni dans l'énoncé.

Les commandes VCI sont stockées dans des registres lors de la réception d'une commande valide (CMDVAL et CMDACK à 1). Nous avons complété le code pour stocker les champs SRCID, TRDID et PKTID dans des registres.

Rappel:
- le paramètre S définit le nombre de bits du champs SRCID, qui permet de coder le numéro de l'initiateur VCI qui a démarré la transaction. Ce paramètre définit donc le nombre maximum d'initiateurs dans le système.
- Le paramètre E définit le nombre de bits permettant de coder le type d'erreur dans le champs RERROR du paquet commande. RERROR = 0 signifie "pas d'erreur".
- Les deux paramètres T et P définissent le nombre de bits des deux champs TRDID et PKTID. Ces deux champs permettent d'étiqueter une commande VCI par une numéro de thread et/ou par un numéro de transaction. Ils peuvent être utilisés par un initiateur pour envoyer une commande (n+1) sans attendre d'avoir reçu la réponse à la commande (n).

=== Commandes VCI
l'automate vci_fsm contrôle l'interface VCI : il répond aux commandes qu'il reçoit, en écrivant dans le registre concerné s'il sagit d'une écriture, ou en renvoyant la valeur demandée s'il s'agit d'une lecture.

```cpp
// store the VCI command in registers
r_srcid = p_vci.srcid.read();
r_trdid = p_vci.trdid.read();
r_pktid = p_vci.pktid.read();
```


= VCI master

L'etat initial est RANDOM, dans lequel on génère des opérandes aléatoires. On passe à l'état SEND_CMD_WRITE pour envoyer la commande d'écriture des opérandes au coprocesseur.


2. VCI Parameters Template
VciParams<4, 8, 32, 1, 1, 1, 12, 1, 1, 1>
These parameters correspond to the commented field widths:

4 → cell_size = 4 - Data cell size in bytes (32-bit words)
8 → plen_size = 8 - Packet length field width (8 bits)
32 → addr_size = 32 - Address field width (32 bits)
1 → rerror_size = 1 - Response error field width (1 bit)
1 → clen_size = 1 - Cache line length field width (1 bit)
1 → rflag_size = 1 - Response flag field width (1 bit)
12 → srcid_size = 12 - Source ID field width (12 bits = up to 4096 initiators)
1 → trdid_size = 1 - Thread ID field width (1 bit)
1 → pktid_size = 1 - Packet ID field width (1 bit)
1 → wrplen_size = 1 - Write packet length field width (1 bit)
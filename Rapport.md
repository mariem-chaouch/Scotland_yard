# Projet Logiciel Transversal

---

#### Prénom NOM – Prénom NOM
Meriem Chaouch
Eya Mezzi
Sarra Mostafavi
Adam Mankai
---

## 1 Objectif

### 1.1 Présentation générale
Le projet consiste à réaliser une version du jeu de société *Scotland Yard*. La partie se déroule sur un plateau représentant Londres, composé de 199 positions reliées entre elles par différents moyens de transport. Un joueur incarne Mister X et tente d'échapper aux quatre détectives qui le poursuivent. Les détectives doivent coopérer et déduire sa position à partir de ses déplacements et des tickets qu'il utilise.

Le plateau comporte des stations accessibles en taxi, en bus ou en métro, ainsi que des liaisons fluviales réservées à Mister X. Cinq pions représentent les joueurs : un pion blanc pour Mister X et quatre pions de couleurs différentes pour les détectives. Vingt-neuf cartes de départ permettent de déterminer les positions initiales. Les déplacements sont effectués à l'aide de tickets : 57 tickets de taxi, 45 tickets de bus, 23 tickets de métro et 5 tickets noirs. Mister X dispose également de 3 tickets « coup double » pour jouer deux déplacements consécutifs.

Le jeu oppose donc deux objectifs complémentaires : Mister X doit dissimuler sa trajectoire et survivre jusqu'à la fin de la partie, tandis que les détectives doivent exploiter les informations visibles pour anticiper ses déplacements et le rejoindre. Un schéma du plateau peut compléter cette présentation afin de montrer les positions et les liaisons entre les stations.

### 1.2 Règles du jeu
#### But du jeu

Mister X gagne s'il échappe aux détectives jusqu'à la fin du vingt-quatrième tour ou si aucun détective ne peut encore effectuer de déplacement. Les détectives gagnent dès que l'un d'eux arrive sur la position occupée par Mister X ou lorsque Mister X ne possède plus aucun déplacement possible.

#### Mise en place

Une partie se joue avec un pion blanc pour Mister X et jusqu'à quatre pions de détectives. Chaque détective tire secrètement une carte de départ « D » et place son pion sur la position indiquée. Mister X tire une carte de départ « X » et prend connaissance de sa position sans la révéler aux autres joueurs.

Chaque détective reçoit 11 tickets de taxi, 8 tickets de bus et 4 tickets de métro. Mister X reçoit 5 tickets noirs et 3 tickets « coup double ». Pour ses autres déplacements, il prend les tickets de transport nécessaires dans la réserve commune. Les positions de départ sont choisies parmi les 29 cartes prévues à cet effet.

#### Déplacements et moyens de transport

Le plateau comporte 199 positions reliées par trois moyens de transport :

- le taxi, représenté en jaune, permet de rejoindre une position voisine ;
- le bus, représenté en turquoise, permet de parcourir une distance moyenne entre deux arrêts reliés ;
- le métro, représenté en rouge, permet de parcourir une grande distance entre certaines stations.

Mister X peut également emprunter une liaison fluviale avec un ticket noir. Un ticket noir lui permet aussi d'utiliser un taxi, un bus ou un métro sans révéler le moyen de transport choisi. Les détectives ne peuvent pas emprunter les liaisons fluviales et ne disposent pas de tickets noirs.

Pour se déplacer, un joueur choisit un ticket qu'il possède puis déplace son pion vers une position voisine accessible avec ce moyen de transport. Un pion ne peut pas se déplacer vers une position occupée par un autre détective. Les détectives ne peuvent pas partager une même position. Chaque déplacement consomme le ticket utilisé. Si un détective ne possède plus le ticket correspondant ou ne dispose d'aucun déplacement possible, il passe son tour.

#### Déroulement d'un tour

Une partie dure au maximum 24 tours. À chaque tour, Mister X joue en premier, puis les détectives se déplacent chacun leur tour. Le tour est terminé lorsque tous les détectives ont joué ou passé leur tour.

Après son déplacement, Mister X indique le ticket utilisé, mais sa position reste cachée. Sa position est révélée après son déplacement lors des tours 3, 8, 13 et 24. Les détectives utilisent alors cette information pour réduire les positions possibles de Mister X et organiser leur poursuite.

Mister X peut dépenser un ticket « coup double » pour effectuer deux déplacements consécutifs. Les détectives jouent ensuite après ces deux déplacements. Chaque déplacement effectué avec un ticket « coup double » consomme également le ticket de transport correspondant. Le ticket « coup double » ne permet pas de supprimer les coûts de déplacement ni les règles d'occupation des positions.

Mister X connaît à tout moment les tickets encore disponibles pour les détectives. Les détectives connaissent les tickets utilisés par Mister X et les positions révélées, mais ils ne connaissent pas sa position entre deux révélations. La partie se poursuit jusqu'à ce que l'une des conditions de victoire soit remplie.

### 1.3 Conception Logiciel
Présenter ici les packages de votre solution, ainsi que leurs dépendances.


## 2 Description et conception des états
L'objectif de cette section est une description très fine des états dans le projet. Plusieurs niveaux de descriptions sont attendus. Le premier doit être général, afin que le lecteur puisse comprendre les éléments et principes en jeux. Le niveau suivant est celui de la conception logicielle. Pour ce faire, on présente à la fois un diagramme des classes, ainsi qu'un commentaire détaillé de ce diagramme. Indiquer l'utilisation de patron de conception sera très apprécié. Notez bien que les règles de changement d'état ne sont pas attendues dans cette section, même s'il n'est pas interdit d'illustrer de temps à autre des états par leurs possibles changements.

### 2.1 Description des états

### 2.2 Conception logicielle
	
### 2.3 Conception logicielle: extension pour le rendu

### 2.4 Conception logicielle: extension pour le moteur de jeu

### 2.5 Ressources

Illustration 1: Diagramme des classes d'état


## 3 Rendu: Stratégie et Conception
Présentez ici la stratégie générale que vous comptez suivre pour rendre un état. Cela doit tenir compte des problématiques de synchronisation entre les changements d'états et la vitesse d'affichage à l'écran. Puis, lorsque vous serez rendu à la partie client/serveur, expliquez comment vous aller gérer les problèmes liés à la latence. Après cette description, présentez la conception logicielle. Pour celle-ci, il est fortement recommandé de former une première partie indépendante de toute librairie graphique, puis de présenter d'autres parties qui l'implémente pour une librairie particulière. Enfin, toutes les classes de la première partie doivent avoir pour unique dépendance les classes d'état de la section précédente.

### 3.1 Stratégie de rendu d'un état

### 3.2 Conception logicielle

### 3.3 Conception logicielle: extension pour les animations

### 3.4 Ressources

### 3.5 Exemple de rendu

Illustration 2: Diagramme de classes pour le rendu

## 4 Règles de changement d'états et moteur de jeu
Dans cette section, il faut présenter les événements qui peuvent faire passer d'un état à un autre. Il faut également décrire les aspects liés au temps, comme la chronologie des événements et les aspects de synchronisation. Une fois ceci présenté, on propose une conception logicielle pour pouvoir mettre en œuvre ces règles, autrement dit le moteur de jeu.

### 4.1 Horloge globale

### 4.2 Changements extérieurs

### 4.3 Changements autonomes

### 4.4 Conception logicielle

### 4.5 Conception logicielle: extension pour l'IA

### 4.6 Conception logicielle: extension pour la parallélisation

Illustration 3: Diagrammes des classes pour le moteur de jeu


## 5 Intelligence Artificielle
Cette section est dédiée aux stratégies et outils développés pour créer un joueur artificiel. Ce robot doit utiliser les mêmes commandes qu'un joueur humain, ie utiliser les mêmes actions/ordres que ceux produit par le clavier ou la souris. Le robot ne doit pas avoir accès à plus information qu'un joueur humain. Comme pour les autres sections, commencez par présenter la stratégie, puis la conception logicielle.
### 5.1 Stratégies

#### 5.1.1 Intelligence minimale

#### 5.1.2 Intelligence basée sur des heuristiques

#### 5.1.3 Intelligence basée sur les arbres de recherche

### 5.2 Conception logicielle

### 5.3 Conception logicielle: extension pour l'IA composée

### 5.4 Conception logicielle: extension pour IA avancée

### 5.5 Conception logicielle: extension pour la parallélisation


## 6 Modularisation
Cette section se concentre sur la répartition des différents modules du jeu dans différents processus. Deux niveaux doivent être considérés. Le premier est la répartition des modules sur différents threads. Notons bien que ce qui est attendu est un parallélisation maximale des traitements: il faut bien démontrer que l'intersection des processus communs ou bloquant est minimale. Le deuxième niveau est la répartition des modules sur différentes machines, via une interface réseau. Dans tous les cas, motivez vos choix, et indiquez également les latences qui en résulte.

### 6.1 Organisation des modules

#### 6.1.1 Répartition sur différents threads

#### 6.1.2 Répartition sur différentes machines

### 6.2 Conception logiciel

### 6.3 Conception logiciel: extension réseau

### 6.4 Conception logiciel: client Android

Illustration 4: Diagramme de classes pour la modularisation

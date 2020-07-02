# Contexte #

Un malheur s'abat sur le Cégep de Joliette. Un litige a empêché Patrick Singcaster, l’enseignant d’informatique du programme SIM, de sortir d’une réunion pour une durée indéterminée. Un siècle passa pour lui à débattre de la position d’une virgule dans une phrase. La nature a repris son contrôle, les forces du mal ont envahi la ville et les enseignants, êtres immortels et prêts à tout pour enseigner leurs matières, se sont installés autour du Cégep pour aménager leur propre donjon spécialisé.
	
Pris de panique et incapable d’envoyer un seul message à qui que ce soit, Patrick finit par prendre les grands moyens... Il envoya un MIO à l’ensemble de ses classes d’informatique comme appel à l’aide. Les MIOs étant devenus un service désuet, son message était destiné à l’oubli de centaines d’élèves pourtant inscrits au dit service.

Un étudiant vu la fameuse notification et lu le message jusqu’au bout. Il ne se sentait pas d’aplomb pour l’épreuve qui l’attendait et ses notes laissaient, il faut l’admettre, à désirer. Par contre, il ne pouvait s’empêcher de ressentir une certaine force à l’intérieur de lui-même qui le guida jusqu’à Joliette pour commencer l’aventure.

# Environnement #

## Monde ##

Monde ouvert dans lequel des parcours doivent être réalisés et des ennemies combattues afin de découvrir des donjons.

Atmosphère selon un système de jours et nuits, ayant une influence sur le comportement des ennemies et des alliés.

La végétation du monde sera générée aléatoirement de manière procédurale.

La caméra sera contrôlée par le joueur.

## Donjon ##

Suite à la résolution d'un casse-tête, un boss, ayant une mécanique personnelle, devra être combattu.

| Boss | Donjon | Mécanique du Boss | Mécanique du Donjon | Lègue |
|-|-|-|-|-|
| Tortue | Circuit Logique | Bouclier vrai ou faux | Circuit logique aléatoire | Bois courbé (Ard 1/2), page du formulaire |
| Alchimiste | Chimie | Se défendre de ses éléments derrières les bons abris. Tirer avec l'arc à flèche | Combiner des blocs d'éléments | Pierre philosophale (Sceptre 1/2), page du formulaire |
| Phénix | Physique (gravité) | Vole, tombe si tiré avec l'arc  | Gravité, plateformer | Mini phénix, page du formulaire |
| Les avocats | Cégep | Point faibles, différentes phases | Exploitation de l'ensemble des objets récoltés | Patrick |


La caméra s'ajustera automatiquement afin de toujours bien cadrer le personnage.

## Interfaces ##

Plusieurs interfaces utilisateur seront disponibles :

- Menu principal : Nouvelle partie, charger une partie, options.
- Menu en jeu : Inventaire, échange, succès, sauvegarde, options.
- IGU : Indicateurs, mini-carte, attaque, visée.

*[ TODO : Prototype (pas une photo de tableau) pour chacune de ces interfaces. ]*

# Personnage #

## Caractéristiques ##

Les caractéristiques du personnage sont :

- Vie : Nombre de dommage que le personnage peut supporter.
- Force : Nombre de dommage que le personnage peut causer.
- Défense : Nombre de dommage que le personnage peut contrer.

## Progression ##

Certaines actions permettront d'accumuler de l’expérience. L'expérience permettra de faire augmenter le niveau. Et chaque niveau permettra d'augmenter les caractéristiques du personnage.

## Forge ##

Permet de créer certains objets du jeu à partir de matériaux

| Objet | Matériaux |
|-|-|
| Épée fer | Morceau de fer, manche en bois |
| Arc | 10 ficelles d'araignées, morceau de bois courbé |
| Sceptre | Ajoute un effet de feu ou de glace à l'arme choisie (épée de fer ou arc) |

## Équipement ##

Obtenu lors de la complétion de donjon, il permettra d'améliorer certaines caractéristiques :

- Armes : Permets de causer des dommages aux ennemies.
  - Arc : Permets de causer des dommages à distance via un système de visée.
  - Épée : Permets de causer des dommages rapprochés selon l'angle d'attaque.
  - Pierre : Ajoute un effet à une arme choisie.

| Arme | Caractéristiques | Mécanique |
|-|-|-|
| Épée de bois | 0-2 dommages<br>1 portée | Aucune |
| Épée fer | 2-4 dommages<br>2 portée | Touche tous les ennemis autour du personnage avec l'attaque spéciale. |
| Arc | 1-3 dommages<br>variée portée | Tir à distance |
| Pierre philosophale | Amélioration d'autres armes<br>fixe à l'arme portée | Ajoute un effet de feu ou de glace à l'arme choisie (épée de fer ou arc) |
  
- Ensemble : Permets d'améliorer la défense contre un type de dommage.

| Ensemble | Caractéristiques | Mécanique |
|-|-|-|
| Linge | 0-1 défense | Peut être déchiré |
| Cotte de maille | 3 défense physique | Indestructible |
| Habit de protection de température | 2 défense physique, protège contre la chaleur intense | Indestructible |

- Autres
  - Botte : Permets d'améliorer la vitesse de déplacement ou la hauteur de saut.

| Autre | Caractéristiques | Mécanique |
|-|-|-|
| Gougounes | + vitesse | Peuvent percer, fait un son très dérengant |
| Souliers | - masse | Usure |
| ... | ... | ... |

# Ennemies #

Contrôlé par des robots simples et des actions prédéfinies, leur niveau dépend de la région où ils se trouvent.

| Ennemie | Mécanique | Lègue |
|-|-|-|
| Goomba | Touchable que par le dessus | Potion santé / Gougounnes |
| Zombie | Marches lentement | Potion santé / Gougounnes |
| Sorcières | Dégats à distances | Pierre philosophale (rare) / Potion santé / Gougounnes |
| Awkwards | Explosent à proximité | Potion santé / Souliers |

## Caractéristiques ##

Les caractéristiques des ennemies sont :

- Vie : Nombre de dommage que l'ennemie peut supporter.
- Mécanique : Corps-à-corps, à distance, par-derrière, etc.

## Dommages ##

Les ennemies causent un type de dommage :

- Physique
- Magique
- ...

# Alliés #

Plusieurs alliés se trouveront un peu partout et pourront interagir avec le personnage sous forme de dialogues à choix multiples. Les dialogues pourront guider le joueur et lui donner des indices tels que l’emplacement de coffres et de donjons. Certains de ces alliés pourront acheter ou vendre des objets au personnage.

| Allié | Indication | Lègue |
|-|-|-|
| Adam | Indique comment atteindre le donjon #1 | Épée de bois |
| ... | ... | ... |

## Objets ##

- Potions : Restaurer la vie, ramener à la vie, etc.

| Objet | Caractéristiques | Mécanique |
|-|-|-|
| Potion santé | Restauration de 25% de la santé | Consommable hors combat |
| ... | ... | ... |

# Déroulement d’une partie #

Une lecture de textes nous permettra de bien comprendre l’histoire du jeu ainsi que la quête. 

La partie débute dans une petite forêt. Le joueur aura comme première quête de se rendre au village. Au fur et à mesure que le personnage se rapproche du village, le joueur apprendra les mécaniques et les contrôles de base comme les touches à appuyer pour sauter, avancer, comment tuer un ennemi, comment interagir avec les alliés, etc. 

À n’importe quel moment, il est possible d'ouvrir un menu pour voir l'inventaire, les succès et les options. Il est possible de parler aux alliés près pour obtenir de l'aide. Ils aideront entre autres, dépendamment de la discussion, à rejoindre le premier donjon. Pour les plus téméraires, il est aussi possible d'aller combattre des ennemies dans les alentours.

Dans un donjon, un casse-tête doit être résolu. Il est toujours possible de sortir d’un donjon. Après avoir réussi le casse-tête, une salle se déverrouille. À l’intérieur se trouve un Boss selon la difficulté du donjon. Plus nous sommes loin dans l’aventure, plus le niveau de difficulté augmente.

Chaque Boss tué permet d'obtenir un objet pouvant être utilisé dans la suite de l’histoire. Chaque monstre tué a un pourcentage de chance de laisser tomber un objet. De plus, chaque monstre ou Boss tué nous rapporte de l’expérience.

## Exemple de Donjon ##

En entrant dans la salle, il y a une porte bloquée par une roche et 3 leviers liés entre eux par des portes-logiques. Les leviers représentent des signaux électriques. On doit activer et désactiver les bons leviers afin de transmettre le signal à la roche qui explosera, ouvrant la porte.

Un directeur de département comme Boss qui, *[ TODO : Exemple concret d'un combat avec un Boss, prérequis, mécaniques pour le vaincre, etc. ]* 

Une fois vaincu, le Boss lègue au joueur une page de formulaire et une arme. Puis le joueur sort du donjon et continue l’aventure.

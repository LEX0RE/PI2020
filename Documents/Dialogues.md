# Syntaxe des dialogues

Les dialogues devront être écris dans le fichier "Dialogues.txt" selon un identifiant par dialogue.
Cet identifiant est précédé d'un '#'.
Chaque dialogue doit aussi finir par un "#end" qui détermine la fin d'un dialogue.
Il doit aussi y avoir un '*' au début de chaque "boîte" de dialogue qui permet, par exemple, d'écrire une seule phrase à la fois.
Finalement, on peut aller chercher un dialogue à l'aide de la méthode 

## '#'

Permet de définir l'identifiant qui précedera le dialogue.
Il peut aussi servir à terminer un dialogue mais cela sera expliqué plus bas.

Exemple : ``` #Premier dialogue ```
## '*'

Permet de séparer les différentes boîtes dans un même dialogues sous forme de phrases ou de paragraphes par exemple.

Exemple : ``` *Première phrase du dialogue. Deuxième phrase.
              *Troisième phrase. ```

## "#end"

Permet de montrer la fin d'un dialoguee.

## Exception

Seule exception: on ne peut pas écrire de '*' dans un dialogue.

Les dialogues de la quête «GUI et actions»:

# Voici votre barre de vie, si elle tombe à 0, vous allez perdre toute votre progression actuelle. #end
# Votre niveau vous rend plus fort. #end
# Quand votre barre d'expérience est remplie, votre niveau augmente de 1 et votre barre recommence à 0. #end
# Voici votre minimap pour ne pas vous perdre. #end
# Voici votre panneau de vos quêtes actuelles. #end
# Appuie sur la touche «r» pour donner un coup d'épée sur la cible. #end
# Tire une flèche sur la cible à l'aide de la touche «r». #end
# Bravo ! Tu connais maintenant le fonctionnement du GUI et les actions qu'il est possible d'éxécuter. #end
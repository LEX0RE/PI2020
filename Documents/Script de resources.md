# Syntaxe du script de resource.

Le script de resource est un fichier nommé "Resources.txt" contenant, en ordre, le *type*, le *chemin* d'accès à la resource, 
un paramètre ayant une certaine fonctionnalité *[optionel]*, ainsi que du nom de la resource, qui sera stocké dans le 
ResourceManager. La première ligne du script *doit* contenir le nombre de resource définie dans le fichier.

## Types 

Voici comment définir le type d'une resource: 

- t : Texture
- m : TexturedMesh
- c : ColoredMesh (N'est pas encore implémenté)
- f : Font 

## Chemin d'accès

Le chemin d'accès doit être entre guillemets ("...").
Exemple: "Resources/image.png"

## Paramètre additionel

*Cette section s'applique uniquement aux Font et aux TextureMesh.*

Pour une TexturedMesh, les paramètre additionnel sera le nombre de frame que la mesh contient suivi du nom d'une texture chargée avant le chargement d'une TextureMesh. 
Par exemple: 
  On définirait une TexturedMesh en commençant par mettre la lettre 'm'. 
  Puis, on spécifirait le chemin d'accès. 
  Puis, on indique le nombre de frame la mesh contient.
  Ensuite, il faut spécifier le nom d'une texture déjà chargée. 
  Puis finalement, on dicte le nom qu'on veut donner à la TexturedMesh.

Pour un Font, le paramètre additionnel sera la taille de la police d'écriture. 
Par exemple:
  On définirait un Font en commençant par mettre la lettre 'f'. 
  Puis, on spécifirait le chemin d'accès. 
  Ensuite, il faut spécifier la taille désirée.
  Puis finalement, on dicte le nom qu'on veut donner au Font.

## Nom d'identification

Le nom d'identification sert à stocker la resource chargée dans une std::map. De cette façon, nous pouvons facilement 
retrouver une resource avec son nom. Pour l'instant, le nom doit être de moins de 30 caractères (tableaux tampons).  

## Exemples

Voici un exemple de script:

```
3
t "Resources/Images/default.png" Default
m "Resources/Mesh/sphere.obj" 1 Default Sphere
f "Resources/Font/RobotoMedium.ttf" 24 Roboto24
```

## Normes de programation

### Fonctions / Méthodes / Classe / Struct / Typedef
- La première lettre d'un nom de fonction / méthode / classe doit être majuscule.
- Si la classe n'est pas dérivée et que son contenu est public, alors utiliser ```struct StructName { };```
- Les éléments privés d'une classe doivent être placés au début de la déclaration de classe. Par exemple:
    ```
    class AClass {
        //private members.
     public:
        //public members.
    };
    ```
- Ne pas utiliser les ```unsigned``` et utiliser les types définits suivants:
    ```
    typedef unsigned long ulong;
    typedef unsigned int uint; 
    typedef unsigned short ushort;
    typedef unsigned char Uint8;
    ```
### Variables
- La première lettre d'un nom de variable doit être minuscule.
- Les noms de variables doivent être écrites en anglais.

### Aération de code
- Un espace avant et après l'opérateur.
- Un espace après une virgule dans les paramètres de fonctions/méthodes
- Un espace après un point-virgule dans les boucles "for". Par Exemple: 
    ```
    for(int i = 0; i < 10; i++) {...}
    ```
- Un saut de ligne seulement pour séparer le code dans une fonction/méthode
- Un espace entre les variables / nombres et les opérateurs. Exemple: ```a += 3;```
  (Exception pour les opérateurs d'incrémentation ```i++``` / décrémentation ```i--```)
- Les méthodes n'ayant qu'une seule ligne doivent sauter une ligne et avoir le mot clée ```inline```. Exemple: 
   ```
   inline void Move(int x, int y) {
       obj->move(x, y);
   }
   ```

### Blocs
- Pas de "inline if".
- Accolade sur la même ligne que la déclaration de bloc. Par exemple:
    ```
    void Function(...) {
         //code
    }
    ```
- Pas d'accolade si le bloc ```if``` / ```else``` / ```for``` / ```while``` n'a qu'une ligne. Par exemple:
    ```
    if(i < 4) 
        DoSomething();
    ```
- Les tabulations de 4 espaces.

### Débuggage
- Pas d'appel de fonction dans la section conditionnelle d'une boucle "for"; Par exemple:
    ```
    int count = list.size();
    for(int i = 0; i < count; i++) ...
    ```

### Headers
- Les fichiers d'en-tête devront terminer par l'extension ".hpp".

###

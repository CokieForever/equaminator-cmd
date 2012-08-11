Equaminator CMD - Un parseur mathématique avec interface en ligne de commande.


*** INFORMATIONS GENERALES ***

Auteur : Cokie
Version : 1.0 bêta
Dernière mise à jour : 11/08/2012
Plateformes : Windows, théoriquement Linux, Mac (non testé)
License : GNU GPL v2.0 (voir le fichier LICENSE.txt pour plus d'informations)
Pour toute demande d'information, remarque, suggestion, etc., contactez-moi à l'adresse "cokie.forever@gmail.com".


*** DESCRIPTION RAPIDE ***

Equaminator CMD est, globalement, une grosse calculatrice en console. Il permet d'analyser syntaxiquement des chaînes de caractères afin de les interpréter pour en tirer des calculs d'ordre mathématique, qu'il effectue ensuite. Comme tout analyseur syntaxique, il demande une forte rigueur dans la syntaxe des expressions qu'on lui soumet, toutefois, cette syntaxe étant globalement la même que celle des expressions mathématiques courantes, il est aisé à prendre en main. Vous pouvez utiliser seulement le moteur d'analyse pour vos programmes, ou utiliser également l'interface afin de s'en servir comme calculatrice.


*** UTILISATION ***

Le programme est compilé pour Windows, l'exécutable se trouve dans le dossier principal sous le nom "EquaCMD.exe". Lancez-le, et lorsqu'il est prêt, vous pourrez entrer vos instructions. Commencez par l'instruction "help"... ;)

Le moteur se base sur deux choses : les Objets et les Opérateurs.

Un Objet, cela peut être un nombre complexe, une fonction, une matrice (d'Objets), un n-uplet (d'Objets), ou un booléen. La liste des Objets pré-enregistrés (notamment des fonctions) est accessible à l'aide de l'instruction "varlist".
	Les Objets les plus simples sont les complexes. Tapez "12", et l'expression sera interprétée comme l'Objet de type complexe de partie réelle 12 et de partie imaginaire 0, soit, le réel 12. Pour spécifier la partie imaginaire, utilisez l'Objet pré-enregistré "i", complexe de partie réelle nulle et de partie imaginaire égale à 1. Par exemple, "1+2*i".
	Les fonctions peuvent être créées à l'aide de l'Opérateur de création de fonction "->" (voir ci-dessous). Par exemple, l'expression "x->x+1" sera interprétée comme l'Objet de type fonction qui à tout Objet x associe l'Objet x incrémenté de 1. Pour crééer des fonctions de plusieurs variables, utilisez la syntaxe suivante : "(x,y)->x+y" créée par exemple la fonction qui à deux Objets x et y associe la somme de ces deux Objets. Pour évaluer une fonction en un point, utilisez l'Opérateur d'évaluation ":" (voir ci-dessous) avec à gauche la fonction à évaluer et à droite le point (éventuellement n-uplet si la fonction a plusieurs variables) en lequel évaluer la fonction.
	Les matrices utilisent la syntaxe suivante : "[[1,2][3,4][5,6]]" sera interprétée comme la matrice 3x2 (3 lignes, 2 colonnes) contenant les réels 1, 2, 3, 4, 5 et 6 dans l'ordre, de gauche à droite et de haut en bas. Tout Objet peut être mis dans une matrice, y compris une autre matrice, et tous les Objets ne sont pas forcés d'être du même type. Pour récupérer l'élément (i,j) d'une matrice, utilisez l'Opérateur d'évaluation ":" (voir ci-dessous) avec à gauche la matrice et à droite le 2-uplet (i,j).
	Les n-uplets sont créées à l'aide de la syntaxe suivante : "(1,2,3,4)" créée le 4-uplet contenant les réels 1, 2, 3 et 4 dans l'ordre. Tout Objet peut être mis dans un n-uplet, y compris un autre n-uplet, et tous les Objets ne sont pas forcés d'être du même type. Pour récupérer le k-ième élément d'un n-uplet, utilisez l'Opérateur d'évaluation ":" (voir ci-dessous) avec à gauche le n-uplet et à droite l'entier k.
	Enfin, les booléens sont les plus simples, ils sont tous les deux définis comme Objets pré-enregistrés : "VRAI" et "FAUX".

Un Opérateur est quand à lui capable de créer un Objet à partir d'un ou plusieurs autres Objets, selon un comportement spécifique. Un exemple simple : l'Opérateur "+" peut additionner deux Objets, autrement dit, il peut créer un Objet (la somme) à partir de deux autres. La liste des Opérateurs est disponible en tapant l'instruction "oplist".
	Les Opérateurs ont tous une priorité déterminée : par exemple, la priorité de l'Opérateur de multiplication "*" est supérieure à celle de l'Opérateur d'addition "+", ce qui fait que l'expression "2+3*4" sera interprétée comme "2+(3*4)", soit le réel 14, plutôt que "(2+3)*4", soit le réel 20.
	L'Opérateur ":" est particulier, il peut être implicite. C'est l'Opérateur d'évaluation. Par exemple, il peut servir à évaluer une fonction en un complexe (voir ci-dessus) : "cos:(0)". La possibilité de le rendre implicite autorise à écrire "cos(0)" pour le même résultat.
	Certains Opérateurs demandent non pas un Objet en entrée, mais du Texte simple, à droite, à gauche, ou des deux côtés. C'est le cas, par exemple, de l'Opérateur de création de fonction "->". Il demande du Texte à droite et à gauche, de sorte que, si vous entrez "x->2*x", bien que ni "x" ni "2*x" ne puissent être interprétées comme des Objets valides, la fonction sera tout de même créée.
	Enfin, certains Opérateurs ont un effet de bord : en plus de renvoyer un résultat, ils ont un effet interne. Par exemple, l'Opérateur de création de constante ":=" renverra la valeur de la constante créée mais l'enregistrera également comme Objet permanent.

Les expressions que vous envoyez au moteur peuvent contenir des espaces n'importe où, ils seront supprimés au traitement. "1  23 2" sera ainsi interprétée comme le réel 1232.

L'interface utilise de plus des Commandes pour gérer le moteur. La liste des Commandes disponibles peut être obtenue en tapant la Commande "help".
Certaines Commandes prennent un argument, parfois facultatif. L'argument se place alors après le nom de la Commande, séparé de celui-ci par un seul espace. Par exemple, "varlist cos" appelle la Commande "varlist" avec l'argument "cos".


*** OBJECTIF ***

Le programme n'a pas été créé pour la vitesse mais pour la réutilisabilité. En effet, il est aisé, si l'on dispose de connaissances suffisantes en langage C, de créer de nouveaux Opérateurs ou Objets ou de modifier ceux définis. C'est pourquoi, le logiciel est sous license GNU GPL, qui permet globalement la modification et la rediffusion libre.
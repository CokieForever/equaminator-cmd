Equaminator CMD - Un parseur math�matique avec interface en ligne de commande.


*** INFORMATIONS GENERALES ***

Auteur : Cokie
Version : 1.0.1 b�ta
Derni�re mise � jour : 11/08/2012
Plateformes : Windows, th�oriquement Linux, Mac (non test�)
License : GNU GPL v2.0 (voir le fichier LICENSE.txt pour plus d'informations)
Pour toute demande d'information, remarque, suggestion, etc., contactez-moi � l'adresse "cokie.forever@gmail.com".


*** DESCRIPTION RAPIDE ***

Equaminator CMD est, globalement, une grosse calculatrice en console. Il permet d'analyser syntaxiquement des cha�nes de caract�res afin de les interpr�ter pour en tirer des calculs d'ordre math�matique, qu'il effectue ensuite. Comme tout analyseur syntaxique, il demande une forte rigueur dans la syntaxe des expressions qu'on lui soumet, toutefois, cette syntaxe �tant globalement la m�me que celle des expressions math�matiques courantes, il est ais� � prendre en main. Vous pouvez utiliser seulement le moteur d'analyse pour vos programmes, ou utiliser �galement l'interface afin de s'en servir comme calculatrice.


*** UTILISATION ***

Le programme est compil� pour Windows, l'ex�cutable se trouve dans le dossier principal sous le nom "EquaCMD.exe". Lancez-le, et lorsqu'il est pr�t, vous pourrez entrer vos instructions. Commencez par l'instruction "help"... ;)

Le moteur se base sur deux choses : les Objets et les Op�rateurs.

Un Objet, cela peut �tre un nombre complexe, une fonction, une matrice (d'Objets), un n-uplet (d'Objets), ou un bool�en. La liste des Objets pr�-enregistr�s (notamment des fonctions) est accessible � l'aide de l'instruction "varlist".
	Les Objets les plus simples sont les complexes. Tapez "12", et l'expression sera interpr�t�e comme l'Objet de type complexe de partie r�elle 12 et de partie imaginaire 0, soit, le r�el 12. Pour sp�cifier la partie imaginaire, utilisez l'Objet pr�-enregistr� "i", complexe de partie r�elle nulle et de partie imaginaire �gale � 1. Par exemple, "1+2*i".
	Les fonctions peuvent �tre cr��es � l'aide de l'Op�rateur de cr�ation de fonction "->" (voir ci-dessous). Par exemple, l'expression "x->x+1" sera interpr�t�e comme l'Objet de type fonction qui � tout Objet x associe l'Objet x incr�ment� de 1. Pour cr��er des fonctions de plusieurs variables, utilisez la syntaxe suivante : "(x,y)->x+y" cr��e par exemple la fonction qui � deux Objets x et y associe la somme de ces deux Objets. Pour �valuer une fonction en un point, utilisez l'Op�rateur d'�valuation ":" (voir ci-dessous) avec � gauche la fonction � �valuer et � droite le point (�ventuellement n-uplet si la fonction a plusieurs variables) en lequel �valuer la fonction.
	Les matrices utilisent la syntaxe suivante : "[[1,2][3,4][5,6]]" sera interpr�t�e comme la matrice 3x2 (3 lignes, 2 colonnes) contenant les r�els 1, 2, 3, 4, 5 et 6 dans l'ordre, de gauche � droite et de haut en bas. Tout Objet peut �tre mis dans une matrice, y compris une autre matrice, et tous les Objets ne sont pas forc�s d'�tre du m�me type. Pour r�cup�rer l'�l�ment (i,j) d'une matrice, utilisez l'Op�rateur d'�valuation ":" (voir ci-dessous) avec � gauche la matrice et � droite le 2-uplet (i,j).
	Les n-uplets sont cr��es � l'aide de la syntaxe suivante : "(1,2,3,4)" cr��e le 4-uplet contenant les r�els 1, 2, 3 et 4 dans l'ordre. Tout Objet peut �tre mis dans un n-uplet, y compris un autre n-uplet, et tous les Objets ne sont pas forc�s d'�tre du m�me type. Pour r�cup�rer le k-i�me �l�ment d'un n-uplet, utilisez l'Op�rateur d'�valuation ":" (voir ci-dessous) avec � gauche le n-uplet et � droite l'entier k.
	Enfin, les bool�ens sont les plus simples, ils sont tous les deux d�finis comme Objets pr�-enregistr�s : "VRAI" et "FAUX".

Un Op�rateur est quand � lui capable de cr�er un Objet � partir d'un ou plusieurs autres Objets, selon un comportement sp�cifique. Un exemple simple : l'Op�rateur "+" peut additionner deux Objets, autrement dit, il peut cr�er un Objet (la somme) � partir de deux autres. La liste des Op�rateurs est disponible en tapant l'instruction "oplist".
	Les Op�rateurs ont tous une priorit� d�termin�e : par exemple, la priorit� de l'Op�rateur de multiplication "*" est sup�rieure � celle de l'Op�rateur d'addition "+", ce qui fait que l'expression "2+3*4" sera interpr�t�e comme "2+(3*4)", soit le r�el 14, plut�t que "(2+3)*4", soit le r�el 20.
	L'Op�rateur ":" est particulier, il peut �tre implicite. C'est l'Op�rateur d'�valuation. Par exemple, il peut servir � �valuer une fonction en un complexe (voir ci-dessus) : "cos:(0)". La possibilit� de le rendre implicite autorise � �crire "cos(0)" pour le m�me r�sultat.
	Certains Op�rateurs demandent non pas un Objet en entr�e, mais du Texte simple, � droite, � gauche, ou des deux c�t�s. C'est le cas, par exemple, de l'Op�rateur de cr�ation de fonction "->". Il demande du Texte � droite et � gauche, de sorte que, si vous entrez "x->2*x", bien que ni "x" ni "2*x" ne puissent �tre interpr�t�es comme des Objets valides, la fonction sera tout de m�me cr��e.
	Enfin, certains Op�rateurs ont un effet de bord : en plus de renvoyer un r�sultat, ils ont un effet interne. Par exemple, l'Op�rateur de cr�ation de constante ":=" renverra la valeur de la constante cr��e mais l'enregistrera �galement comme Objet permanent.

Les expressions que vous envoyez au moteur peuvent contenir des espaces n'importe o�, ils seront supprim�s au traitement. "1  23 2" sera ainsi interpr�t�e comme le r�el 1232.

L'interface utilise de plus des Commandes pour g�rer le moteur. La liste des Commandes disponibles peut �tre obtenue en tapant la Commande "help".
Certaines Commandes prennent un argument, parfois facultatif. L'argument se place alors apr�s le nom de la Commande, s�par� de celui-ci par un seul espace. Par exemple, "varlist cos" appelle la Commande "varlist" avec l'argument "cos".


*** OBJECTIF ***

Le programme n'a pas �t� cr�� pour la vitesse mais pour la r�utilisabilit�. En effet, il est ais�, si l'on dispose de connaissances suffisantes en langage C, de cr�er de nouveaux Op�rateurs ou Objets ou de modifier ceux d�finis. C'est pourquoi, le logiciel est sous license GNU GPL, qui permet globalement la modification et la rediffusion libre.
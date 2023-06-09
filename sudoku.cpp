/***********************************
 * Nom : Côté, Dany			       *
 * Date : 15/12/2008               *
 * Fichier : sudoku.cpp            *
 * But : utiliser la liste dans un *
 *	logiciel qui résout des sudoku *
 **********************************/
#include "sudoku.h"

//-------------
// Constructeur du jeu
// fichier : définit le fichier dans lequel sera le jeu à solutionner
//------------
sudoku::sudoku(const char *fichier)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	SMALL_RECT rect;
	rect.Top=0;
	rect.Bottom=49;
	rect.Left=0;
	rect.Right=79;
	SetConsoleWindowInfo(hConsole,true,&rect);

	debug = false;
	display	= true;
	debugVal = 0;
	nbRestant = 80;
	// On charge le fichier à solutionner
	ifstream in(fichier);
	int val;
	// On boucle toutes les cases (9x9)
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			in >>  val;
			// Si la valeur lue est plus grande que 0 c-à-d que nous n'avons pas
			// à rechercher de valeur, cette case n'a qu'un choix.
			if ( val > 0)
			{
				pos[i][j].L.inserer(val,pos[i][j]);
				connus[i][j]=true;
			}
			// sinon on charge les 9 possibilités
			else
			{
				connus[i][j]=false;
				for(int k=1;k<10;k++)
					pos[i][j].L.inserer(k,pos[i][j]);
			}
			// Toutes les cases sont définies comme non vérifiées
			checked[i][j] = false;
		}
	}
	// Fermeture du fichier
	in.close();
}

//----------
// Constructeur sans paramètres
//----------
sudoku::sudoku()
{
	debug	= false;
	display	= true;
	stepbystep = false;
	debugVal = 0;
}

//------------
// Copieur
// S : sudoku à copier
//-------------
void sudoku::copie(sudoku &S)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	stepbystep = S.stepbystep;
	nbRestant = S.nbRestant;
	display=S.display;
	debug=S.debug;

	// Copie des cases
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			connus[i][j]=S.connus[i][j];
			checked[i][j]=S.checked[i][j];

			pos[i][j].L.vider();
			S.pos[i][j].debut();
			for(int k=0;k<S.pos[i][j].L.taille();k++)
			{
				pos[i][j].L.inserer(*S.pos[i][j]++,pos[i][j]);
			}
		}
	}
}

//----------
// Enlève une valeur de la liste d'une case
// Val : Valeur désirée
// in  : Case dans laquelle chercher
//-----------
void sudoku::flush(int val, ITliste<int> & in)
{
	// Si nous avons plus d'un choix
	// (taille de la liste d'une cellule)
	if ( in.L.taille()>1)
	{
		in.debut();
		for(int i=0;i<in.L.taille();i++)
		{
			if(*in==val)
			{
				in.L.supprimer(in);
				i=10;
			}
			else
				in++;
		}
	}
	// Si la seule valeur restante est égale a la valeur
	// en cours de recherche (cela veut dire qu'il y a eu un
	// problème et qu'il y a une valeur incorrecte
	else if (*in==val)
	{
		debug=true;
	}
}
//------------
// Méthode qui supprime la valeur du point "P" dans la ligne de ce même point
//-------------
void sudoku::clean_row(point P)
{
	for(int i=0;i<9;i++)
		if(P.x != i)
			flush(*pos[P.y][P.x],pos[P.y][i]);
}

//------------
// Méthode qui supprime la valeur du point "P" dans la colonne de ce même point
//-------------
void sudoku::clean_col(point P)
{
	for(int i=0;i<9;i++)
		if(P.y != i)
			flush(*pos[P.y][P.x],pos[i][P.x]);
}

//------------
// Méthode qui supprime la valeur du point "P" dans :
// La ligne, la colonne et la sous-grille
//-------------
void sudoku::clean_all(point P)
{
	clean_row(P);
	clean_col(P);

	point csg = coinSousGrille(P);

	for(int i=csg.y;i<csg.y+3;i++)
	{
		for(int j=csg.x;j<csg.x+3;j++)
		{
			if(P.y != i && P.x != j)
				flush(*pos[P.y][P.x],pos[i][j]);
		}
	}
}

//-------------
// Retourne la position de la sous-grille contenant un point P
// P : point à partir du quel trouver la sous-grille
//-------------
sudoku::point sudoku::coinSousGrille(point P){
	P.x = (P.x/3)*3;
	P.y = (P.y/3)*3;

	return P;
}

//----------
// Détermine si la case en question contient seulement une valeur
// in  : Case dans laquelle chercher
//-----------
bool sudoku::oneLeft(ITliste<int> & in)
{
	if ( in.L.taille() == 1)
		return true;
	return false;
}

//----------
// Recherche le Point suivant dans lequel il reste qu'une seule valeur
// et ce, à partir du point P.
//-----------
sudoku::point sudoku::search(point P)
{
	if (P.x==-1 && P.y==-1 )
	{		
		P.x=0;
		P.y=0;
	}
	for(int i=P.y;i<9;i++)
	{
		for(int j=P.x;j<9;j++)
		{
			if(oneLeft(pos[i][j]))
			{
				if ( checked[i][j] == false )
				{
					checked[i][j] = true;
					nbRestant--;
					P.y = i;
					P.x = j;
					return P;
				}
			}
		}
		P.x = 0;
	}
	point Erreur(-1,-1);
	return Erreur;
}

//------------------------
// Affiche la grille de sudoku avec le point P encadré
// P : Point à encadrer
//-------------------------
void sudoku::afficher(point P, bool tout, int x, int y)
{	
	//// Positionner le curseur
	COORD test;
		
	test.X = x; //1
	test.Y = y; //2

	SetConsoleCursorPosition( hConsole, test ); 

	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			// Si on dessine la grille du bas
			// on affiche la taille des listes
			if(y>20)
			{
				if(pos[i][j].L.taille()==1)
					cout << "  ";
				else
				{
					setcolor(7);
					cout << " " << pos[i][j].L.taille() << " ";
				}
			}
			else if ( pos[i][j].L.taille() == 1 && P.y ==i && P.x ==j)
			{
				setcolor(14);
				cout << "[" << *pos[i][j] << "]";
			}
			else if ( pos[i][j].L.taille() == 1 && connus[i][j]==true)
			{
				setcolor(12);
				cout << " " << *pos[i][j] << " ";
			}
			else if ( pos[i][j].L.taille() == 1 )
			{
				setcolor(10);
				cout << " " << *pos[i][j] << " ";
			}
			else
			{
				setcolor(7);
				cout << " _ ";
			}

			if(j==2||j==5)
				test.X +=3;

			test.X += 3;
			SetConsoleCursorPosition( hConsole, test ); 
		}
	test.X = 1;
	test.Y += 2;
	if(i==2||i==5)
		test.Y +=2;
	SetConsoleCursorPosition( hConsole, test ); 
	}

	test.X = 35;
	test.Y = 2;
	SetConsoleCursorPosition( hConsole, test ); 
}
//-------------
// Dessine la grille vide
//-------------
void sudoku::afficherGrille(int x, int y)
{	
	// Positionner le curseur
	COORD test;

	test.X = x;
	test.Y = y;

	SetConsoleCursorPosition( hConsole, test );

	cout << "\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCD\xCD\xCD\xCD"
		 << "\xCD\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	for(int i=0;i<7;i++)
		cout << "\xBA          \xBA           \xBA          \xBA\n";

	cout << "\xCC\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCD\xCD\xCD\xCD" 
		 << "\xCD\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xB9\n";

	for(int i=0;i<7;i++)
		cout << "\xBA          \xBA           \xBA          \xBA\n";

	cout << "\xCC\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCD\xCD\xCD\xCD"
		 << "\xCD\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xB9\n";

	for(int i=0;i<7;i++)
		cout << "\xBA          \xBA           \xBA          \xBA\n";

	cout << "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCD\xCD\xCD\xCD"
		 << "\xCD\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC";
}

//------------
//Cherche la colone et la ligne la plus remplie (apres 9=9)
//--------------
sudoku::point sudoku::bestPoint()
{
	int nbPts[9][9];
	int nbPtsLigne[9];
	int nbPtsColonne[9];

	//on met à 0
	for(int i=0;i<9;i++)// ligne
	{
		nbPtsLigne[i] = nbPtsColonne[i] = -9;
	}

	//on parcoure les lignes
	for(int i=0;i<9;i++)	// colonne 
		for(int j=0;j<9;j++)// ligne
			nbPtsColonne[i] += pos[j][i].L.taille();

	for(int j=0;j<9;j++)	// ligne 
		for(int i=0;i<9;i++)// colone
			nbPtsLigne[i] += pos[i][j].L.taille();


	for(int j=0;j<9;j++)	// ligne 
		for(int i=0;i<9;i++)// colone
		{
			nbPts[i][j] = nbPtsLigne[i] + nbPtsColonne[j];
		}

	for(int j=0;j<9;j++)	// ligne 
		for(int i=0;i<9;i++)// colone
		{
			if ( nbPtsLigne[i] == 0 || nbPtsColonne[j] ==0) 
				nbPts[i][j] = 99;
			else
				nbPts[i][j] = nbPtsLigne[i]+ nbPtsColonne[j]; 
		}

	point t(0,0);

	int pp=99;

	// RECHERCHER LE POINT LE PLUS PETIT
	for(int i=0;i<9;i++)	//y 
		for(int j=0;j<9;j++)//x 
		{
			if ( nbPts[i][j] < pp && pos[i][j].L.taille() > 1 )
			{
				t.y = i;
				t.x = j;
				pp = nbPts[i][j];
			}

		}

	return t;
}

//-----------
// Méthode qui permet de solutionner le sudoku
//------------
bool sudoku::solutionneur()
{
	sudoku s1,s2;
	point P(0,0);

	// Le jeu est existant en mémoire quand on est rendu ici
	//int i=0,j=0;
	//Répété jusqu'à la fin (nb cases à solutionner == 0)
	int OLD_nbRestant = 0;

	// On boucle tant que il reste des cases à solutionner
	while(nbRestant>0)
	{
		// si y'a un "debug" c'est que le sudoku a fait un
		// mauvais choix, on sort de cette branche de test
		if (debug == true)
			return false;

		// tant que le point trouvé est égale a -1,-1
		// on doit faire un choix puisque il n'y a plus
		// de cases à 1 seule possibilité
		do
		{
			// Recherche du prochain point qui est n'a
			// qu'une possibilité
			P = search(P);
			// Si aucun point n'est trouvé
			if(P.x == -1 && P.y == -1)
			{
				// Si un tour a été fait et 
				// qu'aucune case n'a changé
				if ( nbRestant == OLD_nbRestant )
				{
					// Si c'est la première tentative
					if ( debugVal == 0 )
					{
						// On recherche le meilleur point
						point P = Pdebug = bestPoint();
						debugVal2 = *pos[P.y][P.x].fin();
						debugVal = *pos[P.y][P.x].debut();
						// On copie le sudoku en cours
						s1.copie(*this);
					}
					
					// Si le point retourné est 0,0
					// le sudoku est invalide
					if(Pdebug.x==0 && Pdebug.y==0)
					{
						return false;
					}
					// Sinon on vide la case
					// et on prend une valeur sur les deux possibles
					else
					{
						s1.pos[Pdebug.y][Pdebug.x].L.vider();
						s1.pos[Pdebug.y][Pdebug.x].L.inserer(debugVal2, s1.pos[Pdebug.y][Pdebug.x]);
						s1.clean_all(Pdebug);
					}

					// On teste notre nouveau sudoku
					if(!s1.solutionneur()) // si a un problème
					{
						// On recopie le sudoku d'origine et on
						// insère l'autre valeur possible (celle non tester à date)
						s2.copie(*this);
						s2.pos[Pdebug.y][Pdebug.x].L.vider();
						s2.pos[Pdebug.y][Pdebug.x].L.inserer(debugVal, s2.pos[Pdebug.y][Pdebug.x]);
						s2.clean_all(Pdebug);

						// On lance le deuxìème test
						if(!s2.solutionneur())
						{
							// Si il échoue, tout a été tester, on sort
							return false;
						}
						else
						{
							// Si il réussi à terminer le sudoku
							// on recopie le sudoku gagnant
							copie(s2);
							return true;
						}
					}
					else
					{
						// Si le premier test termine le sudoku
						// on recopie le sudoku gagnant
						copie(s1);
						return true;
					}
				}
				else
				{
					// Remet la variable à jour
					OLD_nbRestant = nbRestant;
				}
			}
		}
		while ( P.x == -1 && P.y == -1 );

		// Enleve la valeur de la position P
		// dans toutes les cases adjacentes
		clean_all(P);
		// Affiche si désiré
		if ( display )
		{
			curseur(35,2);
			if(stepbystep)
				system("pause");
			afficher(P,false);
			afficher(P,false,1,27);
			curseur(35,1);
			cout << "Cases restantes : " << nbRestant << "  ";
		}

		P.x++;
	}
	// Le sudoku est terminé et réussi
	return true;
}

void sudoku::jouer(bool affichage, bool Pas_A_Pas)
{
	point P(-1,-1);
	stepbystep = Pas_A_Pas;
	display=affichage;
	afficherGrille(0,0);
	afficher(P);
	if( affichage )
	{
		afficherGrille(0,25);
		afficher(P,affichage,1,27);
	}
	system("pause");
	
	// Lance le solutionneur
	if (solutionneur())
	{
		afficher(P,false);
		curseur(35,1);
		// Vert
		setcolor(10);
		cout << "Reussi                   \n";
	}
	else
	{
		curseur(35,1);
		// Rouge
		setcolor(12);
		cout << "Impossible a resoudre...       ";
	}
	curseur(35,2);
}

// ------------------
// Détermine la couleur d'écriture
// ----------------
/*******************************************************************  

1   BLUE
2   GREEN
3   CYAN
4   RED
5   MAGENTA
6   BROWN
7   LIGHTGRAY
8   DARKGRAY
9   LIGHTBLUE
10  LIGHTGREEN
11  LIGHTCYAN
12  LIGHTRED
13  LIGHTMAGENTA
14  YELLOW
15  WHITE

***********************************************************************/
void sudoku::setcolor(unsigned int color) 
{                                    
    if (color >15 || color <=0)
    {
        cout <<"Error" <<endl;
    }
    else
    {
        HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hcon,color);
    }
}
// ------------------
// Déplace le curseur dans l'écran
// ----------------
void sudoku::curseur(int x, int y)
{
	COORD emp;

	emp.X = x;
	emp.Y = y;

	SetConsoleCursorPosition( hConsole, emp ); 
}

// ------------------
// Déplace le curseur dans l'écran
// ----------------
void sudoku::curseur(point P)
{
	curseur(P.x,P.y);
}

// Destructeur
sudoku::~sudoku()
{

}
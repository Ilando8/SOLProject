#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

#include <lfucache.h>

extern long memMax;
extern long numMax;

//	creazione di un nuovo nodo
nodo* newNode (int freq, char* nome, char* testo, int stato)
{
	nodo* new;
	if ((new = (nodo*) malloc(sizeof(nodo))) == NULL)	return NULL;	//	perror
	new->freq = freq;
	if ((new->nome = malloc(strlen(nome)* sizeof(char)))  == NULL)	return NULL;	//	perror
	strcpy(new->nome, nome);
	if ((new->testo = malloc(strlen(testo)* sizeof(char))) == NULL)	return NULL;	//	perror
	strcpy(new->testo, testo);
	new->stato = stato;
	new->left = NULL;
	new->right = NULL;

	return new;
}

//	aggiungo un valore nell'albero.
nodo* addTree(nodo* n, int freq, char* nome, char* testo, int stato)
{
	if (n == NULL)	return newNode(freq,nome,testo,stato);
	if (strlen(n->nome) >= strlen(nome))	n->left = addTree(n->left,freq,nome,testo,stato);
	else	n->right = addTree(n->right,freq,nome,testo,stato);
	return n;
}

//	Ricerco il minimo valore nell'intero albero e salvo l'informazione in min e name.
nodo* findTreeMin(nodo* n, int* min, char** name)
{	
	if (n == NULL)	return NULL;
	fprintf(stderr,"minimo attule:%d\n", *min);
	fprintf(stderr,"frequenza attuale:%d - nome attuale:%s\n", n->freq,n->nome);
	if (*min > (n->freq))	
	{
		fprintf(stderr,"frequenza:%d - nome:%s\n", n->freq, n->nome);
		*min = n->freq;
		fprintf(stderr,"nuovo min:%d\n", *min);
		strcpy(*name,n->nome); 
		fprintf(stderr,"nome del minimo:%s\n", *name);
	}

	findTreeMin(n->left,min,name);
	findTreeMin(n->right,min,name);

	return n;
}


//	Ricerco il nome nell'albero
nodo* findTreeFromName(nodo* n, char* str)
{
	nodo* tmp = NULL;
	if (n == NULL)	return NULL;
	if ((strcmp(n->nome, str)) == 0)	return n;

	else 
		if (strlen(str) <= strlen(n->nome))	tmp = findTreeFromName(n->left,str);
		if (strlen(str) > strlen(n->nome))	tmp = findTreeFromName(n->right,str);
	
	return tmp;
}

//	scambio i valori di 2 nodi
int swapTree (nodo* a, nodo* b) 
{
	if (a == NULL || b == NULL)	return -1;
	nodo* tmp = newNode(a->freq, a->nome, a->testo, a->stato);

	a->freq = b->freq;
	strcpy(a->nome, b->nome);
	strcpy(a->testo, b->testo);
	a->stato = b->stato;
	printf("inizio scambio pt2\n");
	b->freq = tmp->freq;
	//	qua non funziona nulla....
	printf("%s;%s\n", b->nome, tmp->nome);
	printf("%zu;%zu\n", strlen(b->nome), strlen(tmp->nome));
	if (realloc(b->nome, strlen(tmp->nome)) == NULL)	return -1;	//EMEMORY
	printf("%s;%s\n", b->nome, tmp->nome);
	printf("aaaaaaaaaaa\n");
	strcpy(b->nome, tmp->nome);
	if (realloc(b->testo, strlen(tmp->testo)) == NULL)	return -1;	//EMEMORY
	strcpy(b->testo, tmp->testo);
	b->stato = tmp->stato;
	printf("finito\n");
	free(tmp);
	return 0;
}	
 
//	creo un nodo temporaneo, controllo effetivamente se nel nodo parent non ci sono ulteriori foglie
//	se non ci sono foglie, scambio la foglia con il nodo temporaneo
nodo* isLeaf (nodo* parent)
{
	if (parent->left == NULL && parent->right == NULL)
	{
		nodo* tmp = newNode(parent->freq,parent->nome,parent->testo,parent->stato);
		printf("sono madre\n");
		return tmp;
	}
	return NULL;
}

 
//	ricerca verso una qualsisasi foglia
//	da notare che se trovo la foglia, restituisco un nodo distaccato dall'albero.
nodo* searchLeaf (nodo* n)
{
	if (n == NULL)
		return NULL;

	nodo* leaf = NULL;
	if (n->left != NULL && n->right != NULL)
	{
		if ((leaf = isLeaf(n->left)) != NULL)
		{
			n->left = NULL;
			return leaf;
		}
		printf("foglia destra\n");
		if ((leaf = isLeaf(n->right)) != NULL)
		{
			n->right = NULL;
			printf("faccio la return\n");
			return leaf;
		}
		printf("ricorsiva\n");
		leaf = searchLeaf(n->left);
	}
	printf("guardo se il nodo sinistro e' nullo\n");
	if (n->left == NULL && n->right != NULL)
	{
		if ((leaf = isLeaf(n->right)) != NULL)
		{
			n->right = NULL;
			return leaf;
		}
		leaf = searchLeaf(n->right);
	}
	printf("guardo il nodo destro se e' nullo\n");
	if (n->left != NULL && n->right == NULL)
	{
		if ((leaf = isLeaf(n->left)) != NULL)
		{
			n->left = NULL;
			return leaf;
		}
		leaf = searchLeaf(n->left);
	}
	return leaf;
}

//	cerca il valore minimo nell'albero, un'altra ricerca per trovare il nome del nodo
//	cosi restituisce il nodo effettevivo, infine si cerca una foglia qualsisi e con le considerazioni della funzione searchLeaf
//	si scambia con il nodo con la frequenza minima trovata con la foglia e si cancella la foglia 
int lfuRemove(nodo* n)
{
	if (n == NULL)	return -3;
	if (n->left == NULL && n->right == NULL)	return -1;
	int min;
	char* name = malloc(MAXS*sizeof(char));

	if (n->left == NULL){	min = n->right->freq;	name = n->right->nome; }
	else { min = n->left->freq;	name = n->left->nome; }
	
	nodo* find = NULL;
	nodo* leaf = NULL;
	
	if (!findTreeMin(n,&min,&name))	return -3;
	printf("Minima frequenza: %d\n", min);
	printf("nome utente: %s\n", name);

	if (!(find = findTreeFromName(n,name)))	return -3;
	printf("convertito in nodo, nome: %s\n", find->nome);

	if (!(leaf = searchLeaf(n)))	return -3;
	printf("foglia:%s\n", leaf->nome);

	if (swapTree(leaf, find) != 0)	return -1;
	printf("find:%s <-> leaf:%s\n", find->nome, leaf->nome);
	free(leaf)
	return 0;
}

//	se viene fatta una operazione di richiesta del cient, aumento la frequenza  
int addFrequenza(int fre)
{
	return fre++;
}

void print (nodo* n){

  if(n == NULL) return;
  printf("stampa elemento:\n");
  
  printf("Frequenza: %d\n", n->freq);
  printf("Nome: %s\n", n->nome);
  printf("Testo: %s\n", n->testo);
  printf("Stato: %d\n", n->stato);
  printf("\n");
  print(n->left);
  print(n->right);
}

int openFile(nodo* root, char* name)
{
	if (numMax == 0)	lfuRemove(root);
	if (strcmp(name, "pRoot") == 0)	return -1;
	if (findTreeFromName(root,name) == NULL){	addTree(root, 0, name, "", 0);	numMax--;}
	else	return -1;
	return 0;
}

int readFile(nodo* root, char* name)
{
	if (strcmp(name, "pRoot") == 0)	return -1;
	if (findTreeFromName(root,name) == NULL)	return -3;
	else if (root->stato != 0)	return -2;
	else addFrequenza(root->freq);	
	return 0;
}

int appendToFile(nodo* root, char* name, char* text)
{
	printf("sto eseguendo la append\n");
	nodo* find = NULL;
	if ((find = findTreeFromName(root,name)) == NULL)	return -3;
	else if (find->stato != 0)	return -2;
	else
	{
		int somma = strlen(find->testo) + strlen(text);
		find->testo = realloc(find->testo, somma+1);
		strncat(find->testo, text, somma);
		memMax = memMax - strlen(find->testo);
		if (memMax <= 0)	{printf("sto cancellando....\n"); lfuRemove(find);}
		addFrequenza(find->freq);
	} 
	return 0;
}

int writeFile(nodo* root, char* name, char* text) 
{
	if ((findTreeFromName(root,name)) == NULL) {	openFile(root,name);	return appendToFile(root,name,text); }
	else if (root->stato != 0)	return -2;
	else	return appendToFile(root,name,text);
}

//	cerca il nome nel nodo poi si cerca una foglia qualsisi e con le considerazioni della funzione searchLeaf
//	si scambia con la foglia e cancello la foglia 
int fileRemove(nodo* root, char* nome)
{
	if (root == NULL)	return -1;

	if (strcmp(nome, "pRoot") == 0)	return -1;

	nodo* find = NULL;
	nodo* leaf = NULL;

	if (!(find = findTreeFromName(root,nome)))	return -3;
	if (find->stato != 0)	return -2;

	if (!(leaf = searchLeaf(root)))	return -3;

	if (swapTree(leaf, find) != 0)	return -1;

	free(leaf);
	addFrequenza(find->freq);
	return 0;
}

//	cambia lo stato del nodo
int changeStatus(nodo* root, char* name, int lb)
{
	nodo* find = NULL;
	if (strcmp(name, "pRoot") == 0)	return -1;
	if ((find = findTreeFromName(root,name)) == NULL)	return -3;
	else
		if (lb == find->stato)	return -1;
		if (find->stato == 0)	find->stato = 1;
		else	if (find->stato == 1)	find->stato = 0;

	addFrequenza(find->freq);
	return 0;
}
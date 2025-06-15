## Segregated Free Lists

### Descriere

#### Structuri de date folosite
In implementarea temei am folosit urmatoarele structuri de date:

##### Block
```c
typedef struct {
	void *start;
	uint size;
	char *data;
} block_t;
```
Un bloc contine o:
* o adresa de inceput
* o dimensiune
* date

Blocurile pot fi nealocate sau alocate, insa structura lor este aceeasi cu diferenta ca blocurile nealocate nu contin nimic in "data".

##### Lista dublu inlantuita
```c
typedef struct node {
	void *data;
	struct node *next;
	struct node *prev;
} node_t;
```

Lista dublu inlantuita este formata din mai multe noduri.
Fiecare bloc contine cate o referinta la nodul urmator si la nodul precedent si date.
Primul nod din lista este intotdeauna o santinela care are data=NULL.
Lista este generica, iar datele pot avea orice forma.

##### Heap
```c
typedef struct {
	node_t *blocks;
	uint block_size;
} heaplist_t;

typedef struct {
	node_t *lists;
	uint list_count;
	uint size;
	char type;
	node_t *used_blocks;
	uint malloc_count;
	uint fragmentation_count;
	uint free_count;
} heap_t;
```
Blocurile nealocate sunt stocate in "lists" - o lista de liste dublu inlantuite sortate dupa dimensiunea blocurilor.
Blocurile alocate sunt stocate in "used_blocks" - lista dublu inlantuita.

#### Comenzi

##### Initializarea heap-ului
Se aloca memorie pentru structura "heap_t" si se creeaza "list_count" liste de blocuri.
Fiecare lista de blocuri se populeaza cu blocuri de dimensiuni egale sortate dupa adresa de start.

##### Alocarea memoriei (malloc)
Se cauta cel mai mic bloc nealocat care are o dimensiune mai mare decat cea ceruta si se scoate din heap.
Se creaza un nou bloc cu adresa de start egala cu cea a blocului gasit si cu dimensiunea egala cu cea ceruta si se insereaza in lista "used_blocks" (sortata in functie de adresa de start).

Daca mai ramane memorie nealocata in blocul gasit, se creaza un nou bloc mai mic si se insereaza inapoi in heap (intr-o lista existenta sau intr-una noua).

##### Eliberarea memoriei (free)
Se cauta blocul in lista "used_blocks", iar daca se gaseste acesta este eliminat si reintrodus in heap.

##### Scrierea in memorie (write)
Se cauta blocul in lista "used_blocks" dupa adresa de start. Daca nu exista se intoarce eroare.
Se calculeaza dimensiunea zonei de memorie continue alocata si se verifica daca datele incap in ea, altfel se intoarce eroare.
Se scriu datele in memorie. In functie de dimensiunea datelor se pot utiliza mai multe blocuri de memorie continue.

##### Citirea din memorie (read)
La fel ca la scrierea in memorie, doar ca in loc de scriere se citesc datele din blocurile alocate.

##### Afisarea memoriei
Se afiseaza datele din heap in formatul cerut.

##### Distrugerea heap-ului
Se elibereaza pe rand listele de blocuri si heap-ul, apoi programul se opreste.

### Comentarii
In implementarea acestei teme am gasit insteresant faptul ca folosind structuri de date generice, acestea se pot folosi in mai multe locuri fara a avea cod duplicat care sa difere doar prin tipul de date.
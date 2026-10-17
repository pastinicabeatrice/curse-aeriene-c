#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned int id;
	char* destinatie;
	char* dataPlecare;
	unsigned short int nrMinIntarziere;
	unsigned short int nrPasageriInregistrati;
} CursaAeriana;

typedef struct Nod {
	CursaAeriana inf;
	struct Nod* next;
	struct Nod* prev;
} Nod;

char* copiereSir(const char* s) {
	char* r = (char*)malloc(strlen(s) + 1);
	strcpy(r, s);
	return r;
}

Nod* inserare(Nod* cap, Nod** coada, CursaAeriana cursa) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->inf = cursa;
	nou->inf.destinatie = copiereSir(cursa.destinatie);
	nou->inf.dataPlecare = copiereSir(cursa.dataPlecare);
	nou->next = NULL;
	nou->prev = *coada;
	if (cap == NULL) {
		cap = nou;
	}
	else {
		(*coada)->next = nou;
	}
	*coada = nou;
	return cap;
}

void afisareCursa(CursaAeriana c) {
	printf("Cursa %u | %s | plecare %s | intarziere %hu min | %hu pasageri\n",
		c.id, c.destinatie, c.dataPlecare, c.nrMinIntarziere, c.nrPasageriInregistrati);
}

void traversare(Nod* cap) {
	for (Nod* t = cap; t != NULL; t = t->next) {
		afisareCursa(t->inf);
	}
}

void traversareInversa(Nod* coada) {
	for (Nod* t = coada; t != NULL; t = t->prev) {
		afisareCursa(t->inf);
	}
}

float intarziereMedie(Nod* cap) {
	int numar = 0;
	float suma = 0;
	for (Nod* t = cap; t != NULL; t = t->next) {
		suma += t->inf.nrMinIntarziere;
		numar++;
	}
	return numar > 0 ? suma / numar : 0;
}

void afisareIntarziate(Nod* cap, unsigned short int prag) {
	for (Nod* t = cap; t != NULL; t = t->next) {
		if (t->inf.nrMinIntarziere > prag) {
			afisareCursa(t->inf);
		}
	}
}

unsigned int totalPasageri(Nod* cap) {
	unsigned int total = 0;
	for (Nod* t = cap; t != NULL; t = t->next) {
		total += t->inf.nrPasageriInregistrati;
	}
	return total;
}

void dezalocare(Nod** cap, Nod** coada) {
	Nod* t = *cap;
	while (t != NULL) {
		Nod* urm = t->next;
		free(t->inf.destinatie);
		free(t->inf.dataPlecare);
		free(t);
		t = urm;
	}
	*cap = NULL;
	*coada = NULL;
}

int main() {
	FILE* f = fopen("Fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul Fisier.txt nu a putut fi deschis.\n");
		return 1;
	}

	Nod* cap = NULL;
	Nod* coada = NULL;
	CursaAeriana cursa;
	char destinatie[80];
	char data[11];

	while (fscanf(f, "%u %79s %10s %hu %hu", &cursa.id, destinatie, data,
		&cursa.nrMinIntarziere, &cursa.nrPasageriInregistrati) == 5) {
		cursa.destinatie = destinatie;
		cursa.dataPlecare = data;
		cap = inserare(cap, &coada, cursa);
	}
	fclose(f);

	printf("Curse aeriene:\n");
	traversare(cap);

	printf("\nCurse in ordine inversa:\n");
	traversareInversa(coada);

	printf("\nIntarziere medie: %.2f minute\n", intarziereMedie(cap));
	printf("Total pasageri inregistrati: %u\n", totalPasageri(cap));

	printf("\nCurse cu intarziere peste 10 minute:\n");
	afisareIntarziate(cap, 10);

	dezalocare(&cap, &coada);
	return 0;
}

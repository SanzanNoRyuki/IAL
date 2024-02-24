/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	for (int i = 0; i < HTSIZE; i++) (*ptrht)[i] = NULL;													// Nastavenie vsetkych prvkov v tabulke na NULL
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	int i = hashCode(key);																												// Najdenie indexu(hashu) prvku v poli zoznamov synonym
	tHTItem* item = (*ptrht)[i];																									// Deklaracia pomocnej premennej

	while (item != NULL) {																												// Vyhladanie polozky s danym klucom v zozname synonym
		if (strcmp(key,item->key) == 0) return item;																// Prvok bol najdeny
		item = item->ptrnext;																												// Posunutie na nasledujuci prvok
	}
	return NULL;																																	// Prvok nebol najdeny
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	tHTItem* item = htSearch(ptrht,key);																					// Deklaracia pomocnej premennej

	if (item != NULL) {																														// Zmena obsahu existujuceho prvku
		item->data = data;
		return;
	}

	item = (tHTItem*) malloc(sizeof(struct tHTItem));															// Alokacia noveho prvku
	if (item == NULL) return;																											// Kontrola alokacie

	item->key = key;
	item->data = data;

	int i = hashCode(key);																												// Najdenie indexu(hashu) prvku
	item->ptrnext = (*ptrht)[i];
	(*ptrht)[i] = item;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	tHTItem* item = htSearch(ptrht,key);																					// Deklaracia pomocnej premennej

	if (item != NULL) return &(item->data);																				// Prvok bol najdeny
	return NULL;																																	// Prvok nebol najdeny
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	int i = hashCode(key);																												// Najdenie indexu(hashu) prvku
	tHTItem* item = (*ptrht)[i];																									// Deklaracia pomocnej premennej
	if (item == NULL) return;																											// Kontrola prazdneho zoznamu
	tHTItem* next = item->ptrnext;																								// Deklaracia pomocnej premennej - nasledujuci prvok

	if (strcmp(key,item->key) == 0) {																							// Prvy prvok je odstranovany
		free(item);																																	// Uvolnenie prvku
		(*ptrht)[i] = next;																													// Nastavenie noveho prveho prvku
		return;
	}

	while (next != NULL) {																												// Vyhladanie polozky s danym klucom v zozname synonym
		if (strcmp(key,next->key) == 0) {																						// Prvok bol najdeny
			item->ptrnext = next->ptrnext;																						// Posunutie ukazatelov
			free(next);																																// Uvolnenie prvku
			return;
		}
		item = item->ptrnext;																												// Posunutie na nasledujuci prvok
		next = item->ptrnext;																												// Prvok po nasledujucom prvku
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	for (int i = 0; i < HTSIZE; i++) {																						// Prechod celou tabulkou
		tHTItem* item = (*ptrht)[i];																								// Deklaracia pomocnej premennej
		tHTItem* next;																															// Deklaracia pomocnej premennej - nasledujuci prvok

		while (item != NULL) {
			next = item->ptrnext;																											// Nasledujuci prvok
			free(item);																																// Uvolnenie prvku
			item = next;																															// Posunutie na nasledujuci prvok
		}
		(*ptrht)[i] = NULL;																													// Nastavenie vsetkych prvkov v tabulke na NULL
	}
}

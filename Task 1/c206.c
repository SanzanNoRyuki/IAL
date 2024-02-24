
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
  L->First = NULL;                                                              // Prazdny zoznam nema prvy prvok
  L->Act = NULL;                                                                // Prazdny zoznam nema aktivny prvok
  L->Last = NULL;                                                               // Prazdny zoznam nema posledny prvok
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
  tDLElemPtr tmp = L->First;                                                    // Deklaracia pomocnej premennej

  while (tmp != L->Last) {                                                      // Uvolnenie prvkov okrem posledneho
    tmp = tmp->rptr;                                                            // Posunutie na nasledujci prvok
    free(tmp->lptr);                                                            // Uvolnenie predosleho prvku
  }
  free(tmp);                                                                    // Uvolnenie posledneho prvku

  L->First = NULL;                                                              // Prazdny zoznam nema prvy prvok
  L->Act = NULL;                                                                // Prazdny zoznam nema aktivny prvok
  L->Last = NULL;                                                               // Prazdny zoznam nema posledny prvok
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  tDLElemPtr newElemPtr = (tDLElemPtr) malloc(sizeof(struct tDLElem));          // Deklaracia a alokacia noveho prvku
  if (newElemPtr == NULL) {                                                     // Kontrola uspesnej alokacie
    DLError();                                                                  // Ohlasenie chyby
    return;                                                                     // Ukoncenie funkcie
  }

  newElemPtr->data = val;                                                       // Vlozenie dat do noveho prvku
  newElemPtr->lptr = NULL;                                                      // Nastavenie ukazatela na predosly prvok
  newElemPtr->rptr = L->First;                                                  // Nastavenie ukazatela na nasledujci prvok

  if (L->First != NULL) L->First->lptr = newElemPtr;                            // Nastavenie ukazatela nasledujceho prvku
  else L->Last = newElemPtr;                                                    // ak novy prvok nie je jediny, inak
                                                                                // nastavenie ukazatela na posledny prvok
  L->First = newElemPtr;                                                        // Nastavenie ukazatela na prvy prvok
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
  tDLElemPtr newElemPtr = (tDLElemPtr) malloc(sizeof(struct tDLElem));          // Deklaracia a alokacia noveho prvku
  if (newElemPtr == NULL) {                                                     // Kontrola uspesnej alokacie
    DLError();                                                                  // Ohlasenie chyby
    return;                                                                     // Ukoncenie funkcie
  }

  newElemPtr->data = val;                                                       // Vlozenie dat do noveho prvku
  newElemPtr->lptr = L->Last;                                                   // Nastavenie ukazatela na predosly prvok
  newElemPtr->rptr = NULL;                                                      // Nastavenie ukazatela na nasledujci prvok

  if (L->Last != NULL) L->Last->rptr = newElemPtr;                              // Nastavenie ukazatela predosleho prvku
  else L->First = newElemPtr;                                                   // ak novy prvok nie je jediny, inak
                                                                                // nastavenie ukazatela na prvy prvok
  L->Last = newElemPtr;                                                         // Nastavenie ukazatela na posledny prvok
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
  L->Act = L->First;                                                            // Nastavenie aktivity na prvy prvok
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
  L->Act = L->Last;                                                             // Nastavenie aktivity na posledny prvok
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
  if(L->First != NULL) *val = L->First->data;                                   // Vratenie hodnoty prveho prvku
  else DLError();                                                               // Ohlasenie chyby
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
  if(L->Last != NULL) *val = L->Last->data;                                     // Vratenie hodnoty posledneho prvku
  else DLError();                                                               // Ohlasenie chyby
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
  if (L->First != NULL) {                                                       // Kontrola prazdnoty zoznamu
    if (L->Act == L->First) L->Act = NULL;                                      // Kontrola aktivity prvku

    if (L->Last == L->First) {                                                  // Jednoprvkovy zoznam
      free(L->First);                                                           // Uvolnenie jedineho prvku
      L->First = NULL;                                                          // Odstranenie ukazatela na prvy prvok
      L->Last = NULL;                                                           // Odstranenie ukazatela na posledny prvok
    }
    else {                                                                      // Viacprvkovy zoznam
      L->First = L->First->rptr;                                                // Nastavenie noveho prveho prvku
      free(L->First->lptr);                                                     // Uvolnenie predosleho prvku
      L->First->lptr = NULL;                                                    // Odstranenie ukazatela na prvy prvok
    }
  }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
  if(L->Last != NULL) {                                                         // Kontrola prazdnoty zoznamu
    if (L->Act == L->Last) L->Act = NULL;                                       // Kontrola aktivity prvku

    if (L->First == L->Last) {                                                  // Jednoprvkovy zoznam
      free(L->Last);                                                            // Uvolnenie jedineho prvku
      L->First = NULL;                                                          // Odstranenie ukazatela na prvy prvok
      L->Last = NULL;                                                           // Odstranenie ukazatela na posledny prvok
    }
    else {                                                                      // Viacprvkovy zoznam
      L->Last = L->Last->lptr;                                                  // Nastavenie noveho posledneho prvku
      free(L->Last->rptr);                                                      // Uvolnenie nasledujuceho prvku
      L->Last->rptr = NULL;                                                     // Odstranenie ukazatela na posledny prvok
    }
  }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
  if (L->Act != NULL && L->Act != L->Last) {                                    // Kontrola vstupneho parametru
    tDLElemPtr delElemPtr = L->Act->rptr;                                       // Deklaracia odstranovaneho prvku

    L->Act->rptr = delElemPtr->rptr;                                            // Odstranenie ukazatela z predosleho prvku

    if (delElemPtr == L->Last) L->Last = L->Act;                                // Nastavenie ukazatela posledneho prvku
    else delElemPtr->rptr->lptr = L->Act;                                       // Odstranenie ukazatela z nasledujuceho prvku

    free(delElemPtr);                                                           // Uvolnenie odstranovaneho prvku
  }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
  if (L->Act != NULL && L->Act != L->First) {                                   // Kontrola vstupneho parametru
    tDLElemPtr delElemPtr = L->Act->lptr;                                       // Deklaracia odstranovaneho prvku

    L->Act->lptr = delElemPtr->lptr;                                            // Odstranenie ukazatela z nasledujceho prvku

    if (delElemPtr == L->First) L->First = L->Act;                              // Nastavenie ukazatela prveho prvku
    else delElemPtr->lptr->rptr = L->Act;                                       // Odstranenie ukazatela z predosleho prvku

    free(delElemPtr);                                                           // Uvolnenie odstranovaneho prvku
  }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if (L->Act != NULL) {
    tDLElemPtr newElemPtr = (tDLElemPtr) malloc(sizeof(struct tDLElem));        // Deklaracia a alokacia noveho prvku
    if (newElemPtr == NULL) {                                                   // Kontrola uspesnej alokacie
      DLError();                                                                // Ohlasenie chyby
      return;                                                                   // Ukoncenie funkcie
    }

    newElemPtr->data = val;                                                     // Vlozenie dat do noveho prvku
    newElemPtr->lptr = L->Act;                                                  // Nastavenie ukazatela na predosly prvok
    newElemPtr->rptr = L->Act->rptr;                                            // Nastavenie ukazatela na nasledujci prvok

    L->Act->rptr = newElemPtr;                                                  // Nastavenie ukazatela predosleho prvku
    if (L->Act == L->Last) L->Last = newElemPtr;                                // Nastavenie ukazatela na posledny prvok
    else newElemPtr->rptr->lptr = newElemPtr;                                   // Nastavenie ukazatela nasledujceho prvku
  }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if (L->Act != NULL) {
    tDLElemPtr newElemPtr = (tDLElemPtr) malloc(sizeof(struct tDLElem));        // Deklaracia a alokacia noveho prvku
    if (newElemPtr == NULL) {                                                   // Kontrola uspesnej alokacie
      DLError();                                                                // Ohlasenie chyby
      return;                                                                   // Ukoncenie funkcie
    }

    newElemPtr->data = val;                                                     // Vlozenie dat do noveho prvku
    newElemPtr->lptr = L->Act->lptr;                                            // Nastavenie ukazatela na predosly prvok
    newElemPtr->rptr = L->Act;                                                  // Nastavenie ukazatela na nasledujci prvok

    L->Act->lptr = newElemPtr;                                                  // Nastavenie ukazatela nasledujceho prvku
    if (L->Act == L->First) L->First = newElemPtr;                              // Nastavenie ukazatela na prvy prvok
    else newElemPtr->lptr->rptr = newElemPtr;                                   // Nastavenie ukazatela predosleho prvku
  }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
  if (L->Act != NULL) *val = L->Act->data;                                      // Vratenie dat aktivneho prvku
  else DLError ();
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
  if (L->Act != NULL) L->Act->data = val;                                       // Aktualizacia dat aktivneho prvku
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
  if (L->Act != NULL) L->Act = L->Act->rptr;                                    // Posunutie aktivity na nasledujci prvok
}

void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
  if (L->Act != NULL) L->Act = L->Act->lptr;                                    // Posunutie aktivity na predosly prvok
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
  return L->Act != NULL;                                                        // Neaktivny zoznam vracia 0, aktivny 1
}

/* Konec c206.c*/

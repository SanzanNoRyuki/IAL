
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
  char tmp;                                                                     // Deklaracia pomocnej premennej

  while (!stackEmpty(s)) {                                                      // Iterovanie do vyprazdnenia zasobniku
    stackTop(s,&tmp);                                                           // Citanie prvku z vrcholu do pomocnej premennej
    stackPop(s);                                                                // Odstranenie prvku na vrchole zasobniku

    if (tmp == '(') break;                                                      // Prerusenie cyklu pri detekcii '('

    postExpr[*postLen] = tmp;                                                   // Zapis do vystupneho pola
    (*postLen)++;                                                               // Zvysenie indexu prvkov
  }
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
  if (stackEmpty(s)) {                                                          // 1. Zasobnik je prazdny
    stackPush(s,c);                                                             // Vlozenie operatora na vrchol zasobniku
    return;                                                                     // Ukoncenie funkcie
  }

  char tmp;                                                                     // Deklaracia pomocnej premennej
  stackTop(s,&tmp);                                                             // Citanie vrcholu zasobniku do pomocnej premennej 

  if (tmp == '(') {                                                             // 2. Prvok na vrchole zasobniku je '('
    stackPush(s,c);                                                             // Vlozenie operatora na vrchol zasobniku
    return;                                                                     // Ukoncenie funkcie
  }

  switch (c) {
    case '*': 
    case '/':
              if (tmp == '+' || tmp == '-') {                                   // 3. Na vrchole zasobniku je operator s nizsou prioritou
                stackPush(s,c);                                                 // Vlozenie operatora na vrchol zasobniku
                return;                                                         // Ukoncenie funkcie
              }
    case '+':                                                                   // 4. Na vrchole zasobniku je operator s vyssou alebo
    case '-':                                                                   // rovnakou prioritou
              stackPop(s);                                                      // Odstranenie operatora na vrchole zasobniku
              postExpr[*postLen] = tmp;                                         // Vlozenie odstraneneho operatora na koniec vystupneho
              (*postLen)++;                                                     // pola a zvysenie indexu prvkov
              doOperation(s,c,postExpr,postLen);                                // Rekurzia funkcie
  }
}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {
  char* postExpr = (char*) malloc(MAX_LEN * sizeof(char));                      // Deklaracia a alokacia vystupneho retazca
  if (postExpr == NULL) return NULL;                                            // Kontrola uspesnej alokacie a pripadne ukoncenie

  tStack* s = (tStack*) malloc(sizeof(tStack));                                 // Deklaracia a alokacia zasobniku
  if (s == NULL) {                                                              // Kontrola uspesnej alokacie
    free(postExpr);                                                             // Uvolnenie alokovaneho miesta v pripade chyby
    return NULL;                                                                // Ukoncenie funkcie
  }
  stackInit(s);                                                                 // Inicializacia zasobniku

  unsigned i = 0, postLen = 0;                                                  // Deklaracia indexu prvkov vystupneho vyrazu a indexu
                                                                                // pre cyklus
  while (infExpr[i] != '=') {                                                   // Spracovanie retazca po '='
    switch (infExpr[i]) {
      case '(':                                                                 // Spracovanie '('
                stackPush(s,infExpr[i]);
                i++;
                break;
      case ')':                                                                 // Spracovanie ')'
                untilLeftPar(s,postExpr,&postLen);
                i++;
                break;
      case '+':
      case '-':
      case '*':
      case '/':
                doOperation(s,infExpr[i],postExpr,&postLen);                    // Spracovanie operatora
                i++;
                break;
      default:  postExpr[postLen] = infExpr[i];                                 // Spracovanie operandu
                postLen++;
                i++;
                break;
    }
  }

  while (!stackEmpty(s)) {                                                      // Spracovanie zasobniku
    stackTop(s,&postExpr[postLen]);
    postLen++;
    stackPop(s);
  }

  postExpr[postLen] = '=';                                                      // Pridanie '='
  postLen++;
  postExpr[postLen] = '\0';                                                     // Pridanie '\0'
  
  free(s);                                                                      // Uvolnenie zasobniku
  return postExpr;
}

/* Konec c204.c */

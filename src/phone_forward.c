/**
 * @file phone_forward.c
 * Klasa przechowująca przekierowania numerów telefonicznych
 * @author Paweł Dec <pd438403@students.mimuw.edu.pl>
 * @date 2022-06-18
* 
 */
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "phone_forward.h"
#include "slowo.h"

/**
 * To jest struktura przechowująca ciąg przekierowań.
 */
struct PhoneForward
{
  Slowo *przekierowanie;            ///< wskaźnik na przekierowanie.
  struct PhoneForward *synowie[12]; ///< wskaźnik na poddrzewa.
};

/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers
{
  Slowo **slowo;  ///< tablica słow.
  size_t rozmiar; ///< rozmiar.
};

/**
 * @brief Wyznacza przekierowanie numeru.
 *
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @param currAns - wskaźnik na aktualnie największą strukturę przechowującą przekierowania
 *  numerów;
 * @param currNum - wskaźnik na aktualnie największy napis reprezentujący numer.
 * @return Slowo* - wskaźnik na nowe Slowo.
 */
static Slowo *phfwdGetRecursive(PhoneForward const *pf, char const *num,
                                PhoneForward const *currAns, char const *currNum)
{
  if (num[0] == '\0')
  {
    if (pf->przekierowanie->dlugosc == 0)
      return slowoAdd(currAns->przekierowanie, currNum);
    else
      return slowoAdd(pf->przekierowanie, num);
  }
  int number = podajWartosc(num[0]);
  if (pf->synowie[number] == NULL)
  {
    if (pf->przekierowanie->dlugosc == 0)
      return slowoAdd(currAns->przekierowanie, currNum);
    else
      return slowoAdd(pf->przekierowanie, num);
  }
  if (pf->przekierowanie->dlugosc == 0)
    return phfwdGetRecursive(pf->synowie[number], &num[1], currAns, currNum);
  else
    return phfwdGetRecursive(pf->synowie[number], &num[1], pf, num);
}

PhoneForward *phfwdNew(void)
{
  PhoneForward *ret = calloc(1, sizeof(PhoneForward));
  if (ret == NULL)
    return NULL;
  ret->przekierowanie = slowoNew();
  for (int i = 0; i < 12; i++)
    ret->synowie[i] = NULL;
  return ret;
}

void phnumDelete(PhoneNumbers *pnum)
{
  if (pnum == NULL)
    return;
  for (size_t i = 0; i < pnum->rozmiar; i++)
    slowoDelete(pnum->slowo[i]);
  free(pnum->slowo);
  free(pnum);
  pnum = NULL;
}

void phfwdDelete(PhoneForward *pf)
{
  if (pf == NULL)
    return;
  for (int i = 0; i < 12; i++)
    phfwdDelete(pf->synowie[i]);
  slowoDelete(pf->przekierowanie);
  free(pf);
  pf = NULL;
}

/**
 * @brief Dodaje rekurencyjnie przekierowanie do struktury.
 *
 * @param pf - wskaźnik na strukturę.
 * @param num1 - numer do jakiego musimy dojść.
 * @param word - przekierowanie.
 * @return true - jeśli udało się stworzyć nowe przekierowanie.
 * @return false - w przeciwnym wypadku.
 */
static bool phfwdAddRecursive(PhoneForward *pf, char const *num1, Slowo *word)
{
  if (num1[0] == '\0')
  {
    slowoDelete(pf->przekierowanie);
    pf->przekierowanie = word;
    return true;
  }

  int number = podajWartosc(num1[0]);
  if (pf->synowie[number] == NULL)
    pf->synowie[number] = phfwdNew();
  return (phfwdAddRecursive(pf->synowie[number], &num1[1], word));
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2)
{
  if (pf == NULL)
    return false;
  if (sprawdzPoprawnosc(num1, num2))
    return phfwdAddRecursive(pf, num1, slowoCreate(num2));
  else
    return false;
}

/**
 * @brief Funkcja pomocznicza - działa identycznie jak phfwdRemove().
 * z różnicą, że nie sprawdza poprawności danych, bo już zostały sprawdzone.
 * @param pf - wskaźnik na strukturę.
 * @param num - numer do usunięcia.
 */
static void phfwdRemoveSprawdzone(PhoneForward *pf, char const *num)
{
  if (num[0] == '\0')
  {
    phfwdDelete(pf);
    return;
  }
  int number = podajWartosc(num[0]);
  if (pf->synowie[number] != NULL)
  {
    phfwdRemoveSprawdzone(pf->synowie[number], &num[1]);
    if (num[1] == '\0')
      pf->synowie[number] = NULL;
  }
}

void phfwdRemove(PhoneForward *pf, char const *num)
{
  if (num == NULL || !czyPoprawny(num, false) || pf == NULL)
    return;
  if (num[0] == '\0')
  {
    phfwdDelete(pf);
    return;
  }
  int number = podajWartosc(num[0]);
  if (pf->synowie[number] != NULL)
  {
    phfwdRemoveSprawdzone(pf->synowie[number], &num[1]);
    if (num[1] == '\0')
      pf->synowie[number] = NULL;
  }
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num)
{
  if (pf == NULL)
    return NULL;
  PhoneNumbers *ret = calloc(1, sizeof(PhoneNumbers));
  if(ret == NULL)
    return NULL;
  if (!czyPoprawny(num, false))
    return ret;
  ret->rozmiar = 1;
  Slowo *word = phfwdGetRecursive(pf, num, pf, num);
  Slowo **tablica = calloc(1, sizeof(Slowo *));
  if(tablica == NULL)
    return NULL;
  tablica[0] = word;
  ret->slowo = tablica;

  return ret;
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx)
{
  if (pnum == NULL || idx > (pnum->rozmiar - 1) || pnum->rozmiar == 0 || pnum->slowo == NULL)
    return NULL;
  if (pnum->slowo[idx]->slowo == NULL)
    return NULL;
  else
    return pnum->slowo[idx]->slowo;
}

/**
 * @brief Dodaje do struktury nowe słowo.
 * Powiększa PhoneNumbers, oraz dodaje nowe słowo.
 * @param numbers - wskaźnik na strukturę.
 * @param doDodania - wskaźnik na nowe słowo.
 */
static void powieksz(PhoneNumbers *numbers, Slowo *doDodania)
{
  numbers->rozmiar++;
  numbers->slowo = realloc(numbers->slowo, (numbers->rozmiar) * sizeof(Slowo));
  numbers->slowo[numbers->rozmiar - 1] = doDodania;
}

/**
 * @brief Funkcja porównująca dwa slowa.
 * Służy do sortowania w qsort().
 * @param s1 - wskaźnik na słowo.
 * @param s2 - wskaźnik na słowo.
 * @return int - ujemny jeśli pierwszy mniejszy, 0 gdy równe, dodatni gdy pierwszy większy.
 */
static int cmpfun(const void *s1, const void *s2)
{
  if (s1 == NULL || s2 == NULL)
  {
    if (s1 == NULL && s2 == NULL)
      return 0;
    else if (s1 == NULL)
      return -1;
    else
      return 1;
  }
  Slowo *slowoA = *(Slowo **)s1;
  Slowo *slowoB = *(Slowo **)s2;
  if (slowoA == NULL || slowoB == NULL)
  {
    if (slowoA == NULL && slowoB == NULL)
      return 0;
    else if (slowoA == NULL)
      return 1;
    else
      return -1;
  }
  int rozmiar = slowoA->dlugosc;
  if (rozmiar > slowoB->dlugosc)
    rozmiar = slowoB->dlugosc;
  for (int i = 0; i < rozmiar; i++)
  {
    if (podajWartosc((slowoA->slowo)[i]) != podajWartosc((slowoB->slowo)[i]))
      return podajWartosc((slowoA->slowo)[i]) - podajWartosc((slowoB->slowo)[i]);
  }
  return (slowoA->dlugosc - slowoB->dlugosc);
}

/**
 * @brief Sortuje PhoneNumbers
 *
 * @param num - wskaźnik na strukturę.
 */
static void posortuj(PhoneNumbers *num)
{
  qsort((num->slowo), num->rozmiar, sizeof(char *), cmpfun);
}

/**
 * @brief Dodaje do struktury słowa z PhoneForward, które spełniają założenia phfwdReverse()
 * Przechodzi całą strukturę i sprawdza czy obecne słowo jest poprawne, jeśli tak do dodaje do PhoneNumbers
 * @param numbers - wskaźnik na strukturę gdzie będą dodawane nowe słowa
 * @param pf - wskaźnik na strukturę, którą będziemy przechodzili
 * @param num - wskaźnik na słowo - wzór.
 * @param currNum - wskaźnik na obecne słowo - musimy wiedzieć, gdzie jesteśmy - początkowo puste.
 */
static void dodaj(PhoneNumbers *numbers, PhoneForward const *pf, Slowo *num, Slowo *currNum)
{
  if (numbers == NULL || pf == NULL || num == NULL)
    return;

  if (pf->przekierowanie != NULL && pf->przekierowanie->dlugosc > 0 && pf->przekierowanie->dlugosc <= num->dlugosc)
  {
    bool czyPrefiks = true;
    for (int i = 0; i < pf->przekierowanie->dlugosc; i++)
    {
      if (pf->przekierowanie->slowo[i] != num->slowo[i])
      {
        czyPrefiks = false;
        break;
      }
    }
    if (czyPrefiks)
    {
      Slowo *doDodania = slowoCopy(currNum);
      for (int i = pf->przekierowanie->dlugosc; i < num->dlugosc; i++)
        slowoAddLetter(doDodania, num->slowo[i]);
      powieksz(numbers, doDodania);
    }
  }

  for (int i = 0; i < 12; i++)
  {
    if (pf->synowie[i] != NULL)
    {
      slowoAddLetter(currNum, podajLitere(i));
      dodaj(numbers, pf->synowie[i], num, currNum);
    }
  }
  slowoDelLetter(currNum);
}

/**
 * @brief  Usuwa duplikaty ze struktury
 * Struktura musi być posortowana.
 * @param num - wskaźnik na strukturę.
 */
static void usunDuplikaty(PhoneNumbers *num)
{
  for (size_t i = 0; i < num->rozmiar - 1; i++)
  {
    if (areEven(num->slowo[i], num->slowo[i + 1]))
    {
      slowoDelete(num->slowo[i]);
      num->slowo[i] = NULL;
    }
  }
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num)
{
  if (pf == NULL)
    return NULL;

  PhoneNumbers *ret = calloc(1, sizeof(PhoneNumbers));
  if(ret == NULL)
    return NULL;
  if (num == NULL || !czyPoprawny(num, false))
    return ret;

  ret->rozmiar = 0;
  ret->slowo = calloc(0, sizeof(Slowo *));
  if(ret->slowo == NULL)
    return NULL;

  Slowo *newWord = slowoCreate(num);
  Slowo *currWord = slowoNew();
  dodaj(ret, pf, newWord, currWord);

  slowoDelete(currWord);
  powieksz(ret, newWord);
  posortuj(ret);
  usunDuplikaty(ret);
  posortuj(ret);
  for (size_t i = 0; i < ret->rozmiar; i++)
  {
    if (ret->slowo[i] == NULL)
    {
      ret->rozmiar = i;
      break;
    }
  }

  return ret;
}

PhoneNumbers* phfwdGetReverse(PhoneForward const *pf, char const *num){
  
  if(pf == NULL)
    return NULL;
  
  PhoneNumbers *ret = calloc(1, sizeof(PhoneNumbers));
  if(ret == NULL)
    return NULL;
  if(num == NULL || !czyPoprawny(num, false))
    return ret;

  PhoneNumbers* temp = phfwdReverse(pf, num);
  ret->rozmiar = 0;
  ret->slowo = calloc(0, sizeof(Slowo *));
  if(ret->slowo == NULL)
    return NULL;

  for(size_t i = 0 ; i < temp->rozmiar ; i++){

    PhoneNumbers *p = phfwdGet(pf, temp->slowo[i]->slowo);
    Slowo *x = slowoCreate(p->slowo[0]->slowo);
    Slowo *y = slowoCreate(num);
  
    if(areEven(x,y))
      powieksz(ret,slowoCreate(temp->slowo[i]->slowo));
    
    phnumDelete(p);
    slowoDelete(x);
    slowoDelete(y);
  }
  phnumDelete(temp);

  return ret;
}
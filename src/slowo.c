/**
 * @file slowo.c
 * Klasa przechowująa przekierowania slów.
 * @author Paweł Dec <pd438403@students.mimuw.edu.pl>
 * @date 2022-06-18
* 
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "slowo.h"

int podajWartosc(char c)
{
  if ('0' <= c && c <= '9')
    return c - '0';
  else if (c == '*')
    return 10;
  else if (c == '#')
    return 11;
  else
    return -1;
}

char podajLitere(int a)
{
  if (0 <= a && a <= 9)
    return a + '0';
  else if (a == 10)
    return '*';
  else if (a == 11)
    return '#';
  else
    return -1;
}

bool czyPoprawny(char const *num, bool canZero)
{
  if (num == NULL)
    return false;
  int rozmiar = 0;
  while (num[rozmiar] != '\0')
  {
    int number = podajWartosc(num[rozmiar]);
    if (number > 11 || number < 0)
      return false;
    rozmiar++;
  }

  if (canZero)
    return true;
  else
    return rozmiar != 0;
}

void slowoDelete(Slowo *word)
{
  if (word == NULL)
    return;
  free(word->slowo);
  word->dlugosc = 0;
  free(word);
  word = NULL;
}

Slowo *slowoNew(void)
{
  Slowo *ret = calloc(1, sizeof(Slowo));
  if (ret == NULL)
  {
    slowoDelete(ret);
    return NULL;
  }
  ret->slowo = calloc(1, sizeof(char));
  if (ret->slowo == NULL)
  {
    slowoDelete(ret);
    return NULL;
  }
  ret->dlugosc = 0;
  return ret;
}

Slowo *slowoCreate(char const *num)
{
  if (!czyPoprawny(num, true))
    return NULL;

  Slowo *ret = slowoNew();
  if (ret == NULL)
  {
    slowoDelete(ret);
    return NULL;
  }

  int rozmiar = 0;
  while (num[rozmiar] != '\0')
    rozmiar++;
  ret->dlugosc = rozmiar;
  free(ret->slowo);
  ret->slowo = calloc((rozmiar + 1), sizeof(char));
  if (ret->slowo == NULL)
  {
    slowoDelete(ret);
    return NULL;
  }
  for (int i = 0; i < rozmiar; i++)
    ret->slowo[i] = num[i];
  return ret;
}

Slowo *slowoAdd(Slowo *word, char const *num)
{

  int rozmiar2 = word->dlugosc;
  Slowo *ret = slowoNew();
  if (ret == NULL)
  {
    slowoDelete(ret);
    return NULL;
  }
  if (!czyPoprawny(num, true))
  {
    slowoDelete(ret);
    return NULL;
  }
  int rozmiar = 0;
  while (num[rozmiar] != '\0')
    rozmiar++;

  char *ret1 = calloc(1 + rozmiar + rozmiar2, sizeof(char));
  if (ret1 == NULL)
  {
    slowoDelete(ret);
    return NULL;
  }

  for (int i = 0; i < rozmiar2; i++)
    ret1[i] = word->slowo[i];
  for (int i = 0; i < rozmiar; i++)
    ret1[i + rozmiar2] = num[i];

  ret->dlugosc = rozmiar + rozmiar2;
  free(ret->slowo);
  ret->slowo = NULL;

  ret->slowo = ret1;
  return ret;
}

bool sprawdzPoprawnosc(char const *num1, char const *num2)
{

  if (!czyPoprawny(num1, false) || !czyPoprawny(num2, false))
    return false;

  int rozmiar1 = 0;
  while (num1[rozmiar1] != '\0')
    rozmiar1++;
  int rozmiar2 = 0;
  while (num2[rozmiar2] != '\0')
    rozmiar2++;

  if (rozmiar1 != rozmiar2)
    return true;
  bool diffrent = false;
  for (int i = 0; i < rozmiar1; i++)
    diffrent = diffrent | (num1[i] != num2[i]);
  return diffrent;
}

bool areEven(Slowo *slowoA, Slowo *slowoB)
{
  if (slowoA->dlugosc != slowoB->dlugosc)
    return false;
  else
    for (int i = 0; i < slowoA->dlugosc; i++)
      if (slowoA->slowo[i] != slowoB->slowo[i])
        return false;
  return true;
}

void slowoAddLetter(Slowo *word, char letter)
{
  word->dlugosc++;
  word->slowo = realloc(word->slowo, sizeof(char) * (word->dlugosc + 1));
  word->slowo[word->dlugosc - 1] = letter;
  word->slowo[word->dlugosc] = '\0';
}

void slowoDelLetter(Slowo *word)
{
  if (word->dlugosc - 1 >= 0)
    word->slowo[word->dlugosc - 1] = '\0';
  word->dlugosc--;
  word->slowo = realloc(word->slowo, sizeof(char) * (word->dlugosc + 1));
}

Slowo *slowoCopy(Slowo *word)
{
  if (word == NULL)
    return NULL;
  Slowo *ret = slowoNew();
  if (ret == NULL)
  {
    slowoDelete(ret);
    return NULL;
  }
  ret->dlugosc = word->dlugosc;
  free(ret->slowo);
  ret->slowo = calloc((ret->dlugosc + 1), sizeof(char));
  if (ret->slowo == NULL)
  {
    slowoDelete(ret);
    return NULL;
  }
  for (int i = 0; i < ret->dlugosc; i++)
    ret->slowo[i] = word->slowo[i];
  return ret;
}
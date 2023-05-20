/** @file
 * Interfejs klasy przechowującej przekierowania slów.
 *
 * @author Paweł Dec <pd438403@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */
#ifndef SLOWO_H
#define SLOWO_H

#include <stdbool.h>
#include <stddef.h>

/**
 * To jest struktura przechowująca słowo.
 */
struct Slowo
{
  char *slowo; ///< string.
  int dlugosc; ///< długość.
};
/**
 * Skrót - dla czytelności
 */
typedef struct Slowo Slowo;

/**
 * @brief Tworzy puste słowo.
 *
 * @return Wskaźnik na utworzone słowo, gdy się nie udało zwraca NULL.
 */
Slowo *slowoNew(void);

/**
 * @brief Tworzy nowe słowo, z podanego string.
 *
 * @param num - wskaźnik na string.
 * @return Wskaźnik na utworzone słowo, gdy się nie udało zwraca NULL.
 */
Slowo *slowoCreate(char const *num);

/**
 * @brief Kasuje słowo.
 *
 * @param word - wskaźnik na słowo.
 */
void slowoDelete(Slowo *word);

/**
 * @brief Tworzy nowe słowo, z połączenia obecnego oraz podanego string.
 *
 * @param word - wskaźnik na słowo.
 * @param num - wskaźnik na string.
 * @return Wskaźnik na utworzone słowo, gdy się nie udało zwraca NULL.
 */
Slowo *slowoAdd(Slowo *word, char const *num);

/**
 * @brief Sprawdza czy podane 2 string są poprawne do dodania.
 *
 * @param num - wkaźnik na string
 * @param num2 - wskaźnik na string
 * @return true
 * @return false
 */
bool sprawdzPoprawnosc(char const *num, char const *num2);

/**
 * @brief Sprawdza czy podany string  jest poprawny.
 * @param num - wskaźnik na string.
 * @param canZero - informacja czy string może być pusty.
 * @return true - jeśli jest poprawny.
 * @return false - gdy jest niepoprawny.
 */
bool czyPoprawny(char const *num, bool canZero);

/**
 * @brief Podaje liczbę dla danego znaku.
 *
 * @param c - znak.
 * @return int - wartość.
 */
int podajWartosc(char c);

/**
 * @brief Podaje znak dla danej liczby.
 *
 * @param a - liczba.
 * @return char - znak.
 */
char podajLitere(int a);

/**
 * @brief Dodaje literę na koniec słowa.
 *
 * @param word - słowo.
 * @param letter - litera.
 */
void slowoAddLetter(Slowo *word, char letter);

/**
 * @brief Usuwa ostatnią literę ze słowa.
 *
 * @param word - słowo.
 */
void slowoDelLetter(Slowo *word);

/**
 * @brief Tworzy kopię słowo.
 *
 * @param word - słowo.
 * @return Slowo* wskaźnik na nowo powstałą kopię.
 */
Slowo *slowoCopy(Slowo *word);

/**
 * @brief Sprawdza czy słowa są takiesame.
 *
 * @param slowoA - pierwsze słowo.
 * @param slowoB - drugie słowo.
 * @return true - gdy są takie same.
 * @return false - w przeciwnym wypadku.
 */
bool areEven(Slowo *slowoA, Slowo *slowoB);

#endif // SLOWO_H

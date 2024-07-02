/** @file
 * Interfejs modułu wypisyjącego
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 10.05.2020
 */

#ifndef GAMMA_PRINTER_H
#define GAMMA_PRINTER_H

#include "gamma.h"

#include <stdint.h>

/** @brief Wypisuje ERROR line na wyjście błedu.
 * Wypisuje ERROR line na wyjście błedu.
 * @param[in] line_number – numer linii polecenia.
 */
void print_error(int line_number);

/** @brief Wypisuje OK line na standardowe wyjście.
 * Wypisuje OK line na standardowe wyjście.
 * @param[in] line_number – numer linii polecenia.
 */
void print_ok(int line_number);

/** @brief Wypisuje wartość @p uint32_t na standardowe wyjście.
 * Wypisuje liczbę @p value typu @p uint32_t na standardowe wyjście.
 * @param[in] value – wartość do wypisania.
 */
void print_uint32(uint32_t value);

/** @brief Podaje ile znaków potrzeba na zapis liczby.
 * Podaje ile znaków potrzeba na zapis liczby @p x typu @p uint32_t.
 * @param[in] x – nieujemna liczba.
 * @return Długość zapisu liczby.
 */
int number_of_digits(uint32_t x);

/** @brief Podaje współrzędną @p y pola.
 * Podaje współrzędną @p y pola, dla pozycji @p y kursora.
 * @param[in] parameters – wskaźnik na tablicę z parametrami gry,
 * @param[in] y          – współrzędna @p y kursora.
 * @return Daną współrzędną pola, przy założeniu poprawności parametrów.
 */
uint32_t get_field_y(uint32_t *parameters, uint32_t y);

/** @brief Podaje współrzędną @p x pola.
 * Podaje współrzędną @p x pola, dla pozycji @p x kursora.
 * @param[in] parameters – wskaźnik na tablicę z parametrami gry,
 * @param[in] x          – współrzędna @p x kursora.
 * @return Daną współrzędną pola, przy założeniu poprawności parametrów.
 */
uint32_t get_field_x(uint32_t *parameters, uint32_t x);

/** @brief Podaje numer pola.
 * Podaje liczbę, reprezentującą pole (@p x, @p y), która jest indeksem
 * do tablicy @p field_owner
 * @param[in] parameters – wskaźnik na tablicę z parametrami gry,
 * @param[in] x          – współrzędna @p x kursora,
 * @param[in] y          – współrzędna @p y kursora.
 * @return Liczbę reprezentującą dane pole.
 */
uint32_t get_field_number(uint32_t *parameters, uint32_t x, uint32_t y);

/** @brief Wypisuje dane z końca gry trybu interaktywnego.
 * Końcowa plansza oraz liczba pól zajętych przez kolejnych graczy.
 * @param[in] g           – struktura przechowująca grę gamma,
 * @param[in] parameters  – wskaźnik na tablicę z parametrami gry,
 * @param[in] field_owner – wskaźnik na tablicę z właścicielami pól.
 */
void print_statictic(gamma_t *g, uint32_t *parameters, uint32_t *field_owner);

/** @brief Wypisuje pustą planszę o zadanych wymiarach.
 * Wypisuje planszę o zadanych wymiarach, której pola są niezajęte.
 * @param[in] parameters – wskaźnik na tablicę z parametrami gry.
 */
void print_empty_board(uint32_t *parameters);

/** @brief Wypisuje numer gracza w odpowiednim miejscu planszy.
 * Wypisuje numer gracza @p player zaczynając na odpowiednich
 * współrzędnych kursora, podświeltony bądź nie.
 * @param[in] parameters   – wskaźnik na tablicę z parametrami gry,
 * @param[in] field_owner  – wskaźnik na tablicę z właścicielami pól,
 * @param[in] x            – współrzędna @p x kursora,
 * @param[in] y            – współrzędna @p y kursora,
 * @param[in] active_field – 1 to pole ma być podświetlone, wpp normalny zapis.
 */
void print_player(uint32_t *parameters, uint32_t *field_owner, uint32_t x,
                  uint32_t y, int active_field);

/** @brief Wypisuje linię pod planszą o graczu wykonującym ruch.
 * Wypisuje linię pod planszą o graczu @p player wykonującym ruch, postaci
 * PLAYER [@p player] [@p busy_fields] [@p free_fields]
 * ['G' if gamma_golden_possible] '\n'.
 * @param[in] g          – struktura przechowująca grę gamma,
 * @param[in] parameters – wskaźnik na tablicę z parametrami gry,
 * @param[in] player     – liczba graczy, liczba nieujemna,
 */
void print_player_line(gamma_t *g, uint32_t *parameters, uint32_t player);

#endif

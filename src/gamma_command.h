/** @file
 * Interfejs struktury @p command oraz fukncji z nią związanych
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 10.05.2020
 */

#ifndef GAMMA_COMMAND_H
#define GAMMA_COMMAND_H

#include <stdbool.h>
#include <stdint.h>

/** @brief Enum informujący o nazwie polecenia.
 */
typedef enum command_name {
  BATCH_MODE,
  INTERACTIVE_MODE,
  MOVE,
  GOLDEN_MOVE,
  BUSY_FIELDS,
  FREE_FIELDS,
  GOLDEN_POSSIBLE,
  BOARD
} command_name_t;

/** @brief Enum informujący o stanie polecenia.
 */
typedef enum signal {
  OK,
  BAD_ALOCATE,
  BAD_PARAMETERS,
  NOT_INTERPRETATE
} signal_t;

/** @brief Struktura przechowująca polecenie.
 */
typedef struct command command_t;

/** @brief Tworzy strukturę przechowującą polecenie.
 * Alokuje pamięć na nową strukturę przechowującą polecenie.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
command_t* command_new();

/** @brief Usuwa strukturę z pamięci.
 * Zwalnia zaalokowaną pamięć przez strukturę przechowującą polecenie.
 * @param[in,out] command – wskaźnik na strukturę polecenia.
 */
void command_delete(command_t *command);

/** @brief Podaje rodzaj sygnału dla polecenia.
 * Podaje jedną z wartości @p signal_t opisującą stan polecenia.
 * @param[in] command – wskaźnik na strukturę polecenia.
 * @return Rodzaj tego sygnału, przy założeniu poprawności parametru.
 */
signal_t get_command_signal(command_t *command);

/** @brief Podaje nazwę polecenia.
 * Podaje jedną z wartości @p command_name_t nazywającą polecenie.
 * @param[in] command – wskaźnik na strukturę polecenia.
 * @return Nazwę tego polecenia, przy założeniu poprawności parametru.
 */
command_name_t get_command_name(command_t *command);

/** @brief Podaje tablicę z parametrami polecenia.
 * Podaje parametry polecenia jako tablica @p uin32_t[4].
 * @param[in] command – wskaźnik na strukturę polecenia.
 * @return Tablicę z parametrami, przy założeniu poprawności parametru.
 */
uint32_t* get_command_parameters(command_t *command);

/** @brief Ustawia rodzaj sygnału dla polecenia.
 * Ustawia parametr @p signal polecenia.
 * @param[in,out] command – wskaźnik na strukturę polecenia,
 * @param[in] signal      – wartość z @p signal_t.
 */
void set_command_signal(command_t *command, signal_t signal);

/** @brief Ustawia nazwę polecenia.
 * Ustawia paramter @p name polecenia.
 * @param[in,out] command – wskaźnik na strukturę polecenia,
 * @param[in] name        – wartość z @p command_name_t.
 */
void set_command_name(command_t *command, command_name_t name);

/** @brief Ustawia nazwę polecenia na podstawie pierwszej litery z wiersza.
 * Ustawia parametr @p name polecenia na podstawie pierwszej litery z wiersza.
 * Inicjuje tę strukturę tak, aby reprezentowała początkowy stan gry.
 * @param[in,out] command – wskaźnik na strukturę polecenia,
 * @param[in] c           – pierwszy znak wiersza.
 */
void set_command_name_from_first_letter(command_t *command, char c);

/** @brief Podaje liczbę parametrów, które wymaga dane polecenie.
 * @param[in] command – wskaźnik na strukturę polecenia.
 * @return Liczbę parametrów dla danego zdefiniowanego polecenia, przy
 * założeniu poprawności parametru.
 */
int number_of_parameters(command_t *command);

/** @brief Podaje wartość logiczną, czy polecenie jest interpretowane.
 * @param[in] command – wskaźnik na strukturę polecenia.
 * @return Zadaną wartość logiczną.
 */
bool is_not_interpretate(command_t *command);

/** @brief Podaje wartość logiczną, czy polecenie jest poprawne.
 * Czy polecenie zostało dobrze zinterpretowane i dobrze skonwertowane.
 * @param[in] command – wskaźnik na strukturę polecenia.
 * @return Zadaną wartość logiczną.
 */
bool is_ok_signal(command_t *command);

/** @brief Podaje wartość logiczną, czy polecenie rozpoczyna grę / włącza tryb.
 * @param[in] command – wskaźnik na strukturę polecenia.
 * @return Zadaną wartość logiczną.
 */
bool is_starting_command(command_t *command);

#endif

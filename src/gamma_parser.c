/** @file
 * Implementacja parsera
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 10.05.2020
 */

#include "gamma_command.h"
#include "gamma_parser.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** @brief Enum informujący o typie znaku.
 */
typedef enum Char_types {
  COMMENT_CHAR,
  WHITE_SPACE,
  END_OF_LINE,
  USED_LETTER,
  UNDEFINED
} Char_t;

/** @brief Zwraca rodzaj znaku.
 * Podaje wartość z @p Char_t, informującą o typir znaku.
 * @param[in] c – znak, wartość @p char.
 * @return .
 */
Char_t classify_type(char c) {
  if (c == '#')
    return COMMENT_CHAR;
  if (c == 'I' || c == 'B' || c == 'm' || c == 'g' || c == 'b'
  ||  c == 'f' || c == 'q' || c == 'p')
      return USED_LETTER;
  if (c == 10)
      return END_OF_LINE;
  if (c == 32 || c == 9 || c == 11 || c == 12 || c == 13)
      return WHITE_SPACE;
  return UNDEFINED;
}

/** @brief Podaje wartość logiczną, czy znak jest znakiem komentarza.
 * Podaje wartość logiczną, czy znak @p c jest znakiem komentarza.
 * @param[in] c – znak, wartość @p char.
 * @return odpowiednią wartość logiczną.
 */
bool is_comment_char(char c) {
  return classify_type(c) == COMMENT_CHAR ? true : false;
}

/** @brief Podaje wartość logiczną, czy znak jest białym znakiem.
 * Podaje wartość logiczną, czy znak @p c jest białym znakiem.
 * @param[in] c – znak, wartość @p char.
 * @return odpowiednią wartość logiczną.
 */
bool is_white_space(char c) {
  return classify_type(c) == WHITE_SPACE ? true : false;
}

/** @brief Podaje wartość logiczną, czy znak jest używany do poleceń.
 * Podaje wartość logiczną, czy znak @p c jest używany do poleceń.
 * @param[in] c – znak, wartość @p char.
 * @return odpowiednią wartość logiczną.
 */
bool is_used_letter(char c) {
  return classify_type(c) == USED_LETTER ? true : false;
}

/** @brief Podaje wartość logiczną, czy znak jest znakiem końca linii.
 * Podaje wartość logiczną, czy znak @p c jest znakiem końca linii.
 * @param[in] c – znak, wartość @p char.
 * @return odpowiednią wartość logiczną.
 */
bool is_end_of_line(char c) {
  return classify_type(c) == END_OF_LINE ? true : false;
}

// Funckja sprawdzająca czy dany znak jest końcem linii.
// int is_undefined(char c) {
//   return classify_type(c) == UNDEFINED ? TRUE : FALSE;
// }

/** @brief Zadany fragment wiersza konwertuje na liczbę.
 * Podaje liczbę, na podstawie fragmentu wiersza @p line, będącego przedziałem
 * [ @p position; @p end ].
 * @param[in] line     – wiersz z poleceniem,
 * @param[in] position – początek przedziału,
 * @param[in] end      – koniec przedziału.
 * @return otrzymaną liczbę lub -1 gdy konwertowanie się nie udało.
 */
int64_t convert_to_int64_t (char *line, int position, int end) {
  if (end - position + 1 > 10)
    return -1;

  int64_t power = 1;
  int64_t number = 0;

  while (end >= position) {
    number += power * (int64_t)(line[end] - '0');
    power *= (int64_t)10;
    end--;
  }

  return number;
}

/** @brief Ustawia parametry w strukturze @p command, opisujące polecenie.
 * Ustawia określoną liczbę parametrów ze względu na typ polecenia.
 * @param[in] line        – wiersz z poleceniem,
 * @param[in] length      – długość wiersza,
 * @param[in,out] command – koniec przedziału.
 */
void read_numbers(char *line, int length, command_t *command) {
  // line[0] to rodzaj polecenia
  int position = 1;

  for (int i = 0; i < number_of_parameters(command); i++) {

    //znalezienie przedziału z liczbą (line[position] -> line[end-1])
    while (is_white_space(line[position])) {
      position++;
    }
    int end = position;
    while ('0' <= line[end] && line[end] <= '9') {
      end++;
    }

    //nie jest "pusta"
    if (position == end) {
      set_command_signal(command, BAD_PARAMETERS);
      return;
    }
    // nie ma zer wiodących
    if (end - position > 1 && line[position] == 0) {
      set_command_signal(command, BAD_PARAMETERS);
      return;
    }
    // nie konwertuje się, bądź za duża
    int64_t number = convert_to_int64_t(line, position, end - 1);
    if (number > (int64_t)4294967295 || number == -1) {
      set_command_signal(command, BAD_PARAMETERS);
      return;
    } else {
      get_command_parameters(command)[i] = number;
    }

    position = end;

  }

  //czy po zebranych parametrach jest pusto
  while (position < length - 1) {
    if (!is_white_space(line[position])) {
      set_command_signal(command, BAD_PARAMETERS);
      return;
    }
    position++;
  }

  set_command_signal(command, OK);
}

int get_line(char **line) {
  size_t bytes_to_read = 0;
  return getline(line, &bytes_to_read, stdin);
}

void set_command_from_the_line(char *line, int length, command_t *command) {

  set_command_signal(command, OK);
  char first_letter = line[0];

  //wierz który nie jest intepretowany
  if (is_end_of_line(first_letter) || is_comment_char(first_letter)) {
    set_command_signal(command, NOT_INTERPRETATE);
    return;
  }

  //litera z polecenia
  if (is_used_letter(first_letter)) {
    set_command_name_from_first_letter(command, first_letter);
    read_numbers(line, length, command);
    return;
  }

  //pierwsza litera niezdefiniowana
  set_command_signal(command, BAD_PARAMETERS);
}

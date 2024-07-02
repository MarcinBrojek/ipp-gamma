/** @file
 * Implementacja modułu wypisyjącego
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 12.06.2020
 */

#include "gamma_printer.h"
#include "gamma.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** @brief Tablica kolorów czcionki
 * Tablica zawiera ansi escape code'y dla 9 kolorów, używane są w trybie
 * interaktywnym gdy jest mniej niż 10 graczy, w celu ich rozróżnienia.
 */
static const char* colors[] = {"\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m",
                               "\e[36m", "\e[31;1m", "\e[33;1m", "\e[35;1m"};

void print_error(int line_number) {
  fprintf(stderr, "ERROR %d\n", line_number);
}

void print_ok(int line_number) {
  fprintf(stdout, "OK %d\n", line_number);
}

void print_uint32(uint32_t value) {
  fprintf(stdout, "%u\n", value);
}

int number_of_digits(uint32_t x) {
  x /= 10;
  int counter = 1;

  while (x != 0) {
    x /= 10;
    counter++;
  }
  return counter;
}

uint32_t get_field_y(uint32_t *parameters, uint32_t y) {
  return parameters[2] - 1 - y;
}

uint32_t get_field_x(uint32_t *parameters, uint32_t x) {
  if (parameters[0] > 1) {
    return x / (parameters[0] + 1);
  }
  else {
    return x / parameters[0];
  }
}

uint32_t get_field_number(uint32_t *parameters, uint32_t x, uint32_t y) {
  uint32_t from_y = get_field_y(parameters, y) * parameters[1];
  return from_y + get_field_x(parameters, x);
}

/** @brief Rysuje linię poziomą do tabeli.
 * Rysuje linię poziomą o zadanej długości z odstępami na jej końcach.
 * @param[in] length – długość linii poziomej.
 */
void print_horizontal(int length) {
  fprintf(stdout, " ");
  for (int i = 0; i < length; i++)
    fprintf(stdout, "-");
  fprintf(stdout, " \n");
}

/** @brief Wypisuje odstęp o odpowiedniej długości.
 * Wypisuje odstęp dla tabel, aby wyrazy mogły być wyrównane z prawej.
 * @param[in] length_of_whole – długość najdłuższego słowa danego rodzaju,
 * @param[in] length_of_word  – długość zadanego słowa danego,
 */
void print_interface(int length_of_whole, int length_of_word) {
  for (int i = 0; i < length_of_whole - length_of_word; i++) {
    fprintf(stdout, " ");
  }
}

void print_statictic(gamma_t *g, uint32_t *parameters, uint32_t *field_owner) {
  fprintf(stdout, "\e[2J");
  fprintf(stdout, "\e[0;0f");

  uint32_t height = parameters[2];
  uint32_t width = parameters[1];
  uint32_t max_length = parameters[0];

  //plansza
  for (uint32_t y = height; y >= 1; y--) {
    for (uint32_t x = 0; x < width; x++) {

      uint32_t field_number = (y - 1) * width + x;
      uint32_t player = field_owner[field_number];
      uint32_t number_of_empty = max_length - number_of_digits(player);

      for (uint32_t i = 0; i < number_of_empty; i++) {
        fprintf(stdout, " ");
      }
      if (player == 0) {
        fprintf(stdout, ".");
      }
      else {
        if (parameters[3] < 10) {
          fprintf(stdout, "%s%u\e[0m", colors[player - 1], player);
        }
        else {
          fprintf(stdout, "%u", player);
        }
      }

      if (x == width - 1) {
        fprintf(stdout, "\n");
      }
      else if (max_length > 1) {
        fprintf(stdout, " ");
      }
    }
  }
  fprintf(stdout, "\n");

  // statystyki pod planszą
  uint32_t length_of_all_areas = number_of_digits(width * height);

  uint32_t length_player = max_length;
  if (length_player < 6)
    length_player = 6;

  uint32_t length_busy = length_of_all_areas;
  if (length_busy < 8)
    length_busy = 8;

  // nagłówek
  print_horizontal(length_player + length_busy + 1 + 4);
  fprintf(stdout, "| ");
  print_interface(length_player, 6);
  fprintf(stdout, "PLAYER");
  fprintf(stdout, " | ");
  print_interface(length_busy, 8);
  fprintf(stdout, "OBTAINED");
  fprintf(stdout, " |\n");
  print_horizontal(length_player + length_busy + 1 + 4);

  // dane graczy
  for (uint32_t i = 1; i <= parameters[3]; i++) {
    uint64_t busy_fields = gamma_busy_fields(g, i);

    fprintf(stdout, "| ");
    print_interface(length_player, number_of_digits(i));
    if (parameters[3] < 10) {
      fprintf(stdout, "%s%u\e[0m", colors[i - 1], i);
    }
    else {
      fprintf(stdout, "%u", i);
    }
    fprintf(stdout, " | ");
    print_interface(length_busy, number_of_digits(busy_fields));
    fprintf(stdout, "%lu", busy_fields);
    fprintf(stdout, " |\n");
  }
  print_horizontal(length_player + length_busy + 1 + 4);

}

void print_empty_board(uint32_t *parameters) {
  fprintf(stdout, "\e[2J");
  fprintf(stdout, "\e[0;0f");

  uint32_t height = parameters[2];
  uint32_t width = parameters[1];
  uint32_t max_length = parameters[0];

  for (uint32_t y = 1; y <= height; y++) {
    for (uint32_t x = 1; x <= width; x++) {

      for (uint32_t i = 0; i < max_length - 1; i++) {
        fprintf(stdout, " ");
      }
      fprintf(stdout, ".");

      if (x == width) {
        fprintf(stdout, "\n");
      }
      else if (max_length > 1) {
        fprintf(stdout, " ");
      }
    }
  }
  fprintf(stdout, "\n");
}

void print_player(uint32_t *parameters, uint32_t *field_owner, uint32_t x,
                  uint32_t y, int active_field) {

  fprintf(stdout, "\e[%d;%df", y + 1, x + 1);

  uint32_t field_number = get_field_number(parameters, x, y);
  uint32_t player = field_owner[field_number];
  uint32_t number_of_empty = parameters[0] - number_of_digits(player);

  if (active_field == 1) {
    for (uint32_t i = 0; i < number_of_empty; i++) {
      fprintf(stdout, "\e[7m \e[0m");
    }
    if (player == 0) {
      fprintf(stdout, "\e[7m.\e[0m");
    }
    else {
      if (parameters[3] < 10) {
        fprintf(stdout, "%s\e[7m%u\e[0m", colors[player - 1], player);
      }
      else {
        fprintf(stdout, "\e[7m%u\e[0m", player);
      }
    }
  }
  else {
    for (uint32_t i = 0; i < number_of_empty; i++) {
      fprintf(stdout, " ");
    }
    if (player == 0) {
      fprintf(stdout, ".");
    }
    else {
      if (parameters[3] < 10) {
        fprintf(stdout, "%s%u\e[0m", colors[player - 1], player);
      }
      else {
        fprintf(stdout, "%u", player);
      }
    }
  }
}

void print_player_line(gamma_t *g, uint32_t *parameters, uint32_t player) {
  fprintf(stdout, "\e[%d;%df", parameters[2] + 2, 1);
  fprintf(stdout, "\e[2K");

  uint64_t busy = gamma_busy_fields(g, player);
  uint64_t free = gamma_free_fields(g, player);
  bool possible = gamma_golden_possible(g, player);

  uint32_t length_of_all_areas = number_of_digits(parameters[1]
                                                  * parameters[2]);

  uint32_t length_player = parameters[0];
  if (length_player < 6)
    length_player = 6;

  uint32_t length_busy = length_of_all_areas;
  if (length_busy < 8)
    length_busy = 8;

  uint32_t length_free = length_of_all_areas;
  if (length_free < 9)
    length_free = 9;

  // nagłówek
  print_horizontal(length_player + length_busy + length_free + 1 + 3 + 8);
  fprintf(stdout, "| ");
  print_interface(length_player, 6);
  fprintf(stdout, "PLAYER");
  fprintf(stdout, " | ");
  print_interface(length_busy, 8);
  fprintf(stdout, "OBTAINED");
  fprintf(stdout, " | ");
  print_interface(length_free, 9);
  fprintf(stdout, "AVAILABLE");
  fprintf(stdout, " | G |\n");
  print_horizontal(length_player + length_busy + length_free + 1 + 3 + 8);

  // dane gracza
  fprintf(stdout, "| ");
  print_interface(length_player, number_of_digits(player));
  if (parameters[3] < 10) {
    fprintf(stdout, "%s%u\e[0m", colors[player - 1], player);
  }
  else {
    fprintf(stdout, "%u", player);
  }
  fprintf(stdout, " | ");
  print_interface(length_busy, number_of_digits(busy));
  fprintf(stdout, "%lu", busy);
  fprintf(stdout, " | ");
  print_interface(length_free, number_of_digits(free));
  fprintf(stdout, "%lu", free);
  if (possible)
    fprintf(stdout, " | T |\n");
  else
    fprintf(stdout, " | F |\n");

  print_horizontal(length_player + length_busy + length_free + 1 + 3 + 8);
}

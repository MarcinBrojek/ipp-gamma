/** @file
 * Implementacja trybu interaktywnego
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 12.06.2020
 */

#include "gamma_interactive.h"
#include "gamma_printer.h"
#include "gamma.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

/** @brief Tablica zmian współrzednej @p x kursora.
 * Tablica zawiera zmiany współrzędnej kursora szerokości w zależności od
 * kierunku strzałki.
 */
static const int move_x[] = {-1, 1, 0, 0};

/** @brief Tablica zmian współrzednej @p y kursora.
* Tablica zawiera zmiany współrzędnej kursora szerokości w zależności od
* kierunku strzałki.
 */
static const int move_y[] = {0, 0, -1, 1};

/** @brief Sprawdza czy zadane pole znajduję się na planszy.
 * Sprawdza czy pole (@p x + @p move_x[@p i], @p y + @p move_y[@p i])
 * znajduje się na planszy o wymiarach @p width i @p height.
 * @param[in] parameters – wskaźnik na tablicę z parametrami gry,
 * @param[in] x          – współrzędna @p x kursora
 * @param[in] y          – współrzędna @p y kursora
 * @param[in] i          – numer indeksu do tablic @ref move_x[]
 *                         i @ref move_y[],
 * @return Liczbę @p true, jeśli dane pole istnieje, a @p false, jeśli nie,
 * przy założeniu poprawności parametrów.
 */
bool in_range(uint32_t *parameters, uint32_t x, uint32_t y, int i) {
  uint32_t width = parameters[1];
  uint32_t height = parameters[2];
  x = get_field_x(parameters, x);
  y = get_field_y(parameters, y);

  if ((x == 0 && i == 0) || (x == (width - 1) && i == 1) ||
      (y == 0 && i == 3) || (y == (height - 1) && i == 2))
    return false;
  return true;
}

/** @brief Podaje wartość logiczną, czy gracz może wykonać ruch.
 * Czy gracz ma jeszcze możliwość postawienia pionka na pustym polu lub może
 * wykonać złoty ruch (określony przez @p gamma_golden_possible).
 * @param[in] g      – struktura przechowująca grę gamma,
 * @param[in] player – nieujemna liczba.
 * @return Tą wartość logiczną.
 */
bool possible_action(gamma_t *g, uint32_t player) {
  if (gamma_free_fields(g, player) || gamma_golden_possible(g, player)) {
    return true;
  }
  return false;
}

bool interactive_mode(gamma_t *g, uint32_t width, uint32_t height,
                      uint32_t players) {

  // alokowanie tablicy z aktualnymi graczami dla pól
  // nieudana próba oznacza error
  uint32_t *field_owner = malloc(sizeof(uint32_t) * width * height);
  if (field_owner == NULL) {
    return false;
  }
  else {
    for (uint32_t i = 0; i < width * height; i++) {
      field_owner[i] = 0;
    }
  }

  // sprawdzenie czy okno terminala nie jest za małe
  // stosowny komunikat to error, gra nie zostaje dopuszczona
  uint32_t length_of_all_areas = number_of_digits(width * height);
  uint32_t length_of_all_players = number_of_digits(players);

  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  uint32_t board_height = height + 6;

  uint32_t board_width = length_of_all_players * width;
  if (players > 9)
    board_width += (width - 1);

  uint32_t length_player = length_of_all_players;
  if (length_player < 6)
    length_player = 6;

  uint32_t length_busy = length_of_all_areas;
  if (length_busy < 8)
    length_busy = 8;

  uint32_t length_free = length_of_all_areas;
  if (length_free < 7)
    length_free = 7;

  uint32_t line_length = length_player + length_busy + length_free + 1 + 5 + 8;
  if (line_length > board_width)
    board_width = line_length;

  if (size.ws_row <= board_height || size.ws_col <= board_width) {
    printf("%u %u\n", board_height, board_width);
    free(field_owner);
    return false;
  }

  // stworzenie pomocniczej tablicy z kolejno max_length, width, height, players
  // tutaj też error jeśli nie zaalokuje
  uint32_t *parameters = malloc(sizeof(uint32_t) * 4);
  if (parameters == NULL) {
    free(field_owner);
    return false;
  }
  else {
    parameters[0] = number_of_digits(players);
    parameters[1] = width;
    parameters[2] = height;
    parameters[3] = players;
  }

  // udane alokacje zatem można przejść do gry
  fprintf(stdout, "\e[?25l");

  //przygotowanie wczytywania
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  // pomocne zmienne
  int x, y;
  int field_length = parameters[0];
  if (field_length > 1) {
    field_length++;
  }
  bool end_game = false;

  //pusta plansza
  print_empty_board(parameters);

  // kursor 0;0 to lewy górny róg gry, sesji
  //gra startuje z lewego dolnego rogu, kursor skacze po pierw. pozycjach liczb
  x = 0;
  y = height - 1;
  print_player(parameters, field_owner, x, y, 1);

  //właściwa część
  // w czasie operacji kursor ma pozycję 0;0
  while (end_game == false) {
    uint32_t number_of_not_possible = 0;

    for (uint32_t i = 1; i <= players && end_game == false; i++) {

      if (!possible_action(g, i)) {
        number_of_not_possible++;
        continue;
      }

      print_player_line(g, parameters, i);

      bool taken_action = false;
      bool escape = false, bracket = false;

      while (taken_action == false && end_game == false) {
        char znak = getchar();

        if (znak == '\e') {
          escape = true;
          bracket = false;
          continue;
        }

        if (znak == '[') {
          if (escape == true)
            bracket = true;
          else
            escape = false;
          continue;
        }

        if (bracket == true) {
          int move_number = 0;
          // kierunek strzałki
          switch (znak) {
              case 'A': move_number = 2; break;
              case 'B': move_number = 3; break;
              case 'C': move_number = 1; break;
              case 'D': move_number = 0; break;
              default: break;
          }
          if (znak == 'A' || znak == 'B' || znak == 'C' || znak == 'D') {
            if (in_range(parameters, x, y, move_number)) {
              print_player(parameters, field_owner, x, y, 0);
              x += move_x[move_number] * field_length;
              y += move_y[move_number];
              print_player(parameters, field_owner, x, y, 1);
            }
            escape = bracket = false;
            continue;
          }
        }

        escape = bracket = false;

        int field_x = get_field_x(parameters, x);
        int field_y = get_field_y(parameters, y);
        // pas
        if (znak == 'c' || znak == 'C') {
          taken_action = true;
        }
        // złoty ruch
        if (znak == 'g' || znak == 'G') {
          if (gamma_golden_move(g, i, field_x, field_y)) {
            field_owner[get_field_number(parameters, x, y)] = i;
            print_player(parameters, field_owner, x, y, 1);
            taken_action = true;
          }
        }
        // zwykły
        if (znak == ' ') {
          if (gamma_move(g, i, field_x, field_y)) {
            field_owner[get_field_number(parameters, x, y)] = i;
            print_player(parameters, field_owner, x, y, 1);
            taken_action = true;
          }
        }
        // ctrl-D, koniec
        if (znak == 4) {
          end_game = true;
        }
      }
    }
    if (number_of_not_possible == players) {
      break;
    }
  }

  // końcowe statystyki gry
  print_statictic(g, parameters, field_owner);

  free(field_owner);
  free(parameters);

  //powrót do domyślnych ustawień
  t.c_lflag |= (ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  fprintf(stdout, "\e[?25h");

  return true;
}

/** @file
 * Implementacja trybu wsadowego
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 10.05.2020
 */

#include "gamma_command.h"
#include "gamma_parser.h"
#include "gamma_printer.h"
#include "gamma.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** @brief Zwraca 0,1 dla wartości logicznej
 * @return 0 dla false, 1 dla true.
 */
int value_of_bool(bool b) {
  return b == true ? 1 : 0;
}

void batch_mode(int line_number, gamma_t *g, command_t *command) {
  char* line = NULL;

  while (feof(stdin) == 0) {
    line_number++;

    //interpretacja wiersza oraz zwolnienie pamięci z linii
    line = NULL;
    int line_length = get_line(&line);

    if (line_length == -1) {
      if (feof(stdin)) {
        free(line);
        break;
      }
      print_error(line_number);
      free(line);
      continue;
    }
    set_command_from_the_line(line, line_length, command);
    free(line);

    if (is_not_interpretate(command)) {
      continue;
    }

    //poprawność wejścia
    if (!is_ok_signal(command)) {
      print_error(line_number);
      continue;
    }
    if (is_starting_command(command)) {
      print_error(line_number);
      continue;
    }

    command_name_t name = get_command_name(command);
    //wykonanie polecenia
    // NULL może zwrócić gamma_board
    if (name == BOARD) {
      char *board = gamma_board(g);
      if (board == NULL) {
        print_error(line_number);
        continue;
      }
      else {
        fprintf(stdout, "%s", board);
        free(board);
      }
    }

    //parametry do poleceń
    uint32_t player, x, y;
    player = get_command_parameters(command)[0];
    x = get_command_parameters(command)[1];
    y = get_command_parameters(command)[2];

    //pozostałe są bez możliwości zwrócenia NULL
    if (name == MOVE) {
      print_uint32(value_of_bool(gamma_move(g, player, x, y)));
    }
    if (name == GOLDEN_MOVE) {
      print_uint32(value_of_bool(gamma_golden_move(g, player, x, y)));
    }
    if (name == BUSY_FIELDS) {
      print_uint32(gamma_busy_fields(g, player));
    }
    if (name == FREE_FIELDS) {
      print_uint32(gamma_free_fields(g, player));
    }
    if (name == GOLDEN_POSSIBLE) {
      print_uint32(value_of_bool(gamma_golden_possible(g, player)));
    }
  }
}

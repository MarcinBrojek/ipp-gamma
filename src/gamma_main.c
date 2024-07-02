/** @file
 * Zawiera funkcję main programu.
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 12.06.2020
 */

#include "gamma_command.h"
#include "gamma_parser.h"
#include "gamma_printer.h"
#include "gamma_batch.h"
#include "gamma_interactive.h"
#include "gamma.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** @brief Wykonuje grę gamma.
 * Wczytuje linie, aż do napotkania linii startowej gry, która się powiedzie.
 * Następnie uruchamia odpowiedni tryb gry batch / interactive.
 * Po zakończeniu gry w danym trybie, gra kończy się.
 */
int main() {
  gamma_t *g = NULL;
  int line_number = 0;
  char *line = NULL;
  command_t *command = command_new();

  //nieudana alokacja command, uniemożliwia wykonanie programu
  if (command == NULL || get_command_signal(command) == BAD_ALOCATE) {
    command_delete(command);
    return 1;
  }

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
    if (!is_starting_command(command)) {
      print_error(line_number);
      continue;
    }

    //parametry do trybów gry
    uint32_t width, height, players, areas;
    width = get_command_parameters(command)[0];
    height = get_command_parameters(command)[1];
    players = get_command_parameters(command)[2];
    areas = get_command_parameters(command)[3];

    // nieudane stworzenie struktury gamma
    g = gamma_new(width, height, players, areas);
    if (g == NULL) {
      print_error(line_number);
      continue;
    }

    // przekierowanie do zadanego trybu
    if (get_command_name(command) == BATCH_MODE) {
      print_ok(line_number);
      batch_mode(line_number, g, command);
      break;
    }
    if (get_command_name(command) == INTERACTIVE_MODE) {
      if (!interactive_mode(g, width, height, players)) {
        print_error(line_number);
        gamma_delete(g);
        continue;
      }
      else {
        break;
      }
    }

    //tu sie nigdy nie pojawię
    return 1;
  }

  command_delete(command);
  gamma_delete(g);
  return 0;
}

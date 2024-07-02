/** @file
 * Implementacja struktury @p command oraz fukncji z nią związanych
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 10.05.2020
 */

#include "gamma_command.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/// @brief Struktura przechowująca polecenie.
typedef struct command {
  /// wartość z @p signal_t, stan polecenia
  signal_t signal;
  /// wartość z @p command_name_t, nazwa polecenia
  command_name_t name;
  /// tablica uint32_t[4] zawierająca parametry polecenia
  uint32_t* parameters;
} command_t;

command_t *command_new() {

  command_t *command = calloc(1, sizeof(command_t));

  if (command != NULL) {
    command->parameters = malloc(sizeof(uint32_t) * 4);
    if (command->parameters == NULL) {
      command->signal = BAD_ALOCATE;
    }
  }

  return command;
}

void command_delete(command_t *command) {
  if (command != NULL) {
    free(command->parameters);
    free(command);
    command = NULL;
  }
}

signal_t get_command_signal(command_t *command) {
  return command->signal;
}

command_name_t get_command_name(command_t *command) {
  return command->name;
}

uint32_t* get_command_parameters(command_t *command) {
  return command->parameters;
}

void set_command_signal(command_t *command, signal_t signal) {
  command->signal = signal;
}

void set_command_name(command_t *command, command_name_t name) {
  command->name = name;
}

void set_command_name_from_first_letter(command_t *command, char c) {
  if (c == 'B') {
    command->name = BATCH_MODE;
  }
  if (c == 'I') {
    command->name = INTERACTIVE_MODE;
  }
  if (c == 'm') {
    command->name = MOVE;
  }
  if (c == 'g') {
    command->name = GOLDEN_MOVE;
  }
  if (c == 'b') {
    command->name = BUSY_FIELDS;
  }
  if (c == 'f') {
    command->name = FREE_FIELDS;
  }
  if (c == 'q') {
    command->name = GOLDEN_POSSIBLE;
  }
  if (c == 'p') {
    command->name = BOARD;
  }
}

int number_of_parameters(command_t *command) {
  if (command->name == BATCH_MODE || command->name == INTERACTIVE_MODE)
    return 4;
  if (command->name == MOVE || command->name == GOLDEN_MOVE)
    return 3;
  if (command->name == BUSY_FIELDS || command->name == FREE_FIELDS
  ||  command->name == GOLDEN_POSSIBLE)
    return 1;
  return 0; // name = BOARD
}

bool is_not_interpretate(command_t *command) {
  if (command != NULL && command->signal == NOT_INTERPRETATE) {
    return true;
  }
  return false;
}

bool is_ok_signal(command_t *command) {
  if (command != NULL && command->signal == OK) {
    return true;
  }
  return false;
}

bool is_starting_command(command_t *command) {
  if (command->name == BATCH_MODE || command->name == INTERACTIVE_MODE) {
    return true;
  }
  return false;
}

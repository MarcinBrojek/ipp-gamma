/** @file
 * Implementacja klasy przechowującej stan gry gamma
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 12.06.2020
 */

#include "gamma_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {

  if (g == NULL)
    return false;

  bool positive_parameters = true;
  positive_parameters &= (player > 0 && player <= g->players);
  positive_parameters &= (x < g->width);
  positive_parameters &= (y < g->height);
  if (!positive_parameters)
    return false;

  if (get_field_owner_id(g, x, y) != 0)
    return false;

  bool exist_common_owner_neighbour =
    (number_of_neighbouring_owner_fields(g, x, y, player) > 0);

  uint32_t player_areas_number = get_player_areas_number(g, player);
  uint64_t field_position = get_position(g, x, y);
  uint32_t owners_of_neighbouring_fields[] = {0, 0, 0, 0, 0};

  if ((exist_common_owner_neighbour && player_areas_number <= g->areas) ||
      (player_areas_number < g->areas)) {
    g->field_owner_id[field_position] = player;

    union_owners_neighbour_areas(g, x, y);

    for (int i = 0; i < 4; i++) {
      if (!field_in_board(g, x, y, i))
        continue;

      uint32_t new_x = x + move_x[i];
      uint32_t new_y = y + move_y[i];

      uint32_t field_owner_id = get_field_owner_id(g, new_x, new_y);

      owners_of_neighbouring_fields[i] = field_owner_id;
    }

    qsort(owners_of_neighbouring_fields, 5, sizeof(uint32_t), cmp_uint32_t);
    for (int i = 1; i < 5; i++) {
      if (owners_of_neighbouring_fields[i] !=
            owners_of_neighbouring_fields[i - 1] &&
          owners_of_neighbouring_fields[i] != 0
      ) {
        g->player_free_neighbouring_fields_number
          [owners_of_neighbouring_fields[i]]--;
      }
    }

    g->player_fields_number[player]++;
    g->player_areas_number[player]++;
    g->busy_fields_number++;
    g->player_free_neighbouring_fields_number[player] +=
      change_of_free_neighbouring_fields(g, x, y, player);

    return true;
  }
  return false;
}

/** @brief Zwraca wartość logiczną złotego rychu na polu.
 * Zwraca wartość logiczną, dostępności złotego rychu na polu @p x @p y.
 * @param[in,out] g  – struktura przechowująca stan gry gamma,
 * @param[in] player – numer gracza wykonującego złoty ruch, liczba dodatnia,
 * @param[in] x      – współrzedna @p x pola, liczba nieujemna,
 * @param[in] y      – współrzedna @p y pola, liczba nieujemna,
 * @return wartość logiczną, przy założeniu poprawności parametrów.
 */
bool gamma_golden_possible_on_position(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
  if ((number_of_neighbouring_owner_fields(g, x, y, player) == 0) &&
      (get_player_areas_number(g, player) == g->areas))
    return false;

  uint32_t field_owner_id = get_field_owner_id(g, x, y);
  if (field_owner_id == 0 || field_owner_id == player)
    return false;

  uint64_t position = get_position(g, x, y);
  g->field_area_id[position] = position;
  uint32_t number_of_differents_area = 0;

  for (int i = 0; i < 4; i++) {
    if (!field_in_board(g, x, y, i))
      continue;

    uint32_t new_x = x + move_x[i];
    uint32_t new_y = y + move_y[i];
    uint64_t new_position = get_position(g, new_x, new_y);

    if (get_field_owner_id(g, new_x, new_y) == field_owner_id)
      g->field_area_id[new_position] = new_position;
  }

  for (int i = 0; i < 4; i++) {
    if (!field_in_board(g, x, y, i))
      continue;

    uint32_t new_x = x + move_x[i];
    uint32_t new_y = y + move_y[i];
    uint64_t new_position = get_position(g, new_x, new_y);

    if (get_field_owner_id(g, new_x, new_y) == field_owner_id) {
      if (get_field_area_id(g, new_x, new_y) == new_position) {
        g->dfs_counter++;
        number_of_differents_area++;

        g->dfs_visited[position] = g->dfs_counter;
        g->dfs_visited[new_position] = g->dfs_counter;

        dfs_on_players_field(g, new_x, new_y);
      }
    }
  }
  g->player_areas_number[field_owner_id] += (number_of_differents_area - 1);

  bool result = true;
  if (g->player_areas_number[field_owner_id] > g->areas) {
    result = false;
  }

  g->player_areas_number[field_owner_id]++;
  union_owners_neighbour_areas(g, x, y);

  return result;
}

/** @brief Sprawdza, czy gracz może wykonać złoty ruch.
 * Sprawdza, czy gracz @p player może wykonać złoty ruch według pierwotnej
 * wersji @p gamma_golden_possible.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli gracz może legalnie wykonać złoty ruch,
 * a @p false w przeciwnym przypadku, przy założeniu poprawności danej planszy.
 */
bool gamma_old_golden_possible(gamma_t *g, uint32_t player) {

  bool positive_parameters = true;
  positive_parameters &= (player > 0 && player <= g->players);

  if (!positive_parameters)
    return false;

  uint64_t player_fields_number = get_player_fields_number(g, player);
  uint64_t others_fields_number = g->busy_fields_number - player_fields_number;
  if (get_player_used_golden_move(g, player) == false &&
      others_fields_number > 0)
    return true;

  return false;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {

  if (g == NULL)
    return false;

  bool positive_parameters = true;
  positive_parameters &= gamma_old_golden_possible(g, player);
  positive_parameters &= (x < g->width);
  positive_parameters &= (y < g->height);
  if (!positive_parameters)
    return false;

  if (gamma_golden_possible_on_position(g, player, x, y) == false)
    return false;

  uint32_t field_owner_id = get_field_owner_id(g, x, y);
  uint64_t position = get_position(g, x, y);
  g->field_area_id[position] = position;
  uint32_t number_of_differents_area = 0;

  for (int i = 0; i < 4; i++) {
    if (!field_in_board(g, x, y, i))
      continue;

    uint32_t new_x = x + move_x[i];
    uint32_t new_y = y + move_y[i];
    uint64_t new_position = get_position(g, new_x, new_y);

    if (get_field_owner_id(g, new_x, new_y) == field_owner_id)
      g->field_area_id[new_position] = new_position;
  }

  for (int i = 0; i < 4; i++) {
    if (!field_in_board(g, x, y, i))
      continue;

    uint32_t new_x = x + move_x[i];
    uint32_t new_y = y + move_y[i];
    uint64_t new_position = get_position(g, new_x, new_y);

    if (get_field_owner_id(g, new_x, new_y) == field_owner_id) {
      if (get_field_area_id(g, new_x, new_y) == new_position) {
        g->dfs_counter++;
        number_of_differents_area++;

        g->dfs_visited[position] = g->dfs_counter;
        g->dfs_visited[new_position] = g->dfs_counter;

        dfs_on_players_field(g, new_x, new_y);
      }
    }
  }
  g->player_areas_number[field_owner_id] += (number_of_differents_area - 1);

  g->player_free_neighbouring_fields_number[field_owner_id] -=
    change_of_free_neighbouring_fields(g, x, y, field_owner_id);

  g->player_used_golden_move[player] = true;
  g->field_owner_id[position] = player;

  g->player_free_neighbouring_fields_number[player] +=
    change_of_free_neighbouring_fields(g, x, y, player);

  g->player_areas_number[player]++;
  union_owners_neighbour_areas(g, x, y);

  g->player_fields_number[player]++;
  g->player_fields_number[field_owner_id]--;

  return true;
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {

  if (g == NULL)
    return 0;

  bool positive_parameters = true;
  positive_parameters &= (player > 0 && player <= g->players);

  if (!positive_parameters)
    return 0;

  return get_player_fields_number(g, player);
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {

  if (g == NULL)
    return 0;

  bool positive_parameters = true;
  positive_parameters &= (player > 0 && player <= g->players);

  if (!positive_parameters)
    return 0;

  uint32_t player_areas_number = get_player_areas_number(g, player);

  if (player_areas_number < g->areas)
    return (number_of_fields(g) - g->busy_fields_number);

  if (player_areas_number == g->areas)
    return get_player_free_neighbouring_fields_number(g, player);

  return 0;
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {

  if (g == NULL)
    return false;

  if (gamma_old_golden_possible(g, player) == false)
    return false;

  for (uint32_t x = 0; x < g->width; x++) {
    for (uint32_t y = 0; y < g->height; y++) {
      if (gamma_golden_possible_on_position(g, player, x, y))
        return true;
    }
  }

  return false;
}

char *gamma_board(gamma_t *g) {

  if (g == NULL)
    return NULL;

  char *board;
  int max_length = length_of_uint32(g->players);
  uint64_t size;

  if (max_length > 1)
    size = (uint64_t)(max_length + 1) * number_of_fields(g) + 1;
  else
    size = number_of_fields(g) + (uint64_t)(g->height) + 1;

  board = malloc(sizeof(char) * size);

  if (board == NULL)
    return NULL;

  uint64_t position = size - 1;
  board[position] = '\0';
  position--;

  for (uint32_t y = 1; y <= g->height; y++) {
    for (uint32_t x = g->width; x >= 1; x--) {

      if (x == g->width) {
        board[position] = '\n';
        position--;
      }
      else if (max_length > 1) {
        board[position] = ' ';
        position--;
      }

      uint32_t player = get_field_owner_id(g, x - 1, y - 1);
      int length = length_of_uint32(player);

      if (player == 0) {
          board[position] = '.';
          position--;

          for (int i = 0; i < max_length - 1; i++) {
            board[position] = ' ';
            position--;
          }
      }
      else {
        for (int i = 0; i < length; i++) {
          board[position] = player % 10 + '0';
          player /= 10;
          position--;
        }

        for (int i = 0; i < (max_length - length); i++) {
          board[position] = ' ';
          position--;
        }
      }
    }
  }
  return board;
}

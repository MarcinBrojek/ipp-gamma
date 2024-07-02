/** @file
 * Implementacja funkcji pomocniczych do gry gamma
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 17.04.2020
 */

#include "gamma_helpers.h"
#include <stdio.h>
#include <stdlib.h>

uint64_t number_of_fields(gamma_t *g) {
  return (g == NULL ? 0 : (uint64_t)(g->width) * (uint64_t)(g->height));
}

uint64_t get_position(gamma_t *g, uint32_t x, uint32_t y) {
  return (uint64_t)y * (uint64_t)(g->width) + (uint64_t)x;
}

bool field_in_board(gamma_t *g, uint32_t x, uint32_t y, int i) {
  if ((x == 0 && i == 0) || (x == (g->width - 1) && i == 1) ||
      (y == 0 && i == 2) || (y == (g->height - 1) && i == 3))
    return false;
  return true;
}

uint32_t get_field_owner_id(gamma_t *g, uint32_t x, uint32_t y) {
  return g->field_owner_id[get_position(g, x, y)];
}

uint32_t get_player_areas_number(gamma_t *g, uint32_t player) {
  return g->player_areas_number[player];
}

uint64_t get_player_fields_number(gamma_t *g, uint32_t player) {
  return g->player_fields_number[player];
}

uint64_t get_player_free_neighbouring_fields_number(gamma_t *g,
                                                    uint32_t player) {
  return g->player_free_neighbouring_fields_number[player];
}

bool get_player_used_golden_move(gamma_t *g, uint32_t player) {
  return g->player_used_golden_move[player];
}

/** @brief Podaje numer obszaru, do którego należy dany obszar.
 * Podaje spójną/obszar, w którego skład wchodzi obszar @p area.
 * @param[in,out] g  – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] area   – numer obszaru, liczba nieujemna mniejsza od łacznej
 *                     liczby pól planszy.
 * @return Numer tego obszaru, przy założeniu poprawności parametrów.
 */
uint64_t find_representant(gamma_t *g, uint64_t area) {
  if (area == g->field_area_id[area])
    return area;

  return g->field_area_id[area] = find_representant(g, g->field_area_id[area]);
}

uint64_t get_field_area_id(gamma_t *g, uint32_t x, uint32_t y) {
  return find_representant(g, g->field_area_id[get_position(g, x, y)]);
}

/** @brief Łączenie dwóch obaszarów w jeden.
 * Łączenie dwóch sąsiadujących ze sobą obszarów, które należą do
 * gracza @p player
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] area_a  – numer obszaru, liczba nieujemna mniejsza od łacznej
 *                      liczby pól planszy,
 * @param[in] area_b  – numer obszaru, liczba nieujemna mniejsza od łacznej
 *                      liczby pól planszy,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 */
void union_of_areas(gamma_t *g, uint64_t area_a, uint64_t area_b,
                    uint32_t player) {
  uint64_t representant_of_area_a = find_representant(g, area_a);
  uint64_t representant_of_area_b = find_representant(g, area_b);
  if (representant_of_area_a != representant_of_area_b)
    g->player_areas_number[player]--;

  g->field_area_id[representant_of_area_b] = representant_of_area_a;
}

void union_owners_neighbour_areas(gamma_t *g, uint32_t x, uint32_t y) {
  uint32_t player = get_field_owner_id(g, x, y);
  uint64_t field_area_id = get_field_area_id(g, x, y);

  for (int i = 0; i < 4; i++) {
    if (!field_in_board(g, x, y, i))
      continue;

    uint32_t new_x = x + move_x[i];
    uint32_t new_y = y + move_y[i];

    uint32_t field_owner_id = get_field_owner_id(g, new_x, new_y);
    uint64_t neighbouring_field_area_id = get_field_area_id(g, new_x, new_y);

    if (player == field_owner_id)
      union_of_areas(g, field_area_id, neighbouring_field_area_id, player);
  }
}

int length_of_uint32(uint32_t x) {
  x /= 10;
  int counter = 1;

  while (x != 0) {
    x /= 10;
    counter++;
  }
  return counter;
}

int cmp_uint32_t(const void *p, const void *q) {
  uint32_t x = *(const uint32_t *)p;
  uint32_t y = *(const uint32_t *)q;

  if (x < y)
    return -1;
  if (x > y)
    return 1;

  return 0;
}

int number_of_neighbouring_owner_fields(gamma_t *g, uint32_t x, uint32_t y,
                                        uint32_t player) {
  int counter = 0;

  for (int i = 0; i < 4; i++) {
    if (!field_in_board(g, x, y, i))
      continue;

    uint32_t new_x = x + move_x[i];
    uint32_t new_y = y + move_y[i];

    if (get_field_owner_id(g, new_x, new_y) == player)
      counter++;
  }

  return counter;
}

int change_of_free_neighbouring_fields(gamma_t *g, uint32_t x, uint32_t y,
                                       uint32_t player) {
  int counter = 0;

  for (int i = 0; i < 4; i++) {
    if (!field_in_board(g, x, y, i))
      continue;

    uint32_t new_x = x + move_x[i];
    uint32_t new_y = y + move_y[i];

    if ((get_field_owner_id(g, new_x, new_y) == 0) &&
        (number_of_neighbouring_owner_fields(g, new_x, new_y, player) == 1))
      counter++;
  }

  return counter;
}

void dfs_on_players_field(gamma_t *g, uint32_t x, uint32_t y) {
  for (int i = 0; i < 4; i++) {
    if (!field_in_board(g, x, y, i))
      continue;

    if (get_field_owner_id(g, x + move_x[i], y + move_y[i]) ==
          get_field_owner_id(g, x, y) &&
        g->dfs_visited[get_position(g, x + move_x[i], y + move_y[i])] !=
          g->dfs_counter
    ) {
      g->field_area_id[get_position(g, x + move_x[i], y + move_y[i])] =
        g->field_area_id[get_position(g, x, y)];
      g->dfs_visited[get_position(g, x + move_x[i], y + move_y[i])] =
        g->dfs_counter;
      dfs_on_players_field(g, x + move_x[i], y + move_y[i]);
    }
  }
}

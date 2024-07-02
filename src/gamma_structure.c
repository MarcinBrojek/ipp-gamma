/** @file
 * Implementacja alokacji i usuwania struktury gamma
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 17.04.2020
 */

#include "gamma_structure.h"
#include <stdio.h>
#include <stdlib.h>

gamma_t *gamma_new(uint32_t width, uint32_t height, uint32_t players,
                   uint32_t areas) {

  bool positive_parameters = true;
  positive_parameters &= (width > 0);
  positive_parameters &= (height > 0);
  positive_parameters &= (players > 0);
  positive_parameters &= (areas > 0);

  bool positive_allocation = true;
  gamma_t *g = malloc(sizeof(gamma_t));

  if (g == NULL) {
    return NULL;
  }

  g->width = width;
  g->height = height;
  g->players = players;
  g->areas = areas;

  uint64_t number_of_all_fields =
    (g == NULL ? 0 : (uint64_t)(g->width) * (uint64_t)(g->height));

  g->field_owner_id = calloc(number_of_all_fields, sizeof(uint32_t));
  positive_allocation &= (g->field_owner_id != NULL);

  g->field_area_id = calloc(number_of_all_fields, sizeof(uint64_t));
  positive_allocation &= (g->field_area_id != NULL);

  g->player_areas_number =
    calloc(((uint64_t)g->players + 1), sizeof(uint32_t));
  positive_allocation &= (g->player_areas_number != NULL);

  g->player_fields_number =
    calloc(((uint64_t)g->players + 1), sizeof(uint64_t));
  positive_allocation &= (g->player_fields_number != NULL);

  g->player_free_neighbouring_fields_number =
    calloc(((uint64_t)g->players + 1), sizeof(uint64_t));
  positive_allocation &= (g->player_free_neighbouring_fields_number != NULL);

  g->player_used_golden_move =
    calloc(((uint64_t)g->players + 1), sizeof(bool));
  positive_allocation &= (g->player_used_golden_move != NULL);

  g->dfs_visited = calloc(number_of_all_fields, sizeof(uint64_t));
  positive_allocation &= (g->dfs_visited != NULL);

  if (positive_allocation && positive_parameters && number_of_all_fields > 0) {

    g->dfs_counter = 0;
    g->busy_fields_number = 0;

    for (uint64_t i = 0; i < number_of_all_fields; i++) {
      g->field_area_id[i] = i;
    }

    return g;
  }
  gamma_delete(g);
  return NULL;
}

void gamma_delete(gamma_t *g) {
  if (g != NULL) {
    free(g->field_owner_id);
    free(g->field_area_id);
    free(g->player_areas_number);
    free(g->player_fields_number);
    free(g->player_free_neighbouring_fields_number);
    free(g->player_used_golden_move);
    free(g->dfs_visited);
    free(g);
    g = NULL;
  }
}

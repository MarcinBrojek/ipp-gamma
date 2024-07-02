/** @file
 * Struktura gamma
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 17.04.2020
 */

#ifndef GAMMA_STRUCTURE_H
#define GAMMA_STRUCTURE_H

#include "gamma.h"

/// @brief Struktura przechowująca stan gry
struct gamma {
 /// szerokość planszy, liczba dodatnia
  uint32_t width;
 /// wysokość planszy, liczba dodatnia
  uint32_t height;
 /// liczba graczy, liczba dodatnia
  uint32_t players;
 /// maksymalna liczba obszarów, jakie może zająć jeden gracz, liczba dodatnia
  uint32_t areas;
 /// łączna liczba aktualnie zajętych pól
  uint64_t busy_fields_number;
 /// tablica indeksowana numerami pól, wartość to numer gracza zajmującego dane
 /// pole
  uint32_t *field_owner_id;
 /// tablica indeksowana numerami pól, wartość to numer obszaru, do kórego
 /// aktualnie należy pole
  uint64_t *field_area_id;
 /// tablica indeksowana numerami graczy, wartość to liczba obszarów, które
 /// aktualnie posiada gracz
  uint32_t *player_areas_number;
 /// tablica indeksowana numerami graczy, wartość to liczba
 /// pól, które aktualnie posiada gracz
  uint64_t *player_fields_number;
 /// tablica indeksowana numerami graczy, wartość to liczba
 /// wolnych pól, które sąsiadują z polami gracza
  uint64_t *player_free_neighbouring_fields_number;
 /// tablica indeksowana numerami graczy, wartość to wartość
 /// logiczna zużycia złotego ruchu przez gracza
  bool *player_used_golden_move;
 /// numer wywołania dfs używany w @ref dfs_on_players_field
  uint64_t dfs_counter;
 /// tablica indeksowana numerami pól, wartość to numer wywołania dfs-sa, który
 /// odwiedził to pole
  uint64_t *dfs_visited;
};

#endif

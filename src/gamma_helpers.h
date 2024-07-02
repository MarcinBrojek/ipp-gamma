/** @file
 * Interfejs funkcji pomocniczych do gry gamma
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 17.04.2020
 */

#ifndef GAMMA_HELPERS_H
#define GAMMA_HELPERS_H

#include "gamma_structure.h"

/** @brief Tablica zmian współrzednej x.
 * Tablica zawiera zmiany współrzędnej szerokości w zależności od przeglądanego
 * sąsiedniego pola.
 */
static const int move_x[] = {-1, 1, 0, 0};

/** @brief Tablica zmian współrzednej y.
 * Tablica zawiera zmiany wpsółrzędnej wysokości w zależności od przeglądanego
 * sąsiedniego pola.
 */
static const int move_y[] = {0, 0, -1, 1};

/** @brief Podaje liczbę wszystkich pól planszy dla zadanej struktury.
 * Podaje liczbę wszystkich pól planszy dla struktury wskazywanej przez @p g.
 * @param[in] g – wskaźnik na strukturę przechowującą stan gry.
 * @return Liczba wszystkich pól lub 0, gdy parametr jest niepoprawny.
 */
uint64_t number_of_fields(gamma_t *g);

/** @brief Podaje numer pola.
 * Podaje liczbę, reprezentującą pole (@p x, @p y), która jest indeksem
 * do tablic @p field_owner_id i @p field_area_id, ze struktury @p g.
 * @param[in] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x – numer kolumny, liczba nieujemna mniejsza od wartości
 *                @p width z funkcji @ref gamma_new,
 * @param[in] y – numer wiersza, liczba nieujemna mniejsza od wartości
 *                @p height z funkcji @ref gamma_new.
 * @return Liczbę reprezentującą dane pole,
 * przy założeniu poprawności parametrów.
 */
uint64_t get_position(gamma_t *g, uint32_t x, uint32_t y);

/** @brief Sprawdza czy zadane pole znajduję się na planszy.
 * Sprawdza czy pole (@p x + @p move_x[@p i], @p y + @p move_y[@p i])
 * znajduje się na planszy.
 * @param[in] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x – numer kolumny, liczba nieujemna mniejsza od wartości
 *                @p width z funkcji @ref gamma_new,
 * @param[in] y – numer wiersza, liczba nieujemna mniejsza od wartości
 *                @p height z funkcji @ref gamma_new,
 * @param[in] i – numer indeksu do tablic @ref move_x[] i @ref move_y[],
 *                liczba nieujemna mniejsza od 4.
 * @return Liczbę @p true, jeśli dane pole istnieje, a @p false, jeśli nie,
 * przy założeniu poprawności parametrów.
 */
bool field_in_board(gamma_t *g, uint32_t x, uint32_t y, int i);

/** @brief Podaje numer gracza, który zajmuje dane pole.
 * Podaje numer gracza, którego pionek znajduje się na polu (@p x, @p y).
 * @param[in] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x – numer kolumny, liczba nieujemna mniejsza od wartości
 *                @p width z funkcji @ref gamma_new,
 * @param[in] y – numer wiersza, liczba nieujemna mniejsza od wartości
 *                @p height z funkcji @ref gamma_new.
 * @return Numer tego gracza, przy założeniu poprawności parametrów.
 */
uint32_t get_field_owner_id(gamma_t *g, uint32_t x, uint32_t y);

/** @brief Podaje liczbę obszarów danego gracza.
 * Podaje liczbę obszarów, które zajął gracz @p player.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczbę obszarów zajętyh przez gracza, przy założeniu poprawności
 * parametrów.
 */
uint32_t get_player_areas_number(gamma_t *g, uint32_t player);

/** @brief Podaje liczbę pól zajętych przez danego gracza.
 * Podaje liczbę pól, które zajmuje gracz @p player.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczbę pól zajętyh przez gracza, przy założeniu poprawności
 * parametrów.
 */
uint64_t get_player_fields_number(gamma_t *g, uint32_t player);

/** @brief Podaje liczbę wolych pól w sąsiedztwie pól danego gracza.
 * Podaje liczbę niezajętych pól, które sąsiadują z polami zajętymi przez
 * gracza @p player.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczbę tych niezajętych pól, przy założeniu poprawności parametrów.
 */
uint64_t get_player_free_neighbouring_fields_number(gamma_t *g,
                                                    uint32_t player);

/** @brief Podaje wartość logiczną, czy gracz wykonał już złoty ruch.
 * Podaje @p true, jeśli gracz @p player wykonał już złoty ruch w rozgrywce, a
 * @p false, w przeciwnym przypadku.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartość logiczną wykonania złotego ruchu przez gracza,
 * przy założeniu poprawności parametrów.
 */
bool get_player_used_golden_move(gamma_t *g, uint32_t player);

/** @brief Podaje numer obszaru, do kórego należy dane pole.
 * Podaje numer obszaru, do kórego należy pole (@p x, @p y)
 * @param[in,out] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x     – numer kolumny, liczba nieujemna mniejsza od wartości
 *                    @p width z funkcji @ref gamma_new,
 * @param[in] y     – numer wiersza, liczba nieujemna mniejsza od wartości
 *                    @p height z funkcji @ref gamma_new.
 * @return Numer tego obszaru, przy założeniu poprawności parametrów.
 */
uint64_t get_field_area_id(gamma_t *g, uint32_t x, uint32_t y);

/** @brief Łączenie obszaru pola z obszarami sąsiednich pól tego gracza.
 * Łączenie obszaru, do kórego należy pole (@p x, @p y), z obszarami
 * sąsiednich pól tego samego gracza.
 * @param[in,out] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x     – numer kolumny, liczba nieujemna mniejsza od wartości
 *                    @p width z funkcji @ref gamma_new,
 * @param[in] y     – numer wiersza, liczba nieujemna mniejsza od wartości
 *                    @p height z funkcji @ref gamma_new.
 */
void union_owners_neighbour_areas(gamma_t *g, uint32_t x, uint32_t y);

/** @brief Podaje ile znaków potrzeba na zapis liczby.
 * Podaje ile znaków potrzeba na zapis liczby @p x typu @p uint32_t.
 * @param[in] x – nieujemna liczba.
 * @return Długość zapisu liczby.
 */
int length_of_uint32(uint32_t x);

/** @brief Standardowa funkcja porównująca dla sortowania.
 */
int cmp_uint32_t(const void *p, const void *q);

/** @brief Podaje liczbę sąsiednich pól, należących do danego gracza.
 * Podaje liczbę sąsiednich pól do pola (@p x, @p y), które należą do gracza
 * @p player.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczbę takich sąsiednich pól, przy założeniu poprawności parametrów.
 */
int number_of_neighbouring_owner_fields(gamma_t *g, uint32_t x, uint32_t y,
                                        uint32_t player);

/** @brief Podaje liczbę sąsiednich wolnych pól, które nie sąsiadują z żadnymi
 * polami gracza.
 * Podaje liczbę niezajętych sąsiednich pól do pola (@p x, @p y), które nie
 * sąsiadują z żadnymi polami gracza @p player.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczbę takich sąsiednich pól, przy założeniu poprawności parametrów.
 */
int change_of_free_neighbouring_fields(gamma_t *g, uint32_t x, uint32_t y,
                                       uint32_t player);

/** @brief Wywołuje dfs-a na polach danego gracza.
 * Dfs startuje z pola (@p x, @p y), przechodzi po sąsiednich polach,
 * takich, że należą tego samego gracza i nie były jeszcze odwiedzone w tym
 * przejściu. Pola odwiedzone należą do jednego obszaru.
 * @param[in,out] g  – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x      – numer kolumny, liczba nieujemna mniejsza od wartości
 *                     @p width z funkcji @ref gamma_new,
 * @param[in] y      – numer wiersza, liczba nieujemna mniejsza od wartości
 *                     @p height z funkcji @ref gamma_new.
 */
void dfs_on_players_field(gamma_t *g, uint32_t x, uint32_t y);

#endif

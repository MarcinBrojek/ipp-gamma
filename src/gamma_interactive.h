/** @file
 * Interfejs trybu interaktywnego
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 10.05.2020
 */

#ifndef GAMMA_INTERACTIVE_H
#define GAMMA_INTERACTIVE_H

#include "gamma.h"

#include <stdint.h>

 /** @brief Wykonuje grę gamma w trybie interaktywnym, zwraca czy udana alokacja
  * Włącza tryb interaktywny dla zadanych parametrów, w przypadku nieudanej
  * alokacji wewnątrz funkcji, zwraca @p false, w przeciwnej sytuacji @p true.
  * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry gamma,
  * @param[in] width   – szerokość planszy,
  * @param[in] height  – wysokość planszy,
  * @param[in] players – liczba graczy.
  * @return Wartość logiczną udanych alokacji, czy udało się uruchomić tryb.
  */
bool interactive_mode(gamma_t *g, uint32_t width, uint32_t height,
                      uint32_t players);

#endif

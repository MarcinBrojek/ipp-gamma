/** @file
 * Interfejs trybu wsadowego
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 10.05.2020
 */

#ifndef GAMMA_BATCH_H
#define GAMMA_BATCH_H

/** @brief Wykonuje grę gamma w trybie wsadowym
 * @param[in] line_number – numer linii,
 * @param[in] g           – struktura przechowująca stan gry gamma,
 * @param[in,out] command – struktura przechowująca polecenie.
 */
void batch_mode(int line_number, gamma_t *g, command_t *command);

#endif

/** @file
 * Interfejs parsera
 *
 * @author Marcin Brojek <mb417749@students.mimuw.edu.pl>
 * @copyright Marcin Brojek
 * @date 10.05.2020
 */

#ifndef GAMMA_PARSER_H
#define GAMMA_PARSER_H

/** @brief Ustawia linię z @p stdin jako @p line i zwraca jej długość.
 * Aktualizuje @line i podaje liczbę znaków w wiersza, nie uwzględniając znaku
 * końca stringa.
 * @param[in,out] line – wskaźnik na tablicę @p char.
 * @return Liczbę znaków w wierszu.
 */
int get_line(char **line);

/** @brief Ustawia @p command na postawie zawartości wiersza.
 * @param[in] line        – wiersz z poleceniem,
 * @param[in] length      – długość wiersza,
 * @param[in,out] command – wskaźnik na tablicę @p char.
 */
void set_command_from_the_line(char *line, int length, command_t *command);

#endif

/**
 * @brief quick and dirty graphics lib
 */

#ifndef GRLIB_H
#define GRLIB_H

int gr_mode(int mode);
int gr_pset(int x, int y, char c);
int gr_text(int x, int y, char *s);
int gr_color(char p, char c);

#endif /* GRLIB_H */

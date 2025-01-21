#ifndef VIDEO_H
#define VIDEO_H

int Active_page(void);
void Goto_XY(int col, int row);
char Get_char(void);
unsigned int Get_key(unsigned char mode);
char Get_attr(void);
int Get_X(void);
int Get_Y(void);
unsigned char Get_mode(void);
void Set_mode(unsigned char mode);
void Clear_screen(void);
void Write_chr(unsigned char chr);
void Write_tty(unsigned char *str); 

#endif /* VIDEO_H */

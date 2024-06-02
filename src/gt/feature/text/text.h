#ifndef TEXT_H
#define TEXT_H

void init_text();

void load_font(char slot);

void print_text(char* text);
void print_char(char c);

extern char text_cursor_x, text_cursor_y, text_print_width, text_print_line_start;
extern char text_use_alt_color;

#endif

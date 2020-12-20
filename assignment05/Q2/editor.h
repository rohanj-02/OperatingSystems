#ifndef EDITOR_H

#define EDITOR_H

typedef struct termios terminal_state;
#define CTRL_(k) ((0x1f & k))

void exit_function();
void init_terminal();
void clear_screen();
char get_key_input();

#endif

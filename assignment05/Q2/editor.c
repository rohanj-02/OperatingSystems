#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct termios terminal_state;

terminal_state original_terminal_state;

/**
 * @brief To restore the state of the terminal before the function ends
 * 
 */
void exit_function()
{
	printf("Exiting text editor now \r\n");
	clear_screen();
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal_state) == -1)
	{
		perror("tcsetattr(): error");
		exit(1);
	}
}

/**
 * @brief TO iniitalise the terminal to enter raw mode from canonical mode 
 * 
 */
void init_terminal()
{
	// Gets terminal attributes and stores it in the original terminal state so it can be restored on exit of this program
	if (tcgetattr(STDIN_FILENO, &original_terminal_state) == -1)
	{
		perror("tcgetattr(): error");
		exit(1);
	}
	// Set exit_function to be called on exit of the program
	atexit(exit_function);

	// For raw mode of terminal
	terminal_state raw_mode_state;
	if (tcgetattr(STDIN_FILENO, &raw_mode_state) == -1)
	{
		perror("tcgetattr(): error");
		exit(1);
	}

	// terminal_state raw = original_terminal_state;
	// ICANON for byte-by-byte input instead of line-by-line input
	// Stopping control signals from affecting our input
	// Disabling :
	// 	ISIG: Ctrl + c, Ctrl + z,
	// 	IXON: Ctrl + s, Ctrl + q
	//	IEXTEN: Ctrl + v
	//	ICRNL: To differentiate between carriage return and new line
	//	OPOST: Don't change \n to \r\n
	raw_mode_state.c_iflag &= ~(IXON | ICRNL);
	raw_mode_state.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw_mode_state.c_oflag &= ~(OPOST);
	// VMIN stores the number of bytes needed for read to return
	raw_mode_state.c_cc[VMIN] = 0;
	// VTIME stores max amount of time before read returns, used for timeout
	raw_mode_state.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_mode_state) == -1)
	{
		perror("tcsetattr(): error");
		exit(1);
	}
}

void clear_screen()
{
	if (write(STDOUT_FILENO, "\x1b[2J", 4) == -1)
	{
		perror("write(): error");
		exit(1);
	}
	if (write(STDOUT_FILENO, "\x1b[H", 3) == -1)
	{
		perror("write(): error");
		exit(1);
	}
}

int main()
{
	init_terminal();
	char c;
	while (1)
	{
		char c = '\0';
		clear_screen();
		if (read(STDIN_FILENO, &c, 1) == -1)
		{
			perror("read(): error");
			exit(1);
		}
		if (iscntrl(c))
		{
			printf("%d\r\n", c);
		}
		else
		{
			printf("%d ('%c')\r\n", c, c);
		}
		// Ctrl + q stops the editor
		if (c == (0x1f & 'q'))
		{
			break;
		}
	}
	// exit_function();
	return 0;
}
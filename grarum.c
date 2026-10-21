#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*
	KEYWORDS LIST:
	
	STDI	= standard input
	BUFR	= buffer
	ATTR	= attributes
*/

//struct that will contains the default mode of the STDI
struct	termios orig_termios;

//disable raw mode
void	disable_raw_mode(void)
{
	//set the ATTR of STDI to the default ATTR
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

//set mode to raw for the STDI
void	enable_raw_mode(void)
{
	//write the ATTR of the STDI in orig_termios
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disable_raw_mode);

	//create a new struct for our raw mode that will first contain the original ATTR
	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO);

	//apply the changes
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int	main(void)
{
	enable_raw_mode();

	// BUFR
	char	c;
	// read 1 byte from STDI into BUFR c
	while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
	return (0);
}

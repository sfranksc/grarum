#include <unistd.h>
#include <termios.h>

/*
	KEYWORDS LIST:
	
	STDI = standard input
	BUFR = buffer
*/

//set mode to raw for the STDI
void	enable_raw_mode(void)
{
	struct	termios raw;

	tcgetattr(STDIN_FILENO, &raw);
	
	raw.c_lflag &= ~(ECHO);

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

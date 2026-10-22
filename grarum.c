/*** includes ***/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*
	KEYWORDS LIST:
	
	STDI	= standard input
	BUFR	= buffer
	ATTR	= attributes
*/

//this functions will mode into an utility file soon.
int iscntrl(int c)
{
	if ((c >= 0 && c <= 31) || c == 127)
		return(1);
	return (0);	
}

//end of utilities

/*** data ***/

//struct that will contains the default mode of the STDI
struct	termios orig_termios;

/*** terminal ***/

//this function is called whenever an error happens
void	die(const char *s)
{
	//print the last error in errno
	perror(s);
	exit(1);
}

//disable raw mode
void	disable_raw_mode(void)
{
	//check for error after calling the function
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("(38) tcsetattr");
}

//set mode to raw for the STDI
void	enable_raw_mode(void)
{
	//write the ATTR of the STDI in orig_termios
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
		die("(46) tcgetattr");
	atexit(disable_raw_mode);

	//create a new struct for our raw mode that will first contain the original ATTR
	struct termios raw = orig_termios;
	//modifiy the flags with bitwise operations until we get to raw mode
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag &= ~(CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	//set the MIN byte to read and MAX time before read returns
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	//apply the changes
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("(60) tcsetattr");
}

/*** init ***/

int	main(void)
{
	enable_raw_mode();

	//BUFR
	char	c;
	//create an infinite loop 
	while (1)
	{
		c = '\0';
		//read from STDI into BUFR c and check for error
		//supports Cygwin
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
			die("(74) read");
		if (iscntrl(c))
			printf("%d\r\n", c);
		else
			printf("%d ('%c')\r\n", c, c);
		//break out of the loop when q is pressed
		if (c == 'q')
			break;
	}
}

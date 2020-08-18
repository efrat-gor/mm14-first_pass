#include "auxiliary.h"
/*initialize the ine and clean the text that was there*/
 void initialize_line(char *line)
 {
    int i;
	for (i = 0; i < MAX_LINE_LENGTH; i++)
	{
		line[i] = '\0';
	}
 }
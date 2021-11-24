#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <tcl8.6/tcl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "functions.h"
#include "instructions.h"

void initString( char* ch, int n ) 
{
    for (int j = 0; j < n; j++)
        *ch = '\0';
}

int InitInterpreter (void) 
{	// Return okay or not //
 	
    tcl_interpreter = Tcl_CreateInterp() ;

    if (tcl_interpreter == NULL) 
    {
        fprintf( stderr, "Could not create interpreter!\n" ) ;
        return (1) ;
    }

	if (Tcl_Init(tcl_interpreter) == TCL_ERROR) 
	{
		fprintf( stderr, "ERROR in Tcl initialization: %s\n", Tcl_GetStringResult(tcl_interpreter) ) ;
       	return (1) ;
	}

	return (0);
}

/********************************************************************************
 * By calling the 'rl_completion_matches', returns an array of strings which is *
 * a list of completions for text. If there are no completions, returns NULL.   *
 *******************************************************************************/

char **custom_completer(const char *text, int start, int end) 
{
	char **name = NULL;
    char *files = NULL;
	rl_attempted_completion_over = 1;	// To avoid standard filename completion by Readline //

    if (!start)
        name = rl_completion_matches(text, instruction_generator);
    else
        rl_attempted_completion_over = 0;
    
    
    return (name);
}

char *instruction_generator (const char *text, int state) 
{
	static int idx = 0;
	static size_t command_size;

	char *match = NULL;
	int i;
	

	if (!state) 
	{
		idx = 0;
		command_size = strlen(text);
	}
	while ( (match = instructions[idx] ) != NULL) 
	{
		idx++;
		if ( ( strlen(match) >= command_size ) && ( !strncmp(text, match, command_size) ) )  
			return (strdup (match));
	}
    
	return (NULL);
}

/**********************************
 * Get executed commands' history *
 **********************************/
void get_history() 
{
    HIST_ENTRY **the_history_list; // readline commands history list - NULL terminated //
    unsigned long i;

    the_history_list = history_list(); // get history list //
    if (the_history_list != NULL)
    {
        i = 0;
        while (*(the_history_list + i) != NULL) // history list - NULL terminated //
        {
            printf("%lu: %s\n", (i + history_base), (*(the_history_list + i))->line);
            i++;
        }
    }
}


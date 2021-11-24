/************** TCL Extended: History, Completion ***************
 *						CE437, Fall 2018                        *
 *						ECE, UTH, Greece                        *                                                               
 * 				Author: Vasileiou Christos (1983)				*
*****************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "instructions.h"

#include <tcl8.6/tcl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define COLOR  "\x1B[33m"
#define COLOR2  "\x1B[12m"
#define RESET "\x1B[0m"
#define SIZE_COMMAND LINE_MAX

static Tcl_Interp *tcl_interpreter ; /* Script interpreter */

/****************** Function Prototypes *******************/
char **custom_completer(const char *text, int start, int end);
char *instruction_generator (const char *text, int state);
int InitInterpreter (void);
void get_history ();
void initParsingCommand( char* ch);

int main(int argc, char *argv[] )
{
    int i = 0;
    char *result = NULL;
    char *text = NULL; // readline result //
    char *textexpansion = NULL; // readline result history expanded //
    int expansionresult, code;
    char command [LINE_MAX]; // current command //
	Tcl_Obj *objPtr = NULL;
    char *searchWhiteSpaces;
    char parsingCommand [SIZE_COMMAND]; 

    printf ( COLOR "Welcome to my custom Tcl Shell!\n" RESET);
    Tcl_FindExecutable( argv[0] );

    if ( InitInterpreter() == TCL_ERROR )
        return ( EXIT_FAILURE );

    // Readline Initialisation //
    rl_completion_entry_function = NULL; // use rl_filename_completion_function(), the default filename completer //
    rl_attempted_completion_function = custom_completer;
    rl_completion_append_character = '\0';
    using_history(); // initialise history functions //
    while (1)
    {
        text = readline("PR> ");
        if (text != NULL)
        {
            expansionresult = history_expand( text, &textexpansion );
            if ( (expansionresult == 0) || // no expansion //
                 (expansionresult == 2) ) // do not execute //
            {
                add_history( text );                  
                strcpy( command, text ); // store command //
            }
            else
            {
                add_history( textexpansion );
                strcpy( command, textexpansion ); // store command //
            }
            free( textexpansion );
            free( text );

            initParsingCommand( parsingCommand );
            for ( searchWhiteSpaces = command; ( searchWhiteSpaces - command ) < sizeof(command); searchWhiteSpaces++)
            {
                if ( !isalpha(*searchWhiteSpaces) )
                    break;
                strcpy( &parsingCommand[i], searchWhiteSpaces );
                i++;
            }

            strcpy( &parsingCommand[i], "\0");
            i = 0;
            
            // handle two basic commands: history and quit //
            if ( (strcmp(command, "quit") == 0) || (strcmp(command, "q") == 0)  )
            {
                return EXIT_SUCCESS;
            }
            else if ( strcmp(command, "history") == 0 )
            {
                get_history();
            }
            else if ( strcmp(parsingCommand, "ls") == 0 )
            {
                system (command);
            }
            else if ( strcmp(parsingCommand, "less") == 0 )
            {
                system (command);
            }
            else if ( strcmp(parsingCommand, "man") == 0 )
            {
                system (command);
            }
            else
            {
                /* Tcl command handling */
                code = Tcl_Eval(tcl_interpreter, command);			// Evaluate the command => Execute it
                objPtr = Tcl_GetObjResult(tcl_interpreter);			// Get the output of the executed command as a tcl object
                result = Tcl_GetStringFromObj(objPtr, NULL);	// Get the string - return value of the executed command
                printf("%s\n", result);
            }
        }
    }
}

void initParsingCommand( char* ch ) 
{
    for (int j = 0; j < SIZE_COMMAND; j++)
        *ch = '\0';
}

int InitInterpreter (void) 
{	/* Return okay or not */
 	
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
	rl_attempted_completion_over = 1;	// To avoid standard filename completion by Readline

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
	free(match);
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

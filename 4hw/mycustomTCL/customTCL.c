/************** TCL Extended: History, Completion ***************
 *						CE437, Fall 2018                                  *
 *						ECE, UTH, Greece                                  *                                                               
 * 				Author: Vasileiou Christos (1983)             				*
 ****************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "functions.h"
#include "instructions.h"

#include <tcl8.6/tcl.h>
#include <readline/readline.h>
#include <readline/history.h>

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
    
    // commands' flags. //
    r_kernels_enable = 0;
    sharpIsActive = 0;
    sharp_2IsActive = 0;
    foundCommand = 0;
    printf ( YELLOW "Welcome to my custom Tcl Shell!\n" WHITE);
    Tcl_FindExecutable( argv[0] );
                       
    if ( InitInterpreter() == TCL_ERROR )
        return ( EXIT_FAILURE );
    // Readline Initialisation //
    rl_completion_entry_function = NULL; // use rl_filename_completion_function(), the default filename completer //
    rl_attempted_completion_function = custom_completer;
    rl_completion_append_character = '\0';
    using_history(); // initialize history functions //
    commandsCreation();
    while (1)
      {
        text = readline(YELLOW"1983-TclSh> "WHITE);
        
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
            initString( parsingCommand, SIZE_COMMAND);
            for ( searchWhiteSpaces = command; ( searchWhiteSpaces - command ) < strlen(command); searchWhiteSpaces++)
              {
                if ( (isgraph(*searchWhiteSpaces) == 0) && (foundCommand == 1) )
                    break;
                else
                  {
                    foundCommand = 1;    // foundCommand turns its value to true (1) //
                    strcpy( &parsingCommand[i], searchWhiteSpaces );
                    i++;
                  }
              }
            foundCommand = 0;
            strcpy( &parsingCommand[i], "\0");
            i = 0;
            
            // handle two basic commands: history and quit //
            if ( (strcmp ( parsingCommand, "quit" ) == 0) || (strcmp ( parsingCommand, "q" ) == 0)  )
              {
                return EXIT_SUCCESS;
              }
            else if ( strcmp(parsingCommand, "history" ) == 0 )
              {
                get_history();
              }
            else if ( strcmp(parsingCommand, "ls" ) == 0 )
              {
                system (command);
              }
            else if ( strcmp(parsingCommand, "less" ) == 0 )
              {
                system (command);
              }
            else if ( strcmp(parsingCommand, "man" ) == 0 )
              {
                system (command);
              }
            else if ( strcmp(parsingCommand, "touch" ) == 0 )
              {
                system (command);
              }
            else if ( strcmp(parsingCommand, "nano" ) == 0 )
              {
                system (command);
              }
            else if ( strcmp(parsingCommand, "mkdir" ) == 0 )
              {
                system (command);
              }
            else if ( strcmp(parsingCommand, "diff  " ) == 0 )
              {
                system (command);
              }
            else
              {
                if ( strcmp(parsingCommand, "sharp_2") == 0 )
                  {
                    sharp_2IsActive = 1;
                  }
                else 
                  {
                    sharp_2IsActive = 0;
                  }
                if (strcmp(parsingCommand, "sharp") == 0)
                  {
                    sharpIsActive = 1;
                  }
                else 
                  {
                    sharpIsActive = 0;
                  }
                if (strcmp(parsingCommand, "r_kernels") == 0)
                  {
                    r_kernels_enable = 1;
                  }
                else 
                  {
                    r_kernels_enable = 0;
                  }
                // Tcl command handling //
                code = Tcl_Eval ( tcl_interpreter, command );			// Evaluate the command => Execute it //
                objPtr = Tcl_GetObjResult ( tcl_interpreter );			// Get the output of the executed command as a tcl object //
                result = Tcl_GetStringFromObj ( objPtr, NULL );	// Get the string - return value of the executed command //
              }
          }
      }
    freeCubes();
  }

/************** TCL Extended: History, Completion ***************
 *						CE437, Fall 2018                        *
 *						ECE, UTH, Greece                        *                                              
 *                                                              *
 * 				Author: Vasileiou Christos (1983)				*
*****************************************************************/

#include <tcl8.6/tcl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define SIZE_COMMAND LINE_MAX

#define WHITE "\x1B[0m"
#define BLACK "\e[30m"
#define RED "\e[0;91m"
#define GREEN "\e[0;92m"
#define YELLOW "\e[0;93m"

Tcl_Interp *tcl_interpreter ; // Script interpreter //

int NotValid; // check for cubes' validity "00" //
int foundCommand;
char *cubeIntersect_2;
char *cubeSuper_2;
char *cubeCover_2;
int sharp_2IsActive;
char ***cubeSharp;
char *argumentSharp;
int sharpIsActive;
int countSharpRows;
int countSharpArrays;
char **array;
int listArgs;
int cover;

/******************   Function Prototypes   *******************/
char **custom_completer(const char *text, int start, int end);
char *instruction_generator (const char *text, int state);
int InitInterpreter (void);
void get_history ();
void initString( char* ch, int n );
void *commandsCreation();
void *freeCubes();

/***********   My Commands functions' prototypes   *************/
Tcl_ObjCmdProc *cube_intersect_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
Tcl_ObjCmdProc *supercube_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *distance_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *cube_cover_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *sharp_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *my_sharp ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *sharp ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
int checkIfValid ( char *checked, int size );



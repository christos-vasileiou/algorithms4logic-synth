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

int foundCommand;
Tcl_Interp *tcl_interpreter ; // Script interpreter //

/********** global variable definition (2nd hw) ***********/
int NotValid; // check for cubes' validity "00" //
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

/********** global variable definition (3rd hw) ***********/

enum nodeStatus { 
	UNEXPLORED,
	EXPLORED
};
typedef enum nodeStatus nodeStatus_t;

struct nodesDist
{
    int node;
    int dist;
    nodeStatus_t status;
};
typedef struct nodesDist nodesDist_t;

struct arc 
{
    int src;
    int dest;
    int weight;
};
typedef struct arc arc_t;

arc_t *arcs; // all graph's arcs. //
int cntArcs; // number of arcs.   //
int size;    // number of nodes.  //

/****************** My function's prototypes (1st hw)  *******************/
char **custom_completer(const char *text, int start, int end);
char *instruction_generator (const char *text, int state);
int InitInterpreter (void);
void get_history ();
void initString( char* ch, int n );
void *freeCubes();

/***********   My functions' prototypes (2nd hw)  *************/
void *commandsCreation();
Tcl_ObjCmdProc *do_cube_intersect_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
Tcl_ObjCmdProc *do_supercube_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *do_distance_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *do_cube_cover_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *do_sharp_2 ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *my_sharp ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *do_sharp ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
int checkIfValid ( char *checked, int size );

/***********   My functions' prototypes (3rd hw)  *************/
void *do_read_graph ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *initIntArray ( int x, int y, int array [x][y], int val );
void *initInt ( int x, int n[x], int val );
int searchNodes ( int x, int n[x], int n2 ); 
void *printGraph ( int x, int graph [x][x] );
void *do_write_graph ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
void *do_draw_graph ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
nodesDist_t find_Shortest_Explored_Node ( int x, nodesDist_t *d );
int maximum ( int a, int b );
int searchNode ( int x, nodesDist_t *nD, int n2 );
int graphIsNotEmpty ( nodesDist_t *n );
void sortGraph ( int x, nodesDist_t *n );
void *do_graph_critical_path ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );
int minimum (int a, int b);
nodesDist_t *back_trace (nodesDist_t *Q, int arcWeight[size][size], int longest_path, int Rslack, int *previous, int maxDistanceNode, int *slack, nodesDist_t *criticalPath );
void *do_free_graph ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] );



#include <tcl8.6/tcl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "functions.h"

// Create 5 new cubes' commands //
void *commandsCreation()
  {
    /**********************************************************************
     * The TclStubs mechanism defines a way to dynamically bind           *
     * extensions to a particular Tcl implementation at run time.         *
     * This provides two significant benefits to Tcl users:               *
     *  1. Extensions that use the stubs mechanism can be loaded into     *
     *     multiple versions of Tcl without being recompiled or relinked. *
     *  2. Extensions that use the stubs mechanism can be dynamically     *
     *     loaded into statically-linked Tcl applications.                *
     **********************************************************************/

    if (Tcl_InitStubs(tcl_interpreter, TCL_VERSION, 0) == NULL) 
      {
        return NULL;
	  }
    Tcl_CreateObjCommand ( tcl_interpreter, "cube_intersect_2", ( Tcl_ObjCmdProc* ) do_cube_intersect_2, NULL, NULL );
    Tcl_CreateObjCommand ( tcl_interpreter, "supercube_2", ( Tcl_ObjCmdProc* ) do_supercube_2, NULL, NULL );
    Tcl_CreateObjCommand ( tcl_interpreter, "distance_2", ( Tcl_ObjCmdProc* ) do_distance_2, NULL, NULL );
    Tcl_CreateObjCommand ( tcl_interpreter, "cube_cover_2", ( Tcl_ObjCmdProc* ) do_cube_cover_2, NULL, NULL );
    Tcl_CreateObjCommand ( tcl_interpreter, "sharp_2", ( Tcl_ObjCmdProc* ) do_sharp_2, NULL, NULL );
    Tcl_CreateObjCommand ( tcl_interpreter, "sharp", ( Tcl_ObjCmdProc* ) do_sharp, NULL, NULL );
		Tcl_CreateObjCommand ( tcl_interpreter, "read_graph", ( Tcl_ObjCmdProc* ) do_read_graph, NULL, NULL );
    Tcl_CreateObjCommand ( tcl_interpreter, "write_graph", ( Tcl_ObjCmdProc* ) do_write_graph, NULL, NULL );
		Tcl_CreateObjCommand ( tcl_interpreter, "draw_graph", ( Tcl_ObjCmdProc* ) do_draw_graph, NULL, NULL );
		Tcl_CreateObjCommand ( tcl_interpreter, "graph_critical_path", ( Tcl_ObjCmdProc* ) do_graph_critical_path, NULL, NULL );
    Tcl_CreateObjCommand ( tcl_interpreter, "free_graph", ( Tcl_ObjCmdProc* ) do_free_graph, NULL, NULL );
		Tcl_CreateObjCommand ( tcl_interpreter, "alg_division", ( Tcl_ObjCmdProc* ) do_alg_division, NULL, NULL );
    Tcl_CreateObjCommand ( tcl_interpreter, "r_kernels", ( Tcl_ObjCmdProc* ) do_r_kernels, NULL, NULL );
	
  }
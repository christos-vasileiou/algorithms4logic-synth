#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <tcl8.6/tcl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "functions.h"

#define INDICATION YELLOW"%d"WHITE

void * do_read_graph ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] ) 
  {
		if ( objc != 2 )
			{
				fprintf ( stderr,"Too few arguments! Input file is needed!\n");
				return NULL;
			}
		int srcNode, destNode, readSrc, readDest, weight, res, i;
		int *nodes, j;
		char c = '\0';
		FILE *fpReader; 

		char *input;
		
		size = strlen ( Tcl_GetString ( objv [1] ) );
		input = (char*) malloc ( size+1 );
		if ( input == NULL )
			{
				fprintf ( stderr, "Error in malloc\n");
			}
		initString ( input, size+1 );
		strncpy ( input, Tcl_GetString ( objv [1] ), size );
		strncpy ( &input[size], "\0", 1);
		printf ("\ninput file: %s\n", input);
		

		fpReader = fopen( input, "r" ); // read mode
		if (fpReader == NULL)
			{
				perror("Error while opening the file.\n");
				exit(EXIT_FAILURE);
			}
		size = 0;
		cntArcs = 0;
		srcNode = 0;
		destNode = 0; 
		readSrc = 1;
		readDest = 0;
		weight = 0;
		arcs = (arc_t*) malloc ( sizeof (arc_t) );
		if ( arcs == NULL )
			{
				fprintf (stderr, "Error with allocation memory in arcs!\n");
				return NULL;
			}
		arcs [0].src = -1;
		arcs [0].dest = -1;
		arcs [0].weight = -1;
		nodes = (int*) malloc ( sizeof (int) );
		if ( nodes == NULL )
			{
				fprintf (stderr, "Error with allocation memory in arcs!\n");
				return NULL;
			}
		nodes [0] = -1;
		while ( fscanf (fpReader, "%c", &c) != EOF ) // reading the graph file //
			{
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
				* readSrc: specifies that source's node is going      *
				* to be read. After character 'n'                     *
				* * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
				if ( readSrc == 1 && c == 'n' )
					{
						// source node is read. //
						fscanf (fpReader, "%d", &srcNode);
						// change readinng mode. //
						readSrc = 0;
						readDest = 1; 
						// searching if srcNode already exists  
						if ( searchNodes ( size, nodes, srcNode ) == 1 ) 
							{
								// node doesn't exist and allocates memory to stores it. // 
								nodes [size] = srcNode;
								size++;  // is the number of nodes. //
								nodes = (int*) realloc ( nodes, (size+1) * sizeof (int) );
								if ( nodes == NULL )
									{
										fprintf (stderr, "Error with allocation memory in nodes!\n");
										return NULL;
									}
							}
					}
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
				* readDest: specifies that destination's node and weight are going      *
				* to be read. After character 'n'                                       *
				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				else if ( readDest == 1 && c == 'n' )
					{
						// destination node and weight are read. //
						fscanf (fpReader, "%d", &destNode);
						fscanf (fpReader, "%d", &weight);
						// change readinng mode. //
						readSrc = 1;
						readDest = 0;
						// searching if srcNode already exists  
						if ( searchNodes ( size, nodes, destNode ) == 1 )
							{
								// node doesn't exist and allocates memory to stores it. //
								nodes [size] = destNode;
								size++;
								nodes = (int*) realloc ( nodes, (size+1) * sizeof (int) );
								if ( nodes == NULL )
									{
										fprintf (stderr, "Error with allocation memory in nodes!\n");
										return NULL;
									}
							}
					}
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
				* when '\n' is read then struct arcs, stores arc's info *
				* in order to create graph static array.                *
				* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if ( c == '\n')
					{
						arcs [cntArcs].src = srcNode;
						arcs [cntArcs].dest = destNode;
						arcs [cntArcs].weight = weight;
						// printf ( "n%d -> n%d %d\n", arcs [cntArcs].src, arcs [cntArcs].dest, arcs [cntArcs].weight );
						cntArcs++;
						arcs = (arc_t*) realloc ( arcs, (cntArcs+1) * sizeof (arc_t) );
						if ( arcs == NULL )
							{
								fprintf (stderr, "Error with allocation memory in arcs!\n");
								return NULL;
							}
					}
			}

		if ( size == 0 )
			{
				// printf ("Something goes wrong with graph's size: %d\n", size);
				return NULL;
			}

		int graphInterconnection [size][size];
		// printf ( "Nodes: %d\n", size );
		// printf ( "Arcs: %d\n", cntArcs );
		initIntArray ( size, size, graphInterconnection, 0 );
		
		for ( i = 0; i < cntArcs; i++)
			{
				graphInterconnection [arcs [i].src] [arcs [i].dest] = arcs [i].weight;
			}

		printGraph ( size, graphInterconnection );

		res = fclose (fpReader);
		if ( res != 0)
			{
				perror ("Error to closing fpReader\n");
				return NULL;
			}
		
		// free (arcs);
		free (nodes);
		free (input);
		return NULL;
  }

void *initIntArray ( int x, int y, int array [x][y], int val )
  {
		int i, j;
		for ( i = 0; i < x; i++ )
	  	{
				for ( j = 0; j < y; j++ )
		  		{
						array [i][j] = val;
		  		}  
	  	}
		return NULL;
  }

void *initInt ( int x, int n[x], int val )
	{
		int i;

		for ( i = 0; i < x; i++ )
			{
				n [i] = val;
			}
		return NULL;
	}

int searchNodes ( int x, int n[x], int n2 )
  {
		int i;

		for ( i = 0; i < x; i++ )
	  	{
				if ( n [i] == n2 )
		  		{
						return 0;
		  		}
	  	}

		return 1;

  }

void *printGraph ( int x, int graph [x][x] )
  {
		int i, j;

		printf ("Nodes: ");
		for ( i = 0; i < x; i++)
	  	{
				printf (" %d ", i);
	  	}
		printf ("\n");
		for ( i = 0; i < x; i++)
	  	{
				if ( i < 10 )
		  		{ 
						printf (WHITE"    %d: ", i);
		  		}
				else if ( i < 100)
		  		{
						printf (WHITE"   %d: ", i);
		  		}
				for ( j = 0; j < x; j++)
		  		{
			
						if ( graph [i][j] != 0 )
			  			{
								printf (" "GREEN"%d"WHITE" ", graph [i][j] );
			  			}
						else 
			  			{
								printf (" "WHITE"%d"GREEN" ", graph [i][j] );
			  			}
		  		}
				printf ("\n");
	  	}
			printf (WHITE"\n");
			
		return NULL;
  }



void *do_write_graph ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] ) 
  {
		if ( objc != 2 )
			{
				fprintf ( stderr, "Wrong arguments number\n");
				return NULL;
			} 
		int i, j, sizeArg, res;
		char *output; 
		char *format; // format is printed in the output file. //
		FILE *fpWriter;
		
		sizeArg = strlen ( Tcl_GetString ( objv [1] ) );
		output = (char*) malloc ( sizeArg+1 );
		if ( output == NULL )
			{
				fprintf ( stderr, "Error in malloc\n");
				return NULL;
			}
		initString ( output, size+1 );
		strncpy ( output, Tcl_GetString ( objv [1] ), sizeArg );
		strncpy ( &output[sizeArg], "\0", 1);
		printf ("\noutput file: %s\n", output);
		
		fpWriter = fopen( output, "w+" ); // write and creation mode
		if ( fpWriter == NULL )
			{
				perror("Error while opening the file.\n");
				exit(EXIT_FAILURE);
			}
			
		format = (char*) malloc ( (15+1) * sizeof(char) );
		for ( i = 0; i < cntArcs-1 ; i++ )
			{
				res = snprintf ( format, 15+1, " n%d -> n%d %d\n", arcs [i].src, arcs [i].dest, arcs [i].weight);
			
				if ( res < 0 )
					{
						fprintf ( stderr, "Error occured in snprintf\n");
						return NULL;
					}

				fprintf ( fpWriter, "%s",  format );
				printf ( "%s", format);
			}
		res = fclose ( fpWriter );
		if ( res != 0)
			{
				perror ("Error to closing fpReader\n");
				return NULL;
			}

		free (format);
		free (output);
		return NULL;
	}



void *do_draw_graph ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] )
  {
    if ( objc < 2 )
			{
				fprintf ( stderr, "Wrong argument's number!\n");
				return NULL;
			}

    int sizeObj1 = 0, sizeFormat = 0, res, i;
		char *drawing, *format;
		char command [LINE_MAX];
		FILE *fpWriter;

		fpWriter = fopen( "draw.dot", "w+" ); // write and creation mode
		if ( fpWriter == NULL )
			{
				perror("Error while opening the file.\n");
				exit(EXIT_FAILURE);
			}

		// "digraph {\n" expression has 10 characters. //
		fprintf ( fpWriter, "digraph {\n" );
		fprintf ( fpWriter, "  node [fontsize=18, fontcolor=\"red\"];\n");
		format = (char*) malloc ( (45+1) * sizeof(char) );
		for ( i = 0; i < cntArcs-1 ; i++ )
			{
				res = snprintf ( format, 45+1, "  n%d -> n%d [label=\"%d\", weight=\"%d\"];\n", arcs [i].src, arcs [i].dest, arcs[i].weight, arcs[i].weight );
			
				if ( res < 0 )
					{
						fprintf ( stderr, "Error occured in snprintf\n");
						return NULL;
					}
				fprintf ( fpWriter, "%s",  format );
			}
		
		fprintf ( fpWriter, "}\n" );
		
		res = fclose ( fpWriter );
		if ( res != 0)
			{
				perror ("Error to closing fpReader\n");
				return NULL;
			}

		sizeObj1 = strlen ( Tcl_GetString ( objv[1]) );
		drawing = (char*) malloc ( (sizeObj1+1) * sizeof(char) );
		
		strncpy ( drawing, Tcl_GetString (objv[1]), sizeObj1 );
		strncpy ( &drawing [sizeObj1], "\0", 1);
		
		sprintf ( command, "dot -Tpng draw.dot -o %s \n", drawing );
		system ( command );

		free (format);
		free (drawing);
		printf (GREEN"SUCCESS!"WHITE" \n");
		return NULL;
	}



nodesDist_t find_Shortest_Explored_Node ( int x, nodesDist_t *d )
	{
		int i, temp = INT_MAX, pos = -1;
		for (i = 0; i < x; i++ )
			{
				if ( d[i].status == EXPLORED && 
						 d[i].dist < temp && 
						 d[i].node > -1 )
					{
						temp = d[i].dist;
						pos = i;
					}
			}
		// printf ("  d["INDICATION"].node: "INDICATION", d["INDICATION"].dist: "INDICATION", d["INDICATION"].status: "INDICATION" \n",
		//  pos, d[pos].node, pos, d[pos].dist, pos, d[pos].status );
		return d[pos];
	}

int maximum ( int a, int b )
	{
		if ( a >= b )
			{
				return a;
			}
		else
			{
				return b;
			}
	}

int searchNode ( int x, nodesDist_t *nD, int n2 )
  {
		int i;
		for ( i = 0; i < x; i++ )
	  	{
				if ( nD[i].node == n2 )
		  		{
						return 0;
		  		}
	  	}
		return 1;
  }

void sortGraph ( int x, nodesDist_t *n )
	{
		int i, j;
		nodesDist_t temp1, temp2;
		temp1.node = -1;
		temp1.dist = -1;
		temp1.status = UNEXPLORED;
		temp2.node = -1;
		temp2.dist = -1;
		temp2.status = UNEXPLORED;
		for ( i = 0; i < x; i++ )
			{
				if ( i != n[i].node )
					{
						temp1 = n[i];
						for ( j = 0; j < x; j++)
							{
								if ( j == temp1.node )
									{
										temp2 = n[j];
										n[j] = temp1;
										if ( i == temp2.node )
											{
												n[i] = temp2;
											}
										break;
									}
							}
					}
			}
	}

int graphIsNotEmpty ( nodesDist_t *n )
	{
		int i, cntEmptyNodes;

		cntEmptyNodes = 0;
		for ( i = 0; i < size; i++)
			{
				// n[i] includes positive nodes' numbers. // 
				if ( n[i].node < 0 )
					{
						cntEmptyNodes++;
					}
			}
		if ( cntEmptyNodes == size )
			return 0;
		else
			return i;
	}

void *initGraph ( int x, nodesDist_t *g )
	{
		int i;
		for ( i = 0; i < x; i++ )
			{
				if ( g[i].node > 0 && g[i].node < size )
					{
						g[i].node = -1;
						g[i].dist = -1;
						g[i].status = UNEXPLORED;
					}
			}
		return NULL;
	}

void *do_graph_critical_path ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] )
  { 
		int i, j, k;
		int arcWeight [size][size], predecessors [size], successors [size], previous [size];
		int cnt, cntGraphSize, cntStoredNodes, cntStoredInputs;
		int exploredNodes = 1;
		nodesDist_t shortestNode, *Q;
		nodesDist_t *graphNodes;
		int longest_path = 0, maxDistanceNode;
		int pos2, relaxing, maxPathCnt = 0;
		nodesDist_t *maxPath;
		int Rslack, *slack;
		nodesDist_t *criticalPath;
		
		if ( objc > 2 )
			{
				fprintf ( stderr, "Wrong arguments' number!\n");
				return NULL;
			}

		initIntArray ( size, size, arcWeight, 0 );
		// storing all arcs' weights. //
		for ( i = 0; i < cntArcs; i++)
			{
				arcWeight [arcs [i].src] [arcs [i].dest] = arcs [i].weight;
			}
		// storing all nodes of the graph. //
		cntGraphSize = 0;
		graphNodes = (nodesDist_t*) malloc ( sizeof(nodesDist_t) );
		if ( graphNodes == NULL )
			{
				fprintf ( stderr, "Error in nodes' allocation: %d", errno );
				return NULL;
			}

		for ( i = 0; i < cntArcs; i++ )
			{
				
				//printf ( "i: "INDICATION", src: "INDICATION", dest: "INDICATION", cntGraphSize: "INDICATION" \n", 
				//           i, arcs[i].src, arcs[i].dest, cntGraphSize );
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
				 * searchNode searches in graphNodes (where cnt is *
				 * the current size) if there is the arcs[i].src node. *
				 * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
				if ( searchNode ( cntGraphSize, graphNodes, arcs [i].src ) == 1 )
					{
						++cntGraphSize;//arcs[i].src + 1;
						graphNodes = (nodesDist_t*) realloc ( graphNodes, cntGraphSize * sizeof(nodesDist_t) );
						if ( graphNodes == NULL )
							{
								fprintf ( stderr, "Error in nodes' allocation: %d", errno );
								return NULL;
							}
							
						graphNodes [cntGraphSize-1].node = arcs [i].src;
						graphNodes [cntGraphSize-1].dist = -1;    // each node has unknown distace. //
						graphNodes [cntGraphSize-1].status = UNEXPLORED;
						// cntStoredNodes++;
					}
				//printf ( "i: "INDICATION", src: "INDICATION", dest: "INDICATION", cntGraphSize: "INDICATION" \n", 
				//         i, arcs[i].src, arcs[i].dest, cntGraphSize );
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
				 * searchNode searches in graphNodes (where cnt is *
				 * the current size) if there is the arcs[i].dest node.*
				 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if ( searchNode ( cntGraphSize, graphNodes, arcs [i].dest ) == 1 )
					{
						++cntGraphSize;
						graphNodes = (nodesDist_t*) realloc ( graphNodes, cntGraphSize * sizeof(nodesDist_t) );
						if ( graphNodes == NULL )
							{
								fprintf ( stderr, "Error in nodes allocation: %d", errno );
								return NULL;
							}
						graphNodes [cntGraphSize-1].node = arcs [i].dest;
						graphNodes [cntGraphSize-1].dist = -1;    // each node has unknown distace. //
						graphNodes [cntGraphSize-1].status = UNEXPLORED;
						// cntStoredNodes++;
					}
			}

		sortGraph ( cntGraphSize, graphNodes );

		// Initializations. //
		initInt ( size, predecessors, 0 );
		initInt ( size, successors, 0 );
		initInt ( size, previous, 0 );

		for ( i = 0; i < size; i++ ) 
			{
				for ( j = 0; j < size; j++ )
					{
						if ( arcWeight [i][j] != 0 )
							{
								predecessors [j]++; // predecessors of node j. //
								successors [i]++;   // successors of node i. //
								previous [j]++; 
							}
						// printf ("predecessors [%d]: %d\n", j, predecessors[j] );
					}
				// printf ("node[%d]: %d, dist: %d, successors: %d\n", cnt-1, graphNodes [cnt-1].node, graphNodes [cnt-1].dist, successors [i] );
			}

		// create a queue for storing explored nodes. //
		Q = (nodesDist_t*) malloc ( sizeof(nodesDist_t) );
		if ( Q == NULL )
			{
				fprintf ( stderr, "Error in queue' allocation: %d", errno );
				return NULL;
			}


		cntStoredInputs = 0;
		for ( i = 0; i < cntGraphSize; i++ )
			{
				// find all inputs. //
				if ( predecessors [i] == 0 )
					{
						// distance from an input to the same input. //
						graphNodes [i].dist = 0;
						graphNodes [i].status = EXPLORED;
						++cntStoredInputs;
						Q = (nodesDist_t*) realloc ( Q, cntStoredInputs * sizeof(nodesDist_t) );
						if ( Q == NULL )
							{
								fprintf ( stderr, "Error in queue' allocation: %d", errno );
								return NULL;
							}

						Q[cntStoredInputs-1].node = graphNodes [i].node;
						Q[cntStoredInputs-1].dist = graphNodes [i].node;
						Q[cntStoredInputs-1].status = graphNodes [i].status;
					}
			}

		//printf ("node: %d with distance: %d\n", graphNodes [i].node, graphNodes [i].dist );
		
		for ( i = 0; ( i < cntGraphSize && graphIsNotEmpty ( graphNodes ) > 0 ); i++ )
			{
				// find the shortest node in explored nodes. //
				shortestNode = find_Shortest_Explored_Node ( cntGraphSize, graphNodes );

				if ( shortestNode.dist == -1 )
					{
						break;
					}

				// remove the shortest node and mark it as explored (-1). // 
				for ( cnt = 0; cnt < cntGraphSize; cnt++ )
					{
						if ( graphNodes [cnt].node == shortestNode.node )
							{
								graphNodes [cnt].node = -1;
								graphNodes [cnt].status = EXPLORED;
							}
					}

				for ( j = 0; ( (j < cntGraphSize) && ( successors [shortestNode.node] > 0 ) ); j++ )
					{
						// if there is an arc from shortest node to j.
						if ( arcWeight [shortestNode.node][j] > 0 )
							{
								// found 1 successor of shortest node. //
								successors [shortestNode.node]--;
								graphNodes [j].dist = maximum ( graphNodes [j].dist, 
									(shortestNode.dist + arcWeight [shortestNode.node][j] ) ) ;

								// reduce by 1 the predecessors of node j, because shortest node is its predecessor.//
								predecessors [j]--;
								if ( predecessors [j] == 0 )
									{
										Q = (nodesDist_t*) realloc ( Q, (exploredNodes + 1) * sizeof (nodesDist_t) );
										Q [exploredNodes].node = j;
										Q [exploredNodes].dist = graphNodes [j].dist;
										Q [exploredNodes].status = EXPLORED;
										graphNodes [j].status = EXPLORED;
										//printf ( "      Q["INDICATION"].node: "INDICATION", Q["INDICATION"].dist: "INDICATION", Q["INDICATION"].status: "INDICATION" \n\n", 
										//  exploredNodes, Q[exploredNodes].node, exploredNodes, Q[exploredNodes].dist, exploredNodes, Q[exploredNodes].status);
										exploredNodes++;
									}
							}
					}
			}

		for ( i = 0; i < exploredNodes; i++ )
			{
				if ( Q[i].dist >= longest_path )
					{
						longest_path = Q[i].dist;
						maxDistanceNode = Q[i].node;
					}
			}
		
		printf ("\n" );
		maxPath = (nodesDist_t*) calloc ( cntGraphSize, sizeof(nodesDist_t) );
		if ( maxPath == NULL )
			{
				fprintf ( stderr, "Error in maxPath' allocation: %d", errno );
				return NULL;
			}

		int pos = maxDistanceNode;
		printf ("Critical path's nodes are: ");
		// pos is the node with the maximum distance in the graph. // 
		while ( previous [pos] > 0 )
			{
				// In maxPath are stored nodes of critical path. //
				maxPath [maxPathCnt] = Q[pos];
				relaxing = 0;
				for ( i = 0; i < cntGraphSize; i++ )
					{
						if ( arcWeight [i][pos] > 0 )
							{
								if ( Q[i].dist >= relaxing )
									{
										relaxing = Q[i].dist;
										pos2 = Q[i].node;
									}
							}
					}
				printf ( INDICATION" <- ", maxPath [maxPathCnt].node );
				maxPathCnt++;
				pos = pos2;
			}
		printf ( INDICATION" \n", maxPath [maxPathCnt].node );
		printf ("Critical path's length is "INDICATION" \n", longest_path );


		slack = (int*) malloc ( size * sizeof(int) );
		if ( slack == NULL )
			{
				fprintf ( stderr, "Error in slack' allocation: %d", errno );
				return NULL;
			}
		criticalPath = (nodesDist_t*) malloc ( sizeof(nodesDist_t) );
		if ( criticalPath == NULL )
			{
				fprintf ( stderr, "Error in slack' allocation: %d", errno );
				return NULL;
			}

		if ( objc == 1 )
			{
				Rslack = 0;
			}
		else if ( Tcl_GetIntFromObj ( interp, objv[1], &Rslack ) == TCL_ERROR )
			{
				fprintf ( stderr, "Error in converting to int %d\n", errno );
				free (slack);
				free (criticalPath);
				free (maxPath);
				free (Q);
				free (graphNodes);
				free (arcs);
				return NULL;
			}
		
		criticalPath = back_trace ( Q, arcWeight, longest_path, Rslack, previous, maxDistanceNode, slack, criticalPath );
		
		free (slack);
		free (criticalPath);
		free (maxPath);
		free (Q);
		free (graphNodes);
		free (arcs);
		return NULL;
	}

int minimum (int a, int b)
	{
		if (a <= b)
			return a;
		else
			return b;
	}

nodesDist_t *back_trace (nodesDist_t *Q, int arcWeight[size][size], int longest_path, int Rslack, int *previous, int maxDistanceNode, int *slack, nodesDist_t *criticalPath )
	{
		int v, a, i, cntQueue = 0, cntCriticalPath = 0;
		nodesDist_t *queue;
		queue = (nodesDist_t*) malloc ( sizeof(nodesDist_t) );
		if ( queue == NULL )
			{
				fprintf ( stderr, "Error in queue' allocation: %d", errno );
				return NULL;
			}

		for ( i = 0; i < size; i++ )
			{
				slack[i] = longest_path; // initialize with cost od longest path. //
			}

		slack [maxDistanceNode] = Rslack;
		criticalPath [cntCriticalPath].node = maxDistanceNode;
		queue [cntQueue].node = maxDistanceNode;
		cntQueue++;
		cntCriticalPath++;
		printf ( "Required slack: "INDICATION" \n", Rslack );
		while ( cntQueue > 0 ) // while queue is not empty 
			{
				// dequeue. //
				v = queue[0].node;
				cntQueue--;

				// a runs all nodes. // 
				for ( a = 0; a < size; a++ ) 
					{
						// for v's predecessors. //
						if ( arcWeight [a][v] > 0 ) 
							{
								slack[a] = minimum ( slack[a], slack[v] + Q[v].dist - ( Q[a].dist + arcWeight [a][v] ) );
								if ( slack [a] == Rslack )
									{
										// add Q[a] in the queue. //
										queue = (nodesDist_t*) realloc (queue, (cntQueue+1) * sizeof(nodesDist_t));
										if ( queue == NULL )
											{
												fprintf ( stderr, "Error in queue' allocation: %d", errno );
												return NULL;
											}
										queue [cntQueue] = Q[a];
										cntQueue++;
										// add Q[a] in the critical path queue. //
										criticalPath = (nodesDist_t*) realloc (criticalPath, (cntCriticalPath+1) * sizeof(nodesDist_t));
										if ( criticalPath == NULL )
											{
												fprintf ( stderr, "Error in queue' allocation: %d", errno );
												return NULL;
											}
										criticalPath [cntCriticalPath] = Q[a];
										cntCriticalPath++;
									}
							}
					}
			}
		
		for ( i = 0; i < size; i++ )
			{
				printf ("slack["INDICATION"] : "INDICATION" \n", i, slack[i] );
			}

		free (queue);
		return criticalPath;
	}

void *do_free_graph ( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] )
	{
		free (arcs);
	}
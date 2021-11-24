
SRC = customTCL.c instructions.h

LDFLAGS= -lm -ltcl -lreadline -lhistory # Linker Flags
DIRS = -I/usr/local/include/tcl8.6 -I/usr/local/include/boost_1_33_0

customTCL: $(SRC) 
	gcc -g $< -o $@ $(LDFLAGS) $(DIRS)

delete:
	rm customTCL

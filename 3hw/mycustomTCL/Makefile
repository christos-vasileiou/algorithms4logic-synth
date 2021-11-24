
SRC = customTCL.c
OBJ = functions_1st.o functions_2nd.o functions_3rd.o commandsCreation.o

LDFLAGS= -lm -ltcl -lreadline -lhistory # Linker Flags
DIRS = -I/usr/local/include/tcl8.6 -I/usr/local/include/boost_1_33_0

customTCL: $(SRC) $(OBJ) functions.h instructions.h
	gcc -g $^ -o $@ $(LDFLAGS) $(DIRS)

commandsCreation.o: commandsCreation.c  
	gcc -c $< -fPIC -o $@

functions_1st.o: functions_1st.c  
	gcc -c $< -fPIC -o $@
	
functions_2nd.o: functions_2nd.c  
	gcc -c $< -fPIC -o $@

functions_3rd.o: functions_3rd.c  
	gcc -c $< -fPIC -o $@

myCommands.o: myCommands.c  
	gcc -c $< -fPIC -o $@

clean:
	rm -f customTCL *.o

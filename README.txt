################ PASSING DATA BETWEEN PROCESSES ################
The program concerns passing data between two different processes and comparing the time needed to compute the action.

Processes are called:
	> producer
	
	> consumer
	
and they have to:
	> (producer) write on a buffer A
	
	> (consumer) read from a buffer B
	

The dimension of the buffer is chosen by the user at the beginning of the execution and it does not change till the program is executed again (so it can be chosen again).

################ PROGRAM PREPARATION ################
1 - type on a shell '$ ./install.sh <path name>' and press enter. The <path name> has to be the name of a folder (the folder will be created inside the actual folder, so do not give a gloabl path since the next step will not be done succesfully);

1.1 - check that, once the install.sh has completed, a message will be shown on the shell;

2 - type '$ ./help.sh' and read the brief description below for each process to understand the general purpose and structure of the program;

Now everything is ready to start the process ..

3 - type  '$ ./run.sh' and, if everything has been done correctly, the program will start automatically.

To be sure the program is started correctly you should see:
- 2 consoles, named inspection and command console, waiting for some inputs;
- the main shell waiting for command too.

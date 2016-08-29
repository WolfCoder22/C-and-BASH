# CS50 Summer 2016 Final project- Maze solver
## By Alexander Wolf, Galen Brown, and Michael Ortiz
## Sheperded by Maksim Bolonkin

### General Descriptions:

Usage:
* Given valid paramters, AMStartup connects to the server to obtain  maze port 
and dimmensions of a specific maze. After initializing the avatars, the program 
communicates back and forth with the maze server. When a successful message is
received from the server, it is passed to the function alg(), which determines 
the direction a specific avatar will go. The maze is displayed after all the 
avatars are initialized and after each move by calling the function drawMaze().
	
Inputs:
* Number of Avatars (1-10)
* Difficulty (0-9)
* Hostname

Output:
* Display of Maze
* Logfile
* Errors to Stdout, if any

Exit Status:
* 0 - success
* 1 - Invalid number of arguments
* 2 - Invalid hostname/Error connecting to server
* 3 - Error sending message to server
* 4 - Error reading message to server
* 5 - Server has sent an error message
* 6 - Error creating thread(s)


### Compiling instructions:
make

### Example usage:

* ./AMStartup 1 3 flume.cs.dartmouth.edu
* ./AMStartup 4 6 flume.cs.dartmouth.edu
* ./AMStartup 3 8 flume.cs.dartmouth.edu


### Assumptions:
* User must use a mac with xqartz (downloadable at https://www.xquartz.org/)
* User access server using ssh format:
  ssh -Y -2 user@flume.cs.dartmouth.edu
* Number of Avatars will remain at 10 or fewer.
* If this error occurs
	 WARING **; Couldn't connect to accessibility bus: Failed to connect to socket
	 run command: export NO_AT_BRIDGE=1 in terminal

	

### Non-Assumptions:

* Server is in charge of checking that the number of Avatars and difficulty
are valid.

### Known Limitations:
* GUI uses GTK, which requires the computer setup described in Assumptions

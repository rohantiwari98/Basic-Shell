# Basic-Shell
Basic command line shell implemented in C 
To compile the files, run the 'make' command in the directory.

After compilation, start the command line by running './commandline'.

Built in functions: ->cd ->pwd ->echo

Notes: Please do not use tab spaces to separate arguments or commands.The program will throw errors.

Use spaces to separates the command from its arguments and a semmicolon ';' to seperate different commands.

The programs supports background processes, a '&' has to be added to the end of the command.User will be able to run the next command while the background process is running.

The user need NOT wait for the shell prompt immediately after the background process is started.

The program does not support flags for cd, pwd or echo.

echo does not support escaping, as it directly prints the given argument(s).

cd supports the use of .. and . and ~ .

Decisions made: Fork and use 'execvp' to run commands except cd, pwd and echo, Files: commandline.c contains the main function which the prints the shell prompt. commandlinefunctions.c contains other functions which the main function calls.

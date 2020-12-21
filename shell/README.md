# Shell program in C language 

A shell is a special user program which provides an interface to the user to use operating system services. 
Shell accept human readable commands from the user and convert them into something which kernel can understand.
It is a command language interpreter that execute commands read from input devices such as keyboards or from files.

## Description

simple shell implements some internal and external commands, like {
  "echo", "environ" , "dir" , "clr" , "quit" , "pause" , "cd" ,"help"
}
It supports I/O redirection, background execution and it accepts commands from a batchfile or from the console

## Installation

- Any IDE that support C language.
- Using Mac or Linux Terminal.

## Command Details 

   - clr : Clear the screen.
   - quit: Quit the shell.
   - pause: Pause operation of the shell until “Enter” is pressed.
   - dir <directory>: List the contents of directory <directory>.
   - echo <comment> : Display <comment> on the display
   - cd <directory>: Change the current default directory to <directory>.
   - environ: List all the environment strings.
   - help: Display the user manual using --More-- filter.
   
## Compiling 

     gcc ./main.c -o shell   

## Running
   - for commands from batchfile:
   
          ./shell batchfile.txt
          
   - for commands from console, simply write:
   
         ./shell
## Authors
  *  Me (IKhlasJihad)
  *  [Hadeel Al-Maqadma](https://github.com/hadeelmaqadma/)

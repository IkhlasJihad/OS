#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>  
#include <sys/wait.h>
#include <fcntl.h>
#define FAIL     1
#define SUCCESS  0
#define MAX_WORD 100
#define MAX_CHAR 1024
#define SPACE_DELIMITER " "
int  backgroundFlag = 0;
int  in_redirection = 0; 
int  out_redirection = 0; 
int  dup_position = 0;
bool dupout_redirection= false ; 
bool file_flag = false;
int  dup_flage = 0 ; 
char * in_file = NULL ; 
char *out_file = NULL ; 
char cwd[1024];
char * username;
void clr(char** argv){
	pid_t pid = fork();
	if(pid < 0)
		perror("\n **** Failed :( ****\n");
	else if(pid == 0) {
		execvp(argv[0],argv);
    //Successful exec shouldn't return here
	} else // parent
    if(!backgroundFlag){
		  wait(NULL);
    }
}
void help(){
  pid_t pid = fork();
  if(pid < 0){
    perror("\n **** Failed :( ****\n");
  }
  else if(pid == 0) {
    char *argv[3];
    argv[0] = "man";
    argv[1] = "./manual";
    argv[2] = NULL;
		execvp(argv[0], argv);
	  perror("\nExecuting command Failed :(\n"); 
    //Successful exec shouldn't return here
	} else // parent
    if(!backgroundFlag){
		  wait(NULL);
    }
}
void dir(char **parsed){
  int pid = fork();
  if(pid < 0){
    perror("\n **** Failed :( ****\n");
    return;
  }
  else if(pid == 0){
    if( parsed[1]== NULL || strcmp(parsed[1],">" ) == 0
      || strcmp(parsed[1],">>" ) == 0 || strcmp(parsed[1],"&" ) == 0 )
        execlp("/bin/ls","ls",NULL);
    else
        execlp("/bin/ls","ls",parsed[1],NULL);
  }
  else if(pid > 0) {
      if(!backgroundFlag)
         wait(NULL);  
  }
}


void displayEnv(char **paresd){
 /* printf("username = %s\nhostname = %s\npathname = %s\nhome = %s\n",
       getenv("USER"),getenv("HOSTNAME"),getcwd(cwd, sizeof(cwd)),getenv("HOME"));*/
  
  pid_t pid= fork();
  if(pid < 0)
		perror("\n **** Failed :( ****\n");
  else if(pid == 0 ){
    char *args[2];
    args[0] = "env";
    args[1] = NULL; 
    execvp(args[0], args) ; 
  }
  else{
   wait(NULL); 
  }
}

void echo(char **parsed){
  pid_t pid = fork();
  if(pid < 0){
    perror("\n **** Failed :( ****\n");
  }
  else if(pid == 0){
    char *args[MAX_WORD];
    int flag = 0;
    if(in_redirection > 0)
      flag = in_redirection;
    else if (out_redirection > 0)
      flag = out_redirection;    
    if(dup_position > 0)
      flag = dup_position;
    char *comment;
    int i = 0;
    if(flag == 0){
        while(parsed[i] != NULL && strcmp(parsed[i],"&") != 0){
          args[i] = parsed[i];
          i++;
        }
    }
    else{
        while(parsed[i] != NULL && i < flag && strcmp(parsed[i],"&") != 0){
          args[i] = parsed[i];
          i++;
        }
    }
    args[i] = NULL;
    execvp(args[0],args);
  }
  else {
    if(!backgroundFlag)
      wait(NULL); 
  }
}

int execDirect(char **parsed){
  const char *CommandsList[] = {"clr", "cd", "pause","quit", "help", "dir", "environ", "echo"};
  int cmdNumber = 0;
	for(int i = 0; i < 8; i++){
		if(strcmp(parsed[0], CommandsList[i]) == 0){
			cmdNumber = i+1;
			break;	
		}
	}
  switch(cmdNumber){
    case 1: parsed[0] = "clear";
            clr(parsed);
            break;
    case 2: if(parsed[1] == NULL){
              printf("%s\n", cwd);
              break;
            }    
            chdir(parsed[1]);    
            if(file_flag){
              getcwd(cwd, sizeof(cwd));	
              printf("cwd : %s\n" ,cwd);
            }
            break;
    case 3: printf("Press [Enter] to continue .... ");
            while( getchar() != '\n'){}  
            printf("Let\'s continue ..\n");
            break;
    case 4: printf("myshell terminated!\n");
            exit(SUCCESS);
    case 5: help() ; 
            break;
    case 6: dir(parsed) ; // acceptes out redirection > or >> 
            break; 
    case 7: displayEnv(parsed); //acceptes out redirection > or >>
            break;
    case 8: echo(parsed); //acceptes out redirection > or >>
            break; 
    default: printf("illegal command");break; 

  }
  return FAIL;
}

//Reseting the flages
void resetFlags(){
  in_redirection =0 ; 
  out_redirection =0 ; 
  dupout_redirection = false ;
  backgroundFlag = 0; 
}

//executing stage 
void execute(char **argv){
   int output = 0;
   int input = 0;
   int save_out = 0;
   int save_in = 0 ; 
   /*Opening files for redirection*/
   if(dupout_redirection  || out_redirection > 0){
      if(out_redirection > 0 && out_file != NULL){
        output = open(out_file, O_WRONLY|O_CREAT| O_TRUNC , 0777);
      }
      else if (dupout_redirection  && out_file != NULL){
        output = open(out_file, O_WRONLY|O_CREAT| O_APPEND , 0777);
      }
      if(output == -1 ){
        perror("\nCANNOT OPEN OUTPUT FILE\n");
        return ; 
      }
      else{
        save_out = dup(fileno(stdout));
        int dup = dup2(output, 1);
        if(dup== -1 ){
          perror("\nCANNOT REDIRECT OUTPUT\n");
          return;
        } 
      }
} 
/*if(in_redirection >0 && in_file != NULL) {
  input= open(in_file, O_RDONLY|O_CREAT , 0777);
  if(input == -1 ){
        perror("\nCANNOT OPEN INPUT FILE\n");
        return ; 
      }
      else{
          save_in = dup(fileno(stdin));
          int dup =dup2(input, 0);
          if(dup == -1 ){
              perror("\nCANNOT REDIRECT INTPUT\n");
              return;
          } 
      }
} */
   /*execute commands*/
   execDirect(argv); 
   fflush(stdout); 
   /*after output redirection done, redirect to stdout */
  if(output > 0){
    dup(fileno(stdout));
    if( dup2(save_out, fileno(stdout)) == -1 ){
      perror("cannot redirect stdout"); 
      return;
    }
    close(save_out);
  }
  resetFlags(); 
  }

/*to display the prompt*/
void displayPrompt(){	
  username = getenv("USER"); //getenv("USERNAME"); machine dependent
	getcwd(cwd, sizeof(cwd));	
	printf("%s@%s~$", username, cwd);
  return;
}

void parse_line(char *args[] , char line[]){
    int i = 0; 
    args[i] = strtok(line, SPACE_DELIMITER); 
    while(args[i] != NULL){
       args[++i] = strtok( NULL ,SPACE_DELIMITER);  
       // null here means , the same line as above ; 
     }
    return ;
    
}
//checking validation of line(no multiple input/output Redirecction and so on) ;
int checkLine(char * args[]) {
   int i= 0 ; 
   int inRe_count =0 ;
   int outRe_count=0 ;
   int dupRe_count=0;
   if(args[0] == NULL) {
      perror("NO COMMAND") ; 
   } 
   while(args[i] != NULL) {
      if(strcmp(args[i] , "<") == 0 ){
        inRe_count++;
      }
      else if(strcmp(args[i],">>") == 0){
        dupRe_count = 1;
        dupout_redirection = true; 
      }        
      else if(strcmp(args[i],">") == 0){
        outRe_count++;
      }
      else if (strcmp(args[i] , "&") == 0 ) {
        if(args[i+1] != NULL){
          printf("illigal command");
          return FAIL; 
        }
        else
          backgroundFlag = 1;
      }
      i++;
   }
   if( inRe_count > 1 || outRe_count > 1 || 
      (dupRe_count == 1 && outRe_count>0 ) ) {
      printf("illigal command") ;
      return FAIL;
   }
   return SUCCESS; 
}
//Finding the position of flags 
void FindRedirection(char * args[]){
  int i = 0 ; 
  while(args[i] != NULL){
    if(strcmp(args[i],">") == 0){// if there this output redirection 
        out_redirection = i ; 
        out_file = args[i+1]; 
    }
    else if(strcmp(args[i],">>") == 0){
         // dupout_redirection = true;
          dup_position = i;
          out_file = args[i+1]; 
      }
    else if (strcmp(args[i] , "<" ) == 0){// input redirection
     in_redirection=  i ;
     in_file = args[i+1] ; 
    }
    i++ ; 
  }
}

int parsingAndChecking(char line[], char *args[]){
  parse_line(args, line) ; 
  if(checkLine(args) == SUCCESS)   
    FindRedirection(args);
  else{
    return FAIL ; 
  }
  return SUCCESS; 
} 

//remove the new line character from the end of line
void remove_EndOfLine(char line[]){
   int i=0 ; 
   while(line[i] != '\n' ){
     i++; 
   }
   line[i] = '\0'; 
}

//reading single line 
int read_line(char line[], FILE *file){ 
  char * ch= fgets(line, MAX_CHAR, file) ; 
  remove_EndOfLine(line); 
  if(strcmp(line, "exit") == 0)
      exit(0); 
  if(ch == NULL){
      return FAIL; 
  }
  return SUCCESS ; 
}

/*reading commands from stdin*/
int liveRun(){
  while(1){
    char inputline[MAX_CHAR];   // line to be executed
    char *parsedArgs[MAX_WORD]; // array of space separated words of the line
	  displayPrompt();
	  if(read_line(inputline, stdin) == FAIL)
		  continue;
    if(parsingAndChecking(inputline, parsedArgs) == FAIL){
       printf("SOME ERROR OCCURED");
       continue; 
    }
    else{
      execute(parsedArgs); 
    }
  }
  return 0;
}

/*reading commands from specific file */
void readbatch(FILE *file){
  char inputline[MAX_CHAR];
  char *parsedArgs[MAX_WORD];
  while(read_line(inputline, file) != FAIL){
    printf("Retrieved line: %s\n", inputline);
    sleep(2);
    if(parsingAndChecking(inputline, parsedArgs) == FAIL){
      printf("Error Occured :( ");
      continue;
    }
    file_flag = true;
    execute(parsedArgs);
  }
}

/*The greeting*/
void initiate_shell(){
	char *welcome = "------------ Hadeel and Ikhlas are saying Hello ------------";
	char *line =    "------------------------------------------------------------";
	printf("\n%s\n%s\n%s\n", line, welcome, line);
	return;
}

int main(int argc, char *argv[]){
  initiate_shell();
  printf("Start Running ....\n");
  /* Check if a barchfile is attached 
  or the commands will be taken from the console */
  if(argc > 1){ 
    /*from batchfile*/
		FILE *input = fopen(argv[1],"r");
		if(input == NULL){
			printf("Unable to open batchfile!\n"); 
			return FAIL;
		}
    readbatch(input);
    fclose(input);
  }
  else{
    /*from console*/
    liveRun();
  }  
  return SUCCESS;
}

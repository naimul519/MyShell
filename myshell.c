
/*
* Naimul Khan
* tuf70149@temple.edu
* 3207
* Program Description: This program creates a simple shell and allows you execute several builtin commands and other external commands
*/

// libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// function prototypes
int ms_cd(char**);
int ms_clr();
int ms_dir(char**);
int ms_environ();
int ms_echo(char**);
int ms_help();
int ms_pause();
int ms_quit();
int myshell();
int execute(char**);
int external_com(char**);
char *readLine(void);
char **parseLine(char*);
void printHostName();
int forking(char**);
extern char **environ;

char *com_str[] = {"cd","clr","dir","environ","echo","help","pause","quit"}; //char pointer to strings relating to builtin commands
int (*com[])(char**) = {&ms_cd,&ms_clr,&ms_dir,&ms_environ,&ms_echo,&ms_help,&ms_pause,&ms_quit};	// pointers to functions for the builtin commands


// cd builtin function
int ms_cd(char **args){
   if (args[1] == NULL){// if no directory mentioned
	char currDir[1024];  
	getcwd(currDir, sizeof(currDir)); //get current directory and put value in currDir    
       printf("Current directory is: %s> \n", currDir);
   }else{
	if(chdir(args[1]) != 0){//changes cwd
	 	printf("Error changing directory\n");

	}
   }

return 1;
}

int ms_clr(char **args){
	printf("\033[H\033[2J");//clears screen
return 1;
}

int ms_dir(char **args){
    struct dirent *d;
    DIR *directory;// directory pointer
    directory = opendir(args[1]);// open dirctory that was specified
    if(directory != NULL){//check if directory exists
	while((d = readdir(directory)) != NULL){
	    printf("%s\n", d->d_name);// print all files in directory
        }
	closedir(directory);// close directory
    }else{
	printf("No files in directory\n");
    } 


return 1;
}


int ms_environ(){// prints all environment variables
   
   int i;
  char *s = *environ;

  for (i=1; s; i++) {//loops until until
    printf("%s\n", s);//prints environment variable
    s = *(environ+i);//sets to next environment variable
  }

return 1;
}


int ms_echo(char **args){//prints the string that came after echo
int i;
for(i =1;*args!=NULL; ++args){// checks while args is not NULL

     printf("%s ", args[i]); // prints all arguments after args[1]

}
	puts("");
return 1;
}


int ms_help(){//displays the readme text file
printf("help function\n");
char c;

FILE *fp;//file pointer

fp = fopen("readme", "r"); // open readme.txt to read from file

c = fgetc(fp); // get first character from file
do{
   printf("%c", c); // prrint every character

    c = fgetc(fp);//get the rest of the characters in file one at a time
    


}while(c != EOF);//continue while the end of file is not reached

fclose(fp);//close file pointer

return 1;
}

int ms_pause(){//pause until ENTER is clicked
    char c;
    //scanf("%c", &c);
    while(c != '\n' ){
	scanf("%c", &c);

    }

return 1;
}

int ms_quit(){//exits program
	system("clear");//clears screen before exiting
return 0;
}


int myshell(){//function that is called to execute batchfile argument
	FILE *fp; // file pointer
	char line[30];// char array with size 30
	char **a; // double pointer to hold arguments in batchfile.txt
	fp = fopen("batchfile.txt", "r");//open batchfile.txt for reading
	
    
	while(fgets(line,20,fp) != NULL){//copy line from file into line until itis NULL
	   // char line[30];
	   // int i = 0;
	        a = parseLine(line);// parse the line that was recieved from the file and let a hold the value
		printf("%s command:\n\n",a[0]);//print the command that was recieved
	    	execute(a);// execute the command
	   
	    if(feof(fp)){//checks if end of file was reached
		break;
	    }

	}

fclose(fp);	// close file pointer
return 1;
}

char *readLine(void){//function to read line from stdin
int i;

    char *line;
    size_t size = 0;
    getline(&line, &size, stdin);// get the line from stdin
    if(line[1] == '\n'){//checks if ENTER was pressed
	printHostName();//if so, print hostname
    }

    return line;// return the line
}


char **parseLine(char *line){// parse the line that was entered in stdin into arguments
    int size = 128;
    
int i = 0;
    char **arguments = malloc(size * sizeof(char*));//  allocates memory for arguments
    char *token;

    char tokenizer[] = {" \n\t"};//tokenizers that the line may be tokenized by

    token = strtok(line, tokenizer);//tokenize the first word of the line by  whitespaces
    while (token != NULL) {// continue while token is not NULL
        arguments[i] = token;//add token
        i++;

        if(i >= size) {//checck if number of slots in the array have been filled
            size = size + 32;//increase value of size
            arguments = realloc(arguments, size * sizeof(char*));//resizes the memory block
        }

        token = strtok(NULL, tokenizer);// tokenize rest of the words in the line by whitespace one at a time
    }
    arguments[i] = NULL;
//	 execute(arguments);
    return arguments;//return the arguments
}

int external_com(char** args){
//    printf("in external com\n");
   // system(*args);
    return 1;
}


int forking(char **args){//function that does the required forking
printf("int fork method\n");
//look for background & check if last argument contains &, fork return to prompt and the passed arguments
int i;
//pid_t pid,wpid;
int result =0;
int status;
int j;
int isBackground = 0;
int isPipe = 0;
int isRedirection = 0;

for(i=0;args[i]!=NULL;i++){//loops through array of arguments
        printf("in redirection loop\n");
        if(*args[i] == '<' || *args[i] == '>'){//checks if redirction
                isRedirection = 1;//if so set flag
            printf("is redirection\n");
        }
}


for(i=0;args[i]!=NULL;i++){//loops through array of arguments
        printf("in piping loop\n");
        if(*args[i] == '|'){// checks if piping
                isPipe = 1;// if so, set flag
            printf("is pipe\n");
	    args[i] = NULL;//get rid of pipe sign
        }
}


for(i=0;args[i]!=NULL;i++){//loops through array of arguments
	printf("in background loop\n");
	if(*args[i] == '&'){//checks if background
		isBackground =1;
	    printf("is background\n");
		args[i] = NULL;// get rid of &
	}
}


if(isRedirection == 1){// if redirection then execute
//int standardIn = dup(0);
  //          int standardOut = dup(1);
//close(0);
//close(1);
    for(i=0;args[i]!=NULL;i++){//loop through arguments
        printf("in redirection loop2\n");
        if(*args[i] == '<'){//check if new stdn
            printf("is <\n");
	    //*args[i] == NULL;
	    int fd,fd2,fdC,held1;
            pid_t pid,wpid;
            pid = fork();//fork
            if(pid < 0){
                printf("Error forking\n");
            }else if(pid == 0){//child process
                fd = open(args[i + 1],O_RDONLY);//open file in args[i+1] for reading only
		//*args[i+1] = NULL;
                //close(0);
                dup2(fd,0);//duplicates fd 
		//close(0);
                //close(fd);// close file descriptor
		args[i] = NULL;//set < to null
		args[i+1] = NULL;//set newstdin file to null
           
		execvp(args[0],args);
                    
		close(fd);

            }else{//parent process
                while((wpid = wait(&held1))>0);//wait till child process terminates
            }
        }


	if(*args[i] == '>'){//check if new stdout
	    printf("is >\n");
	    int fd,fd2,fdC,held2;
	    int holder;
            pid_t pid,wpid;
   	    pid = fork();//fork
            if(pid < 0){
                printf("Error forking\n");
            }else if(pid == 0){//child process
                fd = open(args[i + 1],O_WRONLY|O_CREAT,S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR);//open file at args[i+1] for writing
                dup2(fd,1);	//dupliacte fd
                //close(fd);//close fd
		args[i] = NULL;//set > to null
		args[i+1] = NULL;// set newstdout filename to null

		for(i=0;i<8;i++){//loop throug builtin fuction
        	    if(strcmp(args[0], com_str[i]) == 0 ){//check if builtin
            		return (*com[i])(args);//if so, run builtin
        	    }else{
			execvp(args[0],args);// if not run external commands
		    }
    		}

		close(fd);
            }else{//parent process
                while((wpid = wait(&held2))>0);// wait till child process terminates
            }
				

	}

	if(args[i] == ">>"){
	printf("is >>\n");
            int fd,fd2,fdC,held5;
            int holder;
            pid_t pid,wpid;
            pid = fork();
            if(pid < 0){
                printf("Error forking\n");
            }else if(pid == 0){
                fd = open(args[i + 1],O_WRONLY|O_APPEND|O_CREAT);
                dup2(fd,1);
                close(fd);
                args[i] = NULL;
                args[i+1] = NULL;

                for(i=0;i<8;i++){
                    if(strcmp(args[0], com_str[i]) == 0 ){
                        return (*com[i])(args);
                    }else{
                        execvp(args[0],args);
                    }
                }

            }else{
                while((wpid = wait(&held5))>0);
            }

	}
		
    }

}

if(isBackground == 1){//if background then execute
printf("in background2\n");
 pid_t pid;
            pid = fork();//fork
            if(pid<0){
                printf("Error background forking\n");
            }else if(pid == 0){//child process
		printf("child process\n");
                /*if(execvp(args[0], args) == -1){
                    printf("error in child background\n");
                }
		return 1;*/
		 for(i=0;i<8;i++){//loop through builtin
                    if(strcmp(args[0], com_str[i]) == 0 ){//check if builtin
                        return (*com[i])(args);// if so execute builtin
                    }else{
                        execvp(args[0],args);//if not execute external command
                    }
                }


            }else{
                //parent process

		// dont wait for child process to terminate
            }

}

if(isPipe == 1){//if pipe then execute
printf("in pipe2\n");
    char **a1 =  malloc(sizeof(char) * 32);// allocate memory for array
    char **a2  = malloc(sizeof(char) * 32);// allocate memory for array
    int i = 0;
    int j;
    int k = 0;
    int m;
    while(args[i] != NULL){//loop through while not NULL
	if(strcmp(args[i],"|") == 0){//check if pipe

	    for(j=0;j<i; j++){//loop through left side
		//a1[j] = malloc(sizeof(char) * sizeof(args[i]));// allocate memory for array
		a1[j] = args[j];//copy into left side array
	    }
	    a1[j+1] = NULL;

	    for(m=i+1;args[m] != NULL;m++){//loop after |
		//a2[k] = malloc(sizeof(char) * sizeof(args[m]));// allocate memory
		a2[k] = args[m];// copy into right side array
		k++;
	    }
	
	    a2[k+1] = NULL;
	}
	i++;
    }
	
char* b1[]={"ls", "-l", NULL};
char* b2[]={"wc", "-l", NULL};  


    printf("is piping in fork method\n");
    pid_t pid, pid2;
    int thePipe[2];//file descriptors
    //int in, out;
    pipe(thePipe);// pipe creates connection between the two processes
    pid = fork();//fork

    if(pid == 0){//childprocess
	close(1);
	//child process
	dup2(thePipe[1],1);
	//close(1);
	close(thePipe[0]);//close reading
	close(thePipe[1]);
	//execvp(args[0],args);
	 execvp(b1[0],b1);	
    }else{//parent process
	 pid2 = fork();//fork

	if(pid2 == 0){//2nd child process
		//child process
	close(0);
	 dup2(thePipe[0],0);
	
        //close(0);
        close(thePipe[1]);//close writing
	close(thePipe[0]);
         execvp(b2[0],b2);

	
	}else{
		//parent process
	  close(thePipe[0]);//close read
	  close(thePipe[1]);//close write
	  //waitpid(pid,NULL,0);//wait for first child to terminate
	  //waitpid(pid2,NULL,0);//wait for second child to terminate
		wait(NULL);  
	}




    }
}


if((isRedirection == 0) && (isBackground == 0) && (isPipe ==0)){//check if normal forking
printf("in normal forking\n");

 pid_t pid,wpid;
            pid = fork();//fork
            if(pid < 0){
                printf("Error forking\n");
            }else if(pid == 0){//child process
                if(execvp(args[0],args) == -1){//execute external command
                    printf("Error calling external command\n");
                }
            }else{
                while((wpid = wait(&status))>0);//wait till child terminates
            }

}
    return 1;
}


int execute(char **args){//execute builtin functions
    int i;
    if(args[0] == NULL){//checks if entry is null
	return 1;
    }

for(i=0;args[i]!=NULL;i++){//loops through arguments
        printf("in redirection loop in execute method\n");
        if(*args[i] == '<' || *args[i] == '>'){//checks redirection
            printf("is redirection in execute\n");
	    return forking(args);//if so execute forking
        }
}

for(i=0;args[i]!=NULL;i++){// loop thrhough arguments
        printf("in piping loop in execute method\n");
        if(*args[i] == '|'){// check if piping
            printf("is pipe in execute\n");
	    return forking(args);// if so execute forking
        }
}



for(i=0;args[i]!=NULL;i++){// loop through arguments
        printf("in background loop in execute method\n");
        if(*args[i] == '&'){// check if background
            printf("is execute background\n");
                return forking(args);//if so execute forking
        }
}

//printf("");

    for(i=0;i<8;i++){	// loop through builtins
	if(strcmp(args[0], com_str[i]) == 0 ){// check if builtin
	    return (*com[i])(args);//if so execute builtin
	}
    }
    return forking(args);    //if not execute forking
}


void printHostName(){// prints pathname
char currDir[1024];
char hostName[1024];   
if(getcwd(currDir, sizeof(currDir)) != NULL){
	gethostname(hostName,1024);
        printf("%s%s>", hostName,currDir);


}else{
	printf("Error");
}
}



int main(int argc, char** argv){
if (argc>1){//checks for batchfile
	myshell();//executes function for batchfile
}

    system("clear");//clears previous entries

    char *line;// holds line from stdin
    char **args;//holds the argumentsafter tokenized by whitespaces
    int result;
    int i;
    char c;

    do{
       printHostName();// print pathname everytime
	
       line = readLine();//gets line from stdin
       
        args = parseLine(line);// tokenizes by white space
	puts("Arguments");
	for(i =0;args[i] != NULL; i++){//loops through args
		    printf("args[%d]: %s\n ", i, args[i]);// prints out all arguments
	}


	result = execute(args);	//executes command. returns0 or 1

     free(line);//deallocates memory that was allocated for line
     free(args);//deallocates memory what was allocated for args
    }while(result);//occure while result == 1

    return 0;
}



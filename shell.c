#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/resource.h>

/*
*** pesudo code:

function parent_main()
    register_child_signal(on_child_exit()) #done
    setup_environment() #done
    shell()


function on_child_exit() #done
    reap_child_zombie() #done
    write_to_log_file("Child terminated") #done


function setup_environment() #done
    cd(Current_Working_Directory)


function shell()
    do
        parse_input(read_input()) #done
        evaluate_expression():
        switch(input_type): #done
            case shell_builtin:
                execute_shell_bultin();
            case executable_or_error:
                execute_command():

    while command_is_not_exit


function execute_shell_bultin()
    switch(command_type):
        case cd:
        case echo:
        case export:


function execute_command() #done
    child_pid = fork()
    if child:
        execvp(command argument_list)
        print("Error)
        exit()
    else if parent and foreground:
        waitpid(child)


*** future improvements:
** use perror() to print meaningful error messages #done
** use heder files for protoypes 
*/




/************* Macros **************/
#define CWD_BUF_SIZE 2000
#define CLI_BUF_SIZE 100 


/******* functions' prototype ********/
void parent_main();
void on_child_exit(); //
void setup_environment();
void shell();
char* parse_input();
int execute_shell_builtin();
int execute_command();
void write_to_file(int sig);
/************************************/


 

int main(void){
    
    char* argument_list[] = {"ls", "-l", "-a", NULL};
    // char* command = "ls";

    //parent_main();

    
    char input_line[CLI_BUF_SIZE];
    
    printf("starting main()\n");
    
    char* list[50];
    parse_input(input_line, list);

    // int exe_status = execute_shell_builtin(list[0], list, "FOREGROUND");

    printf("end of main()\n");


    //printf("argument list: %s", list);
    

    //int status_code = execute_command(argument_list[0], argument_list, "BACKGROUND");

    // // driver for write_to_file
    // // write_to_file(0);

    // char* attr[] = {"l", "file", NULL}; 
    // char* command = "nano";

    // execvp(command, attr);

}





/****** functions' declarations ******/

void parent_main(){
    // debug
    printf("inside parent_main\n");
    signal (SIGCHLD, on_child_exit); 
    setup_environment();
    //shell();
}




void on_child_exit(int sig){

    // write child termination message to the log file  
    write_to_file(0);

    //debug 
    printf("\ninside on_child_exit\n");
    wait(NULL);

    // pid_t	pid;

    // while (1) {
    //     pid = wait3 (NULL, WNOHANG, (struct rusage *)NULL );
    //     if (pid == 0)
    //         return;
    //     else if (pid == -1)
    //         return;
    //     else
    //         printf ("Return code: \n");
    // }


    // pid_t pid;

    // while(1){
    //     // pid = wait(NULL);
        
    //     // if(pid == 0){
    //     //     return;
    //     // }
    //     // else if(pid == -1){
    //     //     return;
    //     // }
    //     // else{
    //     //     perror("Child exit error");
    //     // }

    //     printf("\nWaiting for child to terminate..");
    //     end_pid = waitpid(child_pid, NULL, WUNTRACED);

    //     if (end_pid == -1) {  
    //         status_code = -1;
    //         perror("Waitpid Error");
    //         exit(EXIT_FAILURE);
    //     } 

    //     else if(end_pid == child_pid){ 
    //         // child ended
    //         printf("\nChild terminated\n");
    //         break;
    //     }  
    // }

    // waitpid(-1,NULL,WNOHANG | WUNTRACED);
    
}



void setup_environment(){
    // debug
    printf("inside setup_environment\n");

    char* CWD[CWD_BUF_SIZE];
    getcwd(&CWD, CWD_BUF_SIZE);
    chdir(CWD);
}



void shell(){
    char argument_list[CLI_BUF_SIZE];
    char command;
    char* layer;
    int arg_len;

    while(strcmp(command, "exit")){
        // read input from the user
        strcpy(argument_list, parse_input());
        command = argument_list[0];
        arg_len = strlen(argument_list);

        
        // background layer
        if(!strcmp(argument_list[arg_len-1],"&")){
            argument_list[arg_len-1] == NULL;
            layer = "BACKGROUND";
        }

        // foreground layer
        else{
            argument_list[arg_len] == NULL;
            layer = "FOREGROUND";
        }


        // built-in commands
        if(command=="cd" || command=="echo" || command=="export" ){
            execute_shell_builtin(command, argument_list, layer);
        }

        // executable commands
        else{
            execute_command(command, argument_list, layer);
        }

    }
}



char* parse_input(char input_line[], char argument_list[]){
    int i = 0;
    // char* argument_list[CLI_BUF_SIZE];

    //debug
    printf("Inside parse_input\n");

    char *ptr;

    fgets(input_line, CLI_BUF_SIZE, stdin);

    if ((strlen(input_line) > 0) && (input_line[strlen (input_line) - 1] == '\n'))
        	input_line[strlen (input_line) - 1] = '\0';

    ptr = strtok(input_line, " ");

	while(ptr != NULL){
		//printf("%s\n", ptr);
		argument_list[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}


    // fgets(input_line, CLI_BUF_SIZE, stdin);
    // // scanf ("%[^\n]%*c", input_line);

    // printf("got input from fgets()\n");

    // if ((strlen(input_line) > 0) && (input_line[strlen (input_line) - 1] == '\n'))
    //     	input_line[strlen (input_line) - 1] = '\0';
    // printf("len of input_line >> %ld\n", strlen(input_line));
    // // argument_list[strlen(input_line) - 1] = NULL;

    // for (i = 0; i < CLI_BUF_SIZE; i++) {

    //     argument_list[i] = (char*) strsep(&input_line, " ");

    //     printf("token >> %s\n", argument_list[i]);
  
    //     if (!strcmp(argument_list[i], NULL))
    //         break;

    //     if (strlen(argument_list[i]) == 0)
    //         i--;
    // }


    printf("returning from parse_input..\n");

    return(argument_list);   
}




int execute_shell_builtin(char* command, char* argument_list[], char layer[]){
    
    printf("argumennt[0]: %s\n", command);
    printf("argumennt[1]: %s\n", argument_list[1]);


    if(!strcmp(command, "cd")){
        chdir(argument_list[1]); 
        printf("built-in attr for cd command is: %s\n", argument_list[1]);
    }

    else if(!strcmp(command, "echo")){ // not so completely functional
        int i = 1;
        
        // print all arguments to the right of the "echo" command
        while(strcmp(argument_list[i],NULL)){
            printf("%s", argument_list[i]);
            i++;
        }
    }

    else if(!strcmp(command, "export")){
        // implement
    }
       

    else{
        perror("command not found");
    }
}



int execute_command(char*command, char* argument_list[], char layer[]){

    printf("\nexecute_command in action..");
    fflush(stdout);
    
    int status_code = 0;
    pid_t end_pid, child_pid;
    child_pid = fork();

    if(child_pid == -1){
        status_code = -1;
        perror("Fork Error");
        exit(EXIT_FAILURE);
    }

    else if(child_pid == 0){
        printf("\nChild PID is >> %d\n\nExecuting child..\n", getpid());

        execvp(command, argument_list);
        status_code = -1;
        printf("Error");
        exit(EXIT_SUCCESS);
    }

    //debug
    printf("\nchecking if foreground..\n");
    printf("\nParent PID is >> %d\n", getpid());

    if(!strcmp(layer, "FOREGROUND")){
        while (1){
            printf("\nWaiting for child to terminate..");
            end_pid = waitpid(child_pid, NULL, WUNTRACED);

            if (end_pid == -1) {  
                status_code = -1;
                perror("Waitpid Error");
                exit(EXIT_FAILURE);
           } 

           else if(end_pid == child_pid){ 
                // child ended
                printf("\nChild terminated\n");
                break;
           }  
        }
    }

    if(!strcmp(layer, "BACKGROUND")){
        printf("\nBackground detected!\n");
        sleep(3); // to prevent the main process from terminating before the child
    }


    
    return(0);
}



void write_to_file(int sig){
	FILE *pFile;
        pFile = fopen("log.txt", "a");
        if(pFile==NULL) perror("[Error] opening file");
        else fprintf(pFile, "Child terminated\n");
        fclose(pFile);
}
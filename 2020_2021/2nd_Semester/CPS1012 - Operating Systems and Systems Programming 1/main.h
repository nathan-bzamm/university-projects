
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define MAX_ARGS 100
#define MAX_NAME 32
#define MAX_SIZE 4096
#define MAX_VAR_NAME 64

#define METACHARACTERS "|;<> \t"
#define WHITESPACE " \t"
#define CONTROLOPERATOR ";|\n"
#define SHELLPATH "/home/student/cps1012/bin/smash"
#define EXITCODE "0"


/* FUNCTIONS */ 
void vars_on_start();  // Declare Vars
void printAllVars();

int add_or_change_vars(char* shellVarName, char* shellVarContent);
void print_env_vars();

/* ---- Variables ---- */
char CWD[MAX_SIZE]; // Current working directory
int var_counter = 0; // Variable Counter -- Env and Shell Vars



// DO NOT ADD \n TO SHELLNAME OR ELSE INPUT WILL MESS UP
char shellname[MAX_NAME] = "smash 1.0";
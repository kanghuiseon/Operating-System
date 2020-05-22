#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#define MAX 1000

char* new_cmd[MAX];
char* trimLeft(char* cmd){
    int i, j;
    int cnt = 0;
    char* str = (char*)malloc(sizeof(char)*MAX);
    for(i=0;i<strlen(cmd);i++){
        if(cmd[i]==' ')
            cnt++;
        else
            break;
    }
    j=0;
    for(i=cnt;i<strlen(cmd);i++){
        str[j]=cmd[i];
        j++;
    }
    return str;
}
char* trimRight(char* cmd){
    char* str = (char*)malloc(sizeof(char)*MAX);
    char* end;
    int i;
    strcpy(str, cmd);
    end = str + strlen(str)-1;
    for(i=0; isspace(*end) ; i++)
        --end;
    *(end+1) = '\0';
    cmd = str;
    return cmd;
}

int main(int argc, char* argv[]){
    char* cmd = (char*)malloc(sizeof(char)*MAX);
    int i=0,k,p,j;
    char* ptr;
    int size,status;
    char* one_cmd[100000]; 
    pid_t pid;
    if (argc == 1){ 
        while(1){
            printf("prompt> ");
            if(fgets(cmd, MAX, stdin)==NULL){
                exit(0);
            }
            strncpy(cmd, cmd, strlen(cmd)-1); 
            cmd = trimLeft(cmd);
            cmd = trimRight(cmd);
            if(strcmp(cmd, "quit")==0)
                exit(0);
            i =0;
            ptr = strtok(cmd, ";");
            while(ptr != NULL){
                new_cmd[i] = ptr;
                i++;
                ptr = strtok(NULL,";");
            }
            for(k=0; k<i;k++){
                if(new_cmd[k] != NULL){
                    new_cmd[k] = trimLeft(new_cmd[k]);
                    new_cmd[k] = trimRight(new_cmd[k]);
                }
            } 
            size = i;
            for(p=0; p<size; p++){
                pid = fork();
                if(pid < 0){ 
                    printf("fork() error\n");
                    exit(0);
                }
                else if(pid == 0){ 
                    ptr = strtok(new_cmd[p], " ");
                    i=0;
                    memset(one_cmd,0,sizeof(one_cmd)); 
                    while(ptr != NULL){
                        one_cmd[i] = ptr;
                        i++;
                        ptr = strtok(NULL, " ");
                    } 
                    one_cmd[i] = '\0';
                    if(execvp(one_cmd[0], one_cmd)==-1){
                        fprintf(stdout,"%s : command not found\n",one_cmd[0]);
                        exit(0);
                    }
                }
                }//end for
            for(j=0;j<size;j++)
                wait(&status);
                       
            
        }//end while 
        }//end argc==1
    
   else{ //batchfile
        FILE* input = fopen(argv[1], "r");
        if(input == NULL){
            printf("no file!");
            exit(0);
        }
        while(fgets(cmd, MAX, input)!=NULL){ //reading file
            fprintf(stdout, "%s",cmd);
            strncpy(cmd, cmd, strlen(cmd)-1); 
            cmd = trimLeft(cmd);
            cmd = trimRight(cmd);
            if(strcmp(cmd, "quit")==0)
                exit(0); //if cmd == quit, exit(0)
            i=0;
            ptr = strtok(cmd, ";");
            while(ptr!=NULL){
                new_cmd[i] = ptr;
                i++;
                ptr = strtok(NULL, ";");
            }
            for(k=0; k<i;k++){
                if(new_cmd[k] != NULL){
                    new_cmd[k] = trimLeft(new_cmd[k]);
                    new_cmd[k] = trimRight(new_cmd[k]);
                }
            }
                size=i;
                for(p=0; p<size; p++){
                    pid = fork();
                    if(pid<0){
                        printf("fork() error\n");
                        exit(0);
                    }
                    else if(pid ==0){
                        ptr = strtok(new_cmd[p]," ");
                        i=0;
                        memset(one_cmd, 0, sizeof(one_cmd));
                        while(ptr != NULL){
                            one_cmd[i] = ptr;
                            i++;
                            ptr = strtok(NULL," ");
                        }
                        one_cmd[i] = '\0';
                        if(execvp(one_cmd[0], one_cmd)==-1){
                            fprintf(stdout,"%s : no such command\n",one_cmd[0]);
                        }
                    }
                }
                for(j=0; j<size; j++)
                    wait(&status);
        }
               
        fclose(input);
    }
    return 0;
}

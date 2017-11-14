#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "commands.h"
#include "built_in.h"
#define SOCK_PATH "server.txt"
#define SERVER_PATH "server.txt"
#define CLIENT_PATH "client.txt"
#define DATA "I am Client"

void *dread1(void *threadid);
void *dread2(void *threadid);
int back_pid;
char ** ptemp;
char ** ptemp1;
static struct built_in_command built_in_commands[] = {
  { "cd", do_cd, validate_cd_argv },
  { "pwd", do_pwd, validate_pwd_argv },
  { "fg", do_fg, validate_fg_argv }
};

static int is_built_in_command(const char* command_name)
{
  static const int n_built_in_commands = sizeof(built_in_commands) / sizeof(built_in_commands[0]);

  for (int i = 0; i < n_built_in_commands; ++i) {
    if (strcmp(command_name, built_in_commands[i].command_name) == 0) {
      return i;
    }
  }

  return -1; // Not found
}

/*
 * Description: Currently this function only handles single built_in commands. You should modify this structure to launch process and offer pipeline functionality.
 */
int evaluate_command(int n_commands, struct single_command (*commands)[512])
{  
  pid_t child_pid; int status; int backcheck=0;int pipecheck=0; 
 // if(strcmp(com->argv[com->argc-1],"&")==0){ strcpy(com->argv[com->argc-1],"");backcheck=1;}
 // if(child_pid==0){
  if (n_commands > 0) {
    struct single_command* com = (*commands); 
    assert(com->argc != 0);
    if(strcmp(com->argv[com->argc-1],"&")==0){ com->argc=com->argc-1;backcheck=1;}
 //   if(getpid()!=old_pid){child_pid = fork();}
    
   
    int built_in_pos = is_built_in_command(com->argv[0]);
    if (built_in_pos != -1) {
      if (built_in_commands[built_in_pos].command_validate(com->argc, com->argv)) {
        if (built_in_commands[built_in_pos].command_do(com->argc, com->argv) != 0) {
          fprintf(stderr, "%s: Error occurs\n", com->argv[0]);
        }
      } else {
        fprintf(stderr, "%s: Invalid arguments\n", com->argv[0]);
        return -1;
      }
    } else if (strcmp(com->argv[0], "") == 0) {
      return 0;
    } else if (strcmp(com->argv[0], "exit") == 0) {
      return 1;
    }
     else if (com->argv[0][0] == '/'){int check;
    if(getpid()==back_pid){printf("hahaha\n");return 0;}
    child_pid = fork();
    if(child_pid == 0){
   
  
  if((com+1)->argc>0){
  ptemp=(char**)malloc(sizeof(char*)*(com->argc));
  for(int i=0; i<com->argc; i++){
  ptemp[i]= (char*)malloc(sizeof(char)*sizeof(com->argv[i]));
 strcpy(ptemp[i],com->argv[i]);
}
 ptemp1=(char**)malloc(sizeof(char*)*((com+1)->argc));
 for(int i=0; i<(com+1)->argc;i++){
 ptemp1[i] = (char*)malloc(sizeof(char)*sizeof((com+1)->argv[i]));
 strcpy(ptemp1[i],(com+1)->argv[i]);
}
  pthread_t threads[2];
  
  int rc;
  int t1=0;
  int t2=1;
  rc = pthread_create(&threads[0], NULL, dread1, (void *)&t1);
  rc = pthread_create(&threads[1], NULL, dread2, (void *)&t2);
 printf("came here\n");
 return 0;
}//pipecheck
     if(backcheck==1){
     back_pid=getpid();
     char **temp;
     temp = (char**)malloc(sizeof(char*)*(com->argc));
    for(int i=0; i<com->argc; i++){
    temp[i] = (char*)malloc(sizeof(char)*sizeof(com->argv[i]));
   strcpy(temp[i],com->argv[i]);
}
  check=execv(temp[0],temp);
   printf("%d done %s\n", getpid(), *temp); 

 }
     else{check = execv(com->argv[0], com->argv);}
     if(check==-1){return -1;}
    else{ return 0;}
   
}
else{if(backcheck==0){wait(&status);}else{printf("%d\n",child_pid);}}
    } else {
      fprintf(stderr, "%s: command not found\n", com->argv[0]);
      return -1;
    }
 

// else{if(backcheck==0){wait(&status);}else{back_pid=child_pid;}}
  }//ncommands > 0
//} // if childpid == 0
// else{ if(backcheck==0){wait(&status);}}
  return 0;
}

void free_commands(int n_commands, struct single_command (*commands)[512])
{
  for (int i = 0; i < n_commands; ++i) {
    struct single_command *com = (*commands) + i;
    int argc = com->argc;
    char** argv = com->argv;

    for (int j = 0; j < argc; ++j) {
     free(argv[j]);
    }

    free(argv);
  }

  memset((*commands), 0, sizeof(struct single_command) * n_commands);
}

void *dread1(void *threadid){
   
   int server_sock, client_sock, len, rc;
   int bytes_rec=0;
   struct sockaddr_un server_sockaddr;
   struct sockaddr_un client_sockaddr;
   char buf[1000];
   int backlog=10;
   memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
   memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
   memset(buf, 0, 1000);
   server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
   if(client_sock==-1){printf("SOCKET ERROR\n");exit(1);}
   server_sockaddr.sun_family = AF_UNIX;
   strcpy(server_sockaddr.sun_path, SOCK_PATH);
   len = sizeof(server_sockaddr);
   unlink(SOCK_PATH);
   rc = bind(server_sock, (struct sockaddr *) &server_sockaddr,len);
   if(rc==-1){printf("BIND ERROR\n");close(server_sock);exit(1);}
   rc = listen(server_sock, backlog);
   if(rc==-1){printf("LISTEN ERROR\n");close(server_sock);exit(1);}
   printf("sock listening...\n");
   client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);
   if(rc==-1){printf("ACCEPT ERROR\n"); close(server_sock);close(client_sock);exit(1);}
   len = sizeof(client_sockaddr);
   rc = getpeername(client_sock, (struct sockaddr *) &client_sockaddr, &len);
 if(rc==-1){printf("GETPEERNAME ERROR\n");close(server_sock);close(client_sock);exit(1);}
else{printf("Client socket filepath %s\n", client_sockaddr.sun_path);
}
 printf("waiting to read...\n"); bytes_rec = recv(client_sock, buf, sizeof(buf), 0);
 if(bytes_rec==-1){printf("RECV ERROR\n");close(server_sock);close(client_sock);exit(1);}
 else{printf("DATA RECEIVED = %s\n", buf);}
 int fd2 = open("a.txt", O_RDONLY);
  dup2(fd2,0);
 pid_t child2 = fork();
 if(child2==0){
  execv(ptemp1[0],ptemp1);
}
else{int status3;
  wait(&status3);
  close(server_sock);
  close(client_sock);
return 0;
}
}
void *dread2(void *threadid){
 int status1;
 pid_t child;
 int client_sock, rc, len;
 struct sockaddr_un server_sockaddr;
 struct sockaddr_un client_sockaddr;
 char buf[1000];
 memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
 memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
 client_sock = socket(AF_UNIX, SOCK_STREAM,0);
 if(client_sock==-1){printf("SOCKET ERROR\n");exit(1);}
 client_sockaddr.sun_family = AF_UNIX; strcpy(client_sockaddr.sun_path, CLIENT_PATH);len=sizeof(client_sockaddr);
unlink(CLIENT_PATH);
rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
if(rc==-1){printf("BIND ERROR\n");close(client_sock);exit(1);}
server_sockaddr.sun_family = AF_UNIX;strcpy(server_sockaddr.sun_path,SERVER_PATH);
rc = connect(client_sock, (struct sockaddr *) &server_sockaddr,len);
if(rc==-1){printf("CONNECT ERROR\n");close(client_sock);exit(1);}
strcpy(buf,DATA);
int fd= open("a.txt", O_WRONLY | O_CREAT);
int bk = open("dummy",O_WRONLY | O_CREAT);
dup2(fd,1);

child=fork();
if(child==0){
 
 execv(ptemp[0],ptemp);
}
else{
wait(&status1);//here I should write content of temp.txt into buf
close(bk);
dup2(1,bk);
dup2(fd,1);
close(fd);
dup2(bk,1);
remove("dummy");
printf("Sending data...\n");
rc = send(client_sock, buf, strlen(buf), 0);
if(rc == -1){printf("SEND ERROR\n"); close(client_sock); exit(1);}
else{printf("Data Sent!\n");}
close(client_sock);
return 0;
}
}

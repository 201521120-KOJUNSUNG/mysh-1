#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "commands.h"
#include "built_in.h"
#include "signal_handlers.h"
#define SOCK_PATH "server.txt"
#define SERVER_PATH "server.txt"
#define CLIENT_PATH "client.txt"
#define DATA "I am Client"
int mutex=0;
int mutex1=0;
int mutex2=0;
void *dread1(void *threadid);
void *dread2(void *threadid);
int back_pid;
char ** ptemp;
char ** ptemp1;
int pp1;
int pp2;
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
{ pthread_t threads[2]; 
  for(int i=0; i<pp1; i++){
  free(ptemp[i]);
}
free(ptemp);
  for(int j=0; j<pp2; j++){
 free(ptemp1[j]);
}
free(ptemp1);
  signal(SIGINT, (void *)catch_sigint);
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
  pp1=com->argc;
  ptemp=(char**)malloc(sizeof(char*)*(com->argc));
  for(int i=0; i<com->argc; i++){
  ptemp[i]= (char*)malloc(sizeof(char)*sizeof(com->argv[i]));
 strcpy(ptemp[i],com->argv[i]);
}
 pp2=(com+1)->argc;
 ptemp1=(char**)malloc(sizeof(char*)*((com+1)->argc));
 for(int i=0; i<(com+1)->argc;i++){
 ptemp1[i] = (char*)malloc(sizeof(char)*sizeof((com+1)->argv[i]));
 strcpy(ptemp1[i],(com+1)->argv[i]);
}
  int stst;
  int rc;
  int t1=0;
  int t2=1;
//  rc = pthread_create(&threads[0], NULL, dread1, (void *)&t1);
  rc = pthread_create(&threads[1], NULL, dread2, (void *)&t2);
  rc = pthread_create(&threads[0],NULL,dread1,(void *)&t1);
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
   FILE *pfile;
   pfile = fopen("inter2.txt","w+");
   int server_sock, client_sock, len, rc;
   int bytes_rec=0;
   struct sockaddr_un server_sockaddr;
   struct sockaddr_un client_sockaddr;
   char buf[256];
   int backlog=10;
   memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
   memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
   memset(buf, 0, 255);
   server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
   if(client_sock==-1){exit(1);}
   server_sockaddr.sun_family = AF_UNIX;
   strcpy(server_sockaddr.sun_path, SOCK_PATH);
   len = sizeof(server_sockaddr);
   unlink(SOCK_PATH);
   rc = bind(server_sock, (struct sockaddr *) &server_sockaddr,len);
   if(rc==-1){close(server_sock);exit(1);}

   while(listen(server_sock, backlog)==-1){};
   if(rc==-1){close(server_sock);exit(1);}
   client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);
   if(rc==-1){ close(server_sock);close(client_sock);exit(1);}
   len = sizeof(client_sockaddr);
   rc = getpeername(client_sock, (struct sockaddr *) &client_sockaddr, &len);
 if(rc==-1){close(server_sock);close(client_sock);exit(1);}
 int check;  
  while(1){
  bytes_rec = recv(client_sock, buf, sizeof(buf), 0);
  if(strcmp(buf,"end")==0){break;}
  check=write(pfile->_fileno,buf,strlen(buf));
  memset(buf, '\0' ,strlen(buf));
  sleep(0.1);  
}
 memset(buf,'\0',strlen(buf));
// if(bytes_rec==-1){printf("RECV ERROR\n");close(server_sock);close(client_sock);exit(1);}
// else{printf("DATA RECEIVED = %s\n", buf);}
 FILE *pfile1;
 int status3;
 int std_fd;
 pfile1 = fopen("inter2.txt","r");
 std_fd = open("dummy2.txt", O_WRONLY | O_CREAT);
 close(std_fd);
 dup2(0,std_fd);
 dup2(pfile1->_fileno,0);
 while(1){if(mutex1==1)break;}
 pid_t child2;
 child2 = fork();
 if(child2==0){
  execv(ptemp1[0],ptemp1);
}
else{
  wait(&status3);
  mutex=1;
  close(pfile->_fileno);
  close(pfile1->_fileno);
  dup2(std_fd,0);
  remove("dummy2.txt");
  close(server_sock);
  close(client_sock);

//remove("inter2.txt");
for(int i=0; i<pp2; i++){
free(ptemp1[i]);
}
free(ptemp1);
pp2=0;
int status9;
//pthread_join(threads[1],(void**)&status9);
mutex=1;
while(1){printf("waiting for dread2\n");if(mutex1==1)break;}
printf("dread 1 is over\n");
mutex=0;
mutex1=0;
mutex2=0;
exit(1);
}
}
void *dread2(void *threadid){
 int cc;
 char ccc[256];
 FILE *pf;
 int status1;
 pid_t child;
 int client_sock, rc, len;
 struct sockaddr_un server_sockaddr;
 struct sockaddr_un client_sockaddr;
 char buf[256];
 memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
 memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
 client_sock = socket(AF_UNIX, SOCK_STREAM,0);
 if(client_sock==-1){exit(1);}
 client_sockaddr.sun_family = AF_UNIX; strcpy(client_sockaddr.sun_path, CLIENT_PATH);len=sizeof(client_sockaddr);
unlink(CLIENT_PATH);
rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
if(rc==-1){close(client_sock);exit(1);}
server_sockaddr.sun_family = AF_UNIX;strcpy(server_sockaddr.sun_path,SERVER_PATH);
while(connect(client_sock, (struct sockaddr *) &server_sockaddr,len)==-1)
{
//if(rc==-1){printf("CONNECT ERROR\n");close(client_sock);exit(1);}
}
pf = fopen("inter1.txt","w");
if(pf==NULL)printf("open err\n");
int cccheck;
int bk = open("dummy", O_WRONLY | O_CREAT);
close(bk);
dup2(1,bk);
cccheck=dup2(pf->_fileno,1);
child=fork();
if(child==-1)printf("fork err\n");
//printf("argv[0]=%s, argv[1] = %s, argc = %d\n",ptemp[0],ptemp[1],pp1);
if(child==0){

 cc=execv(ptemp[0],ptemp);

// if(cc==-1)printf("execv err\n");
}
else{
wait(&status1);//Here I should send intet.txt content by socket
for(int i=0; i<pp1; i++){
free(ptemp[i]);
}
free(ptemp);
close(pf->_fileno);
dup2(bk,1);
remove("dummy");
mutex=1;
FILE * reading = fopen("inter1.txt","r");
if(reading == NULL)printf("open err\n");
//read the file content to buf
while(fgets(ccc,255,reading)!=NULL){
strcpy(buf,ccc);
sleep(0.1);
rc = send(client_sock, buf, strlen(buf), 0);

}
//remove("inter1.txt");
sleep(0.1);
strcpy(buf,"end");
rc=send(client_sock,buf,strlen(buf),0);
memset(buf,'\0',strlen(buf));
//printf("dread2 almost done\n");
//if(rc == -1){printf("SEND ERROR\n"); close(client_sock); exit(1);}
//else{printf("Data Sent!\n");}
int status6;
close(pf->_fileno);
close(reading->_fileno);
mutex2=1;
//for(int i=0; i<pp1; i++){
//free(ptemp[i]);
//}
//free(ptemp);
pp1=0;
if(cccheck==-1)printf("dup err\n");
if(cc==-1)printf("execv err\n");

//pthread_join(threads[0],(void**)&status6); 
close(client_sock);

//close(pf->_fileno);
mutex1=1;
while(1){printf("waiting for dread 1\n");if(mutex==1)break;}
printf("dread 2 is over\n");
exit(1);
}
}

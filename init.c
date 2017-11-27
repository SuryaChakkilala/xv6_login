// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUFFLEN 24
#define MAX_ATTEMPTS  3
#define REG_USERS = 1

char *argv[] = { "sh", 0 };

char *regusers[] = {"root"};
char *regpass[] = {"password"};

int login(char *u, char *p) {
  int i;
  int loggedIn = 0;
  
  for(i = 0 ; i < REG_USERS ; i++) {
    //printf(1, "%s %s %s %s %d %d\n", u, regusers[i], p, regpass[i], !strcmp(u, regusers[i]), !strcmp(p, regpass[i]));
    if(!strcmp(u, regusers[i]) && !strcmp(p, regpass[i])) {
      loggedIn = 1;
      break;
    }
  }

  return loggedIn;
}

int
main(void)
{
  int pid, wpid, loggedIn;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){
    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      int count=0;

      while(count < MAX_ATTEMPTS){
        printf(1, "Username: ");
        char *user = (char *)malloc(BUFFLEN);
        user = gets(user , 20);
        
        //remove enter key
        switch(user[strlen(user) - 1]) {
          case '\n': case '\r':
          user[strlen(user) - 1] = 0;
        }
        
        printf(1, "Password :  ");
        char *pass = (char *)malloc(BUFFLEN);
        pass = gets_we(pass , 20);
        //remove enter key
        switch(pass[strlen(pass) - 1]) {
          case '\n': case '\r':
          pass[strlen(pass) - 1] = 0;
        }

        loggedIn = login(user, pass);
        if(loggedIn) {
          //opens shell for the user
          printf(1, "Welcome back %s!\n", user);          
          exec("sh", &user);
          printf(1, "init: exec login failed\n");
          exit();
        }
        else {
          printf(1, "User and password do not match, or user does not exist! Try again!\n");
          count++;
        }
      }

      printf(1, "Failed 3 attempts! Please reboot machine!\n");
      while(1); //stall
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}

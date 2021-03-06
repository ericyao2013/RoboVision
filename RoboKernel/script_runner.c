#include "script_runner.h"
#include "command_hal.h"
#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE_SIZE 2048


pthread_t script_kernel_loop_id=0;
void * ScriptKernelLoop(void *ptr );

char running_script_name[512]={0};
char running_from_user[512]={0};
int STOP_SCRIPT_RUNNER=0;
int SCRIPT_RUNNER_OK=0;


int InternalRunScript(char * script_name , char * from, int recursion_level)
{

  char line[MAX_LINE_SIZE];
  char output_string[512]={0};

  FILE * pFile;
  char total_filename[512]={0};
  strcpy(total_filename,"scripts/");
  strcat(total_filename,script_name);
  pFile = fopen (total_filename,"r");
  if (pFile!=0 )
    {
      int line_length=0;
      int c=0;
      do
        {
          c = getc (pFile);

          if ( MAX_LINE_SIZE-1 <= line_length )
             {
               if (strlen(running_script_name)==0) { return 0; } // THE SCRIPT HAS BEEN CANCELED

               fprintf(stderr,"Oveflow while loading configuration file \n");
               line[MAX_LINE_SIZE-1]=0;

               fprintf(stderr,"SCRIPT RUNNER RUNNING \"%s\" \n",line);
               IssueCommandInternal(line,from,output_string,512);

               line[0]=0; line_length=0;
             } else
          if (strncmp(line,"LOOP",4)==0)
             {
              if (strlen(running_script_name)==0) { return 0; } // THE SCRIPT HAS BEEN CANCELED

               rewind (pFile);
               line[0]=0; line_length=0;
               fprintf(stderr,"Script %s looping \n",total_filename);

             }
          if (c == '\n')
            {
              if (strlen(running_script_name)==0) { return 0; } // THE SCRIPT HAS BEEN CANCELED

              fprintf(stderr,"SCRIPT RUNNER RUNNING \"%s\" \n",line);
              IssueCommandInternal(line,from,output_string,512);

              line[0]=0; line_length=0;
            }
          else
            {
              line[line_length]=c;
              ++line_length;
              line[line_length]=0; // always append null termination ;P
            }
        }
      while (c != EOF);
      fclose (pFile);
    }
  else
    {
      fprintf(stderr,"Cannot open script %s \n",total_filename);
    }


   return 1;
}

int StartScript(char * script_name , char * from, int recursion_level)
{

  if ( filename_stripper_found_attack(script_name) )
     {
         fprintf(stderr,"Found an incoming attack on scripting engine from %s \n",from);
         return 0;
     }


  strcpy(running_from_user,from);
  strcpy(running_script_name,script_name);

  return 1;
}

int StopScript(char * script_name)
{
  // THIS FOR NOW STOPS THE ONLY SCRIPT RUNNING
  // ( ONLY ONE SCRIPT IS ALLOWED TO RUN )
  running_script_name[0]=0;
  running_script_name[1]=0;
  return 1;
}

void * ScriptKernelLoop(void *ptr )
{
   SCRIPT_RUNNER_OK=1;
   while ( !STOP_SCRIPT_RUNNER )
     {
        if ( strlen(running_script_name) > 0 )
          {
            InternalRunScript(running_script_name,running_from_user,0);
            running_script_name[0]=0;
            running_script_name[1]=0;
          }


       if ( running_script_name[0] != 0 ) { usleep(100); } else
                                          { usleep(100*1000); } //100 ms sleep time when not having a script loaded

     }
  SCRIPT_RUNNER_OK=0;
  return 0;
}


int StartScriptRunnerServer()
{
    STOP_SCRIPT_RUNNER=0;
    if ( pthread_create( &script_kernel_loop_id , NULL,  ScriptKernelLoop ,(void*) 0) != 0 )
     {
         fprintf(stderr,"Error creating script loop \n");
         return 0;
     }
    return 1;
}

int StopScriptRunnerServer()
{
   STOP_SCRIPT_RUNNER=1;
   return 1;
}

int ScriptRunnerStopped()
{
   if ((!SCRIPT_RUNNER_OK) && (STOP_SCRIPT_RUNNER))
    {
      return 1;
    }
   return 0;
}

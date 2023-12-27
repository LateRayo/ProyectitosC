#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <inistd.h>


int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("error en argumetnos");
        exit(-1);
    }

    struct stat data;
    DIR *dir;
    struct dirent *dp;
    char *file;
    int p1[1],p[2];

    if (stat(argv[1], &data))
    {
        printf("Ha fallado stat\n");
        exit(-1);
    }

    if (!S_ISDIR(data.stmode))
    {
        printf("no es un directorio\n");

    }
    
    dir = opendir(argv[1]);
    dp = readdir(dir);
    while (dp != NULL)
    {   
        if(strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))
        {
            file = malloc(strlen(argv[1] + strlen(dp->d_name) + 2))
            sprintf(file,"%s/%s",argv[1],dp->d_name);
            stat(file, &data)
            if (S_ISDIR(data.st_mode))
            {
                if(fork())
                {
                    wait(NULL);
                }else
                {   
                    pipe(p2);

                    if(fork())
                    {
                        close(0);
                        dup(p2[0]);
                        close(p2[0]);
                        close(p2[1]);

                        printf("numero de ficheros en %s: ",file);
                        fflush(stdout);
                        execlp("wc","wc", "-l", NULL);
                        printf("error wc\n");
                        exit(-1);
                    }else
                    {   
                        pipe(p1);
                        if (fork())
                        {
                            close(0);
                            dup(p1[0]);
                            close(1);
                            close(p2[1]);     

                            close(p1[0]);        
                            close(p1[1]);       
                            close(p2[0]);        
                            close(p2[1]);        

                            execlp("grep","grep", "^-", NULL);
                            printf("error grep\n");
                            exit(-1);  
                        }else
                        {
                            close(1);
                            dup(p1[1]);
                            close(p1[0]);
                            close(p1[1]); 
                            close(p2[0]);  
                            close(p2[1]);  

                            execlp("ls","ls", "-l","file", NULL);
                            printf("error ls\n");
                            exit(-1);  
                        }
                    }
                }
            }
            free(file);
        }
    }
    close(dir);
    
    

    return 0;
}

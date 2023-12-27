#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int pipe1[2];

    if (pipe(pipe1) == -1) {
        perror("Error al crear el tubo");
        exit(EXIT_FAILURE);
    }

    char buf[64];
    for (int i = 0; i < sizeof(buf); i++)
    {
        buf[i] = 0;
    }
    

    if (fork()) 
    {   
        // proceso padre
        close(pipe1[1]);  // cierra el extremo de escritura
        read(pipe1[0], buf, sizeof(buf));  // escribe en el extremo de lectura
        close(pipe1[0]);  // cierra el extremo de lectura
        printf("Soy el padre, FD: %d, caracter: %s \n", pipe1[0], buf);
    } else
    {
        // proceso hijo
        strcpy(buf, "curso de tuberias");
        close(pipe1[0]);  // cierra el extremo de lectura
        write(pipe1[1], buf, strlen(buf));  // lee desde el extremo de escritura
        close(pipe1[1]);  // cierra el extremo de escritura
        printf("Soy el hijo, FD: %d, caracter: %s \n", pipe1[1], buf);
    }

    return 0;
}

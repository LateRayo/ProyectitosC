#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <error.h>

//nombre
//extencion
//cantidad
//hilos(por defecto 1);

int num_threads = 1, num_files;
const char *path, *name, *extencion;

void *createFile(void *idPointer) 
{
    int id = *(int *)idPointer;
    int files_to_create = num_files / num_threads;
    int start_file = files_to_create * id;
    int end_file = start_file + files_to_create;

    // por si la división no termina con números enteros
    if (id == num_threads - 1) 
    {
        end_file = num_files;
    }

    char file[256];

    for (int i = start_file; i < end_file; i++) 
    {
        sprintf(file, "%s/%s%03i.%s", path, name, i + 1, extencion);
        //crear los archivos
        FILE *f = fopen(file, "w");
        if (f == NULL) 
        {
            perror("Error al abrir el archivo: ");
            printf("\nEl fichero \033[01;34m%s\033[0m no se pudo crear\n",file);

        } else 
        {   
            printf("Hilo \033[01;33m%d\033[0m ha crado el fichero \033[01;34m%s\033[0m\n",id,file);
            fclose(f);
        }
    }

    pthread_exit(0);
}

void start_threads() 
{
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    for (int i = 0; i < num_threads; i++) 
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, createFile, &thread_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) 
    {
        pthread_join(threads[i], NULL);
    }
}

// comprueba los argumentos
void evalArg(int argc, char const *argv[]) 
{
    if (argc < 5) 
    {
        printf("uso : ruta nomnbre extencon numero hilos\n");
        exit(-1);
    }

    // comprueba la existencia del directorio
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) 
    {
        perror("Error al abrir el directorio");
        exit(EXIT_FAILURE);
    }
    closedir(dir);

    path = argv[1];
    name = argv[2];
    extencion = argv[3];
    num_files = atoi(argv[4]);

    // comprueba si añadió el argumento de hilos
    // si no hay argumto, num_threads es igual a 1
    if (argc == 6) {
        num_threads = atoi(argv[5]);
    }
}

int main(int argc, char const *argv[]) 
{
    evalArg(argc, argv);
    start_threads();

    return 0;
}

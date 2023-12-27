#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>       
#include <time.h>
#include <string.h>
#include "lista.h"

#define longitud 100

int abrirDB();
int crearDB();
int cargarDatos(equipamiento *buf, int *arraySku, int id);
int verificarExistencia(char *File);
int crearID();
void Inspeccionar(int File);
void anadir(int File);
void colocarFecha(int File);
void modificacion(int File);
void String0(char * str);
void Eliminar(int File);

int main(int argc, char const *argv[])
{   
    char s;
    int id;

    //limpia la consola
    system("clear");

    //bluce principal
    while (s != 'q')
    {
        printf("Base de datos e-Sante\n\n[1]Introduzca una\n[2]cree una\n[q]para salir\n");
        scanf(" %c",&s);

        switch (s)
        {
        case '1':
            system("clear");
            abrirDB();
            break;
        case '2':
            system("clear");
            crearDB();
            break;
        case 'q':
            break;
        default:
            printf("Opcion incorrecta,vuelva a intentarlo\n");
            break;
        }
    }
    return 0;
}

//-------------------------------------------------------------------------------//

int abrirDB()
{   
    char respuesta = 'y';
    int File1 =0;
    char s =0;
    char fecha[11];
    equipamiento *buf = (equipamiento*)malloc(BUFSIZE);
    char *path = (char*)malloc(sizeof(char)*100);

    if (path == 0)
    {
        printf("Error al asignar memoria");
        exit(-1);
    }

    printf("Introduce la ruta del DB:\n");
    scanf(" %s",path);
    
    File1 = open(path,O_RDWR);

    if (File1 == -1) 
    {
        perror("Error al abrir el archivo");
        free(path);
        free(buf);
        return -1; 
    }

    printf("\nQue deseas hacer:\n[1]Insperccionar\n[2]Eliminar elemento\n[3]Añadir elemento\n[4]Modificar elemento\n[5]Mostrar fehca de modificacion/creacion\n");
    scanf(" %c",&s);
    switch (s)
    {
    case '1':
        Inspeccionar(File1);
        break;
    case '2':
        while (respuesta == 'y' || respuesta == 'Y')
        {   
            Eliminar(File1);
            close(File1);
            if (rename("temp", path) != 0) 
            {
                perror("Error al cambiar el nombre del archivo");
                close(File1);
                free(path);
                free(buf);
                return -1;
            }       
            printf("\ndesea Eliminar otro elemento (y/n): ");
            scanf(" %c",&respuesta);
            
            File1 = open(path,O_RDWR);
            if (File1 == -1) 
            {
                perror("Error al abrir el archivo");
                free(path);
                free(buf);
                return -1; 
            }
        }
         
        break;
    case '3':
        colocarFecha(File1);
        while (respuesta == 'y' || respuesta == 'Y')
        {   
            anadir(File1);
            printf("\ndesea cargar otro elemento (y/n): ");
            scanf(" %c",&respuesta);
            close(File1);
            File1 = open(path,O_RDWR);
            if (File1 == -1) 
            {
                perror("Error al abrir el archivo\n");
                free(path);
                free(buf);
                return -1; 
            }
        }
        break;
    case '4':
        colocarFecha(File1);
        while (respuesta == 'y' || respuesta == 'Y')
        {   
            modificacion(File1);
            printf("\ndesea modificar otro elemento (y/n): ");
            scanf(" %c",&respuesta);
            close(File1);
            File1 = open(path,O_RDWR);
            if (File1 == -1) 
            {
                perror("Error al abrir el archivo\n");
                free(path);
                free(buf);
                return -1; 
            }
        }
        break;
    case '5':
        read(File1,fecha,11);
        printf("fecha del archivo %s: %s\n\n",path,fecha);
        break;
    default:
        break;
    }

    close(File1);
    free(path);
    free(buf);
    return 0;
}

//-------------------------------------------------------------------------------//

int crearDB()
{

    int File1 = 0;
    int y = 0;
    int readed = BUFSIZE;
    char respuesta = 'y';
    int id;
    
    time_t tiempo;
    struct tm *infoTiempo;
    char fecha[11];

    equipamiento *buf = (equipamiento*)malloc(BUFSIZE);
    char *name = (char*)malloc(sizeof(char)*longitud);
    int *arraySku = (int*)calloc(longitud,sizeof(int));

    if (name == 0 || buf == 0 || arraySku == 0)
    {
        printf("Error al asignar memoria");
        free(name);
        free(buf);
        free(arraySku);
        exit(-1);
    }

    printf("eliga un nombre para la nueva base de datos: \n");
    scanf(" %s", name);
    printf("\n");

    //verifica si existe un dipliaco.
    if (verificarExistencia(name) == 2)
    {   
        free(name);
        free(buf);
        free(arraySku);
        return 0; //veulve a l menu principal.
    }
    

    printf("a continuacion llena los siguentes campos para añadir UN objeto: \n");
    printf("(Luego podra añadir mas)\n\n");

    File1 = open(name, O_RDWR | O_CREAT | O_TRUNC, 0660);

//---------------
    //obtego la fehca actual
    time(&tiempo);
    infoTiempo = localtime(&tiempo);
    strftime(fecha, sizeof(fecha), "%d/%m/%Y", infoTiempo);
    write(File1,fecha,sizeof(fecha));
 //------------   
    while (respuesta == 'y' || respuesta == 'Y')
    {   
        id = crearID();
        if (id == -1) 
        {   
            free(name);
            free(buf);
            free(arraySku);
            return 0; //veulve a l menu principal.
        }
        *(arraySku+y) = cargarDatos(buf,arraySku,id);
        if (*(arraySku+y) == 101) 
        {   //el usuario eligio volver al menu principal
            free(name);
            free(buf);
            free(arraySku);
            return 0; //veulve a l menu principal.
        }

        write(File1,buf,readed);
        close(id);
        //ImprimirStrcuct(buf);
        printf("\ndesea cargar otro elemento (y/n): ");
        scanf(" %c",&respuesta);
        y++;
        
    }

    free(name);
    free(buf);
    close(File1);
    free(arraySku);

    return 0;
}

//-------------------------------------------------------------------------------//

int cargarDatos(equipamiento *buf, int *arraySku, int id)
{
    int especialidad = 0;
    int sku = 0;
    char respuesta;
//-----------
    int bufaux;

    read(id,&bufaux,sizeof(int));
    //printf("bufaux: %d", bufaux);
    bufaux = bufaux +1;
    lseek(id,0,SEEK_SET);
    write(id,&bufaux,sizeof(int));
//----------------
    (buf->id) = bufaux;
    
    while (sku == 0)
    {
        printf("\nsku: ");
        scanf(" %d", &sku);
        for (int i = 0; i < longitud ; i++)
        {
            if (sku == *(arraySku+i))
            {
                printf("sku ya ingesado, cambielo\n");
                printf("¿Desea volver al menu principla?(y/n): ");
                scanf(" %c", &respuesta);
                if (respuesta == 'y' || respuesta == 'Y') 
                {
                    printf("elegiste volver al menu principla.\n");
                    sku = 101;
                    return sku;//vuelve al menu pricipal
                }
                i = longitud;
                sku = 0;
            }
        }
    }
    (buf->sku) = sku;

    printf("\ndescription (menos de 20 caracteres): ");
    getc(stdin);
    fgets(buf->description, 20, stdin);

    printf("\ndetalles: ");
    fgets(buf->detalles, 200, stdin);

    printf("\ncantidad: ");
    scanf(" %d", &(buf->cantidad));

    while (especialidad < 1 || especialidad > 7)
    {
        printf("\nEspecialidad:\n[1]Cardiologia\n[2]Clinica\n[3]Gastroenterologia\n[4]Cirugia\n[5]Dermatologia\n[6]Oftalmologia\n[7]Traumatologia\n");
        scanf(" %d", &especialidad);

        if (especialidad >= 1 && especialidad <= 7) 
        {
            buf->especialidad = especialidad;
        } else 
        {
            printf("Especialidad no válida. Debe estar entre 1 y 7.\n");
        }
    }
    
    printf("\nprecio: ");
    scanf(" %f", &(buf->precio));

    return sku;

}

//-------------------------------------------------------------------------------//

int verificarExistencia(char *File)
{   
    char respuesta;

    if (access(File, F_OK) != -1) //entra si exixte
    {   
        printf("El archivo ya existe. ¿Deseas sobreescribirlo? (y/n): ");
        getc(stdin);
        //__fpurge(stdin);
        //fflush(stdin);
        scanf("%c", &respuesta);
        if (respuesta == 'y' || respuesta == 'Y') 
        {
            printf("Elegiste sobreescribirlo.\n\n");
            return 1;//desea sobreescribirlo.
        }else
        {
            printf("No lo sobreescibiete.Vuelve al menu Principal.\n\n");
            return 2; //no desea sobreescribirlo.
        }
    }
    return 0; //el archivo no exsite, procede con normalidad.
}

//---------------------------------------------------------------------//

int crearID()
{
    //archivo id.
    int cero = 0;
    int id;
    if (access("id", F_OK) == -1)//entra si no existe
    {
        id = open("id",O_RDWR | O_CREAT , 0666);
        if (id == -1) 
        {
            perror("Error al abrir el archivo\n");
            return -1; 
        }

        write(id,&cero,sizeof(int));
    }else
    {
        id = open("id",O_RDWR);
    }

    return id;
}

//---------------------------------------------------------------//

void Inspeccionar(int File)
{   
    Nodo *lista = NULL;
    int readed = BUFSIZE;
    int especialidad = 0;
    equipamiento *buf = (equipamiento*)calloc(1,BUFSIZE);
    char s = 0;

    lseek(File,11,SEEK_SET);

    readed = read(File,buf,BUFSIZE);
    do
    {
        lista = agregarElemento(lista,*buf);
        readed = read(File,buf,BUFSIZE);
    }while (readed == BUFSIZE);

    printf("Que quieres hacer:\n[1]Ordenar por especialidad\n[2]Ordenar por precio(menor-mayor)\n[3]Ordenar por disponibilidad\n[4]Mostrar determinada especialidad\n[q]salir\n");
    scanf(" %c",&s);
    printf("\n");

    switch (s)
    {
    case '1':
        system("clear");
        ordenarPorEspecialidad(lista);
        imprimirLista(lista);
        break;
    case '2':
        system("clear");
        ordenarPorPrecio(lista);
        imprimirLista(lista);
        break;
    case '3':
        system("clear");
        ordenarPorDisponibilidad(lista);
        imprimirLista(lista);
        break;
    case '4':
        system("clear");
        printf("\nEspecialidad:\n[1]Cardiologia\n[2]Clinica\n[3]Gastroenterologia\n[4]Cirugia\n[5]Dermatologia\n[6]Oftalmologia\n[7]Traumatologia\n");
        scanf(" %d", &especialidad);
        mostrarEspecialidad(lista,especialidad);
        break;
    default:
        free(buf);
        liberarLista(lista);
        return;
        break;
    }

    free(buf);
    liberarLista(lista);
}

//----------------------------------------------------------------------//

void anadir(int File)
{

    int id;
    int readed = BUFSIZE;
    int y = 0;
    equipamiento *buf = (equipamiento*)malloc(BUFSIZE);
    int *arraySku = (int*)calloc(longitud,sizeof(int));

    if (buf == 0 || arraySku == 0)
    {
        printf("Error al asignar memoria\n");
        free(buf);
        free(arraySku);
        exit(-1);
    }

    printf("A continuacion llena los siguentes campos para añadir UN objeto: \n");
    printf("(Luego podra añadir mas)\n\n");
    
    lseek(File,11,SEEK_SET);

    readed = read(File,buf,BUFSIZE);
    do
    {
        arraySku[y] = buf->sku;
        readed = read(File,buf,BUFSIZE);
        y++;
    }while (readed == BUFSIZE);

    // for (int i = 0; i < 100; i++)
    // {
    //     printf("%d,",arraySku[i]);
    // }
    // printf("\n");

    if (lseek(File,0,SEEK_END) == -1) 
    {
        perror("Error al mover el cursor al final del archivo");
        close(File);
        return;
    }

    id = crearID();
    if ( id == -1) 
    {   //huno un error
        free(buf);
        free(arraySku);
        return; //veulve a l menu principal.
    }
    
    *(arraySku + y) = cargarDatos(buf,arraySku,id);
    if (*(arraySku + y) == 101) 
    {   //el usuario eligio volver al menu principal
        free(buf);
        free(arraySku);
        return; //veulve a l menu principal.
    }
        
    write(File,buf,BUFSIZE);
    lseek(File,0,SEEK_END);

    close(id);
    free(buf);
    free(arraySku);
}

void colocarFecha(int File)
{
    time_t tiempo;
    struct tm *infoTiempo;
    char fecha[11];

    time(&tiempo);
    infoTiempo = localtime(&tiempo);
    strftime(fecha, sizeof(fecha), "%d/%m/%Y", infoTiempo);

    if (lseek(File,0,SEEK_SET) == -1) 
    {
        perror("Error al mover el cursor al inicio del archivo");
        close(File);
        return;
    }

    write(File,fecha,sizeof(fecha));
}

void modificacion(int File)
{
    int readed = BUFSIZE;
    int respuesta = 0;
    int respuesta2 = 0;
    equipamiento *buf = (equipamiento*)malloc(BUFSIZE);

    if (buf == 0)
    {
        printf("Error al asignar memoria");
        free(buf);

        exit(-1);
    }
    lseek(File,0,SEEK_SET);

    printf("Elija el producto que desea modificar con el sku\n");
    printf("sku: ");
    scanf(" %d",&respuesta);
    lseek(File,11,SEEK_SET);

    readed = read(File,buf,BUFSIZE);
    do
    {   
        if (buf->sku == respuesta)
        {
            ImprimirStrcuct(buf);

            while (respuesta2 < 1 || respuesta2 > 5)
            {
                printf("\nQue desea cambiar:\n[1]Description\n[2]Destalles\n[3]cantidad\n[4]Especilidad\n[5]precio\n");
                scanf(" %d",&respuesta2);

                switch (respuesta2)
                {
                case 1:
                    printf("Description: ");
                    getc(stdin);
                    String0(buf->description);
                    fgets(buf->description, 20, stdin);
                    break;
                case 2:
                    printf("Detalles: ");
                    getc(stdin);
                    String0(buf->detalles);
                    fgets(buf->detalles, 200, stdin);
                    break;
                case 3:
                    printf("Cantidad: ");
                    scanf(" %d",&respuesta2);
                    buf->cantidad = respuesta2;
                    break;
                case 4:
                    printf("\nEspecialidad:\n[1]Cardiologia\n[2]Clinica\n[3]Gastroenterologia\n[4]Cirugia\n[5]Dermatologia\n[6]Oftalmologia\n[7]Traumatologia\n");
                    scanf(" %d",&respuesta2);
                    buf->especialidad = respuesta2;
                    break;
                case 5:
                    printf("Precio: ");
                    scanf("%d",&respuesta2);
                    buf->precio = respuesta2;
                    break;
                default:
                    printf("respuesta invalidad\n");
                    break;
                }
                respuesta2 = 3;
            }
            lseek(File,-BUFSIZE,SEEK_CUR);
            write(File,buf,BUFSIZE);
            free(buf);
            return;
        }
        readed = read(File,buf,BUFSIZE);
    }while (readed == BUFSIZE);

    printf("sku no encontrado\n");
    free(buf);
}

void String0(char * str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        str[i] = '\0';
    }
}

void Eliminar(int File)
{
    int readed = BUFSIZE;
    int File2;
    int respuesta = 0;
    char respuesta2 = 0;
    equipamiento *buf = (equipamiento*)malloc(BUFSIZE);

    if (buf == 0)
    {
        printf("Error al asignar memoria");
        free(buf);
        exit(-1);
    }
    lseek(File,0,SEEK_SET);

    printf("Elija el producto que desea eliminar con el sku\n");
    printf("sku: ");
    scanf(" %d",&respuesta);
    lseek(File,11,SEEK_SET);

    readed = read(File,buf,BUFSIZE);
    do
    {   
        if (buf->sku == respuesta)
        {
            ImprimirStrcuct(buf);
            printf("¿estas seguro?[y/n]\n");
            scanf(" %c",&respuesta2);
            if (respuesta2 == 'y' || respuesta2 == 'Y')
            {   
                File2 = open("temp", O_RDWR | O_CREAT | O_TRUNC, 0660);

                lseek(File,11,SEEK_SET);
                
                colocarFecha(File2);
                lseek(File2,11,SEEK_SET);

                while ((readed == BUFSIZE))
                {
                    readed = read(File,buf,BUFSIZE);
                    if (buf->sku != respuesta)
                    {
                        write(File2,buf,readed);
                        
                    }
                }
                close(File2);
                free(buf);
                return;                
            }else
            {   
                printf("Eliminacion cancelada\n");
                return;
            }
            
        }
        readed = read(File,buf,BUFSIZE);
    }while (readed == BUFSIZE);

    printf("sku no encontrado\n");
    free(buf);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"


Nodo* crearNodo(equipamiento data) 
{
    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    nuevoNodo->info = data;
    nuevoNodo->prev = NULL;
    nuevoNodo->next = NULL;
    return nuevoNodo;
}

Nodo* agregarElemento(Nodo* lista, equipamiento data)
{ //primer parametro: puntero al primer nodo de la lista
  //segundo parametro: informacion del archivo
    Nodo* nuevoNodo = crearNodo(data);
    
    if (lista == NULL) 
    {
        return nuevoNodo;
    }

    Nodo* actual = lista;

    while (actual->next != NULL) 
    {
        actual = actual->next;
    }

    actual->next = nuevoNodo;
    nuevoNodo->prev = actual;

    return lista;
}

void liberarLista(Nodo* lista) 
{
    while (lista != NULL) 
    {
        Nodo* temp = lista;
        lista = lista->next;
        free(temp);
    }
}

void imprimirLista(Nodo* lista) 
{
    Nodo* actual = lista;

    while (actual != NULL) 
    {
        ImprimirStrcuct(&(actual->info));
        actual = actual->next;
    }
}

void ImprimirStrcuct(equipamiento *buf)
{   
    printf("Id: %d\n", buf->id);
    printf("Sku: %d\n", buf->sku);

    // Imprimir description sin saltos de línea adicionales
    printf("Description: ");
    for (int i = 0; buf->description[i] != '\0'; i++) {
        if (buf->description[i] != '\n') {
            printf("%c", buf->description[i]);
        }
    }
    printf("\n");

    // Imprimir detalles sin saltos de línea adicionales
    printf("Detalles: ");
    for (int i = 0; buf->detalles[i] != '\0'; i++) {
        if (buf->detalles[i] != '\n') {
            printf("%c", buf->detalles[i]);
        }
    }
    printf("\n");

    printf("cantidad: %d\n", buf->cantidad);
    printf("Especialidad: %d\n", buf->especialidad);
    printf("Precio: %f\n", buf->precio);
    printf("\n");
}

void ordenarPorPrecio(Nodo* lista) 
{
    if (lista == NULL || lista->next == NULL) 
    {
        // La lista está vacía o tiene solo un elemento, no se necesita ordenar.
        return;
    }

    Nodo* actual = lista;
    
    while (actual->next != NULL) 
    {
        Nodo* siguiente = actual->next;
        
        while (siguiente != NULL) 
        {
            if (actual->info.precio > siguiente->info.precio) 
            {
                // Intercambiar datos de equipamiento
                equipamiento temp = actual->info;
                actual->info = siguiente->info;
                siguiente->info = temp;
            }
            
            siguiente = siguiente->next;
        }
        
        actual = actual->next;
    }
}

void ordenarPorEspecialidad(Nodo* lista) 
{
    if (lista == NULL || lista->next == NULL) 
    {
        // La lista está vacía o tiene solo un elemento, no se necesita ordenar.
        return;
    }

    Nodo* actual = lista;
    
    while (actual->next != NULL) 
    {
        Nodo* siguiente = actual->next;
        
        while (siguiente != NULL) 
        {
            if (actual->info.especialidad> siguiente->info.especialidad) 
            {
                // Intercambiar datos de equipamiento
                equipamiento temp = actual->info;
                actual->info = siguiente->info;
                siguiente->info = temp;
            }
            
            siguiente = siguiente->next;
        }
        
        actual = actual->next;
    }
}

void ordenarPorDisponibilidad(Nodo* lista) 
{
    if (lista == NULL || lista->next == NULL) 
    {
        // La lista está vacía o tiene solo un elemento, no se necesita ordenar.
        return;
    }

    Nodo* actual = lista;
    
    while (actual->next != NULL) 
    {
        Nodo* siguiente = actual->next;
        
        while (siguiente != NULL) 
        {
            if (actual->info.cantidad > siguiente->info.cantidad) 
            {
                // Intercambiar datos de equipamiento
                equipamiento temp = actual->info;
                actual->info = siguiente->info;
                siguiente->info = temp;
            }
            
            siguiente = siguiente->next;
        }
        
        actual = actual->next;
    }
}

void mostrarEspecialidad(Nodo* lista, int especilidad) 
{
    if (lista == NULL || lista->next == NULL) 
    {
        // La lista está vacía o tiene solo un elemento, no se necesita ordenar.
        return;
    }

    Nodo* actual = lista;
    
    while (actual->next != NULL) 
    {
        Nodo* siguiente = actual->next;
        
            if (actual->info.especialidad ==  especilidad)
            {
                ImprimirStrcuct(&(actual->info));
            }
            
            siguiente = siguiente->next;
        
        
        actual = actual->next;
    }
}

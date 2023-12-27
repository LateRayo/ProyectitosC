#ifndef LISTA_H
#define LISTA_H

struct equipamiento
{
    int id;
    int sku;
    char description[20];
    char detalles[200];
    int cantidad;
    int especialidad;
    float precio;
};

struct Nodo {
    struct equipamiento info;
    struct Nodo* prev;
    struct Nodo* next;
};

struct equipamiento typedef equipamiento;
struct Nodo typedef Nodo;
#define BUFSIZE sizeof(struct equipamiento)

Nodo* crearNodo(equipamiento data);
Nodo* agregarElemento(Nodo* lista, equipamiento data);
void imprimirLista(Nodo* lista);
void liberarLista(Nodo* lista);
void ImprimirStrcuct(equipamiento *buf);
void ordenarPorPrecio(Nodo* lista);
void ordenarPorEspecialidad(Nodo* lista);
void ordenarPorDisponibilidad(Nodo* lista);
void mostrarEspecialidad(Nodo* lista, int especilidad);

#endif
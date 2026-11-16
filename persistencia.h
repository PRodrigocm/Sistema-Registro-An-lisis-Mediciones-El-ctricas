/* =============================================================
   PERSISTENCIA.H  --  Encabezado del Grupo B
   Sistema de Registro y Analisis de Mediciones Electricas
   ------------------------------------------------------------
   SPRINT 1: lectura de archivos de texto.
   En los siguientes sprints se incorporan aqui las funciones
   de guardado, carga y analisis de las mediciones.

   El encabezado contiene los prototipos y las implementaciones
   (static), por lo que el proyecto se compila con un unico
   archivo de traduccion:  gcc main.c -o main.exe
   ============================================================= */

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "mediciones.h"

#include <stdio.h>
#include <string.h>

/* ---------- Archivos que manipula el programa --------------- */
#define ARCHIVO_DESCRIPCION "descripcion.txt"

/* ---------- Archivo de descripcion: 5 lineas de texto ------ */
#define MAX_LINEAS  5
#define LARGO_LINEA 120

/* =============================================================
   PROTOTIPOS -- GRUPO B (Sprint 1: 1 funcion)
   ============================================================= */
static void mostrarDescripcion(void);

/* =============================================================
   IMPLEMENTACION
   ============================================================= */

/* =============================================================
   FUNCION 1: Mostrar el contenido del archivo de descripcion
   (5 lineas de texto). Lee con fgets y recorta el salto de
   linea con strcspn.
   ============================================================= */
static void mostrarDescripcion(void) {
    FILE *archivo;
    char  linea[LARGO_LINEA];
    int   leidas = 0;

    printf("\n=== DESCRIPCION DEL SISTEMA (%s) ===\n", ARCHIVO_DESCRIPCION);

    archivo = fopen(ARCHIVO_DESCRIPCION, "r");
    if (archivo == NULL) {
        printf("  Error: no se pudo abrir '%s' para lectura.\n", ARCHIVO_DESCRIPCION);
        return;
    }

    while (leidas < MAX_LINEAS && fgets(linea, (int) sizeof(linea), archivo) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';
        printf("%d. %s\n", leidas + 1, linea);
        leidas++;
    }

    fclose(archivo);
    printf("Lineas leidas: %d de %d\n", leidas, MAX_LINEAS);
}

#endif /* PERSISTENCIA_H */

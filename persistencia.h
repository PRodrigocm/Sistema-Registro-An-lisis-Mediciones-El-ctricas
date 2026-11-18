/* persistencia.h
   Funcion que lee el archivo de descripcion (5 lineas). */

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <stdio.h>

/* Muestra las 5 lineas del archivo descripcion.txt */
void mostrarDescripcion(void) {
    FILE *f;
    char linea[120];
    int i;

    f = fopen("descripcion.txt", "r");
    if (f == NULL) {
        printf("No se pudo abrir descripcion.txt\n");
        return;
    }

    for (i = 0; i < 5 && fgets(linea, 120, f) != NULL; i++) {
        printf("%d. %s", i + 1, linea);
    }
    fclose(f);
}

#endif

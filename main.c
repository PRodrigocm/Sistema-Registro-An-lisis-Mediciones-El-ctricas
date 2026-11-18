/* main.c
   Programa principal: menu y uso de punteros para recorrer los datos. */

#include <stdio.h>
#include <string.h>
#include "mediciones.h"
#include "persistencia.h"

int main(void) {
    struct Medicion lista[MAX_MEDICIONES];
    struct Medicion *ptr;
    char buscado[80];
    int total = 0;
    int opcion = 0;
    int pos;

    mostrarDescripcion();

    do {
        printf("\n1. Registrar medicion\n");
        printf("2. Listar mediciones\n");
        printf("3. Buscar equipo por nombre\n");
        printf("4. Resumen\n");
        printf("5. Ver descripcion\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                if (total < MAX_MEDICIONES) {
                    ptr = &lista[total];      /* puntero al lugar libre */
                    ingresarMedicion(ptr);
                    total++;
                    printf("Medicion registrada.\n");
                } else {
                    printf("Ya no hay espacio (maximo %d).\n", MAX_MEDICIONES);
                }
                break;

            case 2:
                if (total == 0) {
                    printf("No hay mediciones.\n");
                }
                /* recorre el arreglo con un puntero */
                for (ptr = lista; ptr < lista + total; ptr++) {
                    mostrarMedicion(ptr);
                }
                break;

            case 3:
                printf("Nombre a buscar: ");
                scanf("%79s", buscado);
                pos = buscarPorNombre(lista, total, buscado);
                if (pos == -1) {
                    printf("No se encontro '%s'.\n", buscado);
                } else {
                    mostrarMedicion(&lista[pos]);
                }
                break;

            case 4:
                mostrarResumen(lista, total);
                break;

            case 5:
                mostrarDescripcion();
                break;

            case 0:
                printf("Hasta luego.\n");
                break;

            default:
                printf("Opcion no valida.\n");
                break;
        }
    } while (opcion != 0);

    return 0;
}

/* 
   MAIN.C  --  Programa principal
   Sistema de Registro y Analisis de Mediciones Electricas
   ------------------------------------------------------------
   SPRINT 2: modelo de datos y registro.
     - Arreglo de estructuras que almacena las mediciones
     - Puntero al registro activo para procesar los datos
     - Opcion 1: registrar una medicion nueva
   SPRINT 3: persistencia en archivo (opciones 3 y 4).
   SPRINT 4: listado y resumen estadistico (opciones 2 y 6).

   Compilacion:
     gcc -Wall -Wextra -pedantic -std=c11 main.c -o main.exe
    */

#include <stdio.h>
#include "mediciones.h"
#include "persistencia.h"

/* ---------- Valores por defecto de las variables globales --- */
float TARIFA_KWH       = 0.75f;
int   TOTAL_MEDICIONES = 0;

/* 
   FUNCION 1: Mostrar el menu principal en pantalla.
    */
static void mostrarMenu(void) {
    printf("\n========== MENU PRINCIPAL ==========\n");
    printf("1. Registrar medicion\n");
    printf("2. Listar mediciones registradas\n");
    printf("3. Guardar mediciones en archivo\n");
    printf("4. Cargar mediciones desde archivo\n");
    printf("5. Ver descripcion del sistema (5 lineas)\n");
    printf("6. Resumen estadistico general\n");
    printf("0. Salir\n");
    printf("-------------------------------------\n");
}

/* 
   FUNCION 2: Registrar una medicion nueva a traves de un
   puntero a estructura. Los calculos se almacenan dentro de la
   misma estructura a la que apunta el puntero.
    */
static void registrarMedicion(struct Medicion *medicion) {
    if (TOTAL_MEDICIONES >= MAX_MEDICIONES) {
        printf("\nLimite alcanzado: no se pueden registrar mas de %d mediciones.\n",
               MAX_MEDICIONES);
        return;
    }

    ingresarMedicion(medicion);
    TOTAL_MEDICIONES++;

    mostrarMedicion(medicion);
    evaluarPotencia(medicion->potencia);

    printf("\nMediciones registradas en esta sesion: %d\n", TOTAL_MEDICIONES);
    printf("Use la opcion 3 para guardar el registro en '%s'.\n", ARCHIVO_MEDICIONES);
}

/* 
   PROGRAMA PRINCIPAL
    */
int main(void) {
    /* Arreglo de estructuras que almacena las mediciones */
    struct Medicion lista[MAX_MEDICIONES];

    /* Puntero a una medicion del arreglo: se usa para procesar
       los datos dentro del programa principal (requisito 6). */
    struct Medicion *ptr = &lista[0];

    int opcion = 0;
    char linea[64];

    printf("=========================================\n");
    printf("  SISTEMA DE REGISTRO Y ANALISIS DE\n");
    printf("  MEDICIONES ELECTRICAS\n");
    printf("=========================================\n");

    mostrarDescripcion();

    do {
        mostrarMenu();

        /* La opcion se lee linea a linea con fgets para que el
           registro de mediciones empiece siempre en una linea
           nueva. Si la entrada no es un numero del rango se
           descarta y se vuelve a pedir; si la entrada se agota
           (EOF) el programa se cierra con normalidad. */
        printf("Opcion: ");
        linea[0] = '\0';
        if (fgets(linea, (int) sizeof(linea), stdin) == NULL) {
            printf("\nFin de la entrada.\n");
            opcion = 0;
        } else {
            linea[strcspn(linea, "\r\n")] = '\0';
            if (sscanf(linea, "%d", &opcion) != 1 || opcion < 0 || opcion > 6) {
                opcion = -1;
                printf("Opcion invalida: ingrese un numero entre 0 y 6.\n");
            }
        }

        switch (opcion) {
            case 1:
                if (TOTAL_MEDICIONES < MAX_MEDICIONES) {
                    ptr = &lista[TOTAL_MEDICIONES];
                    registrarMedicion(ptr);
                } else {
                    printf("\nLimite alcanzado: no se pueden registrar mas de %d mediciones.\n",
                           MAX_MEDICIONES);
                }
                break;

            case 2:
                listarMediciones(lista, TOTAL_MEDICIONES);
                break;

            case 3:
                guardarMediciones(lista, TOTAL_MEDICIONES);
                break;

            case 4:
                cargarMediciones(lista, MAX_MEDICIONES, &TOTAL_MEDICIONES);
                break;

            case 5:
                mostrarDescripcion();
                break;

            case 6:
                mostrarEstadisticas(lista, TOTAL_MEDICIONES);
                break;

            case 0:
                printf("\nPrograma finalizado. Hasta luego.\n");
                break;

            default:
                break;
        }
    } while (opcion != 0);

    return 0;
}

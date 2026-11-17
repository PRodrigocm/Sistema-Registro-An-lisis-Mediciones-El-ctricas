/* =============================================================
   PERSISTENCIA.H  --  Encabezado del Grupo B
   Sistema de Registro y Analisis de Mediciones Electricas
   ------------------------------------------------------------
   SPRINT 1: lectura del archivo de descripcion (5 lineas).
   SPRINT 3: guardado y carga de las mediciones en mediciones.txt.
   SPRINT 4: resumen por equipo, listado y estadisticas globales.

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
#define ARCHIVO_MEDICIONES  "mediciones.txt"

/* ---------- Archivo de descripcion: 5 lineas de texto ------ */
#define MAX_LINEAS  5
#define LARGO_LINEA 120

/* ---------- Archivo de mediciones: un registro por linea --- */
#define LARGO_REGISTRO 512

/* =============================================================
   PROTOTIPOS -- GRUPO B (6 funciones)
   ============================================================= */
static void mostrarDescripcion(void);

static int  guardarMediciones(const struct Medicion *lista, int total);
static int  cargarMediciones(struct Medicion *lista, int maximo, int *total);

static void mostrarResumen(const struct Medicion *medicion);
static void listarMediciones(const struct Medicion *lista, int total);
static void mostrarEstadisticas(const struct Medicion *lista, int total);

/* =============================================================
   IMPLEMENTACION
   ============================================================= */

/* 
   FUNCION 1: Mostrar el contenido del archivo de descripcion
   (5 lineas de texto). Lee con fgets y recorta el salto de
   linea con strcspn.
    */
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

/* 
   FUNCION 2: Guardar todo el registro en mediciones.txt
   Escribe un registro por linea con 12 campos separados por
   '|'. Antes de escribir reemplaza el separador '|' dentro de
   los textos (strchr) para no romper el formato. El archivo se
   reescribe completo, de modo que siempre coincide con lo que
   hay en memoria.
   */
static int guardarMediciones(const struct Medicion *lista, int total) {
    FILE *archivo;
    int   i;

    if (total <= 0) {
        printf("\nNo hay mediciones en memoria para guardar.\n");
        printf("El archivo '%s' no se modifico.\n", ARCHIVO_MEDICIONES);
        return 0;
    }

    archivo = fopen(ARCHIVO_MEDICIONES, "w");
    if (archivo == NULL) {
        printf("\nError: no se pudo abrir '%s' para escritura.\n", ARCHIVO_MEDICIONES);
        return 0;
    }

    for (i = 0; i < total; i++) {
        char  nombre[MAX_NOMBRE];
        char  detalle[MAX_DETALLE];
        char *pos;

        snprintf(nombre, sizeof(nombre), "%s", lista[i].nombre);
        snprintf(detalle, sizeof(detalle), "%s", lista[i].detalle);

        while ((pos = strchr(nombre, '|')) != NULL) {
            *pos = '-';
        }
        while ((pos = strchr(detalle, '|')) != NULL) {
            *pos = '-';
        }

        fprintf(archivo, "%d|%s|%s|%.2f|%.2f|%.4f|%.4f|%.2f|%.2f|%.2f|%.6f|%.4f\n",
                lista[i].codigo, nombre, detalle,
                lista[i].voltaje, lista[i].corriente,
                lista[i].potencia, lista[i].potenciaReal,
                lista[i].resistencia, lista[i].horas,
                lista[i].factor, lista[i].energia,
                lista[i].costo);
    }

    fclose(archivo);
    printf("\nSe guardaron %d mediciones en '%s'.\n", total, ARCHIVO_MEDICIONES);
    return 1;
}

/* 
   FUNCION 3: Cargar las mediciones desde mediciones.txt
   Lee linea a linea con fgets y separa los 12 campos con
   sscanf (sin escribir un analizador manual). Los valores
   derivados se recalculan con las formulas del Grupo A para
   garantizar que el registro cargado sea coherente.
 */
static int cargarMediciones(struct Medicion *lista, int maximo, int *total) {
    FILE *archivo;
    char  linea[LARGO_REGISTRO];
    int   leidas = 0;
    int   numero = 0;
    int   extra  = 0;

    archivo = fopen(ARCHIVO_MEDICIONES, "r");
    if (archivo == NULL) {
        printf("\nNo existe '%s' o no se pudo abrir para lectura.\n", ARCHIVO_MEDICIONES);
        printf("Registre mediciones y guarde con la opcion 3.\n");
        return 0;
    }

    while (leidas < maximo && fgets(linea, (int) sizeof(linea), archivo) != NULL) {
        struct Medicion *medicion = &lista[leidas];
        int campos;

        numero++;
        linea[strcspn(linea, "\r\n")] = '\0';
        if (linea[0] == '\0') {
            continue;
        }

        campos = sscanf(linea,
                        "%d|%49[^|]|%79[^|]|%f|%f|%f|%f|%f|%f|%f|%f|%lf",
                        &medicion->codigo, medicion->nombre, medicion->detalle,
                        &medicion->voltaje, &medicion->corriente,
                        &medicion->potencia, &medicion->potenciaReal,
                        &medicion->resistencia, &medicion->horas,
                        &medicion->factor, &medicion->energia,
                        &medicion->costo);
        if (campos != 12) {
            printf("  Linea %d ignorada: formato invalido (%d de 12 campos).\n",
                   numero, campos);
            continue;
        }

        medicion->potencia     = calcularPotencia(medicion->voltaje, medicion->corriente);
        medicion->potenciaReal = calcularPotenciaReal(medicion->potencia, medicion->factor);
        medicion->resistencia  = calcularResistencia(medicion->voltaje, medicion->corriente);
        medicion->energia      = calcularEnergia(medicion->potenciaReal, medicion->horas);
        medicion->costo        = calcularCosto(medicion->energia, TARIFA_KWH);

        leidas++;
    }

    while (fgets(linea, (int) sizeof(linea), archivo) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';
        if (linea[0] != '\0') {
            extra++;
        }
    }

    fclose(archivo);
    *total = leidas;

    printf("\nSe cargaron %d mediciones desde '%s'.\n", leidas, ARCHIVO_MEDICIONES);
    if (extra > 0) {
        printf("  Aviso: %d mediciones no se cargaron (limite %d).\n", extra, maximo);
    }
    return 1;
}

/* 
   FUNCION 4: Resumen breve de una medicion.
   Presenta la energia, el costo y el largo de los textos
   (strlen), que es el conteo de caracteres del requisito 3.
    */
static void mostrarResumen(const struct Medicion *medicion) {
    printf("[%d] %s\n", medicion->codigo, medicion->nombre);
    printf("    Nombre : %2lu caracteres | Detalle: %2lu caracteres\n",
           (unsigned long) strlen(medicion->nombre),
           (unsigned long) strlen(medicion->detalle));
    printf("    Potencia: %8.2f W | Energia: %8.4f kWh | Costo: %8.2f\n",
           medicion->potencia, medicion->energia, medicion->costo);
}

/*
   FUNCION 5: Listar el registro completo en pantalla.
*/
static void listarMediciones(const struct Medicion *lista, int total) {
    int i;

    if (total <= 0) {
        printf("\nNo hay mediciones registradas en memoria.\n");
        printf("Registre una (opcion 1) o cargue el archivo (opcion 4).\n");
        return;
    }

    printf("\n=== LISTADO DE MEDICIONES (%d) ===\n", total);
    for (i = 0; i < total; i++) {
        mostrarResumen(&lista[i]);
    }
    printf("\nTotal listado: %d mediciones.\n", total);
}

/* 
   FUNCION 6: Resumen estadistico general del registro.
   Acumula totales y promedios y localiza la mayor y la menor
   potencia con punteros a estructuras del arreglo.
   */
static void mostrarEstadisticas(const struct Medicion *lista, int total) {
    const struct Medicion *mayor;
    const struct Medicion *menor;
    float  potenciaTotal = 0.0f;
    float  energiaTotal  = 0.0f;
    float  horasTotal    = 0.0f;
    double costoTotal    = 0.0;
    int    i;

    if (total <= 0) {
        printf("\nNo hay mediciones registradas en memoria.\n");
        printf("Registre una (opcion 1) o cargue el archivo (opcion 4).\n");
        return;
    }

    mayor = &lista[0];
    menor = &lista[0];

    for (i = 0; i < total; i++) {
        const struct Medicion *medicion = &lista[i];

        potenciaTotal += medicion->potencia;
        energiaTotal  += medicion->energia;
        horasTotal    += medicion->horas;
        costoTotal    += medicion->costo;

        if (medicion->potencia > mayor->potencia) {
            mayor = medicion;
        }
        if (medicion->potencia < menor->potencia) {
            menor = medicion;
        }
    }

    printf("\n=== RESUMEN ESTADISTICO GENERAL ===\n");
    printf("Mediciones registradas : %d\n", total);
    printf("Potencia total         : %.2f W\n", potenciaTotal);
    printf("Potencia promedio      : %.2f W\n", potenciaTotal / (float) total);
    printf("Energia total          : %.4f kWh\n", energiaTotal);
    printf("Energia promedio       : %.4f kWh\n", energiaTotal / (float) total);
    printf("Horas acumuladas       : %.2f h\n", horasTotal);
    printf("Costo total            : %.2f\n", costoTotal);
    printf("Costo promedio         : %.2f\n", costoTotal / (double) total);
    printf("Mayor potencia         : %s (%.2f W)\n", mayor->nombre, mayor->potencia);
    printf("Menor potencia         : %s (%.2f W)\n", menor->nombre, menor->potencia);
    printf("Tarifa aplicada        : %.2f por kWh\n", TARIFA_KWH);
}

#endif /* PERSISTENCIA_H */

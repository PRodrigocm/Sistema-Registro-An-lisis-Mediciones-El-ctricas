/* =============================================================
   MEDICIONES.H  --  Encabezado del Grupo A
   Sistema de Registro y Analisis de Mediciones Electricas
   ------------------------------------------------------------
   SPRINT 2: modelo de datos y registro.
     - Estructura Medicion con los datos de un equipo electrico
     - Variables globales del sistema
     - Formulas electricas de potencia, resistencia y energia
     - Ingreso validado de datos y ficha del equipo

   El encabezado contiene los prototipos y las implementaciones
   declaradas static, por lo que el proyecto se compila con un
   unico archivo de traduccion:  gcc main.c -o main.exe
  */

#ifndef MEDICIONES_H
#define MEDICIONES_H

#include <stdio.h>
#include <string.h>

/*Limites de las estructuras de texto  */
#define MAX_NOMBRE     50   /* cadena de 50 caracteres */
#define MAX_DETALLE    80   /* cadena de 80 caracteres */

/*Limite de mediciones almacenadas en memoria*/
#define MAX_MEDICIONES 50

/* Variables globales numericas 
   TARIFA_KWH      : costo por kilovatio-hora (float)
   TOTAL_MEDICIONES: cantidad de mediciones en el arreglo (int)
   Se declaran extern aqui y se definen en main.c.
   */
extern float TARIFA_KWH;
extern int   TOTAL_MEDICIONES;

/* 
   ESTRUCTURA DE DATOS  (Requisito 5)
   Almacena una medicion electrica completa.
   Contiene 10 variables numericas y 2 cadenas de texto.
    */
struct Medicion {
    int    codigo;                 /* identificador del equipo       */
    char   nombre[MAX_NOMBRE];     /* nombre del equipo (>=30 chars) */
    char   detalle[MAX_DETALLE];   /* tipo de carga (>=30 chars)     */
    float  voltaje;                /* voltios                        */
    float  corriente;              /* amperios                       */
    float  potencia;               /* P = V * I  (watios)             */
    float  potenciaReal;           /* P real = P * factor (watios)   */
    float  resistencia;            /* R = V / I  (ohmios)             */
    float  horas;                  /* horas de uso                   */
    float  factor;                 /* factor de potencia (0.1 - 1.0) */
    float  energia;                /* kWh consumidas                 */
    double costo;                  /* costo estimado del consumo     */
};

/*
   PROTOTIPOS -- GRUPO A (8 funciones)
 */

/* --- Formulas electricas (5) --- */
static float  calcularPotencia(float voltaje, float corriente);
static float  calcularResistencia(float voltaje, float corriente);
static float  calcularEnergia(float potencia, float horas);
static float  calcularPotenciaReal(float potencia, float factor);
static double calcularCosto(float energia, float tarifa);

/* --- Evaluacion de la carga (1) --- */
static void   evaluarPotencia(float potencia);

/* --- Registro y presentacion (2) --- */
static void   ingresarMedicion(struct Medicion *medicion);
static void   mostrarMedicion(const struct Medicion *medicion);

/* 
   FUNCION 1: Calcular la potencia electrica
   P = V * I   (watios)
    */
static float calcularPotencia(float voltaje, float corriente) {
    return voltaje * corriente;
}

/* 
   FUNCION 2: Calcular la resistencia
   R = V / I   (ohmios). Si la corriente es cero se evita la
   division por cero devolviendo 0.
     */
static float calcularResistencia(float voltaje, float corriente) {
    if (corriente == 0.0f) {
        return 0.0f;
    }
    return voltaje / corriente;
}

/*  
   FUNCION 3: Calcular la energia consumida
   E = (P * horas) / 1000   (kWh)
     */
static float calcularEnergia(float potencia, float horas) {
    return (potencia * horas) / 1000.0f;
}

/* 
   FUNCION 4: Calcular la potencia real
   P_real = P * factor de potencia
    */
static float calcularPotenciaReal(float potencia, float factor) {
    return potencia * factor;
}

/* 
   FUNCION 5: Calcular el costo estimado del consumo
   costo = energia (kWh) * tarifa
    */
static double calcularCosto(float energia, float tarifa) {
    return (double) energia * (double) tarifa;
}

/* 
   FUNCION 6: Evaluar y clasificar la potencia del equipo
    */
static void evaluarPotencia(float potencia) {
    if (potencia >= 1000.0f) {
        printf("Estado: Potencia MUY ALTA (carga industrial)\n");
    } else if (potencia >= 100.0f) {
        printf("Estado: Potencia alta\n");
    } else if (potencia > 0.0f) {
        printf("Estado: Potencia baja\n");
    } else {
        printf("Estado: Sin consumo medido\n");
    }
}

/* 
   FUNCION 7: Ingresar los datos del equipo por puntero.
   Cada campo ocupa una linea y se lee con fgets, que respeta las
   lineas vacias y no desplaza los datos siguientes. La conversion
   se hace con sscanf, que devuelve la cantidad de campos leidos.
   Si el dato no es valido se usa un valor por defecto y se avisa.
   Al terminar se calculan y almacenan todos los resultados.
    */
static void ingresarMedicion(struct Medicion *medicion) {
    char linea[256];

    printf("\n--- REGISTRO DE MEDICION ---\n");

    printf("Nombre del equipo        : ");
    linea[0] = '\0';
    if (fgets(linea, (int) sizeof(linea), stdin) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';
    }
    if (linea[0] == '\0') {
        strcpy(linea, "EQUIPO SIN NOMBRE");
    }
    snprintf(medicion->nombre, MAX_NOMBRE, "%.*s", MAX_NOMBRE - 1, linea);

    printf("Detalle / tipo de carga  : ");
    linea[0] = '\0';
    if (fgets(linea, (int) sizeof(linea), stdin) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';
    }
    if (linea[0] == '\0') {
        strcpy(linea, "SIN DETALLE");
    }
    snprintf(medicion->detalle, MAX_DETALLE, "%.*s", MAX_DETALLE - 1, linea);

    printf("Codigo del equipo        : ");
    linea[0] = '\0';
    if (fgets(linea, (int) sizeof(linea), stdin) == NULL || sscanf(linea, "%d", &medicion->codigo) != 1) {
        medicion->codigo = 0;
        printf("  Codigo no numerico, se usara 0.\n");
    }

    printf("Voltaje (V)              : ");
    linea[0] = '\0';
    if (fgets(linea, (int) sizeof(linea), stdin) == NULL || sscanf(linea, "%f", &medicion->voltaje) != 1) {
        medicion->voltaje = 0.0f;
        printf("  Voltaje no numerico, se usara 0.\n");
    }

    printf("Corriente (A)            : ");
    linea[0] = '\0';
    if (fgets(linea, (int) sizeof(linea), stdin) == NULL || sscanf(linea, "%f", &medicion->corriente) != 1) {
        medicion->corriente = 0.0f;
        printf("  Corriente no numerica, se usara 0.\n");
    }

    printf("Horas de uso             : ");
    linea[0] = '\0';
    if (fgets(linea, (int) sizeof(linea), stdin) == NULL || sscanf(linea, "%f", &medicion->horas) != 1) {
        medicion->horas = 0.0f;
        printf("  Horas no numericas, se usara 0.\n");
    }

    printf("Factor de potencia       : ");
    linea[0] = '\0';
    if (fgets(linea, (int) sizeof(linea), stdin) == NULL || sscanf(linea, "%f", &medicion->factor) != 1) {
        medicion->factor = 1.0f;
        printf("  Factor no numerico, se usara 1.0.\n");
    }

    /* Calculos derivados de los datos ingresados */
    medicion->potencia     = calcularPotencia(medicion->voltaje, medicion->corriente);
    medicion->potenciaReal = calcularPotenciaReal(medicion->potencia, medicion->factor);
    medicion->resistencia  = calcularResistencia(medicion->voltaje, medicion->corriente);
    medicion->energia      = calcularEnergia(medicion->potenciaReal, medicion->horas);
    medicion->costo        = calcularCosto(medicion->energia, TARIFA_KWH);
}

/* 
   FUNCION 8: Mostrar todos los datos de una medicion
    */
static void mostrarMedicion(const struct Medicion *medicion) {
    printf("\n=== DATOS DEL EQUIPO %d ===\n", medicion->codigo);
    printf("Nombre        : %s\n", medicion->nombre);
    printf("Detalle       : %s\n", medicion->detalle);
    printf("Voltaje       : %.2f V\n", medicion->voltaje);
    printf("Corriente     : %.2f A\n", medicion->corriente);
    printf("Potencia      : %.2f W\n", medicion->potencia);
    printf("Potencia real : %.2f W\n", medicion->potenciaReal);
    printf("Resistencia   : %.2f ohmios\n", medicion->resistencia);
    printf("Horas de uso  : %.2f h\n", medicion->horas);
    printf("Factor        : %.2f\n", medicion->factor);
    printf("Energia       : %.4f kWh\n", medicion->energia);
    printf("Costo est.    : %.2f\n", medicion->costo);
}

#endif /* MEDICIONES_H */

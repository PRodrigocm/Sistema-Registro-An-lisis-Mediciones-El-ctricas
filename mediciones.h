/* mediciones.h
   Estructura, tarifa, formulas y funciones para trabajar con los datos.
   Las funciones estan en el encabezado para que el programa se
   compile solo con main.c. */

#ifndef MEDICIONES_H
#define MEDICIONES_H

#include <stdio.h>
#include <string.h>

#define MAX_MEDICIONES 50

/* Variable global: precio de un kilovatio-hora */
float TARIFA = 0.75;

/* Estructura: los datos de un equipo */
struct Medicion {
    int   codigo;
    char  nombre[50];     /* texto de hasta 49 caracteres (sin espacios) */
    char  detalle[80];    /* texto de hasta 79 caracteres (sin espacios) */
    float voltaje;
    float corriente;
    float potencia;
    float resistencia;
    float horas;
    float energia;
    float costo;
};

/* P = V * I */
float calcularPotencia(float voltaje, float corriente) {
    return voltaje * corriente;
}

/* R = V / I (si la corriente es 0, devuelve 0) */
float calcularResistencia(float voltaje, float corriente) {
    if (corriente == 0) {
        return 0;
    }
    return voltaje / corriente;
}

/* E = P * horas / 1000 (kWh) */
float calcularEnergia(float potencia, float horas) {
    return potencia * horas / 1000;
}

/* costo = energia * tarifa */
float calcularCosto(float energia) {
    return energia * TARIFA;
}

/* Calcula todos los valores derivados del equipo */
void calcularTodo(struct Medicion *m) {
    m->potencia    = calcularPotencia(m->voltaje, m->corriente);
    m->resistencia = calcularResistencia(m->voltaje, m->corriente);
    m->energia     = calcularEnergia(m->potencia, m->horas);
    m->costo       = calcularCosto(m->energia);
}

/* Cuenta las vocales del texto, revisando letra por letra */
int contarVocales(char texto[]) {
    int largo = strlen(texto);
    int cuenta = 0;
    int i;

    for (i = 0; i < largo; i++) {
        if (texto[i] == 'a' || texto[i] == 'e' || texto[i] == 'i' ||
            texto[i] == 'o' || texto[i] == 'u' ||
            texto[i] == 'A' || texto[i] == 'E' || texto[i] == 'I' ||
            texto[i] == 'O' || texto[i] == 'U') {
            cuenta++;
        }
    }
    return cuenta;
}

/* Pide los datos de un equipo y guarda en la estructura */
void ingresarMedicion(struct Medicion *m) {
    char temp[80];

    printf("Codigo: ");
    scanf("%d", &m->codigo);

    printf("Nombre (sin espacios): ");
    scanf("%49s", temp);
    strcpy(m->nombre, temp);

    printf("Tipo de carga (sin espacios): ");
    scanf("%79s", temp);
    strcpy(m->detalle, temp);

    printf("Voltaje (V): ");
    scanf("%f", &m->voltaje);
    printf("Corriente (A): ");
    scanf("%f", &m->corriente);
    printf("Horas de uso: ");
    scanf("%f", &m->horas);

    calcularTodo(m);
}

/* Muestra los datos de un equipo */
void mostrarMedicion(const struct Medicion *m) {
    printf("\nCodigo    : %d\n", m->codigo);
    printf("Nombre    : %s (%d caracteres, %d vocales)\n",
           m->nombre, (int) strlen(m->nombre), contarVocales(m->nombre));
    printf("Tipo      : %s\n", m->detalle);
    printf("Voltaje   : %.2f V\n", m->voltaje);
    printf("Corriente : %.2f A\n", m->corriente);
    printf("Potencia  : %.2f W\n", m->potencia);
    printf("Resistencia: %.2f ohmios\n", m->resistencia);
    printf("Horas     : %.2f h\n", m->horas);
    printf("Energia   : %.4f kWh\n", m->energia);
    printf("Costo     : %.2f\n", m->costo);
}

/* Busca un equipo por su nombre. Devuelve la posicion o -1 */
int buscarPorNombre(struct Medicion lista[], int total, char nombre[]) {
    int i;

    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

/* Resumen: totales, promedio, mayor potencia y porcentaje de energia */
void mostrarResumen(struct Medicion lista[], int total) {
    float potenciaTotal = 0;
    float energiaTotal = 0;
    float costoTotal = 0;
    int mayor = 0;
    int i;

    if (total == 0) {
        printf("No hay mediciones.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        potenciaTotal += lista[i].potencia;
        energiaTotal  += lista[i].energia;
        costoTotal    += lista[i].costo;
        if (lista[i].potencia > lista[mayor].potencia) {
            mayor = i;
        }
    }

    printf("\n=== RESUMEN ===\n");
    printf("Equipos registrados : %d\n", total);
    printf("Potencia total      : %.2f W\n", potenciaTotal);
    printf("Potencia promedio   : %.2f W\n", potenciaTotal / total);
    printf("Energia total       : %.4f kWh\n", energiaTotal);
    printf("Costo total         : %.2f\n", costoTotal);
    printf("Mayor potencia      : %s (%.2f W)\n", lista[mayor].nombre, lista[mayor].potencia);

    printf("\nPorcentaje de energia por equipo:\n");
    for (i = 0; i < total; i++) {
        float porcentaje = 0;
        if (energiaTotal > 0) {
            porcentaje = lista[i].energia / energiaTotal * 100;
        }
        printf("  %s: %.2f %%\n", lista[i].nombre, porcentaje);
    }
}

#endif

# Avance de Sprint 2 — Modelo de datos y registro

**Proyecto:** Sistema de Registro y Análisis de Mediciones Eléctricas
**Curso:** Programación Estructurada y Aplicada
**Tipo de trabajo:** Proyecto final de nivel intermedio
**Modalidad:** Consola (entrada y salida por pantalla)
**Etapa:** Sprint 2 de 4

---

## 1. Objetivo del Sprint 2

Incorporar el modelo de datos del sistema y la capacidad de **registrar una medición eléctrica completa**:

- La estructura `struct Medicion`, que representa una equipo eléctrico con todos sus datos.
- Las variables globales que comparte todo el programa.
- Las cinco fórmulas eléctricas que convierten voltaje y corriente en resultados analizables.
- La clasificación de la carga según su potencia.
- El ingreso validado de datos y la ficha del equipo.

Al cierre de este sprint la opción 1 del menú funciona de punta a punta: el usuario registra un equipo y ve su ficha completa con potencia, resistencia, energía y costo.

---

## 2. Archivos del proyecto

| Archivo | Líneas | Contenido |
|---|---|---|
| `main.c` | 143 | Menú, bucle principal, puntero al registro activo, opción 1 |
| `mediciones.h` | 232 | **Nuevo.** Grupo A: estructura, globales, fórmulas, ingreso y ficha |
| `persistencia.h` | 66 | Grupo B: lectura del archivo de 5 líneas (sin cambios de fondo) |
| `descripcion.txt` | 5 | Archivo de texto leído por el programa |

En este sprint **se crea `mediciones.h`** y **se enlaza `persistencia.h` con él** mediante `#include "mediciones.h"`, que fija la jerarquía del proyecto: el Grupo B (persistencia) depende del Grupo A (modelo de datos).

---

## 3. La estructura `struct Medicion` — `mediciones.h:42`

Representa un equipo eléctrico con todos sus datos. Contiene **10 variables numéricas** y **2 cadenas de texto**.

| Campo | Tipo | Descripción | Origen |
|---|---|---|---|
| `codigo` | `int` | Identificador del equipo | Ingresado |
| `nombre` | `char[50]` | Nombre del equipo | Ingresado |
| `detalle` | `char[80]` | Tipo de carga | Ingresado |
| `voltaje` | `float` | Voltaje medido (V) | Ingresado |
| `corriente` | `float` | Corriente medida (A) | Ingresado |
| `potencia` | `float` | `P = V · I` (W) | Calculado |
| `potenciaReal` | `float` | `P · factor` (W) | Calculado |
| `resistencia` | `float` | `R = V / I` (Ω) | Calculado |
| `horas` | `float` | Horas de uso | Ingresado |
| `factor` | `float` | Factor de potencia (0.1–1.0) | Ingresado |
| `energia` | `float` | `E = (P · h) / 1000` (kWh) | Calculado |
| `costo` | `double` | `E · tarifa` | Calculado |

Las cadenas `nombre[50]` y `detalle[80]` superan los 30 caracteres exigidos por la condición mínima 3 de la guía.

### Variables globales

| Variable | Tipo | Valor | Función | Ubicación |
|---|---|---|---|---|
| `TARIFA_KWH` | `float` | `0.75` | Costo por kilovatio-hora, editable para cambiar la tarifa | `mediciones.h:34` (declaración), `main.c:19` (definición) |
| `TOTAL_MEDICIONES` | `int` | `0` | Cantidad de registros activos en el arreglo | `mediciones.h:35` (declaración), `main.c:20` (definición) |

Se declaran `extern` en el encabezado y se definen en `main.c`, que es el archivo que produce el programa.

### Recuento de variables numéricas (condición mínima 2)

| Origen | Cantidad | Detalle |
|---|---|---|
| `struct Medicion` | 10 | `codigo`, `voltaje`, `corriente`, `potencia`, `potenciaReal`, `resistencia`, `horas`, `factor`, `energia`, `costo` |
| Globales | 2 | `TARIFA_KWH`, `TOTAL_MEDICIONES` |
| Locales de función | 2+ | `linea` en `ingresarMedicion`, `opcion` y `linea` en `main`, más las acumuladoras de los cálculos |
| **Total** | **16+** | **La guía exige un mínimo de 10** |

---

## 4. Las cinco fórmulas eléctricas

| Función | Ubicación | Fórmula | Resultado |
|---|---|---|---|
| `calcularPotencia` | `mediciones.h:83` | `P = V · I` | Watts |
| `calcularResistencia` | `mediciones.h:92` | `R = V / I` | Ohmis, con guarda de división entre cero |
| `calcularEnergia` | `mediciones.h:103` | `E = (P · h) / 1000` | kWh |
| `calcularPotenciaReal` | `mediciones.h:111` | `P_real = P · factor` | Watts |
| `calcularCosto` | `mediciones.h:119` | `costo = E · tarifa` | Units monetarias |

`calcularResistencia` incluye una guarda explícita: si la corriente es cero devuelve `0` en lugar de dividir, evitando la división por cero.

`evaluarPotencia` (`mediciones.h:126`) clasifica la carga en cuatro rangos: **MUY ALTA** (≥ 1000 W), **alta** (≥ 100 W), **baja** (> 0 W) y **sin consumo medido** (0 W).

---

## 5. El uso de punteros (condición mínima 6)

El programa principal procesa los datos mediante punteros:

```c
struct Medicion lista[MAX_MEDICIONES];   /* main.c:64  arreglo de estructuras */
struct Medicion *ptr = &lista[0];         /* main.c:68  puntero al primer registro */
```

Y en la opción 1 del menú, el puntero se reposiciona al siguiente espacio libre antes de registrar:

```c
ptr = &lista[TOTAL_MEDICIONES];          /* main.c:105 */
registrarMedicion(ptr);
```

Ese puntero se pasa por puntero a `ingresarMedicion` y `mostrarMedicion`, de modo que **las funciones escriben directamente sobre el arreglo en memoria** sin copias:

```c
static void ingresarMedicion(struct Medicion *medicion) { ... }
static void mostrarMedicion(const struct Medicion *medicion) { ... }
```

`mostrarMedicion` recibe un puntero `const` porque solo lee: la estructura no debe modificarse durante la presentación.

---

## 6. Dos errores encontrados en las pruebas y corregidos

Las pruebas de este sprint detectaron dos defectos reales que estaban arrastrados desde el Sprint 1. Se documentan porque son el resultado del trabajo de verificación.

### 6.1 El nombre vacío desplazaba todos los datos

**Síntoma.** Al registrar con el nombre en blanco, todos los campos siguientes se leían corridos: el voltaje tomaba el valor de la corriente, las horas tomaban el factor, etc.

**Causa.** `scanf(" %49[^\n]", destino)` usa un espacio inicial que **descarta todos los espacios en blanco, incluidos los saltos de línea**. Al encontrar una línea vacía, la lectura continuaba en la línea siguiente y todos los campos se corrían una posición.

**Solución.** Los campos de texto se leen ahora con `fgets`, que consume exactamente una línea y respeta las líneas vacías. Además el menú se pasó a lectura por línea (`fgets` + `sscanf`) para que el registro empiece siempre en una línea nueva.

### 6.2 Bucle infinito al agotar la entrada

**Síntoma.** Al redirigir la entrada desde un archivo y terminar los datos, el programa repetía *"Opción inválida"* indefinidamente hasta agotar el tiempo.

**Causa.** `scanf` devuelve `EOF` cuando la entrada se agota. La condición de validación no lo distinguía de una entrada no numérica, así que el programa reintentaba eternamente.

**Solución.** Se captura el valor de retorno de `fgets`: si devuelve `NULL` se imprime *"Fin de la entrada"* y el programa cierra con normalidad.

### 6.3 Cambio de enfoque en la lectura de datos

Como consecuencia de lo anterior, **la lectura de todos los campos pasa a ser línea a línea**:

```c
linea[0] = '\0';
if (fgets(linea, (int) sizeof(linea), stdin) == NULL || sscanf(linea, "%f", &medicion->voltaje) != 1) {
    medicion->voltaje = 0.0f;
    printf("  Voltaje no numerico, se usara 0.\n");
}
```

Ventajas frente a leer todo con `scanf`:

| Aspecto | Con `scanf` directo | Con `fgets` + `sscanf` |
|---|---|---|
| Línea vacía | Desplaza los campos siguientes | Usa el valor por defecto, sin desplazar |
| Dato no numérico | Deja basura en el búfer | Se detecta y se avisa |
| Fin de la entrada | Provoca bucle infinito | Se cierra con normalidad |
| Espacios dentro del nombre | Se truncan en el separador | Se conservan completos |

El costo es unas líneas más por campo, pero **no se creó ninguna función auxiliar**: todo se resuelve con `fgets`, `sscanf`, `strcspn` y `snprintf`, que son funciones de la librería estándar.

---

## 7. Criterios de aceptación y evidencia

| # | Criterio | Evidencia |
|---|---|---|
| 1 | Compila sin advertencias | 0 avisos con `-Wall -Wextra -pedantic` |
| 2 | La opción 1 registra un equipo completo | Entrada `1` + 7 datos |
| 3 | La ficha muestra los 12 campos | Captura de pantalla |
| 4 | Los cálculos eléctricos son correctos | P = 220 × 5.5 = 1210 W; P_real = 1089 W; R = 40 Ω; E = 8.7120 kWh; costo = 6.53 |
| 5 | `evaluarPotencia` clasifica la carga | *"Estado: Potencia MUY ALTA (carga industrial)"* |
| 6 | Varias mediciones se acumulan | 2 registros → *"Mediciones registradas en esta sesion: 2"* |
| 7 | El puntero procesa los datos del arreglo | `ptr = &lista[TOTAL_MEDICIONES]` |
| 8 | Nombre vacío no desplaza los demás campos | Nombre → `EQUIPO SIN NOMBRE`, voltaje sigue siendo 220 V |
| 9 | Dato no numérico no corrompe el registro | Código `abc` → 0 con aviso, el resto se lee bien |
| 10 | Fin de la entrada cierra el programa | *"Fin de la entrada."* → salida normal |
| 11 | Se respeta el límite de 50 mediciones | 50 registros aceptados, el 51º avisa *"Limite alcanzado"* |

### Pruebas de consola ejecutadas

| Entrada | Resultado obtenido |
|---|---|
| `1` + Refrigerador 220 V / 5.5 A / 8 h / 0.9 | Ficha con P = 1210.00 W, R = 40.00 Ω, E = 8.7120 kWh, costo 6.53 |
| 2 registros seguidos | *"Mediciones registradas en esta sesion: 1"* luego *": 2"* |
| Nombre en blanco | `EQUIPO SIN NOMBRE`; voltaje sigue siendo 220.00 V |
| Código `abc` | *"Codigo no numerico, se usara 0."*; voltaje, corriente, horas y factor correctos |
| Sin la opción de salida (EOF) | *"Fin de la entrada."* y cierre normal, sin bucle infinito |
| `abc`, `9` en el menú | *"Opcion invalida: ingrese un numero entre 0 y 6."* y vuelve a pedir |
| 51 registros seguidos | Los 50 primeros se registran; el 51º muestra *"Limite alcanzado"* |

---

## 8. Decisiones técnicas y su motivo

| Decisión | Motivo |
|---|---|
| `mediciones.h` con prototipos **e** implementaciones `static` | La guía pide agrupar las funciones en dos encabezados. Con un solo archivo de traducción el proyecto sigue compilando con `gcc main.c -o main.exe` |
| `static` en todas las funciones | Evita duplicación de símbolos si el proyecto crece a más de un `.c`. Compila sin avisos con `-Wall -Wextra -pedantic` |
| `extern` para las globales | La declaración va en el encabezado y la definición en `main.c`, que es donde se produce el programa |
| `double` para `costo` | El costo acumula valores decimales de varias mediciones; `double` da más precisión |
| `float` para el resto | Suficiente para magnitudes de una instalación eléctrica y coherente con el resto del proyecto |
| `snprintf` con precisión explícita (`"%.*s"`) | Copia el texto al campo sin desbordarlo y **sin** que el compilador emita avisos de truncamiento |
| Factor de potencia como dato de entrada | La energía se calcula sobre la potencia real (`P · factor`), no sobre la potencia aparente; es el cálculo correcto en una instalación real |
| Sin funciones genéricas de cadenas | Todo lo manipulable usa la librería: `fgets`, `sscanf`, `strcspn`, `snprintf`, `strcpy` |

---

## 9. Estado de las condiciones mínimas de la guía

| # | Condición exigida | Estado al cierre del Sprint 2 | Se cumplirá en |
|---|---|---|---|
| 1 | Tema referente a las especialidades | ✅ Completo | — |
| 2 | Mín. 10 variables numéricas | ✅ **Completo: 16** | — |
| 3 | String de mín. 30 caracteres con conteo o reemplazo, o archivo de 5 líneas | ⏳ Parcial: las cadenas de 50 y 80 caracteres ya se usan; falta el conteo (`strlen`) y el reemplazo (`strchr`) | Sprint 4 |
| 4 | Mín. 8 funciones agrupadas en dos encabezados | ⏳ **Parcial: 9 de 17** (8 en `mediciones.h` + 1 en `persistencia.h`) | Sprints 3-4 |
| 5 | Mín. una estructura en el procesamiento | ✅ **Completo: `struct Medicion`** | — |
| 6 | Punteros en el programa principal | ✅ **Completo** | — |

---

## 10. Qué sigue

### Sprint 3 — Persistencia en archivo

- `guardarMediciones` con `fprintf`: modo *append* al registrar, modo reescritura al guardar todo el registro.
- `cargarMediciones` con `sscanf` sobre el formato de 12 campos separados por `|`, sin escribir un parser manual.
- `mediciones.txt` como base de datos del sistema.
- Cablear las opciones 3 y 4 del menú.

### Sprint 4 — Análisis y cierre

- `mostrarResumen`: energía, costo y conteo de caracteres con `strlen`.
- `listarMediciones`: tabla del registro completo.
- `mostrarEstadisticas`: totales, promedios, máximo, mínimo y costo total.
- Reemplazo de caracteres con `strchr` al guardar, para no romper el separador de campos.
- Cierre de la tabla de cumplimiento de los 6 requisitos y del guion de exposición.

**Estado final proyectado:** 3 archivos de código (`main.c`, `mediciones.h`, `persistencia.h`), 17 funciones (8 en `mediciones.h`, 6 en `persistencia.h`, 3 en `main.c`), 14 funciones en los dos encabezados frente al mínimo de 8 exigido, y 16 variables numéricas frente al mínimo de 10 exigido.

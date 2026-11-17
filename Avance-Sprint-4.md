# Avance de Sprint 4 — Análisis y cierre (sprint final)

**Proyecto:** Sistema de Registro y Análisis de Mediciones Eléctricas
**Curso:** Programación Estructurada y Aplicada
**Tipo de trabajo:** Proyecto final de nivel intermedio
**Modalidad:** Consola (entrada y salida por pantalla)
**Etapa:** Sprint 4 de 4 (final)

---

## 1. Objetivo del Sprint 4

Cerrar el sistema con las funciones de análisis y presentación que faltaban:

- **Opción 2:** listar el registro completo.
- **Opción 6:** resumen estadístico general del consumo.
- Conteo de caracteres de los textos con `strlen` (requisito 3).
- Localización de la mayor y la menor potencia con punteros a estructuras.
- Verificación final del programa completo y de los seis requisitos de la guía.

Aunque el plan original separaba este cierre del Sprint 3, la entrega final se
verificó con las dos etapas integradas: sin el guardado y la carga, las opciones 2 y
6 no podrían demostrarse sobre datos recuperados entre ejecuciones.

---

## 2. Funciones agregadas — `persistencia.h`

| Función | Línea | Responsabilidad |
|---|---|---|
| `mostrarResumen` | `persistencia.h:206` | Ficha breve: nombre, largo de los textos, potencia, energía y costo |
| `listarMediciones` | `persistencia.h:218` | Recorre el arreglo y presenta el registro completo |
| `mostrarEstadisticas` | `persistencia.h:239` | Totales, promedios, máximo y mínimo del registro |

Con estas tres funciones el Grupo B queda en **6 funciones**:

1. `mostrarDescripcion`
2. `guardarMediciones`
3. `cargarMediciones`
4. `mostrarResumen`
5. `listarMediciones`
6. `mostrarEstadisticas`

Las opciones 2 y 6 del menú se cablean en `main.c:115` y `main.c:131`.

---

## 3. `mostrarResumen` y el conteo de caracteres

`mostrarResumen` (`persistencia.h:206`) presenta una medición en tres líneas. Usa
`strlen` sobre `nombre` y `detalle`, que es el **conteo de caracteres** del
requisito 3 de la guía:

```c
printf("    Nombre : %2lu caracteres | Detalle: %2lu caracteres\n",
       (unsigned long) strlen(medicion->nombre),
       (unsigned long) strlen(medicion->detalle));
```

Salida real:

```text
[101] Refrigerador 220V
    Nombre : 17 caracteres | Detalle: 15 caracteres
    Potencia:  1210.00 W | Energia:   8.7120 kWh | Costo:     6.53
```

Los textos `nombre[50]` y `detalle[80]` superan los 30 caracteres exigidos. El
reemplazo de caracteres se cumple en el guardado con `strchr` (`persistencia.h:109`),
de modo que el requisito 3 queda cubierto por las tres vías: cadenas largas, conteo
y reemplazo, además del archivo de 5 líneas.

---

## 4. `listarMediciones` — opción 2

```c
static void listarMediciones(const struct Medicion *lista, int total);
```

Si el registro está vacío, informa y sugiere registrar o cargar. Si tiene datos,
recorre el arreglo con un `for` y llama a `mostrarResumen` por cada medición. La
presentación incluye el total listado.

```text
=== LISTADO DE MEDICIONES (2) ===
[101] Refrigerador 220V
    Nombre : 17 caracteres | Detalle: 15 caracteres
    Potencia:  1210.00 W | Energia:   8.7120 kWh | Costo:     6.53
[102] Foco LED
    Nombre :  8 caracteres | Detalle: 11 caracteres
    Potencia:    60.00 W | Energia:   0.6000 kWh | Costo:     0.45

Total listado: 2 mediciones.
```

---

## 5. `mostrarEstadisticas` — opción 6

```c
static void mostrarEstadisticas(const struct Medicion *lista, int total);
```

Recorre el arreglo una sola vez y acumula:

| Resultado | Cálculo |
|---|---|
| Potencia total | suma de `potencia` |
| Potencia promedio | potencia total / cantidad |
| Energía total | suma de `energia` |
| Energía promedio | energía total / cantidad |
| Horas acumuladas | suma de `horas` |
| Costo total | suma de `costo` |
| Costo promedio | costo total / cantidad |
| Mayor y menor potencia | punteros al registro con el máximo y el mínimo |

La mayor y la menor potencia se localizan con **punteros a estructuras**
(`mayor` y `menor`), no con índices ni copias. Eso refuerza el requisito 6 dentro del
Grupo B.

Salida real para las dos mediciones de prueba:

```text
=== RESUMEN ESTADISTICO GENERAL ===
Mediciones registradas : 2
Potencia total         : 1270.00 W
Potencia promedio      : 635.00 W
Energia total          : 9.3120 kWh
Energia promedio       : 4.6560 kWh
Horas acumuladas       : 18.00 h
Costo total            : 6.98
Costo promedio         : 3.49
Mayor potencia         : Refrigerador 220V (1210.00 W)
Menor potencia         : Foco LED (60.00 W)
Tarifa aplicada        : 0.75 por kWh
```

Verificación manual:

- Potencia: 220 × 5.5 = 1210 W; 120 × 0.5 = 60 W; total 1270 W; promedio 635 W.
- Energía: 1210 × 0.9 × 8 / 1000 = 8.712 kWh; 60 × 1.0 × 10 / 1000 = 0.6 kWh.
- Costo: 8.712 × 0.75 = 6.534; 0.6 × 0.75 = 0.45; total 6.984, mostrado como 6.98.

---

## 6. Integración final del menú

| Opción | Acción | Estado |
|---|---|---|
| 1 | Registrar medición (7 datos y 5 cálculos) | Funcional |
| 2 | Listar mediciones registradas | Funcional |
| 3 | Guardar mediciones en `mediciones.txt` | Funcional |
| 4 | Cargar mediciones desde `mediciones.txt` | Funcional |
| 5 | Ver descripción del sistema (5 líneas) | Funcional |
| 6 | Resumen estadístico general | Funcional |
| 0 | Salir | Funcional |

Ya no queda ningún mensaje de *"En construcción"*.

---

## 7. Pruebas de cierre

| # | Caso | Resultado |
|---|---|---|
| 1 | Compilación | 0 avisos con `-Wall -Wextra -pedantic -std=c11` |
| 2 | Sesión completa: registrar 2, listar, estadísticas, guardar, salir | Todo correcto; archivo con 2 líneas |
| 3 | Nueva ejecución: cargar, listar, estadísticas | Mismos valores que antes de guardar |
| 4 | Cargar 2, registrar un tercero, guardar | 3 registros; archivo con 3 líneas |
| 5 | Totales con 3 registros | Potencia 1435.00 W, energía 9.8730 kWh, costo 7.40 |
| 6 | Nombre con `|` | Guardado como `-`; recarga sin corrupción del formato |
| 7 | Archivo con una línea corrupta | Se ignora con aviso y carga la línea válida |
| 8 | Archivo de 52 líneas | Carga 50 y avisa de las 2 restantes |
| 9 | Sin archivo | Mensajes claros; no se pierde la memoria |
| 10 | Guardar con 0 registros | No modifica el archivo existente |
| 11 | Menú con `abc`, `9`, `5` y EOF | Rechaza inválidas, muestra la descripción y cierra sin bucle |
| 12 | Límite de 50 registros | El registro 51 avisa *"Limite alcanzado"* |

---

## 8. Cumplimiento de las condiciones mínimas de la guía

| # | Condición exigida | Estado final | Evidencia |
|---|---|---|---|
| 1 | Tema referente a las especialidades | Completo | Mediciones eléctricas: potencia, resistencia, energía y costo |
| 2 | Mínimo 10 variables numéricas | Completo (16+) | 10 en `struct Medicion`, 2 globales y locales de cálculo |
| 3 | String de mín. 30 caracteres con conteo o reemplazo, o archivo de 5 líneas | Completo | `nombre[50]`, `detalle[80]`, `strlen` en `mostrarResumen`, `strchr` en `guardarMediciones` y `descripcion.txt` de 5 líneas |
| 4 | Mínimo 8 funciones agrupadas en dos encabezados | Completo (14) | 8 en `mediciones.h` + 6 en `persistencia.h` |
| 5 | Mínimo una estructura en el procesamiento | Completo | `struct Medicion` (`mediciones.h:42`) |
| 6 | Punteros en el programa principal | Completo | `ptr` al registro activo (`main.c:70`), `&TOTAL_MEDICIONES` (`main.c:123`) y punteros `mayor`/`menor` |

---

## 9. Estructura final y compilación

```text
main.c            3 funciones: main, mostrarMenu, registrarMedicion
mediciones.h      8 funciones del Grupo A
persistencia.h    6 funciones del Grupo B
descripcion.txt   archivo de 5 líneas
mediciones.txt    archivo generado al guardar (12 campos por línea)
```

Total: **17 funciones**, de las cuales **14 están en los dos encabezados** (el
mínimo exigido es 8).

```bash
gcc -Wall -Wextra -pedantic -std=c11 main.c -o main.exe
./main.exe
```

El proyecto se compila con un solo archivo de traducción porque los encabezados
contienen prototipos e implementaciones `static`.

---

## 10. Guion sugerido para la exposición

| Momento | Contenido | Duración sugerida |
|---|---|---|
| 1. Introducción | Tema, justificación, alcance y las 7 opciones del sistema | 2 min |
| 2. Modelo de datos | `struct Medicion`, 10 variables numéricas, fórmulas eléctricas | 3 min |
| 3. Demostración | Registrar, listar, guardar, salir, volver a entrar y cargar | 5 min |
| 4. Análisis | Estadísticas, promedios, máximo/mínimo y costo total | 3 min |
| 5. Cierre | Requisitos cumplidos, decisiones técnicas y preguntas | 2 min |

Demostración recomendada (dos equipos de ejemplo):

```text
Refrigerador 220V  | Carga inductiva | 101 | 220 V | 5.5 A  | 8 h  | 0.9
Foco LED           | Iluminacion     | 102 | 120 V | 0.5 A  | 10 h | 1.0
```

---

## 11. Pendientes externos al código

- La guía exige un grupo de **4 a 5 integrantes**; el informe mantiene 3 nombres y
  los roles sugeridos para completar el equipo.
- Completar los nombres reales de los integrantes en `INFORME.md`.
- La carpeta del código se llama `istema de Registro...` (sin la `S` inicial); no
  afecta la compilación ni la ejecución, pero conviene renombrarla antes de entregar.

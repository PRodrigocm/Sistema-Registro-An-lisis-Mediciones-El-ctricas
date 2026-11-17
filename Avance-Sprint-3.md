# Avance de Sprint 3 — Persistencia en archivo

**Proyecto:** Sistema de Registro y Análisis de Mediciones Eléctricas
**Curso:** Programación Estructurada y Aplicada
**Tipo de trabajo:** Proyecto final de nivel intermedio
**Modalidad:** Consola (entrada y salida por pantalla)
**Etapa:** Sprint 3 de 4

---

## 1. Objetivo del Sprint 3

Incorporar la **persistencia en archivo** para que las mediciones sobrevivan al cierre
del programa:

- Guardar todo el registro en `mediciones.txt` con `fprintf`.
- Recargar el registro en una ejecución posterior con `fgets` + `sscanf`.
- Reemplazar el separador `|` dentro de los textos con `strchr`, para no romper el
  formato del archivo.
- Cablear las opciones **3** y **4** del menú.

Al cierre del sprint, el ciclo completo funciona: registrar, guardar, salir, volver a
entrar, cargar, listar y analizar las mismas mediciones.

---

## 2. Archivos del proyecto

| Archivo | Líneas | Contenido |
|---|---|---|
| `main.c` | 144 | Menú, bucle principal, puntero al registro activo y opciones 1 a 6 |
| `mediciones.h` | 228 | Grupo A: estructura, globales, fórmulas, ingreso y ficha (sin cambios) |
| `persistencia.h` | 287 | Grupo B: descripción, guardado, carga y análisis |
| `descripcion.txt` | 5 | Archivo de texto leído al iniciar |
| `mediciones.txt` | — | Se crea al guardar; un registro por línea |

En este sprint `persistencia.h` pasa de 1 a 3 funciones: se agregan
`guardarMediciones` y `cargarMediciones`, base de los datos permanentes. Las tres
funciones de análisis se completan en el Sprint 4, hasta cerrar con 6.

---

## 3. Formato de `mediciones.txt`

Un registro por línea, con **12 campos separados por `|`**:

```text
101|Refrigerador 220V|Carga inductiva|220.00|5.50|1210.0000|1089.0000|40.00|8.00|0.90|8.712000|6.5340
```

| # | Campo | Tipo | Origen |
|---|---|---|---|
| 1 | `codigo` | `int` | Ingresado |
| 2 | `nombre` | texto (máx. 49) | Ingresado |
| 3 | `detalle` | texto (máx. 79) | Ingresado |
| 4 | `voltaje` | `float` | Ingresado |
| 5 | `corriente` | `float` | Ingresado |
| 6 | `potencia` | `float` | Calculado |
| 7 | `potenciaReal` | `float` | Calculado |
| 8 | `resistencia` | `float` | Calculado |
| 9 | `horas` | `float` | Ingresado |
| 10 | `factor` | `float` | Ingresado |
| 11 | `energia` | `float` | Calculado |
| 12 | `costo` | `double` | Calculado |

El texto se escribe sin espacios de relleno y con punto decimal, para que `sscanf`
pueda leerlo en cualquier configuración regional.

---

## 4. `guardarMediciones` — `persistencia.h:85`

```c
static int guardarMediciones(const struct Medicion *lista, int total);
```

Responsabilidades:

1. Si `total <= 0`, avisa y **no toca el archivo** (evita borrar datos por accidente).
2. Abre `mediciones.txt` en modo `"w"`: el archivo se reescribe completo.
3. Copia `nombre` y `detalle` a buffers locales y reemplaza cada `|` por `-` con
   `strchr` (`persistencia.h:109-114`), de modo que un texto ingresado por el usuario
   no rompa el formato.
4. Escribe los 12 campos con `fprintf` y una precisión fija por campo.
5. Cierra el archivo e informa cuántas mediciones guardó.

**Decisión:** se reescribe todo el registro en lugar de agregar al final. Así el
archivo siempre coincide con lo que hay en memoria, se evita duplicar registros al
cargar y volver a guardar, y el archivo se puede revisar y editar a mano.

---

## 5. `cargarMediciones` — `persistencia.h:137`

```c
static int cargarMediciones(struct Medicion *lista, int maximo, int *total);
```

Responsabilidades:

1. Abre `mediciones.txt` en modo `"r"`. Si no existe, informa y deja intacto el
   registro en memoria.
2. Lee línea a línea con `fgets` y recorta el salto con `strcspn`.
3. Omite líneas vacías y valida cada línea con:

   ```c
   sscanf(linea, "%d|%49[^|]|%79[^|]|%f|%f|%f|%f|%f|%f|%f|%f|%lf", ...);
   ```

   Si `sscanf` no devuelve 12 campos, la línea se descarta con un aviso que indica
   el número de línea y cuántos campos se leyeron.
4. **Recalcula** potencia, potencia real, resistencia, energía y costo con las
   funciones del Grupo A. Así el archivo no puede introducir valores derivados
   incoherentes.
5. Respeta `MAX_MEDICIONES` (50) y avisa cuántas líneas quedaron sin cargar.
6. Solo actualiza `*total` cuando la carga terminó. El puntero se usa para devolver
   la cantidad de registros al programa principal.

No se escribió ningún analizador manual: la separación de campos la resuelve
`sscanf` con conjuntos de caracteres (`%[^|]`).

---

## 6. Cableado del menú — `main.c:114`

| Opción | Llamada | Archivo |
|---|---|---|
| 2. Listar mediciones | `listarMediciones(lista, TOTAL_MEDICIONES)` | Sprint 4 |
| 3. Guardar mediciones | `guardarMediciones(lista, TOTAL_MEDICIONES)` | `main.c:119` |
| 4. Cargar mediciones | `cargarMediciones(lista, MAX_MEDICIONES, &TOTAL_MEDICIONES)` | `main.c:123` |
| 6. Resumen estadístico | `mostrarEstadisticas(lista, TOTAL_MEDICIONES)` | Sprint 4 |

La opción 4 pasa la dirección de `TOTAL_MEDICIONES`, de modo que la función de carga
deja el contador actualizado para las opciones siguientes y para el registro de
nuevas mediciones.

---

## 7. Pruebas ejecutadas

| # | Caso | Resultado obtenido |
|---|---|---|
| 1 | Registrar 2 equipos y guardar | `mediciones.txt` con 2 líneas y 12 campos cada una |
| 2 | Nueva ejecución: cargar y listar | *"Se cargaron 2 mediciones"*; fichas idénticas a las originales |
| 3 | Cargar 2, registrar un tercero y guardar | 3 registros en memoria y 3 líneas en el archivo |
| 4 | Nombre y detalle con `|` | Se guardan como `-`; al recargar se conserva el texto sin romper campos |
| 5 | Línea corrupta (`basura`) | *"Linea 1 ignorada: formato invalido (0 de 12 campos)"*; carga la línea válida |
| 6 | Archivo de 52 líneas | Carga 50 y avisa *"2 mediciones no se cargaron (limite 50)"* |
| 7 | Guardar después de cargar 50 | El archivo queda con 50 líneas (no se duplican las 2 extra) |
| 8 | Sin `mediciones.txt` | Avisa que no existe, no borra la memoria y no crea el archivo al guardar con 0 |
| 9 | Guardar con 0 registros | *"El archivo 'mediciones.txt' no se modifico"* |
| 10 | Compilación | 0 avisos con `-Wall -Wextra -pedantic -std=c11` |

Evidencia de ida y vuelta (misma ficha antes y después de cargar):

```text
101|Refrigerador 220V|Carga inductiva|220.00|5.50|1210.0000|1089.0000|40.00|8.00|0.90|8.712000|6.5340
102|Foco LED|Iluminacion|120.00|0.50|60.0000|60.0000|240.00|10.00|1.00|0.600000|0.4500
```

---

## 8. Criterios de aceptación

| # | Criterio | Estado |
|---|---|---|
| 1 | El registro se guarda completo con `fprintf` | Cumplido |
| 2 | El registro se recarga con `sscanf` sin analizador manual | Cumplido |
| 3 | Los datos derivados se recalculan al cargar | Cumplido |
| 4 | El separador `|` se neutraliza con `strchr` | Cumplido |
| 5 | Las opciones 3 y 4 del menú funcionan | Cumplido |
| 6 | El límite de 50 mediciones se respeta al cargar | Cumplido |
| 7 | Un archivo inexistente o con líneas corruptas no bloquea el programa | Cumplido |
| 8 | Compila sin advertencias | Cumplido |

---

## 9. Decisiones técnicas

| Decisión | Motivo |
|---|---|
| Reescribir el archivo completo en cada guardado | Mantiene `archivo == memoria` y evita registros duplicados |
| Recalcular los campos derivados al cargar | Un archivo editado a mano no puede dejar valores incoherentes |
| Reemplazar `|` solo en una copia del texto | El nombre original se conserva en memoria durante la sesión |
| Validar con `sscanf` y comparar con 12 | Detecta líneas truncadas o corruptas sin código de análisis manual |
| No modificar el archivo cuando `total == 0` | Protege datos existentes de un guardado accidental |
| Actualizar `*total` al final de la carga | La carga fallida no deja un contador inconsistente |

---

## 10. Qué sigue

El Sprint 4 completa las opciones 2 y 6: listado del registro, conteo de caracteres
con `strlen`, promedios y totales con `mostrarEstadisticas`, y el cierre de la tabla
de requisitos de la guía.

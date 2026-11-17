# Avance de Sprint 1 — Esqueleto, menú y lectura del archivo `.txt`

**Proyecto:** Sistema de Registro y Análisis de Mediciones Eléctricas
**Curso:** Programación Estructurada y Aplicada
**Tipo de trabajo:** Proyecto final de nivel intermedio
**Modalidad:** Consola (entrada y salida por pantalla)
**Etapa:** Sprint 1 de 4

> Las referencias `archivo:línea` de este documento corresponden al estado del
> código al cierre del Sprint 1. El proyecto evolucionó en los sprints siguientes,
> por lo que esos números han podido variar en las versiones actuales.

---

## 1. Integrantes

| N. | Integrante | Rol en la exposición |
|---|---|---|
| 1 | *Nombre del integrante 1* | Tema, justificación y alcance |
| 2 | *Nombre del integrante 2* | Análisis de los datos a manipular |
| 3 | *Nombre del integrante 3* | Proceso del programa y demostración en vivo |

> **Pendiente:** la guía exige un grupo de **4 a 5 personas** (punto 2 del enunciado).
> Actualmente somos **3**. Roles sugeridos para completar el equipo:
> *4) Datos que se obtendrán a la salida y resumen estadístico*,
> *5) Persistencia en archivos y cierre*.

---

## 2. Objetivo del Sprint 1

Dejar el proyecto en un estado compilable y demostrable que ya muestre la identidad del sistema completo:

- Un menú principal por consola con las 7 opciones del sistema final.
- La lectura de un archivo de texto (`.txt`) y su presentación en pantalla.
- Un control de entradas inválidas que no trabe ni cierre el programa.
- La arquitectura de archivos definitiva, lista para crecer sprint a sprint.

En este sprint **no** se implementan los cálculos eléctricos ni el registro de mediciones; se entregan en los sprints 2, 3 y 4.

---

## 3. Estructura de archivos entregada

| Archivo | Líneas | Contenido |
|---|---|---|
| `main.c` | 89 | Menú, bucle principal y control de entrada |
| `persistencia.h` | 64 | Constantes y función de lectura de archivos |
| `descripcion.txt` | 5 | Archivo de texto leído por el programa |
| `INFORME.md` | 160 | Informe del proyecto (se actualiza por sprint) |

Archivos eliminados en este sprint y su motivo:

| Archivo | Motivo |
|---|---|
| `mediciones.c` | Se creó en el Sprint 2 con la estructura definitiva |
| `mediciones.h` | Se creó en el Sprint 2 con la estructura definitiva |
| `persistencia.c` | Las implementaciones viven dentro del `.h`, este `.c` sobra |
| `main.exe` | Es un binario recompilable; no se versiona |
| `.vscode/` | Configuración del editor, ajena al programa |
| `.gitignore` | El repositorio git está en la carpeta de usuario |

---

## 4. Funciones implementadas (2 de 17)

### 4.1 `mostrarMenu` — `main.c:21`

Presenta en pantalla el menú principal con las 7 opciones del sistema completo. Se invoca en cada iteración del bucle principal.

```text
1. Registrar medicion
2. Listar mediciones registradas
3. Guardar mediciones en archivo
4. Cargar mediciones desde archivo
5. Ver descripcion del sistema (5 lineas)
6. Resumen estadistico general
0. Salir
```

Se declara `static` porque solo se usa dentro de ese archivo de traducción. Al ser una función sin prototipo, se define antes de `main`.

### 4.2 `mostrarDescripcion` — `persistencia.h:41`

Abre el archivo `descripcion.txt`, lee hasta 5 líneas y las imprime numeradas. Es la única función del Sprint 1 que trabaja con archivos.

| Paso | Código | Función |
|---|---|---|
| a | `fopen(ARCHIVO_DESCRIPCION, "r")` | Abre el archivo en modo lectura. Si devuelve `NULL` se avisa y la función retorna, sin romper el programa |
| b | `while (leidas < MAX_LINEAS && fgets(linea, ...) != NULL)` | Lee una línea por vuelta, con el tope de 5 puesto en la condición |
| c | `linea[strcspn(linea, "\r\n")] = '\0';` | `strcspn` devuelve la posición del primer `\r` o `\n`; escribiendo `\0` ahí se recorta el salto de línea |
| d | `printf("%d. %s\n", leidas + 1, linea)` | Imprime cada línea numerada |
| e | `fclose(archivo)` | Cierra el archivo |

El paso **c** evitó un error que el proyecto tuvo antes: un bucle manual donde el índice no se reiniciaba entre líneas y dejaba un `\n` embebido, lo que producía una línea en blanco en la salida.

Constantes definidas en el encabezado:

| Constante | Valor | Uso |
|---|---|---|
| `ARCHIVO_DESCRIPCION` | `"descripcion.txt"` | Ruta del archivo a leer |
| `MAX_LINEAS` | `5` | Cantidad máxima de líneas a leer |
| `LARGO_LINEA` | `120` | Tamaño del buffer de lectura |

---

## 5. Flujo del programa

```text
   +-----------------------------------------+
   |  Se muestra el banner del sistema      |   main.c:39
   +------------------+----------------------+
                      v
   +-----------------------------------------+
   |  mostrarDescripcion()                   |   lee descripcion.txt
   |  (imprime las 5 lineas)                 |   al iniciar
   +------------------+----------------------+
                      v
              +---------------+
              |   mostrarMenu |  <-----------------------+
              +-------+-------+                          |
                      v                                  |
              +---------------+                          |
              |  scanf opcion |                          |
              +-------+-------+                          |
                      v                                  |
        +-----------------------------+                 |
        | scanf fallo, opcion < 0 o   |                 |
        | opcion > 6 ?                |                 |
        +------+--------------+------+                 |
               | SI           | NO                    |
               v              v                       |
     +------------------+  +-----------+                |
     | Descartar la     |  |  switch   |                |
     | entrada, opcion  |  |  opcion   |                |
     | = -1, avisar     |  +-----+-----+                |
     +--------+---------+        |                      |
              |                  +----------------------+
              +------------------------------------->  vuelve al menu
```

---

## 6. Decisiones técnicas y su motivo

| Decisión | Motivo |
|---|---|
| Todo en consola | El enunciado pide que los datos "sean presentados por pantalla". No se usó ninguna librería gráfica |
| Funciones agrupadas en dos encabezados | La guía exige "un mínimo de ocho funciones las que deben de agruparse en dos archivos del tipo encabezado". En el Sprint 1 solo existe `persistencia.h`; `mediciones.h` nace en el Sprint 2 |
| `static` en las funciones de los encabezados | Al estar las implementaciones dentro del `.h`, se declaran `static` para que un `.h` pueda incluirse en más de un `.c` sin duplicar símbolos |
| Un solo archivo de traducción | Como los `.h` llevan los cuerpos, el proyecto se compila con `gcc main.c -o main.exe` |
| `scanf` con validación del valor de retorno | Descarta entradas inválidas sin funciones auxiliares |
| `opcion = -1` antes del `switch` | Si `scanf` falla, la variable conserva su valor anterior. Como el bucle termina con `while (opcion != 0)`, una opción fallida con valor `0` cerraría el programa de golpe. Con `-1` el bucle sigue vivo |
| Cero funciones genéricas de cadenas | Todo lo manipulable usa la librería: `strlen`, `strchr`, `snprintf`, `sscanf`, `strcspn` |

> **Nota:** el comando de compilación es de **un solo archivo**. Si el profesor compila con
> `gcc main.c mediciones.c persistencia.c`, el comando fallará porque esos `.c` ya no existen.

---

## 7. Criterios de aceptación y evidencia

| # | Criterio | Evidencia |
|---|---|---|
| 1 | Compila sin advertencias | 0 avisos con `-Wall -Wextra -pedantic` |
| 2 | Banner del sistema al iniciar | Captura de pantalla |
| 3 | Muestra las 5 líneas de `descripcion.txt` | Al iniciar y con la opción 5 |
| 4 | Menú con las 7 opciones | Captura de pantalla |
| 5 | Opciones 1-4 y 6 con mensaje de avance | Entrada `1,2,3,4,6` |
| 6 | La opción 0 cierra el programa | Entrada `0` |
| 7 | Entrada inválida no traba ni cierra | Entrada `abc`, `9`, `-3` |
| 8 | Si falta el `.txt` avisa sin romperse | Archivo renombrado temporalmente |

### Pruebas de consola ejecutadas

| Entrada | Resultado obtenido |
|---|---|
| `5` → `0` | Muestra la descripción 2 veces y sale limpio |
| `1,2,3,4,6` → `0` | Muestra los 5 mensajes "En construcción" con su sprint |
| `abc,9,-3` → `5` → `0` | Rechaza las 3 entradas inválidas, la opción 5 funciona y el programa sale por la opción 0 |
| Sin `descripcion.txt` | Imprime `Error: no se pudo abrir 'descripcion.txt' para lectura.` y continúa normalmente |

---

## 8. Contenido de `descripcion.txt`

```text
1. SISTEMA DE REGISTRO Y ANALISIS DE MEDICIONES ELECTRICAS
2. Registra el voltaje y la corriente de cada equipo electronico conectado a la instalacion.
3. Calcula potencia en vatios, resistencia en ohmios, energia en kWh y el costo estimado del consumo.
4. Guarda las mediciones en el archivo mediciones.txt para poder consultarlas en otra ejecucion.
5. Presenta un resumen estadistico con los totales de potencia, energia y costo de todo el registro.
```

Este archivo cumple la condición mínima 3 de la guía: *"leer el contenido de un archivo texto con 5 líneas de texto"*.

---

## 9. Estado de las condiciones mínimas de la guía

| # | Condición exigida | Estado al cierre del Sprint 1 | Se cumplirá en |
|---|---|---|---|
| 1 | Tema referente a las especialidades | ✅ Completo | — |
| 2 | Mín. 10 variables numéricas (locales, globales, struct) | ⏳ Pendiente | Sprint 2 |
| 3 | String de mín. 30 caracteres con conteo o reemplazo, o archivo de 5 líneas | ⏳ Parcial: el archivo de 5 líneas ya se lee; faltan el conteo (`strlen`) y el reemplazo (`strchr`) | Sprint 4 |
| 4 | Mín. 8 funciones agrupadas en dos encabezados | ⏳ Pendiente: hay 2 de 17 | Sprints 2-4 |
| 5 | Mín. una estructura en el procesamiento | ⏳ Pendiente | Sprint 2 |
| 6 | Punteros en el programa principal | ⏳ Pendiente | Sprint 2 |

---

## 10. Qué sigue: sprints 2, 3 y 4

**Sprint 2 — Modelo de datos y registro.** `struct Medicion` con 10 variables numéricas y 2 cadenas, las globales `TARIFA_KWH` y `TOTAL_MEDICIONES`, las 5 fórmulas eléctricas, `evaluarPotencia`, `ingresarMedicion` y `mostrarMedicion`, más `registrarMedicion` en `main.c` con el puntero `ptr = &lista[TOTAL_MEDICIONES]`.

**Sprint 3 — Persistencia en archivo.** `guardarMediciones` con `fprintf` y `cargarMediciones` con `sscanf` sobre el formato de 12 campos separados por `|`.

**Sprint 4 — Análisis y cierre.** `mostrarResumen`, `listarMediciones` y `mostrarEstadisticas`, el conteo con `strlen`, el reemplazo con `strchr`, y el cierre de la tabla de cumplimiento y del guion de exposición.

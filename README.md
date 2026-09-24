# 🔐 Detección de Código Malicioso en Transmisiones

**Actividad Integradora — Análisis y Diseño de Algoritmos Avanzados**
Tecnológico de Monterrey · Grupo 602

| Integrante | Matrícula |
|---|---|
| Carlos Delgado Contreras | A01712819 |
| Andrea Iliana Cantú Mayorga | A01753419 |
| Jesus Rodriguez Mendoza | A01713550 |

> 📄 Este README es un **resumen ejecutivo**. Para el análisis completo (contexto, deducciones matemáticas, tablas de complejidad y ejemplos paso a paso) consulta el reporte adjunto: **`Equipo_05_reporte.pdf`**.

---

## 🧩 El problema

Un dispositivo transmite datos a otro como una cadena de bits. Un atacante puede interceptar la transmisión e insertar código malicioso. Con transmisiones de millones de caracteres, se necesitan algoritmos **eficientes en tiempo y memoria** para responder tres preguntas:

1. ¿Podemos **detectar** un código malicioso conocido dentro de una transmisión y ubicar dónde empieza?
2. ¿Podemos **detectar estructuras simétricas** (palíndromos) dentro del contenido?
3. Si dos transmisiones fueron intervenidas con el mismo código, ¿podemos **inferir cuál fue** a partir de lo que tienen en común?

---

## ⚙️ Algoritmos implementados

### 1. Rabin-Karp + Polynomial Rolling Hash — Búsqueda de patrones
Convierte cada subcadena en un valor numérico (hash) tratándola como un polinomio, y desliza la ventana en **O(1)** por posición en vez de recomparar carácter por carácter.

- Compara hashes; solo si coinciden, verifica carácter por carácter (evita falsos positivos por colisión).
- **Complejidad:** `O(N + M)` por patrón, `O(K·(N + M))` para K patrones (o `O(N + K·M)` con la variante multipatrón).

### 2. Algoritmo de Manacher — Palíndromo más largo
Transforma el texto insertando separadores (`|`) entre caracteres para manejar palíndromos pares e impares de forma uniforme, y aprovecha la simetría de palíndromos ya encontrados para evitar comparaciones redundantes.

- **Complejidad:** `O(n)` — lineal, superando el enfoque de fuerza bruta `O(n³)` y el de expansión desde el centro `O(n²)`.

### 3. Programación Dinámica — Subcadena común más larga
Compara dos transmisiones para encontrar el bloque de caracteres **contiguos** más largo que comparten (a diferencia de una subsecuencia, donde los caracteres pueden no ser consecutivos).

- `dp[i][j] = dp[i-1][j-1] + 1` si los caracteres coinciden; `0` si no.
- Optimizado para usar solo **dos filas** de la tabla en vez de la matriz completa.
- **Complejidad:** `O(n·m)` en tiempo, `O(m)` en espacio (más `O(n+m)` para las copias limpias de las cadenas).

---

## 📊 Resumen de complejidades

| Algoritmo | Tiempo | Espacio |
|---|---|---|
| Rabin-Karp (Rolling Hash) | O(N + M) por patrón | O(1) |
| Manacher | O(n) | O(n) |
| LCS (subcadena común) | O(n · m) | O(n + m) |

---

## 🗂️ Estructura de entrada

- Las transmisiones y códigos maliciosos son archivos `.txt` con caracteres **hexadecimales** (0–9, A–F).
- Se limpian saltos de línea (`\n`, `\r`) y espacios antes de procesarlos, para que un patrón pueda detectarse aunque el archivo original tenga saltos de línea en medio.

---

## ✅ Resultado de ejemplo (subcadena común)

| Posición inicial | Posición final | Subcadena | Longitud |
|---|---|---|---|
| 24 | 26 | `ebb` | 3 |

---

## 📚 Referencias

- [String Hashing using Polynomial Rolling Hash — GeeksforGeeks](https://www.geeksforgeeks.org/dsa/string-hashing-using-polynomial-rolling-hash-function/)
- Hernández, P. P. G. — [Subsucesión común máxima (LCS)](https://pier.guillen.com.mx/algorithms/08-dinamica/08.4-lcs.htm)
- [Rolling Hash: A Powerful Technique for String Manipulation — AlgoCademy Blog](https://algocademy.com/blog/rolling-hash-a-powerful-technique-for-string-manipulation-in-programming/)

---

📎 **Reporte completo:** `Equipo_05_reporte.pdf` (incluido en este repositorio)
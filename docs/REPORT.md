# Report
# 1. Runtime complexity: initializing the intersections map

La función `build_street_graph` recorre todos los segmentos de calle una vez para construir la tabla hash del grafo. Si **n** es el número de segmentos, el bucle principal ejecuta **n** iteraciones, llamando dos veces a `add_street_to_node` por segmento (una por cada extremo), lo cual es una constante.

Dentro de `add_street_to_node`, hay un segundo bucle que recorre la cadena de colisiones del bucket correspondiente. En el caso medio, con HASH_SIZE = 10.000 y una distribución uniforme de nodos, la cadena es corta y este bucle cuesta **O(1)**. En el peor caso, si todos los nodos caen en el mismo bucket, la cadena tiene **n** elementos y el bucle cuesta **O(n)**. Como esto ocurre en cada una de las **n** iteraciones del bucle principal, el coste total es O(n) × O(n) = **O(n²)**.

Por tanto, la complejidad en el caso medio es **O(n)** y en el peor caso es **O(n²)**. Aunque en el peor caso sea **O(n²)**, no es probable que ocurra en la práctica, porque la función hash distribuye los nodos de forma uniforme entre los buckets.

- **Caso medio:** O(n) 
- **Peor caso:** O(n²)

# 2. Runtime complexity: finding the coordinates of a street or place given the name

Para buscar un lugar por nombre, el código recorre la lista de places entera comparando nombres, no para aunque encuentre uno *(busca duplicados)*. Por lo tanto, siempre cuesta lo mismo independientemente del caso, **O(P)**, donde **P** es el número de places.

Para buscar una calle por nombre, recorre la lista de houses comparando calle normalizada + número. Si encuentra la casa exacta, para gracias al `break`. En el caso medio, encontraría la casa aproximadamente a la mitad de la lista, costando **O(H/2)**, que simplificado sigue siendo **O(H)**. En el peor caso recorrería toda la lista y costaría **O(H)**, donde **H** es el número de houses. En el mejor caso la casa que buscamos es la primera de la lista y costaría **O(1)**.

- **Búsqueda por place:**
    - **Mejor caso:** O(P)
    - **Caso medio:** O(P)
    - **Peor caso:** O(P)

- **Búsqueda por street:**
    - **Mejor caso:** O(1)
    - **Caso medio:** O(H)
    - **Peor caso:** O(H)


# 3. Runtime complexity: BFS path-finding algorithm

La función `compute_bfs` implementa un BFS sobre el grafo de intersecciones, que está representado como una tabla hash donde cada entrada es una lista enlazada de intersecciones que comparten ese hash. En el bucle exterior cada nodo se desencola como máximo una vez gracias al `is_visited`, por lo tanto el bucle se ejecutará **O(V)** veces, donde **V** es el número de nodos/intersecciones.

En el bucle interior (*vecinos*) por cada nodo se recorre la lista de vecinos, es decir todas sus aristas. Sumando todos los nodos, se visita cada arista una vez en total, por lo tanto el bucle interior se ejecutará **O(E)** veces, donde **E** es el número de aristas.

Por último, `is_visited` y `mark_visited` son operaciones **O(1)** de media, por lo tanto no afectan a la complejidad.

En resumen, la complejidad del algoritmo es **O(V + E)**.

- **Mejor caso:** O(1) — el destino es el primer nodo explorado
- **Caso medio:** O(V + E)
- **Peor caso:** O(V + E)


## 5. Estudio de Rendimiento: Latencia en la Búsqueda de Calles Conectadas
# Datos Experimentales (Media de 5 ejecuciones)
| Mapa | Número de calles | Tiempo Lab 4 (Secuencial) | Tiempo Lab 5 (Tabla Hash) |
| :--- | :--- | :--- | :--- |
| **xs_1** | 13 | 2,0 ms | 3,0 ms |
| **xs_2** | 71 | 3,0 ms | 4,0 ms |
| **md_1** | 1.318 | 5,0 ms | 7,0 ms |
| **lg_1** | 3.790 | 17,0 ms | 13,0 ms |
| **xl_1** | 18.828 | 78,0 ms | 74,0 ms |
| **2xl_1** | 50.623 | 179,0 ms | 173,0 ms |

# Gráfica Comparativa
![Comparativa de tiempos: Lab 4 vs Lab 5](grafica_1.png)

# Análisis de los resultados
1. **Eficiencia de acceso:** La gráfica muestra que a medida que el volumen de datos aumenta (número de calles), la implementación con **Tabla Hash (LAB 5)** resulta ser más eficiente que la **Búsqueda Secuencial (LAB 4)**. Esta diferencia llega a ser más notable conforme el mapa es más grande (como en el caso de `xl_1` y `2xl_1`), donde el acceso directo a la tabla evita el coste de recorrer toda la lista enlazada cada vez que se debe consultar una intersección.

2. **Overhead:** En mapas pequeños (`xs_1`, `xs_2` y `md_1`), el tiempo total de ejecución en el caso del **LAB 5** puede ser muy similar o superior al del **LAB 4**. Esto se debe al *overhead*: la función `build_street_graph` requiere reservar memoria (`malloc`) y estructurar el grafo de la tabla hash antes de comenzar las búsquedas. Este coste fijo es irrelevante en mapas grandes, pero puede ser notable en mapas más pequeños.

3. **Conclusión:** Aunque el tiempo de ejecución medido incluye la carga de datos en memoria, la mejora de rendimiento del **LAB 5** respecto al **LAB 4** en los mapas más grandes demuestra que la estructura de datos (Tabla Hash) es superior para sistemas escalables.


## 6. Estudio de Rendimiento: Latencia en la Navegación
# Datos Experimentales (Media de 5 ejecuciones)
| Mapa | Número de calles | Tiempo Lab 4 (Secuencial) | Tiempo Lab 5 (Tabla Hash) |
| :--- | :--- | :--- | :--- |
| **xs_1** | 13 | 4,0 ms | 1,0 ms |
| **xs_2** | 71 | 5,0 ms | 4,0 ms |
| **md_1** | 1.318 | 7,0 ms | 8,0 ms |
| **lg_1** | 3.790 | 17,0 ms | 14,0 ms |
| **xl_1** | 18.828 | 116,0 ms | 97,0 ms |
| **2xl_1** | 50.623 | 263,0 ms | 238,0 ms |

# Gráfica Comparativa
![Comparativa de tiempos: Lab 4 vs Lab 5](grafica_2.png)

# Análisis de lso resultados
1. **Eficiencia:** A diferencia de la búsqueda simple, el cálculo de una ruta requiere explorar múltiples intersecciones. En el Lab 4, cada paso del algoritmo de búsqueda requería recorrer la lista de calles completa para encontrar los segmentos conectados. En el Lab 5, con la **Tabla Hash**, los segmentos conectados se obtienen mediante acceso directo, optimizando la ejecución del algoritmo `compute_bfs`.

2. **Overhead** Se observa que en mapas pequeños, el tiempo de ejecución de ambos laboratorios es similar. Esto se debe al costo inicial de construir la tabla hash (`build_street_graph`). En cambio en mapas de gran escala (`xl_1`, `2xl_1`), el no tener que realizar búsquedas secuenciales en cada iteración del BFS, mejora la eficacia y rapidez de ejecución.

3. **Conclusión:** Los resultados demuestran que la Tabla Hash mejora el acceso puntual a datos. La reducción de la latencia en los mapas más grandes demuestra que la implementación del Lab 5 es más eficiente para sistemas de gran escala.


## 7. Estudio de Rendimiento: Latencia según la Distancia
#### Datos Experimentales (Media de 5 ejecuciones)
| Escenario (41.3751, 2.1670) | Destino (Lat, Lon) | Lab 4 (Secuencial) | Lab 5 (Tabla Hash) |
| :--- | :--- | :--- | :--- |
| **Cercana** | 41.3764, 2.1715 | 220,0 ms | 224,0 ms |
| **Media** | 41.3919, 2.1379 | 276,0 ms | 247,0 ms |
| **Lejana** | 41.4923, 2.1389 | 319,0 ms | 279,0 ms |

# Gráfica Comparativa
![Comparativa de tiempos según distancia: Lab 4 vs Lab 5](grafica_3.png)
# Gráfica Comparativa de Curvas
![Comparativa de tiempos según distancia: Lab 4 vs Lab 5](grafica_de_corves.png)

# Análisis de los resultados
1. **Escalabilidad:** El algoritmo BFS explora nodos en capas concéntricas desde el origen. Contra mayor sea la distancia, el número de vértices y aristas visitadas mas crecera. En el Lab 4, cada consulta de vecinos tiene un coste **O(N)** (búsqueda en lista), lo que dificulta el rendimiento total a medida que la ruta se alarga. En el Lab 5, la **Tabla Hash** hace que el BFS tenga un coste de acceso de **O(1)**, acelerando el proceso del programa.

2. **Justificación del ajuste de curva:** La curva ajustada es de tipo **lineal**, coherente con la complejidad **O(V+E)** del BFS analizada en la sección 3: a mayor distancia entre origen y destino, el BFS necesita explorar más nodos (V) y más aristas (E), por lo que el tiempo crece de forma aproximadamente lineal. El tiempo de ejecución sigue una curva ascendente: cuanta más distancia cubrimos, el número de calles a procesar crece rápidamente. La curva del **Lab 4** es más pronunciada porque su tiempo de búsqueda aumenta drásticamente con cada nueva calle procesada. En cambio, la curva del **Lab 5** es más plana porque la Tabla Hash permite realizar las consultas de manera constante, independientemente de cuántas calles tenga el mapa en total.

3. **Conclusión:** El uso de una tabla hash es la mejor opción en mapas de gran escala. A pesar de que en rutas cortas el **overhead** de la estructura hash puede igualar e incluso superar los tiempos con una busqueda secuencial simple, a distancias medias y largas, la optimización de la tabla hash acelear el proceso y crea un gran diferencia con la busqueda secuencial simple.


### Mejoras
# Mejora de la estructura de datos 'visited' den el BFS

###Implementación previa
Antes de nuestra mejora, la estructura `visited` era un array dinámico de numeros `long long`. Cada numero guardaba el ID de una intersección ya visitada. Para comprobar si ujn nodo ya habia sido visitado, el algoritmo recorría el array entero de principio a fin, comparando cad aID guardado con el nodo actual, asi que en el peor de los casos (si el nodo no esta en la lista), hay que comparar todos los elementos, que tiene un costo de O(n) para cada comprobacion.


###Implementación mejorada
La hemos sustituido por un hashmap (`visited_entry **visited_map`) con `HASH_SIZE=10000` cajones, donde cada cajón contiene uba lista enlazada de nodos visitados de tipo `visited_entry`. El funcionamiento es el siguiente: cuando queremos saber si un nodo ya fue visitado, en vez de buscar en toda la lista, calculamos directamente en que cajon deberia estar usando 'hash_function' Esta operación divide el ID del nodo entre 10.000 y se queda con el resto. Esto convierte cualquier ID, por grande que sea, en un número entre 0 y 9.999 que usamos como índice directo en la tabla. Esta operación es siempre O(1) — no importa cuántos nodos hayamos visitado, siempre calculamos el cajón en un solo paso. Luego miramos solo ese cajón en vez de toda la lista. Si el cajón está vacío, el nodo no fue visitado. Si tiene elementos, recorremos solo esa lista corta buscando el ID exacto,esto maneja las colisiones, que ocurren cuando dos IDs distintos dan el mismo resto al dividir entre 10.000.

###Complejidad actual vs mejorada
Antes (array dinámico): O(n) por comprobación, donde n es el número de nodos visitados
Después (hashmap): O(m) por comprobación, donde m es el número de elementos en el cajón correspondiente.
-Mejor caso: O(1) — el cajón está vacío
-Caso medio: O(n / HASH_SIZE) ≈ O(1) — con distribución uniforme y una tabla suficientemente grande, cada cajón tiene pocos elementos
-Peor caso: O(n) — todos los IDs caen en el mismo cajón

###Trade-offs
Memoria: el hashmap siempre reserva 10.000 cajones desde el principio aunque solo se usen unos pocos, porque necesitamos la tabla completa para poder calcular el índice directamente. El array dinámico en cambio solo usaba la memoria estrictamente necesaria, creciendo con 'realloc' solo cuando se llenaba.
Peor caso: si muchos IDs de nodo caen en el mismo cajón (colisiones), la lista de ese cajón crece y la búsqueda se degrada a O(n). Esto pasaría si todos los IDs tuvieran el mismo resto al dividir entre 10.000, lo cual es muy improbable con datos reales de un mapa.
Velocidad: para mapas grandes como `xl_1` (18.828 calles), el BFS hace miles de comprobaciones de `is_visited`. Pasar de O(n) a O(1) por comprobación supone una mejora de velocidad muy significativa en la práctica, como se puede observar en los datos experimentales de las gráficas anteriores.

### Mejora del algoritmo para encontrar el segmento de calle más cercano

###Implementación previa
La función `get_closest_street` recorre todos los segmentos de calle de la lista enlazada uno por uno. Para cada segmento calcula su punto medio con `midpoint` y luego calcula la distancia Haversine entre ese punto medio y la posición del usuario. Esto significa que si hay n segmentos de calle, siempre se hacen
exactamente n cálculos de `midpoint` y n cálculos de `haversine`, independientemente de dónde esté el usuario. Para `xl_1` con 18.828 segmentos, esto son 18.828 cálculos cada vez que el usuario introduce una posición. La complejidad es O(n).

###Mejora propuesta: rejilla espacial (Spatial Grid Index)
Usaríamos una rejilla espacial: dividir el mapa en una cuadrículade bloques geográficos, y asignar cada segmento al bloque donde cae su punto medio. Cuando el usuario introduce su posición,  calculamos en qué bloque está y solo miramos los segmentos de ese bloque.
    Explicaión mas detallada: Antes de construir la rejilla necesitamos saber cuáles son las coordenadas mínimas y máximas del mapa. Para eso recorremos todos los segmentos una vez y buscamos: lat_min, lat_max, lon_min, lon_max. Decidimos cuántas filas y columnas tiene la rejilla. Por ejemplo 100×100 = 10.000 bloques. Luego calculamos el tamaño de cada bloque. La rejilla es un array bidimensional de listas enlazadas. Cada celda [fila][columna] contiene una lista de los segmentos que caen en esa zona geográfica. Para cada segmento de la lista enlazada calculamos el midpoint y vemos en que fila y columna cae (se hace una vez al cargar el mapa y cuesta O(n)). Ahora a partir del input calculamos fila ((u_lat - lat_min)) y columna ((u_lon - lon_min)), miramos los segmentos de esa celda y calculamos la distancia Haversine. Caso especial (bordes): para manejar esto correctamente, además de mirar la celda del usuario también habría que mirar las 8 celdas vecinas (arriba, abajo, izquierda, derecha y las 4 diagonales). Esto sigue siendo O(1) porque siempre son exactamente 9 celdas independientemente del tamaño del mapa.

###Complejidad actual vs mejorada
Actual (lista enlazada): O(n) por consulta — siempre se comprueban todos los segmentos
Mejorada (rejilla espacial):
-Construcción: O(n) — recorremos todos los segmentos una vez y los metemos en su bloque
-Consulta: **O(n / (filas × columnas))** de media — si usamos una rejilla de 100×100 bloques y hay 18.828 segmentos, cada bloque tiene de media ~2 segmentos. En vez de mirar 18.828 segmentos, miramos solo los ~2 del bloque del usuario.

###Trade-offs
Memoria: la rejilla reserva memoria para todos los bloques desde el principio, aunque muchos estén vacíos (zonas sin calles como el mar o parques grandes).
Densidad irregular: si hay zonas muy densas (centro de Barcelona) y zonas vacías (mar), algunos bloques tendrán muchos más segmentos que otros y el rendimiento se degrada en esas zonas.
  -El caso peor sigue siendo O(n) si todos los segmentos caen en el mismo bloque.
Beneficio real: para mapas grandes como `xl_1` (18.828 segmentos) o `2xl_1` (50.623 segmentos), pasar de mirar 18.828 segmentos a mirar ~2 supone una mejora enorme. Para mapas pequeños como `xs_1` con solo 13 segmentos, la mejora es insignificante.
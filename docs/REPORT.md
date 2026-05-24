# Report
## 1. Runtime complexity: initializing the intersections map

La función `build_street_graph` recorre todos los segmentos de calle una vez para construir la tabla hash del grafo. Si **n** es el número de segmentos, el bucle principal ejecuta **n** iteraciones, llamando dos veces a `add_street_to_node` por segmento (una por cada extremo), lo cual es una constante.

Dentro de `add_street_to_node`, hay un segundo bucle que recorre la cadena de colisiones del bucket correspondiente. En el caso medio, con HASH_SIZE = 10.000 y una distribución uniforme de nodos, la cadena es corta y este bucle cuesta **O(1)**. En el peor caso, si todos los nodos caen en el mismo bucket, la cadena tiene **n** elementos y el bucle cuesta **O(n)**. Como esto ocurre en cada una de las **n** iteraciones del bucle principal, el coste total es O(n) × O(n) = **O(n²)**.

Por tanto, la complejidad en el caso medio es **O(n)** y en el peor caso es **O(n²)**. Aunque en el peor caso sea **O(n²)**, no es probable que ocurra en la práctica, porque la función hash distribuye los nodos de forma uniforme entre los buckets.

- **Caso medio:** O(n) 
- **Peor caso:** O(n²)

## 2. Runtime complexity: finding the coordinates of a street or place given the name

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


## 3. Runtime complexity: BFS path-finding algorithm

La función `compute_bfs` implementa un BFS sobre el grafo de intersecciones, que está representado como una tabla hash donde cada entrada es una lista enlazada de intersecciones que comparten ese hash. En el bucle exterior cada nodo se desencola como máximo una vez gracias al `is_visited`, por lo tanto el bucle se ejecutará **O(V)** veces, donde **V** es el número de nodos/intersecciones.

En el bucle interior (*vecinos*) por cada nodo se recorre la lista de vecinos, es decir todas sus aristas. Sumando todos los nodos, se visita cada arista una vez en total, por lo tanto el bucle interior se ejecutará **O(E)** veces, donde **E** es el número de aristas.

Por último, `is_visited` y `mark_visited` son operaciones **O(1)** de media, por lo tanto no afectan a la complejidad.

En resumen, la complejidad del algoritmo es **O(V + E)**.

- **Mejor caso:** O(1) — el destino es el primer nodo explorado
- **Caso medio:** O(V + E)
- **Peor caso:** O(V + E)







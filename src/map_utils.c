#include "../include/map.h"
#include "sample_lib.h"
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double toDegrees(double radians) { return radians * (180.0 / M_PI); }

double toRadians(double degree) { return degree * (M_PI / 180.0); }

Position midpoint(Position a, Position b) {
  double lat1 = toRadians(a.lat);
  double lon1 = toRadians(a.lon);
  double lat2 = toRadians(b.lat);
  double lon2 = toRadians(b.lon);

  double x1 = cos(lat1) * cos(lon1);
  double y1 = cos(lat1) * sin(lon1);
  double z1 = sin(lat1);

  double x2 = cos(lat2) * cos(lon2);
  double y2 = cos(lat2) * sin(lon2);
  double z2 = sin(lat2);

  double x = (x1 + x2) / 2.0;
  double y = (y1 + y2) / 2.0;
  double z = (z1 + z2) / 2.0;

  double lon = atan2(y, x);
  double hyp = sqrt(x * x + y * y);
  double lat = atan2(z, hyp);

  Position mid;
  mid.lat = toDegrees(lat);
  mid.lon = toDegrees(lon);
  return mid;
}

long long get_closest_street(edge *list, double u_lat, double u_lon) {
  if (list == NULL) { // si la llista esta buida sortim
    return -1;
  }
  edge *closest_edge = NULL; // aqui guardarem quin sera el carrer mes proper
  double min_dist = DBL_MAX; // comencem amb una distancia infinita
  Position user_pos = {u_lat, u_lon}; // guardem la posicio de l'usuari

  edge *curr = list; // començem pel primer carrer de la llista
  while (curr != NULL) {
    Position p1 = {curr->lat1, curr->lon1}; // agafem les coorednades de inici i
                                            // final del segment de carrer
    Position p2 = {curr->lat2, curr->lon2};

    Position mid = midpoint(p1, p2); // punt que queda al mitg d'aquets carrer
    double d = haversine(
        user_pos,
        mid); // calculem distancia real entre l'usuari i el mig del carrer

    if (d < min_dist) {
      min_dist = d; // guardem aquesta distancia, com la nova distancia a
                    // superar
      closest_edge =
          curr; // apuntem que aquest actualment es el carrer mes proper
    }
    curr = curr->next; // pasem al seguent carrer de la llista
  }
  return (closest_edge != NULL) ? closest_edge->id
                                : -1; // si trobem un carrer donem el seu numero
                                      // d'indentificacio i sino tronem error
}

void unit_test_houses() {
  printf("Starting system security test\n");

  house *tenda = (house *)malloc(sizeof(
      house)); // demanem memoria a l'ordinador per poder guardar una casa
  if (tenda == NULL) { // ens avisa si no te memoria disponible
    printf("Error: Out of memory!\n");
    return;
  }
  strcpy(
      tenda->street,
      "Carrer Test"); // escrivim el nom d'un carrer inventat dins de la memoria
  tenda->num = 99;
  tenda->lat = 10.0;
  tenda->lon = 20.0;
  tenda->next = NULL;

  if (tenda->num == 99 &&
      strcmp(tenda->street, "Carrer Test") ==
          0) { // confirmem si la memoria ha guardat el que hem demanat
    printf("The house was created and saved successfully.\n");
  } else {
    printf("ERROR: Information is corrupted.\n"); // en el cas contrari, avisem
                                                  // de que hi ha un error
  }
  free(tenda); // retornem la memoria a l'ordinador
  printf("Security test completed successfully\n\n");
}
void unit_test_places() {
  printf("Starting places security test\n");

  place *sagrada = (place *)malloc(
      sizeof(place)); // demanem memoria a l'ordinador per poder guardar un lloc
  if (sagrada == NULL) { // ens avisa si no te memoria disponible
    printf("Error: Out of memory!\n");
    return;
  }
  strcpy(sagrada->name,
         "Sagrada Familia"); // escrivim el nom d'un lloc inventat dins de la
                             // memoria
  sagrada->lat = 41.4036;
  sagrada->lon = 2.1744;
  sagrada->next = NULL;

  place *upf = (place *)malloc(sizeof(
      place)); // demanem memoria a l'ordinador per poder guardar un altre lloc
  if (upf == NULL) { // ens avisa si no te memoria disponible
    printf("Error: Out of memory!\n");
    free(sagrada);
    return;
  }
  strcpy(upf->name,
         "UPF"); // escrivim el nom d'un altre lloc inventat dins de la memoria
  upf->lat = 41.3851;
  upf->lon = 2.1734;
  upf->next = NULL;

  sagrada->next =
      upf; // connectem el primer lloc amb el segon per crear la llista

  place *trobat = NULL;
  place *actual = sagrada;
  while (actual != NULL) { // recorrem tota la llista fins a arribar al final
    if (strcmp(actual->name, "UPF") ==
        0) { // si el nom coincideix amb el que busquem, parem
      trobat = actual;
      break;
    }
    actual = actual->next;
  }

  if (trobat != NULL &&
      strcmp(trobat->name, "UPF") ==
          0) { // confirmem si la memoria ha guardat el que hem demanat
    printf("The place was created and found successfully.\n");
  } else {
    printf("ERROR: Place information is corrupted.\n"); // en el cas contrari,
                                                        // avisem de que hi ha
                                                        // un error
  }

  place *p = sagrada;
  while (p != NULL) {
    place *seg = p->next;
    free(p); // retornem la memoria a l'ordinador
    p = seg;
  }
  printf("Places security test completed successfully\n\n");
}

double haversine(Position posA, Position posB) {
  double lat1 = toRadians(posA.lat);
  double lon1 = toRadians(posA.lon);
  double lat2 = toRadians(posB.lat);
  double lon2 = toRadians(posB.lon);

  double dLat = lat2 - lat1;
  double dLon = lon2 - lon1;
  double a =
      pow(sin(dLat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dLon / 2), 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return EARTH_RADIUS * c;
}

edge *get_map_streets(char *map_name) {
  char filename[MAX_STR]; // preparem espai per escriure el nom de fitxer
  sprintf(filename, "maps/%s/streets.txt",
          map_name); // busquem al fitxer del mapa que volem

  FILE *f = fopen(filename, "r"); // obrim el fitxer en mode lectura
  if (f == NULL) { // si no hem pogut obrir el fitxer, avisem i retornem Null
    printf("Error opening streets file.\n");
    return NULL;
  }

  edge *head = NULL;      // punter el primer element de la llista
  edge *tail = NULL;      // punter a l'ultim element de la llista
  char line[MAX_STR * 2]; // espai per poder guardar temporalment cada linea que
                          // hem llegit
  long long edge_id =
      0; // hacemos un contador para asignar correctamente los id de los nodos
  while (fgets(line, sizeof(line), f)) {
    edge *new_e = (edge *)malloc(
        sizeof(edge)); // reservem memoria ram per a una fitxa nova
    if (new_e ==
        NULL) { // si l'rodinandor no te més espai de meomria sortim del bucle
      break;
    }
    line[strcspn(line, "\n")] = '\0'; // esborrem el salt de linea final
    char *token = strtok(
        line, ","); // bsuquem el primer troç de dades abans de la primera coma
    if (token == NULL) { // si la linea esta buida retornem el espai i seguim
      free(new_e);
      continue;
    }
    new_e->node1 = atoll(token); // guardem el nom de la primera cruilla

    token =
        strtok(NULL, ","); // busquem el següent tros despres de la primera coma
    if (token == NULL) {
      free(new_e);
      continue;
    }
    new_e->lat1 = atof(token); // guardem la latitud de la primera cruilla

    token = strtok(NULL, ","); // busquem el següent tros
    if (token == NULL) {
      free(new_e);
      continue;
    }
    new_e->lon1 = atof(token); // guardme la longitud de la primera cruilla

    token = strtok(NULL, ","); // busquem el següent tros
    if (token == NULL) {
      free(new_e);
      continue;
    }
    new_e->node2 = atoll(token); // guardem el numero de la segona cruilla

    token = strtok(NULL, ","); // busquem el següent tros
    if (token == NULL) {
      free(new_e);
      continue;
    }
    new_e->lat2 = atof(token); // guardem la latitud de la segona cruilla

    token = strtok(NULL, ","); // busquem el següent tros
    if (token == NULL) {
      free(new_e);
      continue;
    }
    new_e->lon2 = atof(token); // guardem la longitud de la segona cruilla

    token = strtok(NULL, ","); // busquem el següent tros
    if (token == NULL) {
      free(new_e);
      continue;
    }
    new_e->length =
        atof(token); // guardem quants metres fa de llarga aquest tram de carrer

    token = strtok(NULL, ","); // l'ultim troç de dades es el nom
    if (token == NULL) {
      free(new_e);
      continue;
    }
    strcpy(new_e->name, token); // copiem el nom de carrer dins de la fitxa

    new_e->id = edge_id++; // asignamos el id correspondiente

    new_e->next = NULL; // com aquest sera l'ultima de la llista, seguidament ja
                        // no hi ha cap altre carrrer
    if (head == NULL) { // si la llista estaba buida, el carrer pasa a ser el
                        // primer i ultim de la llista
      head = new_e;
      tail = new_e;
    } else {
      tail->next = new_e; // posem el nou carrer al costat del que ja teniem
      tail = new_e;       // el nou carrer pasa a ser el ultim de la fila
    }
  }
  fclose(f);
  return head;
}

place *get_map_places(char *map_name) {
  char filename[MAX_STR]; // creem un espai per poder guardar el nom del fitxer
  sprintf(filename, "maps/%s/places.txt", map_name); // montem el nom real

  FILE *f = fopen(filename, "r"); // obrim el fitxer en mode lectura
  if (f == NULL) { // si no hem pogut obrir el fitxer, avisem i retornem Null
    printf("Error opening places file.\n");
    return NULL;
  }

  place *head = NULL;     // punter el primer element de la llista
  place *tail = NULL;     // punter a l'ultim element de la llista
  char line[MAX_STR * 2]; // espai per poder guardar temporalment cada linea que
                          // hem llegit

  while (fgets(line, sizeof(line), f)) {
    place *new_p = (place *)malloc(
        sizeof(place)); // reservem memoria ram per a un node nou
    if (new_p == NULL)
      return head; // si la memoria falla, retornem el que ja tenim
    line[strcspn(line, "\n")] = '\0'; // eliminem el salt de linea final

    char *token = strtok(line, ","); // ignorem l'ID

    token =
        strtok(NULL, ","); // busquem el primer tros, que sera el nom del lloc
    if (token == NULL)
      continue;                 // si la linea esta buida, saltem al la seguent
    strcpy(new_p->name, token); // copiem el nom a la fitza del nou lloc

    token = strtok(NULL, ","); // ignorem el tipus

    token = strtok(NULL, ","); // busquem el primer tros, que sera la latitud
    if (token == NULL)
      continue;               // si la linea esta buida, saltem al la seguent
    new_p->lat = atof(token); // convertim el text de latitud a numero real
                              // doble, de ascii a float

    token = strtok(NULL, ","); // busquem el primer tros, que sera la longitud
    if (token == NULL)
      continue;               // si la linea esta buida, saltem al la seguent
    new_p->lon = atof(token); // convertim el text de longitud a numero real
                              // doble, de ascii a float

    new_p->next =
        NULL; // definim que el nou lloc, inicilment no te ningu al darrere

    if (head == NULL) { // si la llista esta buida el nou lloc es el primer i
                        // tmable l'ultim
      head = new_p;
      tail = new_p;
    } else { // si ja hi tenim algun altre element, l'antiga cua ara apunta al
             // nou element, i el nou element pasa a ser la nova cua
      tail->next = new_p;
      tail = new_p;
    }
  }
  fclose(f);   // tanquem el fitxer un cop hem acabat de llegirlo
  return head; // retonrme la capçalera de la llista plena
}

house *get_map_houses(char *map_name) {
  char filename[MAX_STR]; // creem un espai per poder guardar el nom del fitxer
  sprintf(filename, "maps/%s/houses.txt", map_name); // montem el nom real

  FILE *f = fopen(filename, "r"); // obrim el fitxer en mode lectura
  if (f == NULL) { // si no hem pogut obrir el fitxer, avisem i retornem Null
    printf("Error opening houses file.\n");
    return NULL;
  }

  house *head = NULL;     // punter el primer element de la llista
  house *tail = NULL;     // punter a l'ultim element de la llista
  char line[MAX_STR * 2]; // espai per poder guardar temporalment cada linea que
                          // hem llegit

  while (fgets(line, sizeof(line), f)) {
    house *new_h = (house *)malloc(
        sizeof(house)); // reservem memoria ram per a un node nou
    if (new_h == NULL)
      return head; // si la memoria falla, retornem el que ja tenim
    line[strcspn(line, "\n")] = '\0'; // eliminem el salt de linea final

    char *token =
        strtok(line, ","); // busquem el primer tros, que sera el nom del lloc
    if (token == NULL)
      continue; // si la linea esta buida, saltem al la seguent
    strcpy(new_h->street, token); // copiem el nom a la fitza del nou lloc

    token = strtok(NULL, ","); // busquem el primer tros, que sera la latitud
    if (token == NULL)
      continue;               // si la linea esta buida, saltem al la seguent
    new_h->num = atoi(token); // convertim el text de latitud a numero real
                              // doble, de ascii a float

    token = strtok(NULL, ","); // busquem el primer tros, que sera la latitud
    if (token == NULL)
      continue;               // si la linea esta buida, saltem al la seguent
    new_h->lat = atof(token); // convertim el text de latitud a numero real
                              // doble, de ascii a float

    token = strtok(NULL, ","); // busquem el primer tros, que sera la longitud
    if (token == NULL)
      continue;               // si la linea esta buida, saltem al la seguent
    new_h->lon = atof(token); // convertim el text de longitud a numero real
                              // doble, de ascii a float

    new_h->next =
        NULL; // definim que el nou lloc, inicilment no te ningu al darrere

    if (head == NULL) { // si la llista esta buida el nou lloc es el primer i
                        // tmable l'ultim
      head = new_h;
      tail = new_h;
    } else { // si ja hi tenim algun altre element, l'antiga cua ara apunta al
             // nou element, i el nou element pasa a ser la nova cua
      tail->next = new_h;
      tail = new_h;
    }
  }
  fclose(f);   // tanquem el fitxer un cop hem acabat de llegirlo
  return head; // retonrme la capçalera de la llista plena
}

void normalize_street(
    char *str) { // amb aquesta funció ajustarem el text segons majusculas,
                 // minusucules i abreviatures
  for (int i = 0; str[i]; i++) { // anem lletra per lletra del text
    str[i] = tolower(str[i]);    // pasem tot el text a minsucules
  }
  char temp[MAX_STR]; // fem un espai temporal per fesl els canvis en el text
  int offset = 0;     // carecater que saltarem de l'abreviature

  if (strncmp(str, "c.", 2) ==
      0) { // en el cas de que hi hagi l'abreviature c., la convertim en carrer
    if (str[2] == ' ') { // si és c. amb un espai "c. " saltem 3 espais
      offset = 3;
    } else {
      offset = 2; // si no te espai "c." saltem 2 espais
    }
    sprintf(temp, "carrer %s",
            str + offset); // escrivim carrer seguit del nom complet
    strcpy(str, temp);     // guardem aquest resulat en el text original
  }

  else if (strncmp(str, "av.", 3) ==
           0) { // en el cas de que hi hagi l'abreviature av., la convertim en
                // avinguda
    if (str[3] == ' ') { // si és av. amb un espai "av. " saltem 4 espais
      offset = 4;
    } else { // si no te espai "av." saltem 3 espais
      offset = 3;
    }
    sprintf(temp, "avinguda %s",
            str + offset); // escrivim avinguda seguit del nom complet
    strcpy(str, temp);     // guardem aquest resulat en el text original
  }

  else if (strncmp(str, "ptge.", 5) ==
           0) { // en el cas de que hi hagi l'abreviature ptge., la convertim en
                // passatge
    if (str[5] == ' ') { // si és ptge. amb un espai "pte. " saltem 6 espais
      offset = 6;
    } else { // si no te espai "av." saltem 5 espais
      offset = 5;
    }
    sprintf(temp, "passatge %s",
            str + offset); // escrivim passatge seguit del nom complet
    strcpy(str, temp);     // guardem aquest resulat en el text original
  }
}

/*int count_matches(
    const char *s1,
    const char *s2) { // implementamos una funcion que compara los caracteres de
                      // una str con la otra en las miismas posiciones
  int matches = 0;
  for (int i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) {
    if (s1[i] == s2[i]) {
      matches++;
    }
  }
  return matches;
}*/

int min3(int a, int b, int c) { // funcion auxiliar para livenstein
  int m = a;
  if (b < m) {
    m = b;
  }
  if (c < m) {
    m = c;
  }
  return m;
}

int livenstein(const char *s1, const char *s2) {
  int len1 = strlen(s1);
  int len2 = strlen(s2);
  int matrix[len1 + 1][len2 + 1]; // creamos la matriz de las comparaciones

  for (int i = 0; i <= len1; i++) {
    matrix[i][0] = i;
  } // rellenamos la primera fila y columna con el coste inicial (transformar a
    // una cadena vacia)
  for (int j = 0; j <= len2; j++) {
    matrix[0][j] = j;
  }

  for (int i = 1; i <= len1; i++) {
    for (int j = 1; j <= len2; j++) {
      int cost;
      if (s1[i - 1] ==
          s2[j - 1]) { // determinamos el coste dependiendo si la substitucion
                       // por el numero de la diagonal es porque las letras son
                       // las mismas o porque es una sustitucion
        cost = 0;      // letras iguales
      } else {
        cost = 1;
      } // sustitucion

      matrix[i][j] = min3(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1,
                          matrix[i - 1][j - 1] +
                              cost); // buscamos el valor mas pequeños de los
                                     // cercanos(borrar,insertar,sustituir)
    }
  }
  return matrix[len1]
               [len2]; // devolvemos la distancia de livenstein total(total de
                       // cambios para que el str1 sea = a str2)(ultima celda)
}

void free_houses(house *head) {
  house *curr = head;
  while (curr != NULL) { // recorrem tota la llista de cases
    house *next = curr->next;
    free(curr);  // alliberem el node actual
    curr = next; // passem al següent
  }
}

void free_places(place *head) {
  place *curr = head;
  while (curr != NULL) { // recorrem tota la llista de llocs
    place *next = curr->next;
    free(curr);  // alliberem el node actual
    curr = next; // passem al següent
  }
}

void free_edges(edge *head) {
  edge *curr = head;
  while (curr != NULL) { // recorrem tota la llista de llocs
    edge *next = curr->next;
    free(curr);  // alliberem el node actual
    curr = next; // passem al següent
  }
}

void unit_test_streets() {
  printf("Starting streets security test\n");
  // creamos manualmente dos segmentos de calles
  edge *e1 = malloc(sizeof(edge));
  if (e1 == NULL) {
    printf("Error: Out of memory!\n");
    return;
  }
  e1->id = 0;
  e1->node1 = 100;
  e1->lat1 = 41.40;
  e1->lon1 = 2.19;
  e1->node2 = 200;
  e1->lat2 = 41.41;
  e1->lon2 = 2.20;
  e1->length = 150.0;
  strcpy(e1->name, "Street Test A");
  e1->next = NULL;

  edge *e2 = malloc(sizeof(edge));
  if (e2 == NULL) {
    printf("Error: Out of memory!\n");
    free(e1);
    return;
  }
  e2->id = 1;
  e2->node1 = 200;
  e2->lat1 = 41.41;
  e2->lon1 = 2.20;
  e2->node2 = 300;
  e2->lat2 = 41.42;
  e2->lon2 = 2.21;
  e2->length = 200.0;
  strcpy(e2->name, "Street Test B");
  e2->next = NULL;

  e1->next = e2; // enlazamos los elementos para hacer lista enlazada
  // comprobamos que los campos esta correctamente guardados y el node2 de e1
  // coincide con el node1 del e2 (estan conectados)
  if (e1->node1 == 100 && strcmp(e1->name, "Street Test A") == 0 &&
      e1->node2 == e2->node1) {
    printf("Streets linked list is created successfully\n");
  } else {
    printf("ERROR:Street information is corrupted\n");
  }
  // comprobamos que get_closest_street encuetra el segmento mas cercano
  // correctamente
  //: el pt (41.415,2.205) esta mas cerca de la mitad de el e2 que de la del e1
  long long closest = get_closest_street(e1, 41.415, 2.205);
  if (closest == e2->id) {
    printf("Closest street detection works correctly\n");
  } else {
    printf("ERROR: Closest street detection failed\n");
  }

  // libermos la memoria
  edge *curr = e1;
  while (curr != NULL) {
    edge *tmp = curr;
    curr = curr->next;
    free(tmp);
  }
  printf("Street unit test completed successfully\n\n");
}

int hash_function(long long node_id) {
  return (int)(node_id %
               HASH_SIZE); // agafem el num de la cruilla i calculem el residu
                           // de dividirho pero 10000, q sera el nostre calaix
}

void add_street_to_node(hash_entry **hash_table, long long node_id,
                        edge *segment) {
  int index =
      hash_function(node_id); // mirem quin num de calaix li toca a la cruilla

  hash_entry *entry = hash_table[index];
  while (entry != NULL) {
    if (entry->node_id == node_id) { // si tenim la mateixa seccio que el num de
                                     // cruilla, parem de buscar
      break;
    }
    entry = entry->next; // mirem la seguent seccio del calaix
  }

  if (entry == NULL) {
    entry = malloc(sizeof(hash_entry)); // demanem espai nou per poder crear la
                                        // seccio de la cruilla
    entry->node_id = node_id;           // apuntem el numero de la cruilla
    entry->streets =
        NULL; // suposem que inicialment la llista de carreres esta buida
    entry->next = hash_table[index]; // si ja hi havia altres cruilles al
                                     // calaix, les posem al darrere
    hash_table[index] =
        entry; // posem aquesta nova seccio al principi del calaix
  }
  street_node *new_s_node =
      malloc(sizeof(street_node));      // creem una fitza per al carrer
  new_s_node->street_segment = segment; // apuntem quin carrer es
  new_s_node->next =
      entry->streets; // afegim els carrers que ja teniem abans d'aquest
  entry->streets = new_s_node; // aquest carrer pasa a ser el primer de la
                               // llista d'aquesta cruilla
}

hash_entry **build_street_graph(edge *street_list) {
  hash_entry **hash_table =
      calloc(HASH_SIZE,
             sizeof(hash_entry *)); // creem una taula amb 10000 calaixo buits

  edge *curr =
      street_list; // començem per primer carrer de la llista desordenada
  while (curr != NULL) {
    add_street_to_node(hash_table, curr->node1,
                       curr); // afegim al calaix de la cruilla del inici
    add_street_to_node(hash_table, curr->node2,
                       curr); // afegim el calai de la cruilla del final
    curr = curr->next;        // pasem al seguent carrer de la llista
  }
  return hash_table; // retornem la taula ja organitzada
}
void free_hash_map(hash_entry **hash_table) {
  if (hash_table == NULL) { // si la taula no existeix, no cal fer res
    return;
  }
  for (int i = 0; i < HASH_SIZE; ++i) { // revisem els 10000 calaixos
    hash_entry *entry = hash_table[i];
    while (entry != NULL) {
      hash_entry *tmp_entry = entry; // guardem la seccio actual

      street_node *s_node = entry->streets;
      while (s_node != NULL) {
        street_node *tmp_s = s_node; // guardem la fitxa del carrer actual
        s_node = s_node->next;       // mirem quina es la seguent fitxa
        free(tmp_s);
      }
      entry = entry->next; // pasem a la seguent cruilla del calaix
      free(tmp_entry);     // esborrem la seccio de la cruilla ja buida
    }
  }
  free(hash_table);
}

void enqueue(Queue *q, Path *path_data) {
  queue_node *new_node = malloc(
      sizeof(queue_node)); // reservem espai de moemoria per a un node de la cua
  new_node->path_data =
      path_data; // guardem l'adreça de memoria on esta la info del cami
  new_node->next = NULL; // de moment no apunta en lloc
  if (q->rear == NULL) {
    q->front = q->rear = new_node;
    return;
  }
  q->rear->next =
      new_node; // connectem l'ultim bloc actual amb el nou espai de memoria
  q->rear = new_node; // actualitzem el punter final perque apunti al nou bloc
}

Path *dequeue(Queue *q) {
  if (q->front == NULL) {
    return NULL;
  }
  queue_node *temp =
      q->front; // guardem temporalment l'adreça del primer bloc de memoria
  Path *data = temp->path_data; // extreiem la info del cami
  q->front = q->front->next;    // movem el punter inicial al seguent bloc de
                                // memoria de la fila
  if (q->front == NULL) {
    q->rear = NULL;
  }
  free(temp);  // alliberem l'espai de memoria
  return data; // retornme les dades del cami per analitzarles
}

Path *compute_bfs(hash_entry **graph, long long start_node,
                  long long end_node) {
  Queue q = {NULL, NULL}; // inicialitzem la cua buida de punters

  visited_entry **visited_map = calloc(
      HASH_SIZE, sizeof(visited_entry *)); // creem el hashmap de visitats
  if (visited_map == NULL) {
    return NULL;
  } // si falla surtim

  int start_index = hash_function(
      start_node); // caluclem la posicio a la taula o es guarda l'inici
  hash_entry *start_entry =
      graph[start_index]; // accedima  l'espai de memoria d'aquella entrada
  while (start_entry != NULL && start_entry->node_id != start_node) {
    start_entry =
        start_entry
            ->next; // busquem el blocde memoria exacte de la nostra cantonada
  }
  if (start_entry == NULL) {
    return NULL;
  }
  // marquem el node inicial com visitat
  mark_visited(visited_map, start_node);

  street_node *s_node =
      start_entry
          ->streets; // mirem els carrers connectats a aquest espai de memoria
  while (s_node != NULL) { // recorrem cada carrer trobat
    edge *e = s_node->street_segment;
    if (e->node1 == start_node) { // nomes avancem en el sentit correcte del
                                  // carrer (node1 -> node2)
      Path *initial_path = malloc(sizeof(Path));
      initial_path->node_id = e->node2;
      initial_path->edge_taken =
          e; // guardem el punter a la info del carrer usat
      initial_path->parent = NULL;

      enqueue(&q, initial_path); // posem l'adreça d'aquest espai de memoria a
                                 // la fila d'espera
    }
    s_node = s_node->next; // pasem al seguent carrer de la llista
  }

  while (q.front != NULL) {
    Path *current_path =
        dequeue(&q); // agafem el bloc de memoria mes antic de la cua
    long long current_node =
        current_path->node_id; // mirem quin ID DE cantonadate guatrdat
    if (is_visited(visited_map, current_node)) {
      continue;
    } // si ja esta visitat el node, el saltem, nem al seguent

    mark_visited(visited_map,
                 current_node); // marquem el node actual com visitat

    if (current_node ==
        end_node) { // si l'ID coincideix amb el desti, alliberem la memoria
                    // sobran i retornem el punter
      free_queue(&q);
      free_visited(visited_map); // tambe alliberem la llista
      return current_path;
    }

    int idx = hash_function(current_node); // calculem on es guatrda la info
                                           // d'aquesta nova cantonada
    hash_entry *entry = graph[idx]; // anem a l'entrada corresponent del graf
    while (entry != NULL && entry->node_id != current_node) {
      entry = entry->next; // busquem el bloc de memoria de la interseccio
                           // actual
    }

    if (entry != NULL) {
      street_node *next_s =
          entry->streets; // accedim a la llista de carrers connectats
      while (next_s != NULL) {
        edge *e =
            next_s->street_segment; // agafem el punter a les dades del carrer
        if (e->node1 == current_node) {   // nomes avancem en el sentit correcte
                                          // del carrer (node1 -> node2)
          long long next_node = e->node2; // mirem cap a on apunta
          if (!is_visited(visited_map,
                          next_node)) { // nomes si el vei no esta visitat
                                        // afegim el path
            Path *new_step = malloc(sizeof(Path)); // reservem espai en memoria
            new_step->node_id =
                next_node;            // guardem la ID de la seguent cantonada
            new_step->edge_taken = e; // guardem el punter al carrer utilitzat
            new_step->parent = current_path; // conectem aquest bloc amb l'espai
                                             // de memoria anterior

            enqueue(&q, new_step); // afegim l'adreça d'aquest nou bloc a la cua
          }
        }
        next_s = next_s->next; // pasem al seguent carrer en memoria
      }
    }
  }

  free_visited(visited_map);
  return NULL;
}

void free_queue(Queue *q) {
  while (q->front != NULL) {
    dequeue(q);
  }
}

void latlon_to_xy(double lat_ref, double lon_ref, double lat, double lon,
                  double *x, double *y) {
  double lat_ref_rad = toRadians(
      lat_ref); // convertim la diferencaia de lat i lon a metres aprox
  double dlat = toRadians(lat - lat_ref); // diferencia de latitud en radianes
  double dlon = toRadians(lon - lon_ref); // diferencia de longitud en radianes
  *x = EARTH_RADIUS * dlon * cos(lat_ref_rad); // coordenada x en km
  *y = EARTH_RADIUS * dlat;                    // coordenada y en km
}

void print_route(Path *finish_path) {
  if (finish_path ==
      NULL) { // si el punter iniclia esta buit, finalitza la funcio
    return;
  }

  if (finish_path->parent == NULL) { // cas base: si no te pare, es el primer
                                     // pas del cami, el carrer inicail
    printf("  Start at %s\n",
           finish_path->edge_taken->name); // imprimir el nom del carrer inicial
    return;
  }

  print_route(
      finish_path
          ->parent); // cridem recursivament la funcio per imprimir els pasos
                     // anteriors (cami en ordre correcte del principi al final)

  edge *prev_edge = finish_path->parent->edge_taken; // segment anterior AB
  edge *curr_edge = finish_path->edge_taken;         // segment actual BC

  // els pts del gir: A(inici segment anterior), B(pt de connexio entre dos
  // segments), C(final segment actual)
  double ax = 0, ay = 0; // A
  double bx = 0, by = 0; // B
  double cx = 0, cy = 0; // C
  // agafem les referencies a partir de lat1/lonq del node anterior
  double lat_ref = prev_edge->lat1;
  double lon_ref = prev_edge->lon1;
  // ara convertim els tres pts a coordenades
  latlon_to_xy(lat_ref, lon_ref, prev_edge->lat1, prev_edge->lon1, &ax, &ay);
  latlon_to_xy(lat_ref, lon_ref, prev_edge->lat2, prev_edge->lon2, &bx, &by);
  latlon_to_xy(lat_ref, lon_ref, curr_edge->lat2, curr_edge->lon2, &cx, &cy);

  double cross =
      (bx - ax) * (cy - by) -
      (by - ay) * (cx - bx); // producte vectorial fent servir  el cross product
  // determinem cap a on gira i imprimim les indicacions
  if (cross > 0) {
    printf("  Turn left to %s and continue for %dm\n", curr_edge->name,
           (int)curr_edge->length);
  } else {
    printf("  Turn right to %s and continue for %dm\n", curr_edge->name,
           (int)curr_edge->length);
  }
}

int is_visited(visited_entry **visited_map,
               long long node_id) { // comprovem si el node ja ha estat visitat
                                    // mirant al seu calaix del hash map
  int idx = (int)(node_id % HASH_SIZE);    // calculem el calaix
  visited_entry *entry = visited_map[idx]; // anem al calaix
  while (entry != NULL) {
    if (entry->node_id ==
        node_id) { // si trobem el node es perque ha estat visitat
      return 1;
    }
    entry = entry->next;
  }
  return 0;
}

void mark_visited(
    visited_entry **visited_map,
    long long node_id) { // aafegim un node al hash map de visitats
  int idx = (int)(node_id % HASH_SIZE); // calculem el calaix
  visited_entry *new_entry = malloc(sizeof(visited_entry)); // resem la memoria
  if (new_entry == NULL) {
    return;
  } // si falla la memoria surtim
  new_entry->node_id = node_id;       // guardem Id del node
  new_entry->next = visited_map[idx]; // posem els que havien
  visited_map[idx] = new_entry;       // el nou pasa al davant de la cola
}

void free_visited(visited_entry **visited_map) { // alliberem la memoria de
                                                 // hashmap de visitats
  for (int i = 0; i < HASH_SIZE; i++) {
    visited_entry *entry = visited_map[i];
    while (entry != NULL) {
      visited_entry *tmp = entry;
      entry = entry->next;
      free(tmp);
    }
  }
  free(visited_map);
}

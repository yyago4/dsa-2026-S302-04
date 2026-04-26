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
  long long edge_id=0; // hacemos un contador para asignar correctamente los id de los nodos
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

    new_e->id=edge_id++; //asignamos el id correspondiente

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
#include "../include/map.h"
#include "sample_lib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  place *sagrada = (place *)malloc(sizeof(
      place)); // demanem memoria a l'ordinador per poder guardar un lloc
  if (sagrada == NULL) { // ens avisa si no te memoria disponible
    printf("Error: Out of memory!\n");
    return;
  }
  strcpy(
      sagrada->name,
      "Sagrada Familia"); // escrivim el nom d'un lloc inventat dins de la memoria
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
  strcpy(
      upf->name,
      "UPF"); // escrivim el nom d'un altre lloc inventat dins de la memoria
  upf->lat = 41.3851;
  upf->lon = 2.1734;
  upf->next = NULL;

  sagrada->next = upf; // connectem el primer lloc amb el segon per crear la llista

  place *trobat = NULL;
  place *actual = sagrada;
  while (actual != NULL) { // recorrem tota la llista fins a arribar al final
    if (strcmp(actual->name, "UPF") == 0) { // si el nom coincideix amb el que busquem, parem
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
    printf("ERROR: Place information is corrupted.\n"); // en el cas contrari, avisem
                                                        // de que hi ha un error
  }

  place *p = sagrada;
  while (p != NULL) { 
    place *seg = p->next;
    free(p); // retornem la memoria a l'ordinador
    p = seg;
  }
  printf("Places security test completed successfully\n\n");
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
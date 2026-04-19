#include "../include/map.h"
#include "sample_lib.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/*#define MAX_STR 150

typedef struct house {
  char street[MAX_STR];
  int num;
  double lat;
  double lon;
  struct house *next;
} house;

typedef struct place {
  char name[MAX_STR]; // nom de lloc
  double lat;         // latitud
  double lon;         // longitud
  struct place *next; // punter al seguent lloc de la llista
} place;

void unit_test_houses(){
  printf("Starting system security test\n");

  house *tenda =  (house *)malloc(sizeof(house)); //demanem memoria a
l'ordinador per poder guardar una casa if (tenda == NULL){ //ens avisa si no te
memoria disponible printf("Error: Out of memory!\n"); return;
  }
  strcpy(tenda->street, "Carrer Test"); //escrivim el nom d'un carrer inventat
dins de la memoria tenda->num = 99; tenda->lat = 10.0; tenda->lon = 20.0;
  tenda->next = NULL;

  if (tenda->num == 99 && strcmp(tenda->street, "Carrer Test") == 0){
//confirmem si la memoria ha guardat el que hem demanat printf("The house was
created and saved successfully.\n"); }else{ printf("ERROR: Information is
corrupted.\n"); //en el cas contraro, avisem de que hi ha un error
  }
  free(tenda);  //retornem la memoria a l'ordinador
  printf("Security test completed successfully\n\n");
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

    char *token = strtok(line, ","); //ignorem l'ID

    token = strtok(NULL, ","); // busquem el primer tros, que sera el nom del
lloc if (token == NULL) continue;                 // si la linea esta buida,
saltem al la seguent strcpy(new_p->name, token); // copiem el nom a la fitza del
nou lloc

    token = strtok(NULL, ","); //ignorem el tipus

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
      continue;                 // si la linea esta buida, saltem al la seguent
    strcpy(new_h->street, token); // copiem el nom a la fitza del nou lloc

    token = strtok(NULL, ","); // busquem el primer tros, que sera la latitud
    if (token == NULL)
      continue;               // si la linea esta buida, saltem al la seguent
    new_h->num   = atoi(token); // convertim el text de latitud a numero real
                              // doble, de ascii a float

    token = strtok(NULL, ","); // busquem el primer tros, que sera la latitud
    if (token == NULL)
      continue;               // si la linea esta buida, saltem al la seguent
    new_h->lat   = atof(token); // convertim el text de latitud a numero real
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
}*/

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

/*int min3(int a, int b, int c){// funcion auxiliar para livenstein
  int m=a;
  if(b<m){m=b;}
  if(c<m){m=c;}
  return m;
}

int livenstein(const char *s1, const char *s2){
  int len1=strlen(s1);
  int len2=strlen(s2);
  int matrix[len1+1][len2 +1];//creamos la matriz de las comparaciones

  for(int i=0; i<=len1; i++){matrix[i][0]=i;}//rellenamos la primera fila y
columna con el coste inicial (transformar a una cadena vacia) for(int
j=0;j<=len2;j++){matrix[0][j]=j;}

  for(int i=1; i<=len1; i++){
     for(int j=1;j<=len2;j++){
      int cost;
      if(s1[i-1]==s2[j-1]){//determinamos el coste dependiendo si la
substitucion por el numero de la diagonal es porque las letras son las mismas o
porque es una sustitucion cost=0;//letras iguales }else{cost=1;}//sustitucion

      matrix[i][j]=min3(matrix[i-1][j]+1,matrix[i][j-1]+1,matrix[i-1][j-1]+cost);//buscamos
el valor mas pequeños de los cercanos(borrar,insertar,sustituir)
     }
  }
  return matrix[len1][len2];//devolvemos la distancia de livenstein total(total
de cambios para que el str1 sea = a str2)(ultima celda)
}*/

int main() {
  unit_test_houses(); // cridem a la prova de seguretat abans d'executar el
                      // nostre programa

  char map_name[MAX_STR];
  char input_type[MAX_STR];
  int valid = 0; // 0 vol dir Fals (no vàlid), 1 vol dir Cert (vàlid)

  // Validació del mapa
  while (valid == 0) {
    printf("Introduce the map you want to use: (xs_1, xs_2, md_1, lg_1, xl_1 "
           "or 2xl_1)\n");
    scanf("%s", map_name);

    // Si es valid canviam la variable a 1 per a sortir del while
    if (strcmp(map_name, "xs_1") == 0 || strcmp(map_name, "xs_2") == 0 ||
        strcmp(map_name, "md_1") == 0 || strcmp(map_name, "lg_1") == 0 ||
        strcmp(map_name, "xl_1") == 0 || strcmp(map_name, "2xl_1") == 0) {
      valid = 1; // si el mapa existeix, sortim del bucle
    } else {
      printf("Invalid map name. Try again.\n");
    }
  }

  house *list = get_map_houses(
      map_name); // carreguem les dades del mapa triat a la memoria
  place *place_list = get_map_places(map_name); // carreguem la llista e llocs

  printf("How do you want to input your position (address, coordinate or "
         "place)?\n");
  scanf("%s", input_type);

  // Comprovem si ha triat "coordinate" o "place"
  if (strcmp(input_type, "coordinate") == 0) {
    printf("Not implemented yet.\n");
  }

  else if (strcmp(input_type, "place") == 0) {
    char search_place[MAX_STR]; // creem espai per a guardar el nom que escrigui
                                // l'usuari

    printf("Enter a place name: ");
    scanf(" %[^\n]", search_place); // lleguim tota la frese que escrigui
                                    // l'usuari incloent espais

    place *curr_p = place_list; // ens posem al principi de la llista de llocs
    int found_p = 0; // fem una variable per saber si hem trobat el lloc o no

    while (curr_p != NULL) { // mirem cada lloc de la llista fins al final
      if (strcmp(curr_p->name, search_place) ==
          0) { // si el nom es exactament igual al buscat
        printf("Found at (%f, %f)\n", curr_p->lat,
               curr_p->lon); // mostrem les coordenades
        found_p = 1;         // marquerm que l'hem trobat
        break;               // sortim del bucle
      }
      curr_p = curr_p->next; // pasem al seguent lloc de la cadena
    }

    if (!found_p) { // si la cerca exacta ha fallat, foun_p = 0
      printf("Place not know. Did you mean: \n"); // avisem que ara buscarem
                                                  // similars
      char best_names[5][MAX_STR]; // creamos matriz para guardar los 5 nombres
                                   // del top
      int best_dist[5] = {999, 999, 999, 999,
                          999}; // inicializamos los valores del top en valores
                                // altos ya que buscamos los mas pequeños
      curr_p =
          place_list; // tonem a començar des de l'inici de la llista de llocs
      int sug_count =
          0; // contador per nomes mostrar certa quantitat de sugeriments

      while (curr_p != NULL) { // sequential serching desde el rpincipio hasta
                               // el final de la lista
        int dist = livenstein(search_place, curr_p->name);
        for (int i = 0; i < 5; i++) {
          if (dist < best_dist[i]) { // si el sitio actual es mejor que el del
                                     // top en posicion i
            for (int j = 4; j > i;
                 j--) { // movemos todos los de abajo una posicion abajo
              best_dist[j] = best_dist[j - 1];
              strcpy(best_names[j], best_names[j - 1]);
            }
            best_dist[i] = dist; // insertamos el nuevo en el top
            strcpy(best_names[i], curr_p->name);
            if (sug_count < 5) {
              sug_count++;
            };
            break;
          }
        }
        curr_p = curr_p->next;
      }
      if (sug_count == 0) { // si no hemos encontrado ningun parecido lo desimos
        printf("No similar places found\n");
      } else { // imprimimos el top de parecidos
        printf("El top:\n");
        for (int i = 0; i < sug_count; i++) {
          printf("%s (Distancia: %d)\n ", best_names[i], best_dist[i]);
        }
      }
    }
  }

  else if (strcmp(input_type, "address") == 0) {
    char address[MAX_STR]; // espais per al carrer que escriura l'usuari
    int address_number;    // espai per al numero

    printf("Enter a street name (e.g. Carrer de Roc Boronat).\n");
    scanf(" %[^\n]", address); // llegim el carrer fins que trobi un salt de
                               // linea ( incloent si te espais)
    printf("Enter a house number (e.g. 138).\n");
    scanf("%d", &address_number); // llegim el numero de la casa

    // fem una copia  la cual normalitzarem per poder comparar sense errors
    char search_norm[MAX_STR];
    strcpy(search_norm, address);
    normalize_street(search_norm);

    int found = 0; // varibale per saber si hem trobat la casa
    int street_exists =
        0; // variable para saber si es el numero que no existe dentro de una
           // calle real, o porque la calle no existe
    house *current = list; // comencem a busacr desde el principi de la llista

    while (current !=
           NULL) { // mirem casa per casa fins arribar al final de la llista
      char current_street_norm[MAX_STR]; // fem espai per poder normalitzar el
                                         // carrer de la llista
      strcpy(current_street_norm, current->street);
      normalize_street(
          current_street_norm); // normaliztem el carrer de la llista

      if (strcmp(current_street_norm, search_norm) ==
          0) { // comprovem si el carrer normalitzat .
        street_exists = 1;
        if (current->num == address_number) { // si el numero tambe coincideix
          printf("Coordinates: Lat %f, Lon %f\n", current->lat,
                 current->lon); // printejem el resultat
          found = 1;            // marquem que l0hem trobat
          break;                // sortim del bucle ara queja l'hem trobat
        }
      }
      current = current->next; // pasem a la seguent casa
    }
    if (!found) {          // si hem mirat totes i no hem trobat res
      if (street_exists) { // caso en que la calle es correcta pero el numero no
        printf("Street found but number %d is invalid. Please choose a valid "
               "number:\n",
               address_number);
        current =
            list; // posicionamos al principio de la lista de todas las casas
        while (current != NULL) {
          char csn[MAX_STR];
          strcpy(csn, current->street); // copiamos la calle en variable aux
          normalize_street(csn);
          if (strcmp(csn, search_norm) ==
              0) { // miramos si scn(calle actual normalizada) y
                   // serach_norm(calle buscada normalizada) son las mismas
            printf("-%d\n",
                   current->num); // imprimimos la dirrecion para el usuario
          }
          current =
              current
                  ->next; // recorremos toda la lista(lista de todas las casas
                          // del mapa) para ver las coincidencias de las calles
        }
      } else { // caso que la calle no se sepa
        printf("Street not known. Did you mean one of these?\n");
        current = list;
        char best_streets[5][MAX_STR]; // hacemos matriz para guardar las 5
                                       // sugerencias
        int sug_count = 0;             // contador de sugerencias
        int best_dist[5] = {
            999, 999, 999, 999,
            999}; // para guardar los matches que tiene cada calle
                  // (para poner el top y no sol primeros 5)

        while (current != NULL) {
          char csn[MAX_STR];
          strcpy(csn, current->street);
          normalize_street(csn);
          int dist = livenstein(search_norm, csn); // num de parecidos
          int already_in =
              0; // auxiliar para ver si la calle ya esta en la lista

          for (int i = 0; i < sug_count;
               i++) { // la primera iteracion se lo salta ya que es la primera
                      // calle y si o si no esta en la lista
            if (strcmp(best_streets[i], current->street) == 0) {
              already_in = 1;
            }
          }
          if (!already_in) { // si la calle es nueva probamos de meterla en el
                             // top
            for (int i = 0; i < 5; i++) {
              if (dist < best_dist[i]) { // si la calle actual es mejor
                                         // que la del top en posicion i
                for (int j = 4; j > i;
                     j--) { // movemos todos los de abajo una posicion abajo
                  best_dist[j] = best_dist[j - 1];
                  strcpy(best_streets[j], best_streets[j - 1]);
                }
                best_dist[i] = dist; // metemos el nuevo
                strcpy(best_streets[i], current->street);
                if (sug_count < 5) {
                  sug_count++;
                }
                break;
              }
            }
          }
          current = current->next;
        }
        for (int i = 0; i < sug_count; i++) {
          printf("%d: %s (Coincidencias: %d)\n", i + 1, best_streets[i],
                 best_dist[i]); // imprimimos el top para el usuario
        }
      }
    }
  }

  return 0;
}

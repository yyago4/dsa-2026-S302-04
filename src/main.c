#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR 150

typedef struct house{
    char street[MAX_STR];
    int num;
    double lat;
    double lon;
    struct house* next;
} house;

void normalize_street(char* str){   //amb aquesta funció ajustarem el text segons majusculas, minusucules i abreviatures
  for(int i = 0; str[i]; i++){  //anem lletra per lletra del text
    str[i] = tolower(str[i]); //pasem tot el text a minsucules
  }
  char temp[MAX_STR]; //fem un espai temporal per fesl els canvis en el text
  int offset = 0;   //carecater que saltarem de l'abreviature

  if (strncmp(str, "c.", 2)==0){  //en el cas de que hi hagi l'abreviature c., la convertim en carrer
    if (str[2]== ' '){  // si és c. amb un espai "c. " saltem 3 espais
      offset = 3;
    }else{
      offset = 2; //si no te espai "c." saltem 2 espais
    }
    sprintf(temp, "carrer %s", str + offset); //escrivim carrer seguit del nom complet
    strcpy(str, temp);  //guardem aquest resulat en el text original
  }

  else if(strncmp(str, "av.", 3) == 0){ //en el cas de que hi hagi l'abreviature av., la convertim en avinguda
    if (str[3] == ' '){ // si és av. amb un espai "av. " saltem 4 espais
      offset = 4;
    }else{  //si no te espai "av." saltem 3 espais
      offset = 3; 
    }
    sprintf(temp, "avinguda %s", str + offset); //escrivim avinguda seguit del nom complet
    strcpy(str, temp);  //guardem aquest resulat en el text original
  }

  else if(strncmp(str, "ptge.", 5) == 0){ //en el cas de que hi hagi l'abreviature ptge., la convertim en passatge
    if (str[5] == ' '){ // si és ptge. amb un espai "pte. " saltem 6 espais
      offset = 6;
    }else{  //si no te espai "av." saltem 5 espais
      offset = 5;
    }
    sprintf(temp, "passatge %s", str + offset); //escrivim passatge seguit del nom complet
    strcpy(str, temp);  //guardem aquest resulat en el text original
  }
}

//void createaleak() {
  //char *foo = malloc(20 * sizeof(char));
  //printf("Allocated leaking string: %s", foo);
//}

int main() {
  char map_name[MAX_STR];
  char input_type[MAX_STR];
  int valid = 0; // 0 vol dir Fals (no vàlid), 1 vol dir Cert (vàlid) 

  // Validació del mapa
  while (valid == 0) {
      printf("Introduce the map you want to use: (xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1)\n");
      scanf("%s", map_name);

      // Si es valid canviam la variable a 1 per a sortir del while
      if (strcmp(map_name, "xs_1") == 0 || strcmp(map_name, "xs_2") == 0 ||
          strcmp(map_name, "md_1") == 0 || strcmp(map_name, "lg_1") == 0 ||
          strcmp(map_name, "xl_1") == 0 || strcmp(map_name, "2xl_1") == 0) {
          valid = 1;  //si el mapa existeix, sortim del bucle
      } else {
          printf("Invalid map name. Try again.\n");
      }
    }

    house* list = get_map_houses(map_name); //carreguem les dades del mapa triat a la memoria

    printf("How do you want to input your position (address, coordinate or place)?\n");
    scanf("%s", input_type);

  // Comprovem si ha triat "coordinate" o "place" 
    if (strcmp(input_type, "coordinate") == 0 || strcmp(input_type, "place") == 0) {
        printf("Not implemented yet.\n");
    }
    else if (strcmp(input_type, "address") == 0) {
        char address[MAX_STR];  //espais per al carrer que escriura l'usuari
        int address_number; //espai per al numero

        printf("Enter a street name (e.g. Carrer de Roc Boronat).\n");
        scanf(" %[^\n]", address);  //llegim el carrer fins que trobi un salt de linea ( incloent si te espais)
        printf("Enter a house number (e.g. 138).\n");
        scanf("%d", &address_number);  //llegim el numero de la casa

        //fem una copia  la cual normalitzarem per poder comparar sense errors
        char search_norm[MAX_STR];
        strcpy(search_norm, address);
        normalize_street(search_norm);

        int found = 0;  //varibale per saber si hem trobat la casa
        house* current = list;  //comencem a busacr desde el principi de la llista

        while (current!=NULL){  //mirem casa per casa fins arribar al final de la llista
          char current_street_norm[MAX_STR];  //fem espai per poder normalitzar el carrer de la llista
          strcpy(current_street_norm, current ->street);
          normalize_street(current_street_norm);  //normaliztem el carrer de la llista


          if (strcmp(current_street_norm, search_norm)== 0 && current->num == address_number){  //comprovem si el carrer normalitzat i el numero coincideixen.
            printf("Coordinates: Lat %f, Lon %f\n", current->lat, current->lon);  //printejem el resultat
            found = 1;  //marquem que l0hem trobat
            break;  // sortim del bucle ara queja l'hem trobat
          }
          current = current->next;  //pasem a la seguent casa
        }
        if (!found){  //avisem si hem mirat totes i no hem trobat res
          printf("Address not found in this map.\n");
        }
    }

    return 0;
  }
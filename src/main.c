#include "../include/map.h"
#include "sample_lib.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

int main() {
  unit_test_houses(); // cridem a la prova de seguretat abans d'executar el
                      // nostre programa
  unit_test_places(); // cridem a la prova abans d'executar el
                      // nostre programa
  unit_test_streets();
  char map_name[MAX_STR];
  char input_type[MAX_STR];
  int valid = 0; // 0 vol dir Fals (no vàlid), 1 vol dir Cert (vàlid)

  long long start_node = -1; // variable per guardar el node on comencem la ruta
  long long end_node = -1;   // variable per guardar el node on volem arribar

  // Validació del mapa
  while (valid == 0) {
    printf("Introduce the map you want to use: (xs_1, xs_2, md_1, lg_1, xl_1 "
           "or 2xl_1)\n");
    scanf("%149s", map_name);

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
  int house_count = 0;
  for (house *h = list; h != NULL; h = h->next)
    house_count++;
  printf("%d houses loaded\n", house_count);

  place *place_list = get_map_places(map_name); // carreguem la llista de llocs
  int place_count = 0;
  for (place *p = place_list; p != NULL; p = p->next)
    place_count++;
  printf("%d places loaded\n\n", place_count);

  edge *street_list = get_map_streets(map_name);
  int street_count = 0;
  for (edge *e = street_list; e != NULL; e = e->next)
    street_count++;
  printf("%d street loaded\n\n", street_count);

  hash_entry **street_hash = build_street_graph(
      street_list); // organitzem els carrers en calaixos segons la seva cruilla

  for (int step = 0; step < 2; step++) {
    if (step == 0) {
      printf("\n --- ORIGIN ---\n");
    } else {
      printf("\n--- DESTINATION ---\n");
    }
    printf("How do you want to input your position (address, coordinate or "
           "place)?\n");
    scanf("%149s", input_type);

    long long current_node = -1; // node temporal per aquesta volta

    // Comprovem si ha triat "coordinate", "place" o "address"
    if (strcmp(input_type, "coordinate") == 0) {
      double u_lat;
      double u_lon;
      if (scanf("%lf %lf", &u_lat, &u_lon) !=
          2) { // llegim els 2 numeros posats per l'usuari, si no son numeros
               // printeja
        printf("Invalid coordinates.\n");
      } else {
        long long closest_id = get_closest_street(
            street_list, u_lat,
            u_lon); // busquem quin es el carrer més proper al punt

        if (closest_id == -1) {
          printf("No streets found.\n"); // si no trobem cap carrer avisem
        } else {
          edge *target = street_list; // comencem a mirar la llista de carrers
                                      // des de l'inici
          while (target != NULL &&
                 target->id !=
                     closest_id) { // busquem fins trobar el carrer amb
                                   // el mateix ID que el mes proper
            target = target->next; // pasem al seguent carrer de la llista
          }
          if (target !=
              NULL) { // si hem trobat les ddades del carrer mes proper
                      // mostrem el nom del carrer i les 2 interseccions
            current_node = target->node1; // guardem el node per a la ruta
            printf("\nClosests street segment: %s (ID: %lld)\n", target->name,
                   target->id);
            printf("Between intersections: %lld and %lld\n", target->node1,
                   target->node2);

            printf("\nConnected street segments:\n");
            /*edge *conn = street_list; // tornem al principi de la llista per
                                      // buscar connexions
            int found_connections = 0;
            while (conn != NULL) {          // mirem tots els carrers del mapa
              if (conn->id != target->id) { // un carrer esta connectat sempre
            que
                                            // no sigui ell mateix
                if (conn->node1 == target->node1 ||
                    conn->node1 ==
                        target->node2 || // i si comparteix algun dels 2 nodes
                    conn->node2 == target->node1 ||
                    conn->node2 == target->node2) {
                  long long common_node =
                      (conn->node1 == target->node1 ||
                      conn->node1 == target->node2)
                          ? conn->node1
                          : conn->node2; // mirem el punt exacte on es toquen
            esl
                                        // 2 carres
                  printf("- %s (ID:  %lld) connects at Node %lld\n", conn->name,
                        conn->id, common_node);
                  found_connections++; // sumem 1 al contador de connexions
                                      // trobades
                }
              }
              conn = conn->next; // pasem a revisar el seguent carrer de la
            llista
            }
            if (found_connections == 0) {
              printf("No connected segments found.\n");
            }*/
            long long nodes_to_check[2] = {
                target->node1,
                target->node2}; // apuntem als 2 extresm del carrer on estem
            int found_any =
                0; // marcasdor per saber si trobe algun carrer connectat

            for (int i = 0; i < 2; ++i) { // revisem tots 2 extrems
              int idx =
                  hash_function(nodes_to_check[i]); // calculem a quin calaix de
                                                    // la taula hem de mirar
              hash_entry *entry =
                  street_hash[idx]; // busquem el continfut d'aquell calaix

              while (entry != NULL) {
                if (entry->node_id == nodes_to_check[i]) {
                  street_node *s_curr = entry->streets;
                  while (s_curr !=
                         NULL) { // mirem tots els carrers de la cruilla
                    if (s_curr->street_segment->id != target->id) {
                      printf("- %s (id: %lld) connects at Node %lld\n",
                             s_curr->street_segment->name,
                             s_curr->street_segment->id,
                             nodes_to_check[i]); // si el carrer no es el mateix
                                                 // on ja estem, mostrem el
                                                 // carrer que hi ha connectat
                      found_any = 1; // marquem que hem trobat una connexio
                    }
                    s_curr = s_curr->next; // mirem el seguent carrer de la
                                           // llista del calaix
                  }
                }
                entry = entry->next; // revisem si hi ha un altre cruilla al
                                     // mateix calaix
              }
            }
            if (!found_any) { // avisem si la taula estava buida en aquells
                              // calaixos
              printf("No connected segments found.\n");
            }
          }
        }
      }
    }

    else if (strcmp(input_type, "place") == 0) {
      char search_place[MAX_STR]; // creem espai per a guardar el nom que
                                  // escrigui l'usuari

      printf("Enter a place name: ");
      scanf(" %[^\n]", search_place); // lleguim tota la frese que escrigui
                                      // l'usuari incloent espais

      place *curr_p = place_list; // ens posem al principi de la llista de llocs
      place *matches[MAX_STR];    // llista per si trobem més d'un lloc igual
      int match_count = 0;        // comptador de llocs trobats
      int found_p = 0;            // variable per saber si n'hem trobat algun

      while (curr_p != NULL) { // mirem cada lloc de la llista fins al final
        if (strcasecmp(curr_p->name, search_place) ==
            0) { // si el nom coincideix (insensible a majúscules)
          matches[match_count++] = curr_p; // el guardem a la llista de trobats
          if (match_count >= MAX_STR)
            break; // si la llista s'omple, parem
        }
        curr_p = curr_p->next; // pasem al seguent lloc de la cadena
      }

      if (match_count > 0) { // si n'hem trobat almenys un
        found_p = 1;         // marquem que l'hem trobat
        int choice = 0;
        if (match_count > 1) { // si n'hi ha més d'un, deixem triar a l'usuari
          printf("Multiple places found. Please choose one:\n");
          for (int i = 0; i < match_count; i++) {
            printf("%d: %s at (%.6f, %.6f)\n", i + 1, matches[i]->name,
                   matches[i]->lat, matches[i]->lon);
          }
          printf("Choice: ");
          if (scanf("%d", &choice) != 1 || choice < 1 || choice > match_count) {
            choice = 1; // si s'equivoca, agafem el primer per defecte
          }
        } else {
          choice = 1; // si només n'hi ha un, l'agafem directament
        }
        printf("\n    Found at (%.6f, %.6f)\n", matches[choice - 1]->lat,
               matches[choice - 1]->lon);

        long long cl_id = get_closest_street(
            street_list, matches[choice - 1]->lat,
            matches[choice - 1]
                ->lon); // busquem el carrer + proper al punt que hem trobat
        edge *t = street_list;
        while (t && t->id != cl_id) { // busquem la fitxa d'aquest carrer
          t = t->next;
        }
        if (t) { // guardem el numero de la cruilla per al GPS
          current_node = t->node1;
          printf("    Closest street: %s\n", t->name); // nom del carrer proper
          printf("    Between %lld (%.6f, %.6f) and %lld (%.6f, %.6f)\n",
                 t->node1, t->lat1, t->lon1, t->node2, t->lat2,
                 t->lon2); // les 2 interseccions del segment
        }
      }

      if (!found_p) { // si la cerca exacta ha fallat, foun_p = 0
        printf("Place not know. Did you mean: \n"); // avisem que ara buscarem
                                                    // similars
        char best_names[5][MAX_STR]; // creamos matriz para guardar los 5
                                     // nombres del top
        int best_dist[5] = {
            999, 999, 999, 999,
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
        if (sug_count ==
            0) { // si no hemos encontrado ningun parecido lo desimos
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
      scanf(" %149[^\n]", address); // llegim el carrer fins que trobi un salt
                                    // de linea ( incloent si te espais)
      printf("Enter a house number (e.g. 138).\n");
      if (scanf("%d", &address_number) !=
          1) { // comprovem que hagi escrit un numero
        printf("Invalid number format.\n");
        free_houses(list); // si falla, alliberem la memoria abans de sortir
        free_places(place_list);
        return 1;
      }

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
            printf("\n    Found at (%.6f, %.6f)\n", current->lat,
                   current->lon); // printejem el resultat
            found = 1;            // marquem que l0hem trobat

            long long cl_id = get_closest_street(
                street_list, current->lat,
                current
                    ->lon); // busquem el carrer + proper al punt que hem trobat
            edge *t = street_list;
            while (t && t->id != cl_id) { // busquem la fitxa d'aquest carrer
              t = t->next;
            }
            if (t) { // guardem el numero de la cruilla per al GPS
              current_node = t->node1;
              printf("    Closest street: %s\n",
                     t->name); // nom del carrer proper
              printf("    Between %lld (%.6f, %.6f) and %lld (%.6f, %.6f)\n",
                     t->node1, t->lat1, t->lon1, t->node2, t->lat2,
                     t->lon2); // les 2 interseccions del segment
            }
            break; // sortim del bucle ara queja l'hem trobat
          }
        }
        current = current->next; // pasem a la seguent casa
      }
      if (!found) {          // si hem mirat totes i no hem trobat res
        if (street_exists) { // caso en que la calle es correcta pero el numero
                             // no
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
            current = current->next; // recorremos toda la lista(lista de todas
                                     // las casas del mapa) para ver las
                                     // coincidencias de las calles
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
    if (step == 0) {
      start_node = current_node;
    } else {
      end_node = current_node;
    }
  }
  if (start_node != -1 && end_node != -1) {
    printf("\n--- ROUTE ---\n");
    Path *route =
        compute_bfs(street_hash, start_node, end_node); // Busca el camí
    if (route) {
      print_route(route);
      printf("You have arrived at your destination!\n");
    } else {
      printf("No path found between thes points.\n");
    }
  }
  free_houses(list);       // retornem la memoria de les cases a l'ordinador
  free_places(place_list); // retornem la memoria dels llocs a l'ordinador
  free_edges(street_list); // retornem la memoria dels carrers a l'ordinador
  free_hash_map(street_hash);

  return 0;
}
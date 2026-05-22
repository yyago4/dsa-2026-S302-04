#include "../include/map.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_hash_map() {
  runningtest("test_hash_map");

  // Creem manualmente dos carrers enllaçats per provar la taula hash
  edge *e1 = malloc(sizeof(edge));
  edge *e2 = malloc(sizeof(edge));

  e1->id = 101;
  e1->node1 = 10;
  e1->node2 = 20;
  strcpy(e1->name, "Carrer A");
  e1->next = e2;

  e2->id = 102;
  e2->node1 = 20;
  e2->node2 = 30;
  strcpy(e2->name, "Carrer B");
  e2->next = NULL;

  // Construïm el graf de carrers usant la taula hash
  hash_entry **graph = build_street_graph(e1);

  // Comprovem que el graf s'ha construït correctament
  // El node 20 ha d'estar a la taula hash i tenir connectats els carrers A i B
  int idx = hash_function(20);
  hash_entry *entry = graph[idx];
  int found_node = 0;
  int count_streets = 0;

  while (entry != NULL) {
    if (entry->node_id == 20) {
      found_node = 1;
      street_node *curr_s = entry->streets;
      while (curr_s != NULL) {
        count_streets++;
        curr_s = curr_s->next;
      }
    }
    entry = entry->next;
  }

  assertEqualsInt(found_node, 1); // El node ha de ser trobat a la taula hash
  assertEqualsInt(count_streets,
                  2); // El node 20 està connectat tant a e1 com a e2

  // Alliberem la memòria utilitzada
  free_hash_map(graph);
  free(e1);
  free(e2);

  successtest();
}

void test_bfs_directed() {
  runningtest("test_bfs_directed");

  // Creem un circuit dirigit de 4 carrers de sentit únic:
  // e1: 1 -> 2
  // e2: 2 -> 3
  // e3: 3 -> 4
  // e4: 4 -> 1
  edge *e1 = malloc(sizeof(edge));
  edge *e2 = malloc(sizeof(edge));
  edge *e3 = malloc(sizeof(edge));
  edge *e4 = malloc(sizeof(edge));

  e1->id = 1;
  e1->node1 = 1;
  e1->node2 = 2;
  strcpy(e1->name, "Carrer 1-2");
  e1->next = e2;

  e2->id = 2;
  e2->node1 = 2;
  e2->node2 = 3;
  strcpy(e2->name, "Carrer 2-3");
  e2->next = e3;

  e3->id = 3;
  e3->node1 = 3;
  e3->node2 = 4;
  strcpy(e3->name, "Carrer 3-4");
  e3->next = e4;

  e4->id = 4;
  e4->node1 = 4;
  e4->node2 = 1;
  strcpy(e4->name, "Carrer 4-1");
  e4->next = NULL;

  hash_entry **graph = build_street_graph(e1);

  // 1. Ruta directa: de 1 a 3. Ha de ser: 1 -> 2 -> 3.
  Path *path1 = compute_bfs(graph, 1, 3);
  int path1_ok = 0;
  if (path1 != NULL) {
    // Reconstruïm anant cap enrere
    // El final ha de ser el node 3 (arribat des d'e2)
    if (path1->node_id == 3 && path1->edge_taken->id == 2) {
      Path *parent1 = path1->parent;
      // L'anterior ha de ser el node 2 (arribat des d'e1)
      if (parent1 != NULL && parent1->node_id == 2 &&
          parent1->edge_taken->id == 1) {
        if (parent1->parent == NULL) {
          path1_ok = 1;
        }
      }
    }
  }
  assertEqualsInt(path1_ok, 1);

  // Alliberem el camí trobat
  // El BFS només allibera la cua i el visited_map, però no el camí retornat,
  // així que cal alliberar-lo a mà
  Path *curr = path1;
  while (curr != NULL) {
    Path *tmp = curr;
    curr = curr->parent;
    free(tmp);
  }

  // 2. Ruta indirecta per sentit únic: de 3 a 2.
  // Com que no es pot anar enrere (3 -> 2 és prohibit ja que e2 va de 2 a 3),
  // la ruta obligada dirigit ha de fer tota la volta: 3 -> 4 -> 1 -> 2.
  Path *path2 = compute_bfs(graph, 3, 2);
  int path2_ok = 0;
  if (path2 != NULL) {
    // Reconstruïm anant cap enrere
    // Final: node 2 (des de e1)
    if (path2->node_id == 2 && path2->edge_taken->id == 1) {
      Path *parent1 = path2->parent;
      // Node anterior: node 1 (des de e4)
      if (parent1 != NULL && parent1->node_id == 1 &&
          parent1->edge_taken->id == 4) {
        Path *parent2 = parent1->parent;
        // Node anterior: node 4 (des de e3)
        if (parent2 != NULL && parent2->node_id == 4 &&
            parent2->edge_taken->id == 3) {
          if (parent2->parent == NULL) {
            path2_ok = 1;
          }
        }
      }
    }
  }
  assertEqualsInt(path2_ok, 1);

  // Alliberem el segon camí
  curr = path2;
  while (curr != NULL) {
    Path *tmp = curr;
    curr = curr->parent;
    free(tmp);
  }

  // Alliberem la taula hash i els carrers
  free_hash_map(graph);
  free(e1);
  free(e2);
  free(e3);
  free(e4);

  successtest();
}

void map_utils_test() {
  running("map_utils_test");
  {
    test_hash_map();
    test_bfs_directed();
  }
  success();
}

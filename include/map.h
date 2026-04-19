#ifndef MAP_H
#define MAP_H

#define MAX_STR 150

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

void unit_test_houses();
place *get_map_places(char *map_name);
house *get_map_houses(char *map_name);
void normalize_street(char *str);
int livenstein(const char *s1, const char *s2);
int min3(int a, int b, int c);

#endif
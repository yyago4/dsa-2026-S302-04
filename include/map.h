#ifndef MAP_H
#define MAP_H

#include <math.h>
#define MAX_STR 150
#define EARTH_RADIUS 6371.0

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct position {
  double lat;
  double lon;
} Position;

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

typedef struct node {
  int id;
  double lat;
  double lon;
  struct node *next;
} node;

typedef struct edge {
  long long id;    // identificador del segment
  long long node1; // id de la cantonada inicial
  double lat1;     // cooredenades inicials
  double lon1;
  long long node2; // id de la cantonada final
  double lat2;     // coordenades finals
  double lon2;
  double length;
  char name[MAX_STR]; // nomd el carrer
  struct edge *next;
} edge;

void unit_test_houses();
void unit_test_places();
place *get_map_places(char *map_name);
house *get_map_houses(char *map_name);
node *get_map_nodes();
edge *get_map_streets(char *map_name);
void normalize_street(char *str);
int livenstein(const char *s1, const char *s2);
int min3(int a, int b, int c);
void free_houses(house *head);
void free_places(place *head);
<<<<<<< HEAD
edge *get_mep_streets(char *map_name);
void free_nodes(node *head);
void free_edges(edge *head);

double toRadians(double degree);
double toDegrees(double radians);
double haversine(Position posA, Position posB);
Position midpoint(Position a, Position b);
long long get_closest_street(edge *list, double u_lat, double u_lon);
=======
>>>>>>> 6540fd8652f1abb16f26b5cd3a4d2f0924600d83

#endif
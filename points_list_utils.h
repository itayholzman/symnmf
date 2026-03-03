#ifndef POINTS_LIST_UTILS_H
#define POINTS_LIST_UTILS_H

#include <stdlib.h>
#include <stdio.h>

typedef struct point_coordinates_cell {
    double data;
    struct point_coordinates_cell *next;
} point_coordinates_cell;

typedef struct point_coordinates_list {
    point_coordinates_cell *head;
    point_coordinates_cell *tail;
} point_coordinates_list;

typedef struct points_cell {
    point_coordinates_list *point;
    struct points_cell *next;
} points_cell;

typedef struct points_list {
    points_cell *head;
    points_cell *tail;
    unsigned int length;
} points_list;

point_coordinates_list *create_point_coordinates_list();

int add_coordinate(point_coordinates_list *list, double value);

points_cell *create_points_cell(point_coordinates_list *point);

int add_point(points_list *plist, point_coordinates_list *point);

points_list *create_points_list();

void free_point_coordinates_list(point_coordinates_list *list);
void free_points_list(points_list *plist);
point_coordinates_list *parse_line(const char *line, unsigned int expected_dim);

points_list *read_points(unsigned int *dim, FILE* file);
double **points_to_matrix(points_list *plist, unsigned int dim);

#endif
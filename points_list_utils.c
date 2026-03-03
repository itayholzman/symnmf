#define _GNU_SOURCE

#include "points_list_utils.h"
#include "matrix_utils.h"

point_coordinates_list *create_point_coordinates_list() {
    point_coordinates_list *list = malloc(sizeof(point_coordinates_list));
    if (!list) return NULL;
    list->head = list->tail = NULL;
    return list;
}

int add_coordinate(point_coordinates_list *list, double value) {
    point_coordinates_cell *cell = malloc(sizeof(point_coordinates_cell));
    if (!cell) return 0;
    cell->data = value;
    cell->next = NULL;
    if (!list->head) list->head = list->tail = cell;
    else { list->tail->next = cell; list->tail = cell; }
    return 1;
}

points_cell *create_points_cell(point_coordinates_list *point) {
    points_cell *cell = malloc(sizeof(points_cell));
    if (!cell) return NULL;
    cell->point = point;
    cell->next = NULL;
    return cell;
}

int add_point(points_list *plist, point_coordinates_list *point) {
    points_cell *cell = create_points_cell(point);
    if (!cell) return 0;
    if (!plist->head) plist->head = plist->tail = cell;
    else { plist->tail->next = cell; plist->tail = cell; }
    plist->length++;
    return 1;
}

points_list *create_points_list() {
    points_list *plist = malloc(sizeof(points_list));
    if (!plist) return NULL;
    plist->head = plist->tail = NULL;
    plist->length = 0;
    return plist;
}

void free_point_coordinates_list(point_coordinates_list *list) {
    point_coordinates_cell *c, *next;
    if (!list) return;
    c = list->head;
    while (c) {
        next = c->next;
        free(c);
        c = next;
    }
    free(list);
}

void free_points_list(points_list *plist) {
    points_cell *p, *next_p;
    if (!plist) return;
    p = plist->head;
    while (p) {
        free_point_coordinates_list(p->point);
        next_p = p->next;
        free(p);
        p = next_p;
    }
    free(plist);
}

point_coordinates_list *parse_line(const char *line, unsigned int expected_dim) {
    const char *ptr = line;
    unsigned int dim_count = 0;
    int n;
    double value;
    point_coordinates_list *coords = create_point_coordinates_list();
    if (!coords) return NULL;
    while (*ptr) {
        if (sscanf(ptr, "%lf%n", &value, &n) != 1) { free_point_coordinates_list(coords); return NULL; }
        if (!add_coordinate(coords, value)) { free_point_coordinates_list(coords); return NULL; }
        ptr += n;
        dim_count++;
        while (*ptr == ' ' || *ptr == '\t') { ptr++; }
        if (*ptr == ',') ptr++;
        else if (*ptr != '\0') { free_point_coordinates_list(coords); return NULL; }
    }
    if (expected_dim != 0 && dim_count != expected_dim) { free_point_coordinates_list(coords); return NULL; }
    return coords;
}

points_list *read_points(unsigned int *dim, FILE* file) {
    points_list *plist;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    *dim = 0;

    plist = create_points_list();
    if (!plist) return NULL;

    while ((nread = getline(&line, &len, file)) != -1) {
        point_coordinates_list *coords;
        if (nread == 1 && line[0] == '\n') continue;
        if (line[nread - 1] == '\n') line[nread - 1] = '\0';
        coords = parse_line(line, *dim);
        free(line);
        line = NULL;
        if (!coords) { free_points_list(plist); return NULL; }
        if (plist->length == 0) {
            point_coordinates_cell *c = coords->head;
            unsigned int count = 0;
            while (c) { count++; c = c->next; }
            *dim = count;
        }
        if (!add_point(plist, coords)) { free_point_coordinates_list(coords); free_points_list(plist); return NULL; }
    }
    free(line);
    if (plist->length == 0) { free_points_list(plist); return NULL; }
    return plist;
}

double **points_to_matrix(points_list *plist, unsigned int dim) {
    double **matrix;
    points_cell *p;
    unsigned int i, j;

    matrix = generate_matrix(plist->length, dim);
    if (!matrix) return NULL;
    p = plist->head;
    i = 0;
    while (p) {
        point_coordinates_cell *c;
        c = p->point->head;
        j = 0;
        while (c) {
            matrix[i][j++] = c->data;
            c = c->next;
        }
        p = p->next;
        i++;
    }
    return matrix;
}
/**
 * @file points_list_utils.h
 * @brief Utility functions for managing lists of points and their coordinates.
 *
 * Provides linked-list structures and functions for storing points,
 * parsing them from files or strings, and converting to matrices.
 */

#ifndef POINTS_LIST_UTILS_H
#define POINTS_LIST_UTILS_H

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Node representing a single coordinate in a point.
 */
typedef struct point_coordinates_cell {
    double data;
    struct point_coordinates_cell *next;
} point_coordinates_cell;

/**
 * @brief Linked list of coordinates representing a single point.
 */
typedef struct point_coordinates_list {
    point_coordinates_cell *head;
    point_coordinates_cell *tail;
} point_coordinates_list;

/**
 * @brief Node containing a point in a list of points.
 */
typedef struct points_cell {
    point_coordinates_list *point;
    struct points_cell *next;
} points_cell;

/**
 * @brief Linked list of points.
 */
typedef struct points_list {
    points_cell *head;
    points_cell *tail;
    unsigned int length;
} points_list;

/**
 * @brief Create an empty point coordinates list.
 * 
 * @return Pointer to a newly allocated point_coordinates_list.
 */
point_coordinates_list *create_point_coordinates_list();

/**
 * @brief Add a coordinate value to a point coordinates list.
 * 
 * @param list Pointer to the list.
 * @param value Coordinate value to add.
 * @return 1 on success, 0 on failure.
 */
int add_coordinate(point_coordinates_list *list, double value);

/**
 * @brief Create a new points list node containing a point.
 * 
 * @param point Pointer to a point_coordinates_list.
 * @return Pointer to a newly allocated points_cell.
 */
points_cell *create_points_cell(point_coordinates_list *point);

/**
 * @brief Add a point to a points list.
 * 
 * @param plist Pointer to the points list.
 * @param point Pointer to the point_coordinates_list to add.
 * @return 1 on success, 0 on failure.
 */
int add_point(points_list *plist, point_coordinates_list *point);

/**
 * @brief Create an empty points list.
 * 
 * @return Pointer to a newly allocated points_list.
 */
points_list *create_points_list();

/**
 * @brief Free a point coordinates list.
 * 
 * @param list Pointer to the list to free.
 */
void free_point_coordinates_list(point_coordinates_list *list);

/**
 * @brief Free a points list and all points it contains.
 * 
 * @param plist Pointer to the points list to free.
 */
void free_points_list(points_list *plist);

/**
 * @brief Read points from a file into a points list.
 * 
 * @param dim Pointer to store the dimensionality of points.
 * @param file File pointer to read from.
 * @return Pointer to a points_list containing the read points.
 */
points_list *read_points(unsigned int *dim, FILE* file);

/**
 * @brief Convert a points list to a contiguous matrix of doubles.
 * 
 * @param plist Pointer to the points list.
 * @param dim Dimensionality of each point.
 * @return Pointer to a newly allocated matrix (double**), or NULL on failure.
 */
double **points_to_matrix(points_list *plist, unsigned int dim);

#endif
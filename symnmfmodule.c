/**
 * @file symnmfmodule.c
 * @brief Python C API wrapper for Symmetric Non-negative Matrix Factorization (SymNMF) functions.
 *
 * This module exposes three functions to Python:
 *  - sym(matrix): Compute the similarity matrix
 *  - ddg(matrix): Compute the diagonal degree matrix
 *  - norm(matrix): Compute the normalized adjacency matrix
 *
 * The module converts Python lists of lists to C double matrices, performs computations using
 * symnmf functions, and converts results back to Python lists.
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"
#include "matrix_utils.h"

/* ---------------- Helper Declarations ---------------- */

/**
 * @brief Convert a Python list-of-lists to a C double matrix.
 *
 * @param obj Python object (list of lists of floats)
 * @param rows Pointer to store number of rows
 * @param cols Pointer to store number of columns
 * @return Pointer to newly allocated C matrix (rows x cols) or NULL on failure.
 */
static double **py_to_c_matrix(PyObject *obj, unsigned int *rows, unsigned int *cols);

/**
 * @brief Convert a C double matrix to a Python list-of-lists.
 *
 * @param matrix C matrix (rows x cols)
 * @param rows Number of rows
 * @param cols Number of columns
 * @return New Python list of lists representing the matrix
 */
static PyObject *c_to_py_matrix(double **matrix, unsigned int rows, unsigned int cols);


/* ---------------- Python-exposed Functions ---------------- */

/**
 * @brief Compute similarity matrix from Python list-of-lists using RBF kernel.
 *
 * @param self Module object (unused)
 * @param args Python argument tuple containing one list-of-lists
 * @return Python list-of-lists representing similarity matrix, or NULL on error
 */
static PyObject* py_sym(PyObject *self, PyObject *args) {
    PyObject *input;
    double **matrix, **result;
    unsigned int n, dim;

    if (!PyArg_ParseTuple(args, "O", &input)) return NULL;

    matrix = py_to_c_matrix(input, &n, &dim);
    if (!matrix) return NULL;

    result = generate_similarity_matrix(matrix, dim, n);
    free_matrix(matrix);

    if (!result) return PyErr_NoMemory();

    PyObject *output = c_to_py_matrix(result, n, n);
    free_matrix(result);
    return output;
}

/**
 * @brief Compute diagonal degree matrix from Python list-of-lists.
 *
 * @param self Module object (unused)
 * @param args Python argument tuple containing one list-of-lists
 * @return Python list-of-lists representing diagonal degree matrix, or NULL on error
 */
static PyObject* py_ddg(PyObject *self, PyObject *args) {
    PyObject *input;
    double **matrix, **result;
    unsigned int n, dim;

    if (!PyArg_ParseTuple(args, "O", &input)) return NULL;

    matrix = py_to_c_matrix(input, &n, &dim);
    if (!matrix) return NULL;

    result = ddg(matrix, dim, n);

    if (!result) { free_matrix(matrix); return PyErr_NoMemory(); }

    PyObject *output = c_to_py_matrix(result, n, n);
    free_matrix(matrix);
    free_matrix(result);
    return output;
}

/**
 * @brief Compute normalized adjacency matrix from Python list-of-lists.
 *
 * @param self Module object (unused)
 * @param args Python argument tuple containing one list-of-lists
 * @return Python list-of-lists representing normalized adjacency matrix, or NULL on error
 */
static PyObject* py_norm(PyObject *self, PyObject *args) {
    PyObject *input;
    double **matrix, **result;
    unsigned int n, dim;

    if (!PyArg_ParseTuple(args, "O", &input)) return NULL;

    matrix = py_to_c_matrix(input, &n, &dim);
    if (!matrix) return NULL;

    result = norm(matrix, dim, n);

    if (!result) { free_matrix(matrix); return PyErr_NoMemory(); }

    PyObject *output = c_to_py_matrix(result, n, n);
    free_matrix(matrix);
    free_matrix(result);
    return output;
}


/* ---------------- Helper Functions ---------------- */

static double **py_to_c_matrix(PyObject *obj, unsigned int *rows, unsigned int *cols) {
    *rows = PyList_Size(obj);
    PyObject *row = PyList_GetItem(obj, 0);
    *cols = PyList_Size(row);

    double **matrix = generate_matrix(*rows, *cols);
    if (!matrix) return NULL;

    for (unsigned int i = 0; i < *rows; i++) {
        row = PyList_GetItem(obj, i);
        for (unsigned int j = 0; j < *cols; j++) {
            matrix[i][j] = PyFloat_AsDouble(PyList_GetItem(row, j));
        }
    }
    return matrix;
}

static PyObject *c_to_py_matrix(double **matrix, unsigned int rows, unsigned int cols) {
    PyObject *outer = PyList_New(rows);

    for (unsigned int i = 0; i < rows; i++) {
        PyObject *inner = PyList_New(cols);
        for (unsigned int j = 0; j < cols; j++) {
            PyList_SetItem(inner, j, PyFloat_FromDouble(matrix[i][j]));
        }
        PyList_SetItem(outer, i, inner);
    }

    return outer;
}


/* ---------------- Module Definition ---------------- */

static PyMethodDef SymNMFMethods[] = {
    {"sym",  py_sym,  METH_VARARGS, "Compute similarity matrix from input points."},
    {"ddg",  py_ddg,  METH_VARARGS, "Compute diagonal degree matrix from input points."},
    {"norm", py_norm, METH_VARARGS, "Compute normalized adjacency matrix from input points."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmf_capi",
    "Python C API wrapper for SymNMF functions",
    -1,
    SymNMFMethods
};

/**
 * @brief Module initialization function for Python.
 *
 * Called when Python imports the module.
 *
 * @return New module object.
 */
PyMODINIT_FUNC PyInit_symnmf_capi(void) {
    return PyModule_Create(&symnmfmodule);
}
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"
#include "matrix_utils.h"

static double **py_to_c_matrix(PyObject *obj, unsigned int *rows, unsigned int *cols);
static PyObject *c_to_py_matrix(double **matrix, unsigned int rows, unsigned int cols);


/* ---------------- sym ---------------- */

static PyObject* py_sym(PyObject *self, PyObject *args) {
    PyObject *input;
    double **matrix, **result;
    unsigned int n, dim;

    if (!PyArg_ParseTuple(args, "O", &input)) { return NULL; }

    matrix = py_to_c_matrix(input, &n, &dim);
    if (!matrix) { return NULL; }

    result = generate_similarity_matrix(matrix, dim, n);
    free_matrix(matrix);

    if (!result) { return PyErr_NoMemory(); }

    PyObject *output = c_to_py_matrix(result, n, n);
    free_matrix(result);

    return output;
}


/* ---------------- ddg ---------------- */

static PyObject* py_ddg(PyObject *self, PyObject *args) {
    PyObject *input;
    double **matrix, **result;
    unsigned int n, dim;

    if (!PyArg_ParseTuple(args, "O", &input)) { return NULL; }

    matrix = py_to_c_matrix(input, &n, &dim);
    if (!matrix) { return NULL; }

    result = ddg(matrix, dim, n);

    if (!result) { return PyErr_NoMemory(); }

    PyObject *output = c_to_py_matrix(result, n, n);
    free_matrix(result);

    return output;
}


/* ---------------- norm ---------------- */

static PyObject* py_norm(PyObject *self, PyObject *args) {
    PyObject *input;
    double **matrix, **result;
    unsigned int n, dim;

    if (!PyArg_ParseTuple(args, "O", &input)) { return NULL; }

    matrix = py_to_c_matrix(input, &n, &dim);
    if (!matrix) { return NULL; }

    result = norm(matrix, dim, n);

    if (!result) { return PyErr_NoMemory(); }

    PyObject *output = c_to_py_matrix(result, n, n);
    free_matrix(result);

    return output;
}


/* ---------------- Helpers ---------------- */

static double **py_to_c_matrix(PyObject *obj, unsigned int *rows, unsigned int *cols) {
    unsigned int i, j;
    PyObject *row;

    *rows = PyList_Size(obj);
    row = PyList_GetItem(obj, 0);
    *cols = PyList_Size(row);

    double **matrix = generate_matrix(*rows, *cols);
    if (!matrix) { return NULL; }

    for (i = 0; i < *rows; i++) {
        row = PyList_GetItem(obj, i);
        for (j = 0; j < *cols; j++) {
            matrix[i][j] = PyFloat_AsDouble(PyList_GetItem(row, j));
        }
    }

    return matrix;
}


static PyObject *c_to_py_matrix(double **matrix, unsigned int rows, unsigned int cols) {
    unsigned int i, j;
    PyObject *outer = PyList_New(rows);

    for (i = 0; i < rows; i++) {
        PyObject *inner = PyList_New(cols);
        for (j = 0; j < cols; j++) {
            PyList_SetItem(inner, j, PyFloat_FromDouble(matrix[i][j]));
        }
        PyList_SetItem(outer, i, inner);
    }

    return outer;
}


/* ---------------- Module Definition ---------------- */

static PyMethodDef SymNMFMethods[] = {
    {"sym",  py_sym,  METH_VARARGS, "Compute similarity matrix"},
    {"ddg",  py_ddg,  METH_VARARGS, "Compute diagonal degree matrix"},
    {"norm", py_norm, METH_VARARGS, "Compute normalized matrix"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmf_capi",
    NULL,
    -1,
    SymNMFMethods
};

PyMODINIT_FUNC PyInit_symnmf_capi(void) {
    return PyModule_Create(&symnmfmodule);
}
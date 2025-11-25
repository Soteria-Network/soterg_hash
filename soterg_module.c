#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include "soterg.h"

static PyObject *soterg_getpowhash(PyObject *self, PyObject *args)
{
    char *output;
    PyObject *value;
    PyBytesObject *input;

    if (!PyArg_ParseTuple(args, "S", &input))
        return NULL;
    Py_INCREF(input);
    output = PyMem_Malloc(32);

   soterg_hash((char *)PyBytes_AsString((PyObject *)input), output);

    Py_DECREF(input);
    value = Py_BuildValue("y#", output, 32);
    PyMem_Free(output);
    return value;
}

static PyMethodDef SoterGMethods[] = {
    {"getPoWHash", soterg_getpowhash, METH_VARARGS, "Returns the proof of work hash using SoterG hash"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef SoterGModule = {
    PyModuleDef_HEAD_INIT,
    "soterg_hash",
    "Python bindings for the soterg hash function used in Soter cryptocurrency PoW",
    -1,
    SoterGMethods};

PyMODINIT_FUNC PyInit_soterg_hash(void)
{
    return PyModule_Create(&SoterGModule);
}

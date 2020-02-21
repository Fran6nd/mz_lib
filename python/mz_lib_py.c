#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "mz_lib.h"

static pyobject *method_fputs(pyobject *self, pyobject *args) {

    char *str, *filename = null;

    int bytes_copied = -1;


    /* parse arguments */

    if(!pyarg_parsetuple(args, "ss", &str, &filename)) {

        return null;

    }


    file *fp = fopen(filename, "w");

    bytes_copied = fputs(str, fp);

    fclose(fp);


    return pylong_fromlong(bytes_copied);

}

static pyobject *method_mz_new(pyobject *self, pyobject *args) {

    char *str, *filename = null;

    int bytes_copied = -1;


    /* parse arguments */

    if(!pyarg_parsetuple(args, "ss", &str, &filename)) {

        return null;

    }


    file *fp = fopen(filename, "w");

    bytes_copied = fputs(str, fp);

    fclose(fp);


    return pylong_fromlong(bytes_copied);

}



static PyMethodDef FputsMethods[] = {
    {"fputs", method_fputs, METH_VARARGS, "Python interface for fputs C library function"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef fputsmodule = {
    PyModuleDef_HEAD_INIT,
    "fputs",
    "Python interface for the fputs C library function",
    -1,
    FputsMethods
};

PyMODINIT_FUNC PyInit_fputs(void) {
    /* Assign module value */
    PyObject *module = PyModule_Create(&fputsmodule);

    /* Add int constant by name */
    PyModule_AddIntConstant(module, "FPUTS_FLAG", 64);

    /* Define int macro */
    #define FPUTS_MACRO 256

    /* Add macro to module */
    PyModule_AddIntMacro(module, FPUTS_MACRO);

    return module;
}


#include <stdio.h>
// #include <Python.h>

//make my linter happy
#include "/usr/include/python3.6m/Python.h"

#include <curl/curl.h>

CURL *curl;

static PyObject* scan(PyObject *self, PyObject *args) {
    int ssl;
    const char* target;

    PyArg_ParseTuple(args, "si", &target, &ssl);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
      printf("ERROR in if (!curl)\n");
      Py_RETURN_NONE;
    }
    curl_easy_setopt(curl, CURLOPT_URL, target);
    if (ssl == 1) {
      // verify the peer with own CA bundle.
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      // if target uses a diffrent hostname to what the cert says
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    Py_RETURN_NONE;
}

static PyMethodDef xsser_methods[] = {
    {
        "xsser", scan, METH_VARARGS, "Perform xxs check on web page"
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef xsser_definition = {
    PyModuleDef_HEAD_INIT,
    "xsser",
    "A Python module to perform xxs checks on a web page",
    -1,
    xsser_methods
};

PyMODINIT_FUNC PyInit_xsser(void) {
    Py_Initialize();
    return PyModule_Create(&xsser_definition);
}

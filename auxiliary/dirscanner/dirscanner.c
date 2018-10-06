#include <stdio.h>
// #include <Python.h>

//make my linter happy
#include "/usr/include/python3.6m/Python.h"

#include <curl/curl.h>

FILE *fp;
CURL *curl;
CURLcode res;
PyObject* python_dict;

long doscan(){
  long http_code = 0;

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
  res = curl_easy_perform(curl);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  curl_easy_cleanup(curl);

  return http_code;
}

static PyObject* scan(PyObject *self, PyObject *args) {
    int ssl;
    const char* target;

    PyArg_ParseTuple(args, "si", &target, &ssl);
    printf("%s\n", target);

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
    python_dict = PyDict_New();
    fp = fopen("/dev/null", "w");

    // append to python dictionary.
    PyDict_SetItem(python_dict, Py_BuildValue("s", target), Py_BuildValue("i", doscan()));

    fclose(fp);

    return python_dict;
}

// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef dirscanner_methods[] = {
    {
        "scan", scan, METH_VARARGS, "Perform directory scan"
    },
    {NULL, NULL, 0, NULL}
};

// Module definition
// The arguments of this structure tell Python what to call your extension,
// what it's methods are and where to look for it's method definitions
static struct PyModuleDef dirscanner_definition = {
    PyModuleDef_HEAD_INIT,
    "dirscanner",
    "A Python module scans web directorys",
    -1,
    dirscanner_methods
};

// Module initialization
// Python calls this function when importing your extension. It is important
// that this function is named PyInit_[[your_module_name]] exactly, and matches
// the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit_dirscanner(void) {
    Py_Initialize();
    return PyModule_Create(&dirscanner_definition);
}

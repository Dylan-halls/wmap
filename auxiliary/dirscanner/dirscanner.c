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
    long http_status;
    const char* target;

    //memset(url, '\0', strlen(url));
    PyArg_ParseTuple(args, "si", &target, &ssl);
    // printf("%s/%s\n", target, uri);

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

    //kinda hacky but it works
    fp = fopen("/dev/null", "w");

    /*
    Iterate through the wordlist, create a url then scan that
    url and append the status code to a python dictionary.
    */
    http_status = doscan();
    PyDict_SetItem(python_dict, Py_BuildValue("s", target), Py_BuildValue("i", http_status));
    if (http_status != 404) {
      printf("[%ld] %s\n", http_status, target);
    }

    fclose(fp);

    return python_dict;
}

static PyMethodDef dirscanner_methods[] = {
    {
        "scan", scan, METH_VARARGS, "Perform directory scan"
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef dirscanner_definition = {
    PyModuleDef_HEAD_INIT,
    "dirscanner",
    "A Python module scans web directorys",
    -1,
    dirscanner_methods
};

PyMODINIT_FUNC PyInit_dirscanner(void) {
    Py_Initialize();
    return PyModule_Create(&dirscanner_definition);
}

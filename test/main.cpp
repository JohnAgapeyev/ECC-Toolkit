#include <Python.h>
#include <string>

#define ECC_TOOLKIT_IMPLEMENTATION
#include "ecctoolkit.h"

int main(void) {
    const std::string print_catcher =
        "from sage.all import *\n"
        "import sys\n"
                            "class Io_catcher:\n"
                            "    def __init__(self):\n"
                            "        self.value = ''\n"
                            "    def write(self, txt):\n"
                            "        self.value += txt\n"
                            "io_catcher = Io_catcher()\n"
                            "sys.stdout = io_catcher\n"
                            "sys.stderr = io_catcher\n";

    Py_Initialize();
    PyObject *mod = PyImport_AddModule("__main__");
    PyRun_SimpleString(print_catcher.c_str());
    PyRun_SimpleString("print(1+1)");
    PyRun_SimpleString("print(hex(2**255-19))");
    PyRun_SimpleString("print(EllipticCurve(GF(2**255-19),[0,486662,0,1,0]))");
    PyObject *catcher = PyObject_GetAttrString(mod, "io_catcher");

    PyObject *output = PyObject_GetAttrString(catcher, "value");
    PyObject *output2 = PyUnicode_AsEncodedString(output, "ascii", NULL);

    std::string res{PyBytes_AsString(output2)};

    Py_DecRef(output);
    Py_DecRef(output2);
    Py_Finalize();

    printf("Here's the output:\n%s", res.c_str());

    ecc::fe_25519 x = ecc::impl::prime_element();

    ecc::fe_25519 x2 = ecc::impl::prime_element();

#if 0
    x2[3] -= 10;

    x = x - x2;
#endif

    printf("This is a test\n");

    for (const auto b : x.data) {
        printf("%08x", b);
    }
    printf("\n");


    return 0;
}

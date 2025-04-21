#include <pybind11/pybind11.h>

int Add(int a, int b) {
  return a + b;
}

PYBIND11_MODULE(my_module, m) {
  m.def("add", &Add, "A function that adds two numbers");
}

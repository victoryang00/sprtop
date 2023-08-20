#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <math.h>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <assert.h>
#include <bitset>
#include "cpucounters.h"
#include "utils.h"
// ------------------
// regular C++ header
// ------------------

namespace sprtop {

class Foo
{
public:

    Foo() = default;

    template <class T>
    T bar(const T& a) const
    {
        T ret = a;
        for (auto& i : ret) {
            i *= 2.0;
        }
        return ret;
    }

    template <class T>
    T bar(const T& a, double f) const
    {
        T ret = a;
        for (auto& i : ret) {
            i *= f;
        }
        return ret;
    }

};

} // namespace sprtop

// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(sprtop_core, m)
{
    py::class_<sprtop::Foo>(m, "Foo")
        .def(py::init<>())

        .def("bar",
             static_cast<std::vector<double> (sprtop::Foo::*)(const std::vector<double>&) const>(&sprtop::Foo::bar),
             py::arg("a"))

        .def("bar",
             static_cast<std::vector<double> (sprtop::Foo::*)(const std::vector<double>&, double) const>(&sprtop::Foo::bar),
             py::arg("a"),
             py::arg("f"));
}
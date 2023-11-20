#include "../lib/MeshUp/MeshReverseEngineering/core2cha_layout.c"
#include <assert.h>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <unistd.h>
// ------------------
// regular C++ header
// ------------------

namespace sprtop {

class SPRCoreCHA {
public:
    SPRCoreCHA() { get_core2cha(); };

    uint64_t get_core_cha(const size_t &index) const { return core2cha_map[index]; }
};

} // namespace sprtop

// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(sprtop_core, m) {
    py::class_<sprtop::SPRCoreCHA>(m, "SPRCoreCHA")
        .def(py::init<>())

        .def("get_core_cha",
             static_cast<size_t (sprtop::SPRCoreCHA::*)(const size_t &) const>(&sprtop::SPRCoreCHA::get_core_cha),
             py::arg("index"));
}
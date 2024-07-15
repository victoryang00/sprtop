#include "MeshUp/MeshReverseEngineering/cha_coremapping.cpp"
#include "MeshUp/MeshReverseEngineering/disabled_core.cpp"
#include "MeshUp/MeshReverseEngineering/logging.h"
#include <cstdint>
#include <iostream>
#include <map>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
// ------------------
// regular C++ header
// ------------------
enum tile_type { XCC, LCC, MCC, UNKNOWN };

std::vector<std::vector<int>> XCCTemplate = {
    {3, 4, 4, 3, 4, 0, 0}, //
    {1, 1, 1, 1, 5, 1, 5}, // 1 is a core location
    {2, 1, 1, 1, 1, 1, 2}, // 2 is IMC (internal memory controller)
    {1, 1, 1, 1, 1, 1, 1}, // 3 UPI
    {1, 1, 1, 1, 1, 1, 1}, // 4 PCIE/CXL
    {2, 1, 1, 1, 1, 1, 2}, // 5 Disabled Cores
    {3, 4, 4, 1, 4, 1, 1}, //
};

std::vector<std::vector<int>> LCCTemplate = {
    {3, 4, 4, 3, 4, 0, 0}, //
    {1, 1, 1, 1, 1, 1, 1}, //
    {2, 1, 1, 1, 1, 1, 2}, //
    {1, 1, 1, 1, 1, 1, 1}, //
    {1, 1, 1, 1, 1, 1, 1}, //
    {2, 1, 1, 1, 1, 1, 2}, //
    {3, 4, 4, 1, 4, 1, 1}, //
};
std::vector<std::vector<int>> MCCTemplate = {
    {3, 4, 4, 3, 4, 0, 0}, //
    {1, 1, 1, 1, 1, 1, 1}, //
    {2, 1, 1, 1, 1, 1, 2}, //
    {1, 1, 1, 1, 1, 1, 1}, //
    {1, 1, 1, 1, 1, 1, 1}, //
    {2, 1, 1, 1, 1, 1, 2}, //
    {3, 4, 4, 1, 4, 1, 1}, //
};
std::map<std::string, enum tile_type> cpu_map = {
    {"Bronze 3408U", MCC},    {"Silver 4410T", MCC},       {"Silver 4410Y", MCC},    {"Silver 4416+", MCC},
    {"Gold 5411N", MCC},      {"Gold 5412U", MCC},         {"Gold 5415+", MCC},      {"Gold 5416S", MCC},
    {"Gold 5418N", MCC},      {"Gold 5418Y", MCC},         {"Gold 5420+", MCC},      {"Gold 5423N", LCC},
    {"Gold 5433N", LCC},      {"Gold 6403N", MCC},         {"Gold 6414U", XCC},      {"Gold 6416H", MCC},
    {"Gold 6418H", MCC},      {"Gold 6421N", MCC},         {"Gold 6423N", MCC},      {"Gold 6426Y", MCC},
    {"Gold 6428N", MCC},      {"Gold 6430", XCC},          {"Gold 6433N", MCC},      {"Gold 6433NE", MCC},
    {"Gold 6434", MCC},       {"Gold 6434H", MCC},         {"Gold 6438M", MCC},      {"Gold 6438N", MCC},
    {"Gold 6438Y+", MCC},     {"Gold 6442Y", MCC},         {"Gold 6443N", MCC},      {"Gold 6444Y", MCC},
    {"Gold 6448H", MCC},      {"Gold 6448Y", MCC},         {"Gold 6454S", XCC},      {"Gold 6458Q", MCC},
    {"Platinum 8444H", XCC},  {"Platinum 8450H", XCC},     {"Platinum 8452Y", XCC},  {"Platinum 8454H", XCC},
    {"Platinum 8458P", XCC},  {"Platinum 8460H", XCC},     {"Platinum 8460Y+", XCC}, {"Platinum 8461V", XCC},
    {"Platinum 8462Y+", XCC}, {"Platinum 8468", XCC},      {"Platinum 8468H", XCC},  {"Platinum 8468V", XCC},
    {"Platinum 8470", XCC},   {"Platinum 8470N", XCC},     {"Platinum 8470Q", XCC},  {"Platinum 8471N", XCC},
    {"Platinum 8480+", XCC},  {"Platinum 8480C", UNKNOWN}, {"Platinum 8490H", XCC},  {"Max 9460", XCC},
    {"Max 9462", XCC},        {"Max 9468", XCC},           {"Max 9470", XCC},        {"Max 9480", XCC}};
namespace sprtop {

class SPRCoreCHA {
public:
    SPRCoreCHA() {
        core2cha_map.reserve(get_diecorenum());
        int count;
        std::vector<std::vector<int>> f;
        switch (type_) {
        case XCC:
            f = disabled_core(XCCTemplate);
            break;
        case LCC:
            f = disabled_core(LCCTemplate);
            break;
        case MCC:
            f = disabled_core(MCCTemplate);
            break;
        }
        coremap_ = get_coremapping(f);
        for (size_t j = 0; j < coremap_[0].size(); j++) {
            for (size_t i = 0; i < coremap_.size(); i++) {
                if (coremap_[i][j] <= 0) {
                    core2cha_map[coremap_[i][j]] = count;
                    count++;
                }
            }
        }
    };
    ~SPRCoreCHA() = default;
    enum tile_type type_;
    std::vector<std::vector<int>> coremap_;
    std::vector<uint64_t> core2cha_map;
    uint64_t get_core_cha(const size_t &index) const { return core2cha_map[index]; }

    uint64_t get_diecorenum() {
        std::string output = exec("lscpu");
        std::string searchTerm = "Model name:";
        std::string modelLine = parseLscpuOutput(output, searchTerm);
        if (modelLine != "Not found") {
            std::cout << modelLine << std::endl;
        } else {
            std::cout << "CPU model name not found in lscpu output." << std::endl;
        }

        auto it = cpu_map.find(modelLine);
        if (it != cpu_map.end()) {
            type_ = it->second; // Return the complexity class if model is found
        } else {
            LOG_INFO << "Not found"; // Model not found in the map
        }
        switch (type_) {
        case XCC:
            return 28;
        case LCC:
            return 18;
        case MCC:
            return 8;
        }
    };
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
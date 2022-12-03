#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T> T* getPointerToNumpData(py::array arr)
{
    py::buffer_info buffer = arr.request();
    return static_cast<T*>(buffer.ptr);
}

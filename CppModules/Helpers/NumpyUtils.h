#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T> T* getPointerToNumpyData(py::array arr)
{
    py::buffer_info buffer = arr.request();
    return static_cast<T*>(buffer.ptr);
}


template <typename T> py::array_t<T> constructNumpyArray(std::vector<T> &vec, int height, int width)
{
    py::array arr = py::array_t<T>({ height, width });
    auto ptr = getPointerToNumpyData<T>(arr);
    std::move(vec.begin(), vec.end(), ptr);
    return arr;
}

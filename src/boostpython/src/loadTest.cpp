#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API

#include <map>
#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/python.hpp>
#include <cstdlib>

#include "../include/Detector.h"
#include "../include/PyBoostConverter.hpp"

namespace py = boost::python;
using namespace std;

typedef unsigned char uchar_t;

int main()
{
    // Allow Python to load modules from the current directory.
    string envStr(getenv("PYTHONPATH"));
    envStr += ":./src/boostpython/scripts";
    setenv("PYTHONPATH", envStr.c_str(), 1);
    // Initialize Python.

    pbcvt::initPyBindings();

    // Create basis polar image
    cv::Mat temp_mat(2, 3, CV_32FC1);
    temp_mat.at<float>(0, 0) = 0.0;
    temp_mat.at<float>(0, 1) = 1.0;
    temp_mat.at<float>(0, 2) = 2.0;
    temp_mat.at<float>(1, 0) = 3.0;
    temp_mat.at<float>(1, 1) = 4.0;
    temp_mat.at<float>(1, 2) = 5.0;

    try
    {
        // >>> import MyPythonClass
        py::object my_python_class_module = py::import("interopLoad");
        
        // >>> dog = MyPythonClass.Dog()
        py::object dog = my_python_class_module.attr("Dog")();

        dog.attr("bark")();

        // >>> dog.bark("woof");
        dog.attr("learn")("woof", temp_mat);

        Detector res = py::extract<Detector>(dog.attr("bark")());
        cout << "From C++: " << res.name << " is around " << res.value << endl;

        dog = my_python_class_module.attr("Dog")();
        dog.attr("bark")();
    }
    catch (const py::error_already_set&)
    {
        PyErr_Print();
        return 1;
    }

    return 0;
    // Do not call Py_Finalize() with Boost.Python.
}

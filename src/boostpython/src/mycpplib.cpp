#include <iostream>
#include <string.h>
#include <boost/python.hpp>

using namespace std;
using namespace boost::python;

void hello()
{
    cout << "Hello Python. I'm C++, looking forward to work with you!" << endl;
}

void print(PyObject *s)
{
   // Print it using printf
   string str = boost::python::extract<string>(s);
   cout << str << endl;

   // Get the Python string's length and convert it to an int
   //return str.length();
}

// name has to match with the name in CMakeLists `add_library` + prefix 'lib'
BOOST_PYTHON_MODULE(mycpplib)
{
   Py_Initialize();
   def("hello", hello);
   def("print", print);
}

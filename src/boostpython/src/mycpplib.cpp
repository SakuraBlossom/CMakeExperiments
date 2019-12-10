#include <iostream>
#include <string.h>
#include <boost/python.hpp>

using namespace std;
using namespace boost::python;

void hello()
{
    cout << "Hello Python. I'm C++, looking forward to work with you!" << endl;
}

// name has to match with the name in CMakeLists `add_library` + prefix 'lib'
BOOST_PYTHON_MODULE(mycpplib)
{
   Py_Initialize();
   def("hello", hello);
}

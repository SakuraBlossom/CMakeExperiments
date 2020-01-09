#include <iostream>
#include <string.h>
#include "../include/PyBoostConfig.h"

using namespace std;
using namespace boost::python;

void printStr(std::string str) {
    cout << str << endl;
}

void hello() {
    cout << "Hello Python. I'm C++, looking forward to work with you!" << endl;
}

// name has to match with the name in CMakeLists `add_library` + prefix 'lib'
BOOST_PYTHON_MODULE(mycpplib)
{
    cout << "Initialising Phase 2" << endl;
   Py_Initialize();
    cout << "Done" << endl;

   def("hello", hello);
   def("printStr", printStr);
}

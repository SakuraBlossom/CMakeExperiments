# A Collection of various Catkin builds


## dynamicMakeFile

An experiment to perform custom CMake builds depending on a value of a system environment flag.
[Note]Safety feature: A build error should be thrown if the system environment flag is missing.

### To targeted ASV 2.0 platform:
`rm -r build && export PLATFORM=ASV2 && catkin_make`

### To targeted AUV 3.X platform:
`rm -r build && export PLATFORM=AUV3 && catkin_make`

### To Target to the AUV 4.0 platform:
`rm -r build && export PLATFORM=AUV4 && catkin_make`

### Run the sample
`rosrun sample TestDemo`


## Boost Python Binding
Please build with `export PLATFORM=AUV4 && catkin_make`

### To run the python calling a custom C++ lib example
`rosrun boost_python_catkin_example test.py`

### To run the C++ calling python (which uses a custom boost C++ lib) example
`rosrun boost_python_catkin_example loadtest`


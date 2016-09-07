# modbus

On Linux:

1. Make sure you have CMake. E.g. in Ubuntu: `sudo apt-get install cmake`
2. Compile the project as a regular CMake one:
   1. `mkdir build`
   2. `cd build`
   3. `cmake ..`
   4. `make`
3. Connect the PLCs to through ethernet
4. Run `bin/modbus_cli_x.x.x`

# roboball2d_interface

[roboball2d](https://roboball2d.readthedocs.io/) is a python based simulated tennis playing robot.
This catkin package contains drivers to connect to python processes running roboball2d robots.
"Driver" here means implementing the common [robot_interfaces](https://github.com/open-dynamic-robot-initiative/robot_interfaces) driver. These drivers allow easy encapsulation by [o80](https://github.com/intelligent-soft-robots/o80). This encapsulation is done in the [o80_roboball2d](https://github.com/intelligent-soft-robots/o80_roboball2d) package.

There are three processes running roboball2d, and there source codes are [here](https://github.com/intelligent-soft-robots/roboball2d_interface/tree/master/python/roboball2d_interface).

The code of the driver is [here](https://github.com/intelligent-soft-robots/roboball2d_interface/blob/master/include/roboball2d_interface/driver.hpp)

roboball2d_interface was created by developpers of o80 with the purpose of testing o80. It is not exepected to be used beyond this, and no documentation is offered.

For installation and usage, see: [o80_roboball2d_example](https://github.com/intelligent-soft-robots/o80_roboball2d_example)

# Author

Vincent Berenz, Max Planck Institute for Intelligent Systems

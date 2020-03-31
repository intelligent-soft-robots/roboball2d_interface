from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

d = generate_distutils_setup(
    packages=['roboball2d_interface'],
    package_dir={'roboball2d_interface': 'python/roboball2d_interface'}
)

setup(**d)

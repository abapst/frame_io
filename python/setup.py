# python setup.py build_ext --inplace
#   cython f.pyx -> f.cpp
#   g++ -c f.cpp -> f.o
#   g++ -c fc.cpp -> fc.o
#   link f.o fc.o -> f.so

# distutils uses the Makefile distutils.sysconfig.get_makefile_filename()
# for compiling and linking: a sea of options.

# http://docs.python.org/distutils/introduction.html
# http://docs.python.org/distutils/apiref.html  20 pages ...
# http://stackoverflow.com/questions/tagged/distutils+python
import numpy

import sys
import subprocess

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext


args = sys.argv[1:]

if "cleansrc" in args:
    subprocess.Popen("rm -rf build", shell=True, executable="/bin/bash")
    subprocess.Popen("rm -rf *.c", shell=True, executable="/bin/bash")
    sys.argv[1] = "clean"

if "cleanall" in args:
    print("Deleting cython files...")
    subprocess.Popen("rm -rf build", shell=True, executable="/bin/bash")
    subprocess.Popen("rm -rf *.c", shell=True, executable="/bin/bash")
    subprocess.Popen("rm -rf *.so", shell=True, executable="/bin/bash")

    sys.argv[1] = "clean"

ext_modules = [Extension(
    name="frameio",
    sources=["frameio.pyx",
             "../src/frameio.c",
             "../src/imtools.c",
             "../src/draw.c",
             "../src/image.c"],
    include_dirs = [numpy.get_include()],
    language="c",
        extra_compile_args = "-Wno-cpp -I../include".split()
    )]

setup(
    name = 'frameio',
    cmdclass = {'build_ext': build_ext},
    ext_modules = ext_modules,
    )

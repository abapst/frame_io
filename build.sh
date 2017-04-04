make all

cd python
python setup.py build_ext --inplace
python setup.py cleansrc
cd ..

mv python/frameio.so lib/.

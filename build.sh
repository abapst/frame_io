RUN_TESTS=1

make all

cd python
python setup.py build_ext --inplace
python setup.py cleansrc
cd ..

mv python/*.so python/frameio.so

if [ $RUN_TESTS -eq 1 ]
then
    ./bin/run_tests
    rm *.mp4
    rm *.jpg
fi

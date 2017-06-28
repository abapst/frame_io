RUN_TESTS=1

make all

cd python
python setup.py build_ext --inplace
python setup.py cleansrc
cd ..

mv python/*.so python/frameio.so

if [ $RUN_TESTS -eq 1 ]
then
    ./build/run_tests
    rm video_out.mp4
    rm image_out.jpg
fi

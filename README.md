# frame_io

Simple library for doing common image processing tasks like reading/writing video and images, and image resizing. This is a work in progress and I add to it whenever I feel like it.

There is also a python library written using cython. It is a work in progress, mainly for me to learn cython.

The main purpose of this library in the was to avoid the hassle of having to install and compile OpenCV to do relatively simple and common image processing tasks. The goal is to provide a very simple API and easy insertion into existing projects. The only requirement is ffmpeg, for reading and writing image and video codecs.

## Installation instructions

Install prerequisites:
```
sudo apt-get install ffmpeg
```

Download the repo and make the examples:
```
git clone https://github.com/abapst/frame_io
cd frame_io
make
```

Build the python library (if desired):
```
cd python
./build.sh
```

## Using python to read an image, double its size, and write it to a new file

```python
import frameio as fio

img = fio.imread('../test/bear.jpg')
im_resized = fio.imresize(img,scale=2,mode='bilinear')
fio.imwrite(im_resized,'bear_resized.jpg')
```

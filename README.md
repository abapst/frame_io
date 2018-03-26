# frame_io

Simple library for doing common image processing tasks like reading/writing video and images, and image resizing. This is a work in progress and I add to it whenever I feel like it.

The motivation behind this library was to avoid the hassle of having to install and compile OpenCV to do relatively simple and common image processing tasks. The goal is to provide a simple API and easy inclusion into existing projects. The only requirement is ffmpeg, for reading and writing image and video codecs.

## Python Wrapper

There is also a python wrapper available. Available methods include:
```
class VideoReader
    .open(fname, mode={'r','w'},shape=(-1,-1))
    .close()
    .readframe()
    .writeframe(frame)
imread(fname,shape=(-1,-1))
imwrite(frame,fname)
imresize(frame,scale=-1,shape=(-1,-1),mode={'nearest','bilinear'})
draw_box(frame,topleft,bottomright,color,thickness=1)
draw_marker(frame,point,color,thickness=1)
rgb2gray(frame)
gray2rgb(frame)
equalizeHist(im)
```

## Installation
Install prerequisites:
```
sudo apt-get install ffmpeg
```

If you are using Ubuntu 14.04, you may need to follow the instructions <a href="https://www.faqforge.com/linux/how-to-install-ffmpeg-on-ubuntu-14-04/">here</a> in order to install ffmpeg.

Download the repo and make the examples:
```
git clone https://github.com/abapst/frame_io
cd frame_io
./build.sh
```

This will build the test examples and the python library. Note that if you want to use the python wrappers system-wide, you will need to add the following line to your .bashrc:

```
# frame_io path
export PYTHONPATH=<dir>/python:$PYTHONPATH
```
where \<dir> is the path to the frame_io root.

## Using python to read an image, double its size, and write it to a new file

```python
import frameio as fio

img = fio.imread('../test/bear.jpg')
im_resized = fio.imresize(img, scale=2, mode='bilinear')
fio.imwrite(im_resized, 'bear_resized.jpg')
```

##Support
Questions or suggestions are greatly appreciated. If you spot a mistake in this repo, please file an issue:

```
https://github.com/abapst/frameio
```

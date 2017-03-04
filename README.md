## frame_io

Simple library for doing common image processing tasks like reading/writing video and images, and image resizing. This is a work in progress and I plan to add more functionality over time.

There is also a python library written using cython. It is a work in progress, mainly for me to learn cython.

The main purpose of this library in the was to avoid the hassle of having to install and compile OpenCV to do relatively simple and common image processing tasks. The goal is to provide a very simple API and easy insertion into existing projects. The only requirement is ffmpeg, for reading and writing image and video codecs.

# Installation instructions

Prerequisites:
  - ffmpeg

Install prerequisites:
<pre>
sudo apt-get install ffmpeg
</pre>

Download the repo and make the examples:
<pre>
git clone https://github.com/abapst/frame_io
cd frame_io
make
</pre>

Build the python library (if desired):
<pre>
cd python
./build.sh
</pre>

# Using python to read an image, resize it, and write it to a new file

```python
import frameio as fio
img = fio.imread('../test/bear.jpg')
im_resized = imresize(img,shape=(500,500),mode='bilinear')
imwrite(im_resized,'bear_resized.jpg')
```

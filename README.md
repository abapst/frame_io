## frame_io

Simple library for doing common image processing tasks like reading/writing video and images, and image resizing. This is a work in progress and I plan to add more functionality over time.

The main purpose of this library in the beginning was to avoid the hassle of having to install and compile OpenCV to do relatively simple and common image processing tasks. The goal is to provide a very simple API and easy insertion into existing projects. The only requirement is ffmpeg, for reading and writing image and video codecs.

# Installation instructions

Prerequisites:
  - ffmpeg

<pre>
sudo apt-get install ffmpeg
git clone https://github.com/abapst/frame_io
cd frame_io
make
</pre>

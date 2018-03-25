import numpy as np

def rgb2yuv(rgb):

    yuv = np.empty_like(rgb)
    xform = np.array([[.299, .587, .114], [-.1687, -.3313, .5], [.5, -.4187, -.0813]])
    yuv = rgb.dot(xform.T)
    yuv[:,:,[1,2]] += 128

    return yuv

def yuv2rgb(yuv):

    rgb = np.empty_like(yuv)
    xform = np.array([[1, 0, 1.402], [1, -0.34414, -.71414], [1, 1.772, 0]])
    yuv[:,:,[1,2]] -= 128
    rgb = yuv.dot(xform.T)

    return rgb

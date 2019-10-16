import numpy as np
import os
import matplotlib.pyplot as plt
from task2ab import save_im


def convolve_im(im, kernel):
    """ A function that convolves im with kernel
    
    Args:
        im ([type]): [np.array of shape [H, W, 3]]
        kernel ([type]): [np.array of shape [K, K]]
    
    Returns:
        [type]: [np.array of shape [H, W, 3]. should be same as im]
    """
    kernel = np.flipud(np.fliplr(kernel))    # Flip the kernel
    output = np.zeros_like(image)            # convolution output
    # Add zero padding to the input image
    image_padded = np.zeros((image.shape[0] + 2, image.shape[1] + 2))   
    image_padded[1:-1, 1:-1] = image
    for x in range(image.shape[1]):     # Loop over every pixel of the image
        for y in range(image.shape[0]):
            # element-wise multiplication of the kernel and the image
            output[y,x]=(kernel*image_padded[y:y+3,x:x+3]).sum()        

    return output


if __name__ == "__main__":
    # Read image
    impath = os.path.join("images", "lake.jpg")
    im = plt.imread(impath)

    # Define the convolutional kernels
    h_a = np.ones((3, 3)) / 9
    h_b = np.array([
        [1, 4, 6, 4, 1],
        [4, 16, 24, 16, 4],
        [6, 24, 36, 24, 6],
        [4, 16, 24, 16, 4],
        [1, 4, 6, 4, 1],
    ]) / 256
    # Convolve images
    smoothed_im1 = convolve_im(im.copy(), h_a)
    smoothed_im2 = convolve_im(im, h_b)

    # DO NOT CHANGE
    assert isinstance(smoothed_im1, np.ndarray), \
        f"Your convolve function has to return a np.array. " +\
        f"Was: {type(smoothed_im1)}"
    assert smoothed_im1.shape == im.shape, \
        f"Expected smoothed im ({smoothed_im1.shape}" + \
        f"to have same shape as im ({im.shape})"
    assert smoothed_im2.shape == im.shape, \
        f"Expected smoothed im ({smoothed_im1.shape}" + \
        f"to have same shape as im ({im.shape})"

    save_im("convolved_im_h_a.jpg", smoothed_im1)
    save_im("convolved_im_h_b.jpg", smoothed_im2)

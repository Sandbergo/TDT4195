import skimage
import numpy as np
import utils


def MaxPool2d(im: np.array,
              kernel_size: int):
    """ A function that max pools an image with size kernel size.
    Assume that the stride is equal to the kernel size, and that the kernel size is even.

    Args:
        im: [np.array of shape [H, W, 3]]
        kernel_size: integer
    Returns:
        im: [np.array of shape [H/kernel_size, W/kernel_size, 3]].
    """
    stride = kernel_size
    new_im = np.zeros((im.shape[0]//kernel_size, im.shape[1]//kernel_size, im.shape[2]))
    
    for y in range(new_im.shape[0]):
        for x in range(new_im.shape[1]):
            for i in range(new_im.shape[2]):
                new_im[y, x, i] = im[y*stride:(y+1)*stride, x*stride:(x+1)*stride, i].max()

    return new_im
    




if __name__ == "__main__":
    # DO NOT CHANGE
    im = skimage.data.chelsea()
    im = utils.uint8_to_float(im)
    max_pooled_image = MaxPool2d(im, 4)

    utils.save_im("chelsea.png", im)
    utils.save_im("chelsea_maxpooled.png", max_pooled_image)

    im = utils.create_checkerboard()
    im = utils.uint8_to_float(im)
    utils.save_im("checkerboard.png", im)
    max_pooled_image = MaxPool2d(im, 2)
    utils.save_im("checkerboard_maxpooled.png", max_pooled_image)
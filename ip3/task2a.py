import numpy as np
import skimage
import utils
import pathlib


def otsu_thresholding(im: np.ndarray) -> int:
    """
        Otsu's thresholding algorithm that segments an image into 1 or 0 (True or False)
        The function takes in a grayscale image and outputs a boolean image

        args:
            im: np.ndarray of shape (H, W) in the range [0, 255] (dtype=np.uint8)
        return:
            (int) the computed thresholding value
    """
    assert im.dtype == np.uint8
    ### START YOUR CODE HERE ### (You can change anything inside this block) 
    # You can also define other helper functions
    # Compute normalized histogram
    threshold = 128
    num_bins = 256
    histogram, bins = np.histogram(a = im, bins = num_bins)
    total_vals = np.sum(histogram)

    for i in range(1,len(bins)):      
        prob_1 = np.sum(histogram[:i])/total_vals
        prob_2 = np.sum(histogram[i:])/total_vals

        mean_1 = np.sum(histogram[:i] * bins[:i])/np.sum(histogram[:i])
        mean_2 = np.sum(histogram[i:] * bins[i:-1] )/np.sum(histogram[i:])
        
        var_1 = np.sum(((bins[:i] - mean_1)**2 ) * histogram[:i])/np.sum(histogram[:i])
        var_2 = np.sum(((bins[i:-1] - mean_2)**2 ) * histogram[i:])/np.sum(histogram[i:])

        var_tot = (prob_1 * var_1) + (prob_2 * var_2)

        if i == 1:
            var_tot_max = var_tot
            threshold = i

        if var_tot < var_tot_max:
            var_tot_max = var_tot
            threshold = i


    return threshold
    ### END YOUR CODE HERE ### 


if __name__ == "__main__":
    # DO NOT CHANGE
    impaths_to_segment = [
        pathlib.Path("thumbprint.png"),
        pathlib.Path("polymercell.png")
    ]
    for impath in impaths_to_segment:
        im = utils.read_image(impath)
        threshold = otsu_thresholding(im)
        print("Found optimal threshold:", threshold)

        # Segment the image by threshold
        segmented_image = (im >= threshold)
        assert im.shape == segmented_image.shape, \
            "Expected image shape ({}) to be same as thresholded image shape ({})".format(
                im.shape, segmented_image.shape)
        assert segmented_image.dtype == np.bool, \
            "Expected thresholded image dtype to be np.bool. Was: {}".format(
                segmented_image.dtype)

        segmented_image = utils.to_uint8(segmented_image)

        save_path = "{}-segmented.png".format(impath.stem)
        utils.save_im(save_path, segmented_image)



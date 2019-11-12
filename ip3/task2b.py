import utils
import numpy as np

def get8n(x, y, shape):
    out = lambda x_out, y_out : [(x2, y2) for x2 in range(x-1, x+2)
                               for y2 in range(y-1, y+2)
                               if (-1 < x <= shape[0] and
                                   -1 < y <= shape[1] and
                                   (x != x2 or y != y2) and
                                   (0 <= x2 <= shape[0]) and
                                   (0 <= y2 <= shape[1]))]
    print(out)
    return out



def region_growing(im: np.ndarray, seed_points: list, T: int) -> np.ndarray:
    """
        A region growing algorithm that segments an image into 1 or 0 (True or False).
        Finds candidate pixels with a Moore-neighborhood (8-connectedness). 
        Uses pixel intensity thresholding with the threshold T as the homogeneity criteria.
        The function takes in a grayscale image and outputs a boolean image

        args:
            im: np.ndarray of shape (H, W) in the range [0, 255] (dtype=np.uint8)
            seed_points: list of list containing seed points (row, col). Ex:
                [[row1, col1], [row2, col2], ...]
            T: integer value defining the threshold to used for the homogeneity criteria.
        return:
            (np.ndarray) of shape (H, W). dtype=np.bool
    """
    ### START YOUR CODE HERE ### (You can change anything inside this block)
    # You can also define other helper functions

    segmented = np.zeros_like(im).astype(bool)
    x_max, y_max  = segmented.shape

    # https://stackoverflow.com/questions/1620940/determining-neighbours-of-cell-two-dimensional-list
    moore = lambda x, y : [(x_n, y_n) for x_n in range(x-1, x+2)
                                for y_n in range(y-1, y+2)
                                if (-1 < x < x_max and
                                    -1 < y < y_max and
                                    (x != x_n or y != y_n) and
                                    (0 <= x_n < x_max) and
                                    (0 <= y_n < y_max))]
    

    processed = []
    while(len(seed_points)> 0):
        (row, col) = seed_points[0]
        segmented[row, col] = True 
      
        neighbours = moore(row, col)
        for row_n, col_n in neighbours:
            if im[row_n, col_n] > (np.max(im)-T):
                segmented[row_n, col_n] = True
                if not (row_n, col_n) in processed:
                    seed_points.append((row_n, col_n))
                processed.append((row_n, col_n))
        seed_points.pop(0)
    return segmented

    ### END YOUR CODE HERE ### 



if __name__ == "__main__":
    # DO NOT CHANGE
    im = utils.read_image("defective-weld.png")

    seed_points = [ # (row, column)
        [254, 138], # Seed point 1
        [253, 296], # Seed point 2
        [233, 436], # Seed point 3
        [232, 417], # Seed point 4
    ]


    intensity_threshold = 50
    segmented_image = region_growing(im, seed_points, intensity_threshold)

    assert im.shape == segmented_image.shape, \
        "Expected image shape ({}) to be same as thresholded image shape ({})".format(
            im.shape, segmented_image.shape)
    assert segmented_image.dtype == np.bool, \
        "Expected thresholded image dtype to be np.bool. Was: {}".format(
            segmented_image.dtype)

    segmented_image = utils.to_uint8(segmented_image)
    utils.save_im("defective-weld-segmented.png", segmented_image)


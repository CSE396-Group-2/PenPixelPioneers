import numpy as np
import cv2

def order_coordinates(pts):
    rectangle = np.zeros((4, 2), dtype = "float32")
    s = pts.sum(axis = 1)
    rectangle[0] = pts[np.argmin(s)]
    rectangle[2] = pts[np.argmax(s)]
    difference = np.diff(pts, axis = 1)
    rectangle[1] = pts[np.argmin(difference)]
    rectangle[3] = pts[np.argmax(difference)]
    return rectangle


def point_transform(image, pts):
    rect = order_coordinates(pts)
    (upper_left, upper_right, bottom_right, bottom_left) = rect
    width1 = np.sqrt(((bottom_right[0] - bottom_left[0]) ** 2) + ((bottom_right[1] - bottom_left[1]) ** 2))
    width2 = np.sqrt(((upper_right[0] - upper_left[0]) ** 2) +((upper_right[1] - upper_left[1]) ** 2))
    Width = max(int(width1), int(width2)) #considers maximum width value as Width
    height1 = np.sqrt(((upper_right[0] - bottom_right[0]) ** 2) +((upper_right[1] - bottom_right[1]) ** 2))
    height2 = np.sqrt(((upper_left[0] - bottom_left[0]) ** 2) + ((upper_left[1] - bottom_left[1]) ** 2))
    Height = max(int(height1), int(height2)) #considers maximum height value as Height
    distance = np.array([[0, 0],[Width - 1, 0],[Width - 1, Height - 1],[0,Height - 1]], dtype ="float32")
    Matrix = cv2.getPerspectiveTransform(rect, distance)
    warped_image = cv2.warpPerspective(image, Matrix, (Width, Height))

    return warped_image

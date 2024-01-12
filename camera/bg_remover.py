import cv2
import imutils
from skimage.filters import threshold_local
from transform import point_transform
import sys
import os

def scan_img(a, b):
    # Loading and displaying the original image
    original_img = cv2.imread(a)

    copy = original_img.copy()
    cv2.waitKey(1)
    ratio = original_img.shape[0] / 500.0
    img_resize = imutils.resize(original_img, height=500)

    gray_image = cv2.cvtColor(img_resize, cv2.COLOR_BGR2GRAY)

    blurred_image = cv2.GaussianBlur(gray_image, (3, 3), 0)

    edged_img = cv2.Canny(blurred_image, 50, 150)

    cnts, _ = cv2.findContours(edged_img, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    cnts = sorted(cnts, key=cv2.contourArea, reverse=True)[:5]
    doc = None
    for c in cnts:
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.02 * peri, True)
        if len(approx) == 4:
            doc = approx
            break

    p = []
    try:
        if doc is None:
            raise ValueError("Test image couldn't scanned.")

        warped_image = point_transform(copy, doc.reshape(4, 2) * ratio)
        warped_image = cv2.cvtColor(warped_image, cv2.COLOR_BGR2GRAY)

        T = threshold_local(warped_image, 41, offset=20, method="gaussian")
        warped = (warped_image > T).astype("uint8") * 255
        cv2.imwrite(b,warped)
        cv2.destroyAllWindows()
        print("True")
        return True
    except ValueError as e:
        print("False")
        return False

if __name__ == "__main__":
    a = sys.argv[1]
    b = sys.argv[2]
    a = a.replace("\\", "/")
    b = b.replace("\\", "/")
    current_directory = os.getcwd()
    scan_img(a, b)


import cv2
from cvzone.HandTrackingModule import HandDetector

cap = cv2.VideoCapture(0)
detector = HandDetector(detectionCon=0.8)

while True:
    success, img = cap.read()
    hands, img = detector.findHands(img)
    img = cv2.flip(img, 1)  # 水平翻转
    cv2.imshow("Hands", img)
    cv2.waitKey(1)

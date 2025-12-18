import cv2
import numpy as np

cap = cv2.VideoCapture("torcs_drive.mp4")

if not cap.isOpened():
    print("❌ Cannot open video")
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        break

    h, w, _ = frame.shape
    roi = frame[int(h*0.6):h, :]  # bottom region (road area)

    # ======================
    # 1️⃣ GRASS / SAND DETECTION
    # ======================
    hsv = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)

    # Green (grass)
    lower_green = np.array([35, 40, 40])
    upper_green = np.array([85, 255, 255])
    green_mask = cv2.inRange(hsv, lower_green, upper_green)

    grass_ratio = cv2.countNonZero(green_mask) / (roi.shape[0] * roi.shape[1])

    off_road = grass_ratio > 0.20  # threshold

    # ======================
    # 2️⃣ EDGE DETECTION
    # ======================
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    edges = cv2.Canny(blur, 50, 150)

    contours, _ = cv2.findContours(
        edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
    )

    decision = "FORWARD"

    # ======================
    # 3️⃣ OBSTACLE + FENCE DETECTION
    # ======================
    for cnt in contours:
        area = cv2.contourArea(cnt)

        if area > 3000:
            x, y, bw, bh = cv2.boundingRect(cnt)
            cx = x + bw // 2

            # Focus on forward view
            if cx < w * 0.2 or cx > w * 0.8:
                continue

            cv2.rectangle(frame, (x, y), (x + bw, y + bh), (0, 255, 0), 2)

            # Fence = tall vertical object
            if bh > bw * 2:
                decision = "STOP"
                break

            # Distance estimation
            if bh > 180:
                decision = "STOP"
            elif bh > 100:
                decision = "SLOW"
            else:
                decision = "FORWARD"

            break

    # ======================
    # 4️⃣ OFF-ROAD OVERRIDE
    # ======================
    if off_road:
        decision = "STOP"

    # ======================
    # VISUAL FEEDBACK
    # ======================
    cv2.rectangle(frame, (0, int(h*0.6)), (w, h), (255, 0, 0), 2)
    cv2.putText(frame, "Road ROI", (10, int(h*0.6)-10),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255,0,0), 2)

    cv2.putText(frame, f"Decision: {decision}", (20, 40),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)

    cv2.imshow("TORCS Lane Assist + Obstacle Detection", frame)

    if cv2.waitKey(30) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

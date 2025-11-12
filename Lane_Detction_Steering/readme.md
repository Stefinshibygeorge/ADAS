# 🚗 Lane Detection and Steering Simulation

This project implements **lane detection** and **steering angle visualization** from a road video feed using **computer vision techniques**. It simulates how an autonomous vehicle detects lanes and adjusts its steering dynamically.

---

## 🎯 Objective

To detect road lane markings in a live or recorded driving video, estimate the deviation from the lane center, and visualize the steering correction using a rotating steering wheel image.

---

## ⚙️ System Overview

The system captures video frames from a driving video (`Final_vdo.mp4`), applies **perspective transformation** to create a **bird’s-eye view**, isolates lane colors using **HSV thresholding**, and detects the lane boundaries through **histogram and sliding window techniques**.  
The deviation of the vehicle from the lane center is then calculated, and the steering correction is displayed by rotating a **steering wheel image** (`steering_wheel.png`).

---

## 🧠 Algorithms and Techniques Used

### 1. **Perspective Transformation**
- Converts the camera’s front view into a **top-down (bird’s-eye) perspective**.  
- Helps in parallelizing the lane lines, simplifying further analysis.  
- Implemented using a 3×3 transformation matrix obtained from four manually chosen source points (road ROI).

### 2. **Color Space Conversion and Thresholding**
- Input frames are converted from **BGR → HSV color space**.
- Lane colors (white and yellow) are extracted using carefully tuned HSV thresholds:
  - **White lanes:** low saturation, high value
  - **Yellow lanes:** high saturation and medium-high value range

### 3. **Histogram-Based Lane Base Detection**
- The binary thresholded mask is split horizontally.
- A histogram of pixel intensities is generated to find the **starting base points** of left and right lanes.
- These base points initialize the sliding windows.

### 4. **Sliding Window Search**
- The image is divided into horizontal slices.
- For each slice, **connected contours** of lane pixels are identified.
- Centroids of these contours are used to update the next window position dynamically.
- Produces a continuous path of lane pixels across the image.

### 5. **Lane Center and Deviation Estimation**
- Once left and right lane points are identified, the **lane midpoint** is computed.
- The **vehicle center** is assumed at the image midpoint.
- The deviation from the lane center gives the offset:
  \[
  \text{Deviation} = (\text{Lane Center}) - (\text{Frame Center})
  \]

### 6. **Steering Angle Visualization**
- A proportional control approach is used to convert deviation into a **steering angle**.  
- The **steering wheel image** is rotated by the corresponding angle using OpenCV’s affine transformation.  
- The angle magnitude and direction are displayed visually in real time.

---

## ⚙️ Technical Details

| Parameter | Description |
|------------|--------------|
| Programming Language | Python |
| Libraries Used | OpenCV, NumPy |
| Input | Road video (`Final_vdo.mp4`) |
| Output | Multiple OpenCV display windows showing each stage |
| Processing Resolution | 640×480 |
| Transformation Points | Manually selected ROI corners (tl, bl, tr, br) |
| Lane Colors Detected | White and Yellow |
| Steering Visualization | Rotating PNG image of a steering wheel |

---

## 🧪 Results and Observations

### 1. **Lane Detection Output**
- The system successfully detects both **white** and **yellow** lanes on standard road footage.  
- The **bird’s-eye transformed frame** clearly displays parallel lanes.  
- Sliding window rectangles dynamically follow the curvature of the road.

### 2. **Steering Response**
- The **steering wheel image** rotates left or right based on the detected deviation.  
- Small deviations yield minor steering angles, while large deviations result in noticeable rotations.  
- The response is smooth and real-time at video frame rates.

### 3. **Performance**
- Average frame processing time: ~25–30 FPS on a standard CPU (Intel i5).  
- Works efficiently on well-lit and moderately curved roads.  
- Accuracy reduces slightly in dim lighting or with faded lane markings.

---

## 📸 Visual Results

**Detected Lane & Bird’s Eye View:**

![Lane Detection Result](results_image.jpg)

**Video Demonstration:**

[🎥 Watch the Output Video](results_video.mp4)

*(Replace the file names above with your actual image and video paths)*

---

## 🚀 Future Improvements

- Integration of **Canny edge detection** for lane refinement.  
- Use of **Kalman filter** for temporal smoothing of detected lanes.  
- Real-time implementation on embedded hardware (e.g., Raspberry Pi).  
- Incorporation of **CNN-based lane segmentation models** for robustness.

---

## 👨‍💻 Developed By

**Stefin Shiby George**  
M.Tech – Signal Processing, NIT Calicut  
Focus Areas: Deep Learning, Computer Vision, and Intelligent Systems


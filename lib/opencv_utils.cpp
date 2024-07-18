#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <time.h>
#include "opencv_utils.h"

using namespace cv;

static CascadeClassifier face_cascade;
static time_t last_face_detect_time;

int load_face_cascade(const char* cascade_path) {
    printf("Loading face cascade from path: %s\n", cascade_path);
    if (!face_cascade.load(cascade_path)) {
        fprintf(stderr, "Error loading face cascade\n");
        return -1;
    }
    last_face_detect_time = 0;
    printf("Face cascade loaded successfully\n");
    return 0;
}

void detect_and_draw_faces(void* frame_data, int width, int height) {
    Mat frame(height, width, CV_8UC3, frame_data);
    std::vector<Rect> faces;
    Mat frame_gray;

    time_t now = time(NULL);
    if (difftime(now, last_face_detect_time) >= 1.0) {
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        equalizeHist(frame_gray, frame_gray);

        face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(20, 20));

        //printf("Number of faces detected: %lu\n", faces.size());
        for (size_t i = 0; i < faces.size(); i++) {
            Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
            ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 2);
        }
        last_face_detect_time = now;
    }

    // Add timestamp
    char timestamp[100];
    struct tm* tm_info;
    tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    putText(frame, timestamp, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);

    // Copy the modified frame back to the buffer
    memcpy(frame_data, frame.data, frame.total() * frame.elemSize());
}

void rotate_frame_90_clockwise(void* frame_data, int width, int height) {
    Mat frame(height, width, CV_8UC3, frame_data);
    Mat rotated_frame;
    rotate(frame, rotated_frame, ROTATE_90_CLOCKWISE);
    memcpy(frame_data, rotated_frame.data, rotated_frame.total() * rotated_frame.elemSize());
}

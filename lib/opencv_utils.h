#ifndef OPENCV_UTILS_H
#define OPENCV_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

int load_face_cascade(const char* cascade_path);
void detect_and_draw_faces(void* frame_data, int width, int height);
void rotate_frame_90_clockwise(void* frame_data, int width, int height);
void mat_to_mjpeg_buffer(void* frame_data, int width, int height, void* jpg_buffer_data);

#ifdef __cplusplus
}
#endif

#endif // OPENCV_UTILS_H

#ifndef DET_AND_TRACK_H
#define DET_AND_TRACK_H

#include <opencv2/core/utility.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/tracking.hpp> 
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class DetAndTrack
{
    bool get_new_detection_;
    std::vector<cv::Rect> det_boxes_;
    std::vector<cv::Rect> track_boxes_;
    std::vector<cv::Rect> final_boxes_;

    cv::Mat current_frame_;
    cv::Mat last_frame_;

    cv::CascadeClassifier face_cascade_;
    std::vector<cv::Ptr<cv::Tracker>> trackers_;

    int detection_sleep_time_; //milliseconds
    int track_sleep_time_;

    public:
        std::mutex mutex_;
        DetAndTrack();
        DetAndTrack(int _detection_sleep_time, int _track_sleep_time);
        void detectionTask();
        void trackTask();
        
        std::vector<cv::Rect> getBox()
        {
            return final_boxes_;
        }
        void setFrame(const cv::Mat& _new_frame);
};

#endif

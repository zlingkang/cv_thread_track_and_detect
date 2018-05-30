#include "det_and_track.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

DetAndTrack::DetAndTrack():
    get_new_detection_(false)
{
    face_cascade_.load("../haarcascade_frontalface_alt.xml");
    detection_sleep_time_ = 300;
    track_sleep_time_ = 30;
}

DetAndTrack::DetAndTrack(int _detection_sleep_time, int _track_sleep_time):
    get_new_detection_(false),
    detection_sleep_time_(_detection_sleep_time),
    track_sleep_time_(_track_sleep_time)
{
    face_cascade_.load("../haarcascade_frontalface_alt.xml");
}

void DetAndTrack::detectionTask()
{
    while(1)
    {
        mutex_.lock();
        cv::Mat local_frame = current_frame_.clone();
        mutex_.unlock();
       
        std::vector<cv::Rect> local_boxes;
        face_cascade_.detectMultiScale(local_frame, local_boxes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30)); 
        std::this_thread::sleep_for(std::chrono::milliseconds(detection_sleep_time_));
        
        std::lock_guard<std::mutex> lockGuard(mutex_);
        det_boxes_ = local_boxes;
        final_boxes_ = local_boxes;
        get_new_detection_ = true;
    }
}

void DetAndTrack::trackTask()
{
    while(1)
    {
        mutex_.lock();
        cv::Mat local_frame = current_frame_.clone();
        mutex_.unlock();

        if(get_new_detection_)
        {
            std::vector<cv::Ptr<cv::Tracker>> local_trackers;
            for(auto box : det_boxes_)
            {
                cv::Ptr<cv::Tracker> tracker = cv::Tracker::create("MEDIANFLOW");
                tracker->init(local_frame, box);
                local_trackers.push_back(tracker);
            }

            std::lock_guard<std::mutex> lockguard(mutex_);
            trackers_.clear();
            trackers_ = local_trackers;
            get_new_detection_ = false;
        }
        else
        {
            track_boxes_.clear();
            for(auto tracker : trackers_)
            {
                cv::Rect2d box;
                tracker->update(local_frame, box);
                track_boxes_.push_back(box);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(track_sleep_time_));
            
            std::lock_guard<std::mutex> lockguard(mutex_);
            final_boxes_ = track_boxes_;
        }
    }
}

void DetAndTrack::setFrame(const cv::Mat& _new_frame)
{
    current_frame_ = _new_frame.clone(); 
}

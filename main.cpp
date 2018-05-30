#include "det_and_track.h"
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main( int argc, char** argv )
{
    DetAndTrack dtObject;

    cv::VideoCapture cap(0);
    cv::Mat frame;
    cap >> frame;

    dtObject.setFrame(frame);
    
    std::thread detThread(&DetAndTrack::detectionTask, &dtObject);
    std::thread trackThread(&DetAndTrack::trackTask, &dtObject);

    while(true)
    {
        cap >> frame;
        //uncomment the next line when reading from a video
        //std::this_thread::sleep_for(std::chrono::milliseconds(30));
        std::vector<cv::Rect> boxes;
        
        dtObject.mutex_.lock();
        boxes = dtObject.getBox();
        dtObject.setFrame(frame);
        dtObject.mutex_.unlock();

        for(auto box:boxes)
        {
            cv::rectangle(frame, box, cv::Scalar(255, 0, 0), 2, 1);
        }
        cv::imshow("detandtrack", frame);
        if(cv::waitKey(1) == 27)
        {
            break;
        }
    }
    
    detThread.join();
    trackThread.join();
    return 0;
}

# cv_thread_track_and_detect
Use OpenCV face detection and tracking in multiple threads

# Dependencies
OpenCV3 and OpenCV3 contrib

# Usage
* change the OpenCV path in CMakeLists.txt
* [optional] change detection_sleep_time and track_sleep_time, the default is 300 and 30 milliseconds for each. Longer sleep time lowers the CPU load but makes it less robus.

# Known problems and possible solutions
The trackers used here are simply naively calling median flow trackers in OpenCV3 contrib, on ARM cpu platforms (such as Odroid XU4 I am using), they are pretty slow.
On Odroid XU4, it takes ~20ms for a single median flow tracker, and it takes more time with more trackers, which means 10 trackers would be 200ms in total and makes it 
basically unusable.  
Since we have detection results every ~150ms(if using Odroid XU4 for face detection), we actually do not need very accurate long-term trackers as those built in the OpenCV3 contrib. 
Thus I am considering building a faster tracker solution for short-term use cases. And it should not take more time with more trackers linearly.
So I build my own Optical flow based short-term trackers in another repo(TBD), which is still in an experimental stage.

#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>

int main()
{
    std::cout << cv::getBuildInformation();
    cv::VideoCapture vid("rtsp://admin:!CzMmAk20@192.168.2.232:554/Streaming/Channels/1/");

    // For mac, need to wait for auth, so uncomment this
    /*
    std::cout << "Checking for camera auth...";
    while (!vid.isOpened())
    {
        std::cout << "\n.";
        sleep(1);
        vid.open(0);
    }
    */

    // Check if camera opened successfully
    if (!vid.isOpened())
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    while (1)
    {
        // Capture the video frame
        // by frame
        cv::Mat frame;
        vid >> frame;

        // Display the resulting frame
        cv::imshow("frame", frame);

        // the 'q' button is set as the
        // quitting button you may use any
        // desired button of your choice
        char c = (char)cv::waitKey(1);
        if (c == 27)
        {
            break;
        }
    }

    // After the loop release the cap object
    vid.release();
    // Destroy all the windows
    cv::destroyAllWindows();

    return 0;
}

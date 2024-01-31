#include <iostream>
#include <string>
#include <map>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <thread>

struct RTSPVideoStreamConfig
{
    std::string src;
    bool hasGi;
    int width;
    int height;
    int camId;
    std::string camName;

    bool isUrl() const
    {
        return src.compare(0, 7, "rtsp://") == 0 ||
               src.compare(0, 7, "rtmp://") == 0 ||
               src.compare(0, 7, "http://") == 0 ||
               src.compare(0, 8, "https://") == 0;
    }
};

class RTSPVideoStream
{
private:
    cv::VideoCapture stream;
    std::mutex readLock;
    cv::Mat lastFrame;

    std::thread captureThread;
    std::atomic<bool> stopFlag;

    RTSPVideoStreamConfig config;
    int fps;
    bool started;

    void _reconnect(int retryCount, int maxRetryCount);
    void setLastFrame(cv::Mat);

public:
    RTSPVideoStream();
    RTSPVideoStream(std::string src, bool has_gi, int width, int height, int cam_id, std::string cam_name);
    RTSPVideoStream(RTSPVideoStreamConfig config);
    RTSPVideoStream(std::map<std::string, std::string> config);
    ~RTSPVideoStream();

    void initializeCapture();
    void reconnect(int maxRetryCount = 10);
    void update();
    void grabFrame();
    cv::Mat getLastFrame();
    void start();
    void stop();
};
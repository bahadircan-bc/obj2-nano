#include "readMultipleCam.h"
using namespace std;

RTSPVideoStream::RTSPVideoStream()
{
    cout << "RTSPVideoStream constructor" << endl;
}

RTSPVideoStream::RTSPVideoStream(string src, bool has_gi, int width, int height, int cam_id, string cam_name)
{
    cout << "RTSPVideoStream constructor" << endl;
    this->config.src = src;
    this->config.hasGi = has_gi;
    this->config.width = width;
    this->config.height = height;
    this->config.camId = cam_id;
    this->config.camName = cam_name;
    this->initializeCapture();
}

RTSPVideoStream::RTSPVideoStream(RTSPVideoStreamConfig config)
{
    cout << "RTSPVideoStream constructor" << endl;
    this->config = config;
    this->initializeCapture();
}

RTSPVideoStream::RTSPVideoStream(map<string, string> config)
{
    cout << "RTSPVideoStream constructor" << endl;
    this->config.src = config["src"];
    this->config.hasGi = config["has_gi"] == "True";
    this->config.width = stoi(config["width"]);
    this->config.height = stoi(config["height"]);
    this->config.camId = stoi(config["cam_id"]);
    this->config.camName = config["cam_name"];
    this->initializeCapture();
}

RTSPVideoStream::~RTSPVideoStream()
{
    cout << "RTSPVideoStream destructor" << endl;
}

void RTSPVideoStream::initializeCapture()
{
    this->stream = cv::VideoCapture(this->config.src, this->config.hasGi ? cv::CAP_GSTREAMER : 0);
    this->fps = int(this->stream.get(cv::CAP_PROP_FPS));
    this->lastFrame = cv::Mat(cv::Size(this->config.width, this->config.height), CV_8UC3, cv::Scalar(0, 0, 0));
    if (!this->stream.isOpened())
    {
        cout << "Failed to open " << this->config.src << endl;
        this->reconnect();
    }
}

void RTSPVideoStream::_reconnect(int retryCount, int maxRetryCount)
{
    if (retryCount > maxRetryCount)
    {
        cout << "Failed to reconnect to " << this->config.src << endl;
        return;
    }
    cout << retryCount << " - Reconnecting to " << this->config.src << endl;
    this->stream.release();
    this->initializeCapture();
    if (!this->stream.isOpened())
    {
        sleep(5);
        this->_reconnect(retryCount + 1, maxRetryCount);
    }
}

/*

ping cams 554 port
ping google to check internet connection
abort

*/

void RTSPVideoStream::reconnect(int maxRetryCount)
{
    this->_reconnect(0, maxRetryCount);
}

void RTSPVideoStream::update()
{
    while (!this->stopFlag)
    {
        this->grabFrame();
    }
}

void RTSPVideoStream::grabFrame()
{
    cv::Mat frame;
    bool grabbed = this->stream.read(frame);
    if (!grabbed)
    {
        this->lastFrame = cv::Mat(cv::Size(this->config.width, this->config.height), CV_8UC3, cv::Scalar(0, 0, 0));
        auto now = chrono::system_clock::now();
        cout << "DATE:" << chrono::system_clock::to_time_t(now) << endl
             << "Failed to grab frame from " << this->config.src << endl;
        this->reconnect(10);
    }
    frame.copyTo(this->lastFrame);
}

void RTSPVideoStream::setLastFrame(cv::Mat frame)
{
    std::lock_guard<std::mutex> lock(this->readLock);
    frame.copyTo(this->lastFrame);
}

cv::Mat RTSPVideoStream::getLastFrame()
{
    std::lock_guard<std::mutex> lock(this->readLock);
    return this->lastFrame;
}

void RTSPVideoStream::start()
{
    this->captureThread = std::thread(&RTSPVideoStream::update, this);
}

void RTSPVideoStream::stop()
{
    this->stopFlag = true;
    if (this->captureThread.joinable())
    {
        this->captureThread.join();
    }
}

int main()
{
    RTSPVideoStreamConfig config;
    config.src = "rtsp://admin:!CzMmAk20@192.168.2.232:554/Streaming/Channels/1/";
    config.camId = 1;
    config.hasGi = false;
    config.height = 680;
    config.width = 1024;
    config.camName = "cam1";

    auto cap = new RTSPVideoStream(config);
    cap->start();

    // RTSPVideoStreamConfig config2;
    // config.src = "rtsp://admin:!CzMmAk20@192.168.2.234:554/Streaming/Channels/1/";
    // config.camId = 2;
    // config.hasGi = false;
    // config.height = 680;
    // config.width = 1024;
    // config.camName = "cam2";

    // auto cap2 = new RTSPVideoStream(config2);
    // cap2->start();



    while (true)
    {
        auto frame = cap->getLastFrame();
        // auto frame2 = cap2->getLastFrame();
        cv::imshow("frame", frame);
        // cv::imshow("frame2", frame2);

        char c = cv::waitKey(1);

        if (c == 27)
        {
            break;
        }
    }

    cap->stop();
    return 0;
}
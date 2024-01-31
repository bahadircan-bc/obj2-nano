#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>        
#include "src/simpleini-4.22/SimpleIni.h"

int main() {
    std::cout << "reading from ini" << std::endl;
    CSimpleIniA ini;
    SI_Error rc = ini.LoadFile("8Cam_hakmar_zone1.ini");
    if (rc < 0) { /* handle error */ };
    const char* pv;
    pv = ini.GetValue("CAMERA", "fps");
    std::cout << pv << std::endl;
    return 0;
}
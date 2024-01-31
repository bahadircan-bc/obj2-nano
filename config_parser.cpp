#include <iostream>
#include <string>
#include "src/simpleini-4.22/SimpleIni.h"

using namespace std;

class CameraConfig
{
public:
    int height;
    int width;
    int fps;

    CameraConfig(const char *iniDir)
    {
        CSimpleIniA ini;
        if (ini.LoadFile(iniDir) < 0)
        {
            throw "Error loading ini file";
        }

        const char *pv;
        pv = ini.GetValue("CAMERA", "cam_width");
        width = stoi(pv);

        pv = ini.GetValue("CAMERA", "cam_height");
        height = stoi(pv);

        pv = ini.GetValue("CAMERA", "fps");
        fps = stoi(pv);
    }
};

class SourcesConfig
{
public:
    int numSources;
    string *links;

    int *ids;
    string *camNames;
    bool *isGi;
    int *widths;
    int *heights;

    SourcesConfig(const char *iniDir)
    {
        CSimpleIniA ini;
        if (ini.LoadFile(iniDir) < 0)
        {
            throw "Error loading ini file";
        }

        string pv;
        pv = ini.GetValue("SOURCES", "number_of_sources");
        numSources = stoi(pv);

        links = new string[numSources];
        pv = ini.GetValue("SOURCES", "links");
        for (int i = 0; i < numSources; i++)
        {
            links[i] = pv.substr(0, pv.find("~"));
            pv = pv.substr(pv.find("~") + 1);
        }

        ids = new int[numSources];
        pv = ini.GetValue("SOURCES", "id");
        for (int i = 0; i < numSources; i++)
        {
            ids[i] = stoi(pv.substr(0, pv.find("~")));
            pv = pv.substr(pv.find("~") + 1);
        }

        camNames = new string[numSources];
        pv = ini.GetValue("SOURCES", "cam_name");
        for (int i = 0; i < numSources; i++)
        {
            camNames[i] = pv.substr(0, pv.find("~"));
            pv = pv.substr(pv.find("~") + 1);
        }
    }
};

class TillerConfig
{
public:
    int screenWidth;
    int screenHeight;

    TillerConfig(const char *iniDir)
    {
        CSimpleIniA ini;
        if (ini.LoadFile(iniDir) < 0)
        {
            throw "Error loading ini file";
        }

        const char *pv;
        pv = ini.GetValue("TILLERSETTINGS", "screen_width");
        screenWidth = stoi(pv);

        pv = ini.GetValue("TILLERSETTINGS", "screen_height");
        screenHeight = stoi(pv);
    }
};

int main()
{
    CameraConfig camConfig("8Cam_hakmar_zone1.ini");
    cout << camConfig.height << endl;
    cout << camConfig.width << endl;
    cout << camConfig.fps << endl;

    SourcesConfig srcConfig("8Cam_hakmar_zone1.ini");
    cout << srcConfig.numSources << endl;
    for (int i = 0; i < srcConfig.numSources; i++)
    {
        cout << srcConfig.links[i] << endl;
        cout << srcConfig.ids[i] << endl;
        cout << srcConfig.camNames[i] << endl;
    }

    TillerConfig tillerConfig("8Cam_hakmar_zone1.ini");
    cout << tillerConfig.screenWidth << endl;
    cout << tillerConfig.screenHeight << endl;

    return 0;
}

//todo refactor config with subclasses etc.
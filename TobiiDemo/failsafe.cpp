#include "tobii/tobii.h"
#include "tobii/tobii_streams.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>


using namespace std;


ofstream eyeStream;

int turnPageSig = 0;
double counter = 0;
int iteration = 0;

double lastXPos = -1.0;
double lastYPos = -1.0;
double limit = 0;
int tempo = 120;

void gaze_point_callback(tobii_gaze_point_t const* gaze_point, void* /* user_data */)
{
    // Check that the data is valid before using it

    double xPos = gaze_point->position_xy[0] * 1000;
    double yPos = gaze_point->position_xy[1] * 1000;
    //override turn right button gaze on Rohan's computer numbers
    if (xPos >= 330 && xPos <=500 && yPos >= 200 && yPos<=280) {
        turnPageSig = 1;
    }
                          //override turn left button gaze on Rohan's computer numbers
    else if (xPos >= 20 && xPos <= 50 && yPos >= 240 && yPos <= 280) {
        turnPageSig = 2;
    }
    else {
        turnPageSig = 0;
    }

    //looking at last 2 measures
    /*if (xPos >= 620 && yPos >= 850) {
        if (lastYPos >= 620 && yPos >= 850) {
            if (counter < limit) {
                counter++;
                    
            }
                    
            else {
                turnPageSig = 1;
                counter = 0;
             }
        }
        else {
            counter = 0;
            turnPageSig = 0;
        }
    }
    else {
        turnPageSig = 0;
    }*/


    if (gaze_point->validity == TOBII_VALIDITY_VALID) {

        //printf("Gaze point: %f, %f, %\n",
          //  gaze_point->position_xy[0],
           // gaze_point->position_xy[1]);

        //printf("Gaze point: %f, %f, %d \n", xPos, yPos, turnPageSig);
        cout << turnPageSig << endl;
        //printf("\ntempo: hehereushweu %d\n", tempo);
        lastXPos = xPos;
        lastYPos = yPos;
        // if((gaze_point->position_xy[0]*1000))
    }


    if (eyeStream.is_open()) {
        //eyeStream << gaze_point->position_xy[0] << "," << gaze_point->position_xy[1] << "\n";
        eyeStream << "Sample# " << iteration << " :" << xPos << "," << yPos << ", turn page: "
            << turnPageSig << "\n";
        iteration++;
    }

}

void url_receiver(char const* url, void* user_data)
{
    char* buffer = (char*)user_data;
    if (*buffer != '\0') return; // only keep first value

    if (strlen(url) < 256)
        strcpy_s(buffer, 256, url);
}

// Tempo 120
int main()
{
    // Create API
    cout << "Code Start..." << endl;

    tobii_api_t* api = NULL;
    tobii_error_t result = tobii_api_create(&api, NULL, NULL);
    assert(result == TOBII_ERROR_NO_ERROR);
    tobii_version_t version;
    tobii_error_t version_error = tobii_get_api_version(&version);
    if (version_error == TOBII_ERROR_NO_ERROR)
        printf("Current API version: %d.%d.%d\n", version.major, version.minor,
            version.revision);


    // Enumerate devices to find connected eye trackers, keep the first
    char url[256] = { 0 };
    result = tobii_enumerate_local_device_urls(api, url_receiver, url);
    assert(result == TOBII_ERROR_NO_ERROR);
    if (*url == '\0')
    {
        //printf("Error: No device found\n");
        cout << "Error: No Device Found" << endl;
        return 1;
    }

    // Connect to the first tracker found
    tobii_device_t* device = NULL;
    result = tobii_device_create(api, url, TOBII_FIELD_OF_USE_INTERACTIVE, &device);
    assert(result == TOBII_ERROR_NO_ERROR);

    // Subscribe to gaze data
    result = tobii_gaze_point_subscribe(device, gaze_point_callback, 0);
    assert(result == TOBII_ERROR_NO_ERROR);



    //open file
    eyeStream.open("eyeStream_sightread3.txt");

    //while (true) {

    for (int i = 0; i < 100000; i++)
    {
        // Optionally block this thread until data is available. Especially useful if running in a separate thread.
        result = tobii_wait_for_callbacks(1, &device);
        assert(result == TOBII_ERROR_NO_ERROR || result == TOBII_ERROR_TIMED_OUT);

        // Process callbacks on this thread if data is available
        result = tobii_device_process_callbacks(device);
        assert(result == TOBII_ERROR_NO_ERROR);
        if (GetKeyState('K') & 0x8000) {
            eyeStream << "Start" << endl;

        }

        if (GetKeyState('L') & 0x8000) {
            eyeStream << "end " << endl;

        }

        if (GetKeyState(VK_RIGHT)) {
            eyeStream << "right page turn" << endl;
            cout << "right page turn" << endl;
        }

        if (GetKeyState(VK_LEFT)) {
            eyeStream << "left page turn" << endl;
            cout << "left page turn" << endl;
        }

        if (GetKeyState(VK_RIGHT) & 0x8000) {
            eyeStream << "right page turn" << endl;
            cout << "right page turn" << endl;
        }

        if (GetKeyState(VK_LEFT) & 0x8000) {
            eyeStream << "left page turn" << endl;
            cout << "left page turn" << endl;
        }




    }



    //}




    //close file
    eyeStream.close();

    // Cleanup
    result = tobii_gaze_point_unsubscribe(device);
    assert(result == TOBII_ERROR_NO_ERROR);
    result = tobii_device_destroy(device);
    assert(result == TOBII_ERROR_NO_ERROR);
    result = tobii_api_destroy(api);
    assert(result == TOBII_ERROR_NO_ERROR);
    return 0;
}
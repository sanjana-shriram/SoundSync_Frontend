#include "tobii/tobii.h"
#include "tobii/tobii_streams.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include <iomanip>
#include <ctime>
#include <thread>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;



ofstream eyeStream;

int turnPageSig = 0;
double counter = 0;
int iteration = 0;

double lastXPos = -1.0;
double lastYPos = -1.0;
double limit;
int tempo = 120;

double xBar0 = 100.0;
double xBar1 = 143.0;
double xBar2 = 215.0;
double xBar3 = 275.0;
double xEndBar = 335.0;

//(+/- 20)
double yLine0 = 320;
double yLine1 = 385;
double yLine2 = 465;
double yLine3 = 525;
double yLine4 = 600;
double yLine5 = 658;
double yLine6 = 740;
double yLine7 = 804;
bool startNow = false;
std::chrono::time_point<std::chrono::system_clock> start, ending, duration;
std::chrono::duration<double> elapsed_seconds;
char tmBuff[40];
bool nextPage = false;
bool seenLine0 = false;
bool seenLine1 = false;
bool seenLine2 = false;
bool seenLine3 = false;
bool seenLine4 = false;
bool seenLine5 = false;
bool seenLine6 = false;
bool seenLine7 = false;



void gaze_point_callback(tobii_gaze_point_t const* gaze_point, void* /* user_data */)
{
    // Check that the data is valid before using it

    double xPos = gaze_point->position_xy[0] * 1000;
    double yPos = gaze_point->position_xy[1] * 1000;
    //override turn right button gaze on Rohan's computer numbers
    if (360 <= xPos && xPos <= 460 && 250 <= yPos && yPos<=360) {
        turnPageSig = 1;
    }
                          //override turn left button gaze on Rohan's computer numbers
    else if (10 <= xPos && xPos <= 70 && 250 <= yPos && yPos <= 360) {
        turnPageSig = 2;
    }
         //looking at last 2 measures
    else if (xPos >= 90 && yPos >= 880) {

        if (counter < limit) {
            counter++;

        }

        else {
            turnPageSig = 1;
            // counter = 0;
        }

    } 
    else {
        counter = 0;
        turnPageSig = 0;
    }


    //linear interpolation
    /*
    if (xBar0 - 10 <= xPos && xPos <= xBar0 + 10) {
        printf("\n\nseen Bar 0!!\n\n");
        eyeStream << "\n\nseen Bar 0!!\n\n" << endl;
        startNow = true;
        start = std::chrono::system_clock::now();
    }
    
    if (xBar1 - 10 <= xPos && xPos <= xBar1 + 10) {
        printf("\nseen Bar 1!!\n");
        eyeStream << "\n\nseen Bar 1!!\n\n" << endl;
        ending = std::chrono::system_clock::now();
        elapsed_seconds = ending - start;
        printf("\nduration: %f\n", elapsed_seconds);
        eyeStream << "\nduration: " << elapsed_seconds.count() << endl;
    }
    */

    //checking which line u r looking at

    
    if (yLine0 - 20 <= yPos && yPos <= yLine0 + 20 && xBar0 - 10 <= xPos && xPos <= xBar0 + 10 && !seenLine0) {
        seenLine0 = true;
        //if (!nextPage) {
            //cout << "New Line: 0" << endl;
            start = std::chrono::system_clock::now();
        //}
        /*else {
            ending = std::chrono::system_clock::now();
            elapsed_seconds = ending - start;
            cout << "New Line: 0" << endl;
            cout << "Duration 7-0: " << elapsed_seconds.count() << endl;
            start = std::chrono::system_clock::now();
        }*/
        
    }
    else if (yLine1 - 20 <= yPos && yPos <= yLine1 + 20 && xBar0 - 10 <= xPos && xPos <= xBar0 +10 && !seenLine1) {
        seenLine1 = true;
        ending = std::chrono::system_clock::now();
        elapsed_seconds = ending - start;
       // cout << "New Line: 1" << endl;
       // cout << "Duration 0-1: " << elapsed_seconds.count() << endl;
        eyeStream << "New Line: 1" << endl;
        eyeStream << "Duration 0-1: " << elapsed_seconds.count() << endl;
        start = std::chrono::system_clock::now();
    }
    else if (yLine2 - 20 <= yPos && yPos <= yLine2 + 20 && xBar0 - 10 <= xPos && xPos <= xBar0 + 10 && !seenLine2) {
        seenLine2 = true;
        ending = std::chrono::system_clock::now();
        elapsed_seconds = ending - start;
       // cout << "New Line: 2" << endl;
       // cout << "Duration 1-2: " << elapsed_seconds.count() << endl;
        eyeStream << "New Line: 2" << endl;
        eyeStream << "Duration 1-2: " << elapsed_seconds.count() << endl;
        start = std::chrono::system_clock::now();
        

    }
    else if (yLine3 - 20 <= yPos && yPos <= yLine3 + 20 && xBar0 - 10 <= xPos && xPos <= xBar0 + 10 && !seenLine3) {
        seenLine3 = true;
        ending = std::chrono::system_clock::now();
        elapsed_seconds = ending - start;
       // cout << "New Line: 3" << endl;
       // cout << "Duration 2-3: " << elapsed_seconds.count() << endl;
        eyeStream << "New Line: 3" << endl;
        eyeStream << "Duration 2-3: " << elapsed_seconds.count() << endl;
        start = std::chrono::system_clock::now();
    }
    else if (yLine4 - 20 <= yPos && yPos <= yLine4 + 20 && xBar0 - 10 <= xPos && xPos <= xBar0 + 10 && !seenLine4) {
        seenLine4 = true;
        ending = std::chrono::system_clock::now();
        elapsed_seconds = ending - start;
       // cout << "New Line: 4" << endl;
      //  cout << "Duration 3-4: " << elapsed_seconds.count() << endl;
        eyeStream << "New Line: 4" << endl;
        eyeStream << "Duration 3-4: " << elapsed_seconds.count() << endl;
        start = std::chrono::system_clock::now();
    }
    else if (yLine5 - 20 <= yPos && yPos <= yLine5 + 20 && xBar0 - 10 <= xPos && xPos <= xBar0 + 10 && !seenLine5) {
        seenLine5 = true;
        ending = std::chrono::system_clock::now();
        elapsed_seconds = ending - start;
       // cout << "New Line: 5" << endl;
        //cout << "Duration 4-5: " << elapsed_seconds.count() << endl;
        eyeStream << "New Line: 5" << endl;
        eyeStream << "Duration 4-5: " << elapsed_seconds.count() << endl;
        start = std::chrono::system_clock::now();
    }
    else if (yLine6 - 20 <= yPos && yPos <= yLine6 + 20 && xBar0 - 10 <= xPos && xPos <= xBar0 + 10 && !seenLine6) {
        seenLine6 = true;
        ending = std::chrono::system_clock::now();
        elapsed_seconds = ending - start;
       // cout << "New Line: 6" << endl;
       // cout << "Duration 5-6: " << elapsed_seconds.count() << endl;
        eyeStream << "New Line: 6" << endl;
        eyeStream << "Duration 5-6: " << elapsed_seconds.count() << endl;
        start = std::chrono::system_clock::now();
    }
    else if (yLine7 - 20 <= yPos && yPos <= yLine7 + 20 && xBar0 - 10 <= xPos && xPos <= xBar0 + 10 && !seenLine7) {
        seenLine7 = true;
        ending = std::chrono::system_clock::now();
        elapsed_seconds = ending - start;
        //cout << "New Line: 7" << endl;
       // cout << "Duration 6-7: " << elapsed_seconds.count() << endl;
        eyeStream << "New Line: 7" << endl;
        eyeStream << "Duration 6-7: " << elapsed_seconds.count() << endl;
        start = std::chrono::system_clock::now();
        nextPage = true;
    }
    else if (yLine7 - 20 <= yPos && yPos <= yLine7 + 20 && xBar3 - 10 <= xPos && xPos <= xBar3 + 10 && seenLine7) {
        eyeStream << "Reached End of  Line: 7" << endl;
        seenLine0 = false;
        seenLine1 = false;
        seenLine2 = false;
        seenLine3 = false;
        seenLine4 = false;
        seenLine5 = false;
        seenLine6 = false;
        seenLine7 = false;

    }


    //page turn stream
    cout << turnPageSig << endl;

    if (gaze_point->validity == TOBII_VALIDITY_VALID) {

       /*printf("Gaze point: %f, %f, %d,\n",
            xPos,
            yPos,
            turnPageSig);*/

       // printf("Gaze point: %f, %f, %d \n", xPos, yPos, turnPageSig);
       // cout << turnPageSig << endl;
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
    cout << "Code INIT..." << endl;

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

    //sleep_for(seconds(4));
    cout << "Code Start..." << endl;
    limit = ((60 * 8) / tempo) * 30;


    //open file
    eyeStream.open("eyeStream_sightread7.txt");

    //while (true) {

    for (int i = 0; i < 3000; i++)
    {
        // Optionally block this thread until data is available. Especially useful if running in a separate thread.
        result = tobii_wait_for_callbacks(1, &device);
        assert(result == TOBII_ERROR_NO_ERROR || result == TOBII_ERROR_TIMED_OUT);

        // Process callbacks on this thread if data is available
        result = tobii_device_process_callbacks(device);
        assert(result == TOBII_ERROR_NO_ERROR);
      /*  if (GetKeyState('K') & 0x8000) {
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
*/



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
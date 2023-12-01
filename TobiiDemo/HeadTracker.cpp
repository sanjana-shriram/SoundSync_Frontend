#include <tobii/tobii_streams.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tobii/tobii.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
ofstream headStream;
int sample = 0;
int turnPage = 0;
double dataSamples[8] = { -1000.0,-1000.0,-1000.0,-1000.0, -1000.0,-1000.0,
            -1000.0,-1000.0};
int pulseThreshold = 10;
int pulseCounter = 10;
double neutralPos = 1000.0;

void printArr() {
    for (int g = 0; g < 8; g++)
    {
        printf("Sample[%d]: %f neutralPos:%f\n", g,dataSamples[g],neutralPos);
    }
}



double takeAverage() {
    double avg = 0;
    double sum = 0;
    for (int i = 0; i < 8; i++)
    {
        sum += dataSamples[i];
    }
    avg = sum / 8.0;
    return avg;
}


bool isAscending() {
    if ((dataSamples[0] > (neutralPos + 20)) || (dataSamples[0] < (neutralPos - 20)))
    {
        return false;
    }
    for (int i = 0; i < 8; i++) {
        if (i != 7)
        {
            if (dataSamples[i] > dataSamples[i + 1]) {
                return false;
            }
        }
    }
    return true;
}

bool isDescending() {
    if ((dataSamples[0] > (neutralPos + 20)) || (dataSamples[0] < (neutralPos - 20)))
    {
        return false;
    }
    for (int i = 0; i < 8; i++) {
        if (i != 7)
        {
            if (dataSamples[i] < dataSamples[i + 1]) {
                return false;
            }
        }
    }
    return true;
}

int detectChange() {
    double diff = 1.0;
    if (isAscending() == true)
    {
        diff = dataSamples[7] - dataSamples[0];
        if (diff >= 40)
        {
            return 1;
        }
    }
    else  if (isDescending() == true) {
        diff = dataSamples[0] - dataSamples[7];
        if (diff >= 40)
        {
            return -1;
        }
    }
    else {
        return 0;
    }
    
}

void head_pose_callback(tobii_head_pose_t const* head_pose, void* user_data)
{
    double newXRot = head_pose->position_xyz[0];
    double newYRot = head_pose->position_xyz[1];
    double newZRot = head_pose->position_xyz[2];

    if (sample < 200) {
        if (sample < 8) {
            switch (sample) {
            case 0:
                dataSamples[0] = newXRot;
                break;
            case 1:
                dataSamples[1] = newXRot;
                break;
            case 2:
                dataSamples[2] = newXRot;
                break;
            case 3:
                dataSamples[3] = newXRot;
                break;
            case 4:
                dataSamples[4] = newXRot;
                break;
            case 5:
                dataSamples[5] = newXRot;
                break;
            case 6:
                dataSamples[6] = newXRot;
                break;
            case 7:
                dataSamples[7] = newXRot;
                break;
            }
        }
        else {
            for (int k = 0; k < 7; k++) {
                dataSamples[k] = dataSamples[k + 1];
            }
            dataSamples[7] = newXRot;
            neutralPos = takeAverage();
        }
        
    }
    else {
        for (int k = 0; k < 7; k++) {
            dataSamples[k] = dataSamples[k + 1];
        }
        dataSamples[7] = newXRot;
        if (pulseCounter >= pulseThreshold) {
            if (detectChange() == 1)
            {
                turnPage = 1;
                pulseCounter = 0;

            }
            else if (detectChange() == -1)
            {
                turnPage = -1;
                pulseCounter = 0;

            }
            else if (detectChange() == 0)
            {
                turnPage = 0;
            }
        }
        else {
            pulseCounter++;
        }
      



    }
    
    
    if (head_pose->position_validity == TOBII_VALIDITY_VALID)
        printf("Sample#: %d, neutralPos: %f, Position: (%f, %f, %f) turnPage: %d\n",
            sample,
            neutralPos,
            head_pose->position_xyz[0],
            head_pose->position_xyz[1],
            head_pose->position_xyz[2],
            turnPage);
        

    //printf("Rotation: ");
    //for (int i = 0; i < 3; ++i)
      //  if (head_pose->rotation_validity_xyz[i] == TOBII_VALIDITY_VALID)
        //    printf("%f, ", head_pose->rotation_xyz[i]);
   // printf(" \n");


    if (headStream.is_open()) {
        //eyeStream << gaze_point->position_xy[0] << "," << gaze_point->position_xy[1] << "\n";
        headStream << "Sample# " << sample << " : neutral pos- " << neutralPos << ", "
            << newXRot << ", " << newYRot << ", " 
            << newZRot << ", turn page right: " << turnPage << " \n";
        sample++;
    }
    
}

static void url_receiver(char const* url, void* user_data)
{
    char* buffer = (char*)user_data;
    if (*buffer != '\0') return; // only keep first value

    if (strlen(url) < 256)
        strcpy_s(buffer, 256, url);
}

int main2()
{
    tobii_api_t* api;
    tobii_error_t error = tobii_api_create(&api, NULL, NULL);
    assert(error == TOBII_ERROR_NO_ERROR);

    char url[256] = { 0 };
    error = tobii_enumerate_local_device_urls(api, url_receiver, url);
    assert(error == TOBII_ERROR_NO_ERROR && *url != '\0');

    tobii_device_t* device;
    error = tobii_device_create(api, url, TOBII_FIELD_OF_USE_INTERACTIVE, &device);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_head_pose_subscribe(device, head_pose_callback, 0);
    assert(error == TOBII_ERROR_NO_ERROR);

    int is_running = 1500; // in this sample, exit after some iterations

    //open file
    headStream.open("headStream_test20.txt");
    while (--is_running > 0)
    {
        error = tobii_wait_for_callbacks(1, &device);
        assert(error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT);

        error = tobii_device_process_callbacks(device);
        assert(error == TOBII_ERROR_NO_ERROR);
  
    }
    //close file
    headStream.close();
    error = tobii_head_pose_unsubscribe(device);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_device_destroy(device);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_api_destroy(api);
    assert(error == TOBII_ERROR_NO_ERROR);
    return 0;
}
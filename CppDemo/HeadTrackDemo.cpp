#include <tobii/tobii_streams.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

void head_pose_callback(tobii_head_pose_t const* head_pose, void* user_data)
{
    if (head_pose->position_validity == TOBII_VALIDITY_VALID)
        printf("Position: (%f, %f, %f)\n",
            head_pose->position_xyz[0],
            head_pose->position_xyz[1],
            head_pose->position_xyz[2]);

    printf("Rotation:\n");
    for (int i = 0; i < 3; ++i)
        if (head_pose->rotation_validity_xyz[i] == TOBII_VALIDITY_VALID)
            printf("%f\n", head_pose->rotation_xyz[i]);
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

    int is_running = 1000; // in this sample, exit after some iterations
    while (--is_running > 0)
    {
        error = tobii_wait_for_callbacks(1, &device);
        assert(error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT);

        error = tobii_device_process_callbacks(device);
        assert(error == TOBII_ERROR_NO_ERROR);
    }

    error = tobii_head_pose_unsubscribe(device);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_device_destroy(device);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_api_destroy(api);
    assert(error == TOBII_ERROR_NO_ERROR);
    return 0;
}
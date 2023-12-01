/*
COPYRIGHT 2015 - PROPERTY OF TOBII AB
-------------------------------------
2015 TOBII AB - KARLSROVAGEN 2D, DANDERYD 182 53, SWEDEN - All Rights Reserved.

NOTICE:  All information contained herein is, and remains, the property of Tobii AB and its suppliers, if any.
The intellectual and technical concepts contained herein are proprietary to Tobii AB and its suppliers and may be
covered by U.S.and Foreign Patents, patent applications, and are protected by trade secret or copyright law.
Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
permission is obtained from Tobii AB.
*/

#ifndef tobii_config_h_included
#define tobii_config_h_included

#include "tobii.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tobii_calibration_point_status_t
{
    TOBII_CALIBRATION_POINT_STATUS_FAILED_OR_INVALID,
    TOBII_CALIBRATION_POINT_STATUS_VALID_BUT_NOT_USED_IN_CALIBRATION,
    TOBII_CALIBRATION_POINT_STATUS_VALID_AND_USED_IN_CALIBRATION,
} tobii_calibration_point_status_t;

typedef struct tobii_calibration_point_data_t
{
    float point_xy[ 2 ];

    tobii_calibration_point_status_t left_status;
    float left_mapping_xy[ 2 ];

    tobii_calibration_point_status_t right_status;
    float right_mapping_xy[ 2 ];
} tobii_calibration_point_data_t;


TOBII_API tobii_error_t TOBII_CALL tobii_set_enabled_eye(tobii_device_t* device, tobii_enabled_eye_t enabled_eye);
TOBII_API tobii_error_t TOBII_CALL tobii_get_enabled_eye(tobii_device_t* device, tobii_enabled_eye_t* enabled_eye);

TOBII_API tobii_error_t TOBII_CALL tobii_calibration_start( tobii_device_t* device, tobii_enabled_eye_t enabled_eye );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_stop( tobii_device_t* device );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_2d( tobii_device_t* device,
    float x, float y );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_3d( tobii_device_t* device,
    float x, float y, float z );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_per_eye_2d( tobii_device_t* device,
    float x, float y, tobii_enabled_eye_t requested_eyes, tobii_enabled_eye_t* collected_eyes );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_2d( tobii_device_t* device,
    float x, float y );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_3d( tobii_device_t* device,
    float x, float y, float z );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_per_eye_2d( tobii_device_t* device,
    float x, float y, tobii_enabled_eye_t eyes );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_clear( tobii_device_t* device );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_compute_and_apply( tobii_device_t* device );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_compute_and_apply_per_eye( tobii_device_t* device,
    tobii_enabled_eye_t* calibrated_eyes );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_retrieve( tobii_device_t* device,
    tobii_data_receiver_t receiver, void* user_data );
typedef void (*tobii_calibration_point_data_receiver_t)( tobii_calibration_point_data_t const* point_data, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_parse( tobii_api_t* api, void const* data, size_t data_size,
    tobii_calibration_point_data_receiver_t receiver, void* user_data );

TOBII_API tobii_error_t TOBII_CALL tobii_calibration_apply( tobii_device_t* device,
    void const* data, size_t size );


typedef struct tobii_geometry_mounting_t
{
    int guides;
    float width_mm;
    float angle_deg;
    float external_offset_mm_xyz[ 3 ];
    float internal_offset_mm_xyz[ 3 ];
} tobii_geometry_mounting_t;

TOBII_API tobii_error_t TOBII_CALL tobii_get_geometry_mounting( tobii_device_t* device,
    tobii_geometry_mounting_t* geometry_mounting );


TOBII_API tobii_error_t TOBII_CALL tobii_get_display_area( tobii_device_t* device,
    tobii_display_area_t* display_area );
TOBII_API tobii_error_t TOBII_CALL tobii_set_display_area( tobii_device_t* device,
    tobii_display_area_t const* display_area );
TOBII_API tobii_error_t TOBII_CALL tobii_calculate_display_area_basic( tobii_api_t* api,
    float width_mm, float height_mm, float offset_x_mm, tobii_geometry_mounting_t const* geometry_mounting,
    tobii_display_area_t* display_area );

typedef char tobii_device_name_t[ 64 ];
TOBII_API tobii_error_t TOBII_CALL tobii_get_device_name( tobii_device_t* device,
    tobii_device_name_t* device_name );
TOBII_API tobii_error_t TOBII_CALL tobii_set_device_name( tobii_device_t* device,
    tobii_device_name_t const device_name );

typedef void (*tobii_output_frequency_receiver_t)( float output_frequency, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_enumerate_output_frequencies( tobii_device_t* device,
    tobii_output_frequency_receiver_t receiver, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_set_output_frequency( tobii_device_t* device,
    float output_frequency );
TOBII_API tobii_error_t TOBII_CALL tobii_get_output_frequency( tobii_device_t* device,
    float* output_frequency );
TOBII_API tobii_error_t TOBII_CALL tobii_output_frequency_subscribe( tobii_device_t* device,
    tobii_output_frequency_receiver_t callback, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_output_frequency_unsubscribe( tobii_device_t* device );


typedef char tobii_display_id_t[ 256 ];

TOBII_API tobii_error_t TOBII_CALL tobii_get_display_id( tobii_device_t* device, tobii_display_id_t* display_id );
TOBII_API tobii_error_t TOBII_CALL tobii_set_display_id( tobii_device_t* device, tobii_display_id_t const display_id );

typedef void ( *tobii_display_id_callback_t )( tobii_display_id_t const display_id, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_display_id_subscribe( tobii_device_t* device, tobii_display_id_callback_t callback, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_display_id_unsubscribe( tobii_device_t* device );

typedef enum tobii_calibration_stimulus_point_status_t
{
    TOBII_CALIBRATION_STIMULUS_POINT_STATUS_FAILED_OR_INVALID,
    TOBII_CALIBRATION_STIMULUS_POINT_STATUS_VALID_NOT_USED,
    TOBII_CALIBRATION_STIMULUS_POINT_STATUS_VALID_USED,
} tobii_calibration_stimulus_point_status_t;

typedef struct tobii_calibration_stimulus_point_data_t
{
    float point_xyz[ 3 ];
    tobii_calibration_stimulus_point_status_t left_status;
    float left_bias;
    float left_precision;
    tobii_calibration_stimulus_point_status_t right_status;
    float right_bias;
    float right_precision;
} tobii_calibration_stimulus_point_data_t;

typedef struct tobii_calibration_stimulus_points_t
{
    int stimulus_point_count;
    tobii_calibration_stimulus_point_data_t stimulus_points[ 32 ];
} tobii_calibration_stimulus_points_t;

TOBII_API tobii_error_t TOBII_CALL tobii_calibration_stimulus_points_get( tobii_device_t* device,
     tobii_calibration_stimulus_points_t* stimulus_points );

#ifdef __cplusplus
}
#endif

#endif /* tobii_config_h_included */

/**
@defgroup tobii_config tobii_config.h

tobii_config.h
==============

The tobii_config.h header file contains functionality to configure the tracker, such as calibration and display area
setup. Note that using the configuration APIs incorrectly may cause some tracker functionality to work incorrectly.
Please refer to the calibration sample for recommendations on how to implement a correct calibration.

All functions in the configuration API which modify state (i.e. everything except get- and enumerate- functions)
require a license on at least config level, and a device created through tobii_device_create_ex.
*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_enabled_eye( tobii_device_t* device, tobii_enabled_eye_t enabled_eye );
@ingroup tobii_config

tobii_set_enabled_eye
--------------------------

### Function

Controls which eyes are used when calculating the combined gaze.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_set_enabled_eye( tobii_device_t* device, tobii_enabled_eye_t enabled_eye );


### Remarks

Controls which eyes are used when calculating the combined gaze point in the gaze point stream.

If for example the eye tracker user has a visual impairment in one eye or suffer from an eye disorder such as strabismus,
then it would be beneficial to only enable the dominant eye.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create_ex with a valid config level license.

*enabled_eye* contains the value to which the enabled eye property of the device shall be set: **TOBII_ENABLED_EYE_LEFT**, **TOBII_ENABLED_EYE_RIGHT** or
**TOBII_ENABLED_EYE_BOTH**


### Return value

If the operation is successful, tobii_set_enabled_eye returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_set_enabled_eye returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter passed was NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    Setting the enabled eye property is not supported by the device.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_output_frequency from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_get_enabled_eye()](#tobii_get_enabled_eye)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy(),
        // tobii_licensing.h on how to create a device using tobii_device_create_ex()
        // and tobii.h on how to destroy a device using tobii_device_destroy().
        tobii_device_t* device = 0;

        tobii_enabled_eye_t enabled_eye_props[] = { TOBII_ENABLED_EYE_BOTH, TOBII_ENABLED_EYE_LEFT, TOBII_ENABLED_EYE_RIGHT };

        for( size_t ix = 0; ix < sizeof( enabled_eye_props ) / sizeof( enabled_eye_props[ 0 ] ); ++ix )
        {
            tobii_error_t error = tobii_set_enabled_eye( device, enabled_eye_props[ ix ] );
            if( error != TOBII_ERROR_NO_ERROR )
            {
                printf( "Failure " );
            }
            else
            {
                printf( "Success " );
            }

            printf( "setting enabled eye property to: " );
            switch( enabled_eye_props[ ix ] )
            {
                case TOBII_ENABLED_EYE_LEFT: printf( "TOBII_ENALBED_EYE_LEFT" );
                    break;
                case TOBII_ENABLED_EYE_RIGHT: printf( "TOBII_ENALBED_EYE_RIGHT" );
                    break;
                case TOBII_ENABLED_EYE_BOTH: printf( "TOBII_ENALBED_EYE_BOTH" );
                    break;
            }
            printf( "\n" );
        }
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_enabled_eye( tobii_device_t* device, tobii_enabled_eye_t* enabled_eye );
@ingroup tobii_config

tobii_get_enabled_eye
--------------------------

### Function

Queries the enabled eye property of the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_enabled_eye( tobii_device_t* device, tobii_enabled_eye_t* enabled_eye );


### Remarks

Queries which eyes are used when calculating the combined gaze point in the gaze point stream.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*enabled_eye* is a valid pointer to the value containing the retrieved enabled eye property of the device: **TOBII_ENABLED_EYE_LEFT**, **TOBII_ENABLED_EYE_RIGHT** or
**TOBII_ENABLED_EYE_BOTH**.


### Return value

If the operation is successful, tobii_get_enabled_eye returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_get_enabled_eye returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *enabled_eye* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    Getting the enabled eye property is not supported by the device.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_enabled_eye from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_set_enabled_eye()](#tobii_set_enabled_eye)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy()
        // and on how to create/destroy a device using tobii_device_create/destroy().
        tobii_device_t* device = 0;

        tobii_enabled_eye_t enabled_eye = TOBII_ENABLED_EYE_BOTH;
        tobii_error_t error = tobii_get_enabled_eye( device, &enabled_eye );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to get enabled eye property\n" );
            // destroy api and device, omitted here for brevity
            return error;
        }

        switch( enabled_eye )
        {
            case TOBII_ENABLED_EYE_LEFT: printf( "TOBII_ENALBED_EYE_LEFT" ); break;
            case TOBII_ENABLED_EYE_RIGHT: printf( "TOBII_ENALBED_EYE_RIGHT" ); break;
            case TOBII_ENABLED_EYE_BOTH: printf( "TOBII_ENALBED_EYE_BOTH" ); break;
        }

        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_start( tobii_device_t* device, tobii_enabled_eye_t enabled_eye );
@ingroup tobii_config

tobii_calibration_start
-----------------------

### Function

Starts the calibration process, placing the tracker in a state ready to receive data collection requests.

### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_start( tobii_device_t* device,
        tobii_enabled_eye_t enabled_eye );


### Remarks

If this call is successful, the client receives exclusive access to the calibration process, no other clients can start a
calibration process until tobii_calibration_stop() has been called.

These functions requires that tobii_calibration_start() has been called successfully:

-   tobii_calibration_stop()
-   tobii_calibration_collect_data_2d()
-   tobii_calibration_collect_data_3d()
-   tobii_calibration_collect_data_per_eye_2d()
-   tobii_calibration_discard_data_2d()
-   tobii_calibration_discard_data_3d()
-   tobii_calibration_discard_data_per_eye_2d()
-   tobii_calibration_clear()
-   tobii_calibration_compute_and_apply()
-   tobii_calibration_compute_and_apply_per_eye()

Here follows a basic calibration process example where a 7-point calibration is performed, it's recommended to do a
7-point calibration in-order to get good accuracy of the calibration:

* Call tobii_calibration_start() to initiate the calibration process
* Start by doing a one point calibration at center screen
    - Plot a stimuli point in UI and subscribe to gaze point to check when user is in the vicinity of the stimuli point.
    - Once user has directed its gaze to the stimuli point, call tobii_calibration_collect_data_X().
    - Call tobii_calibration_compute_and_apply() to commit the one point calibration.
* Improve the calibration by doing a three point calibration
    - Plot stimuli points in a specific pattern in UI and check when user is in the vicinity of a one of the stimuli
    points.
    - Once user has directed its gaze to a specific the stimuli point, call tobii_calibration_collect_data_X() for that
    stimuli point, repeat for each stimuli point.
    - If a call to tobii_calibration_collect_data_X() should fail, re-run it until a successful collection is achieved.
    - Once all points have been successfully collected call tobii_calibration_compute_and_apply() to commit the three
    point calibration.
* Improve the calibration further by doing another round of three point calibration
    - Plot stimuli points inverting the pattern from previous step, and repeat the above steps for each point.
    - Call tobii_calibration_compute_and_apply() to commit the last three point calibration.
* End the calibration process by calling tobii_calibration_stop(), this stops the exclusive calibration access.

For a more detailed calibration process description please consult:
https://developer.tobii.com/download-packages/how-to-create-a-customized-calibration

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*DEPRECATED* *enabled_eye* must ALWAYS be **TOBII_ENABLED_EYE_BOTH**, this parameter is deprecated and will be removed
in the next major release.


### Return value

If the operation is successful, tobii_calibration_start returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_start returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    A value other than TOBII_ENABLED_EYE_BOTH was passed for the *enabled_eye* parameter.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_ALREADY_STARTED**

    tobii_calibration_start has already been called, and not yet been stopped by calling tobii_calibration_stop.
    A started calibration must always be stopped before a new calibration is started.

-   **TOBII_ERROR_CALIBRATION_BUSY**

    Another client is already calibrating the device. Only one calibration can be running at a time, across all
    connected clients.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_start from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

See [tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_stop( tobii_device_t* device );
@ingroup tobii_config

tobii_calibration_stop
----------------------

### Function

Signals that the calibration process has been completed and that no further data collection will be requested.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_stop( tobii_device_t* device );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.


### Return value

If the operation is successful, tobii_calibration_stop returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_stop returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_stop from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

See [tobii_calibration_collect_data_2d())](#tobii_calibration_collect_data_2d).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_2d( tobii_device_t* device, float x, float y );
@ingroup tobii_config

tobii_calibration_collect_data_2d
---------------------------------

### Function

Performs data collection for the specified screen coordinate.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_collect_data_2d( tobii_device_t* device,
        float x, float y );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

This function is synchronous and may block for several seconds while the eye tracker collects calibration data.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

*y* the y-coordinate (vertical) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

### Return value

If the operation is successful, tobii_calibration_collect_data_2d returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_collect_data_2d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_OPERATION_FAILED**

    The tracker failed to collect a sufficient amount of data. It is recommended to performing the operation again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_collect_data_2d from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

@code{.c}

    #include <tobii/tobii_licensing.h>
    #include <tobii/tobii_config.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <memory.h>
    #include <assert.h>
    #include <string.h>

    static size_t read_license_file( uint16_t* license )
    {
        FILE *license_file = fopen( "se_license_key_sample", "rb" );

        if( !license_file )
        {
            printf( "License key could not be found!\n" );
            return 0;
        }

        fseek( license_file, 0, SEEK_END );
        long file_size = ftell( license_file );
        rewind( license_file );

        if( file_size <= 0 )
        {
            fclose( license_file );
            printf( "Unable to read License file!\n" );
            return 0;
        }

        if( license )
        {
            fread( license, sizeof( uint16_t ), file_size / sizeof( uint16_t ), license_file );
        }

        fclose( license_file );
        return (size_t)file_size;
    }

    tobii_error_t create_device( tobii_api_t* api, tobii_device_t** device, char* url )
    {
        tobii_error_t error;

        size_t license_size = read_license_file( 0 );
        assert( license_size > 0 );

        uint16_t* license_key = (uint16_t*)malloc( license_size );
        memset( license_key, 0, license_size );
        read_license_file( license_key );
        tobii_license_key_t license = { license_key, license_size };

        tobii_license_validation_result_t license_result;
        error = tobii_device_create_ex( api, url, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &license, 1, &license_result, device );
        free( license_key );
        license_key = 0;

        if( error == TOBII_ERROR_CONNECTION_FAILED )
        {
            printf( "Failed to connect to tracker.\n" );
            return error;
        }

        return error;
    }

    tobii_error_t calibrate( tobii_device_t* device )
    {
        tobii_error_t error = tobii_calibration_start( device, TOBII_ENABLED_EYE_BOTH );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        printf( "Look at the top left!\n" );
        error = tobii_calibration_collect_data_2d( device, 0.1f, 0.1f );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        printf( "Look at the bottom right!\n" );
        error = tobii_calibration_collect_data_2d( device, 0.9f, 0.9f );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        printf( "Look at the center!\n" );
        error = tobii_calibration_collect_data_2d( device, 0.5f, 0.5f );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        error = tobii_calibration_compute_and_apply( device );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        return tobii_calibration_stop( device );
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = create_device( api, &device, url );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to create device!\n" );
            tobii_api_destroy( api );
            return error;
        }

        error = calibrate( device );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "Calibration succeed!\n" );
        }
        else
        {
            printf( "Calibration failed!\n" );
        }

        tobii_device_destroy( device );
        tobii_api_destroy( api );

        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_3d( tobii_device_t* device, float x, float y, float z );
@ingroup tobii_config

tobii_calibration_collect_data_3d
---------------------------------

### Function

Performs data collection for the specified 3d coordinate.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_collect_data_3d( tobii_device_t* device,
        float x, float y, float z );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

This function is synchronous and may block for several seconds while the eye tracker collects calibration data.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to collect calibration data for, in millimeters.

*y* the y-coordinate (vertical) of the point to collect calibration data for, in millimeters.

*z* the z-coordinate (depth) of the point to collect calibration data for, in millimeters.


### Return value

If the operation is successful, tobii_calibration_collect_data_3d returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_collect_data_3d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_OPERATION_FAILED**

    The tracker failed to collect a sufficient amount of data. It is recommended to perform the operation again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_collect_data_3d from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

@code{.c}

    #include <tobii/tobii_licensing.h>
    #include <tobii/tobii_config.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <memory.h>
    #include <assert.h>

    static size_t read_license_file( uint16_t* license )
    {
        FILE *license_file = fopen( "se_license_key_sample", "rb" );

        if( !license_file )
        {
            printf( "License key could not be found!\n" );
            return 0;
        }

        fseek( license_file, 0, SEEK_END );
        long file_size = ftell( license_file );
        rewind( license_file );

        if( file_size <= 0 )
        {
            fclose( license_file );
            printf( "Unable to read License file!\n" );
            return 0;
        }

        if( license )
        {
            fread( license, sizeof( uint16_t ), file_size / sizeof( uint16_t ), license_file );
        }

        fclose( license_file );
        return (size_t)file_size;
    }

    tobii_error_t create_device( tobii_api_t* api, tobii_device_t** device, char* url )
    {
        tobii_error_t error;

        size_t license_size = read_license_file( 0 );
        assert( license_size > 0 );

        uint16_t* license_key = (uint16_t*)malloc( license_size );
        memset( license_key, 0, license_size );
        read_license_file( license_key );
        tobii_license_key_t license = { license_key, license_size };

        tobii_license_validation_result_t license_result;
        error = tobii_device_create_ex( api, url, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &license, 1, &license_result, device );
        free( license_key );
        license_key = 0;

        if( error == TOBII_ERROR_CONNECTION_FAILED )
        {
            printf( "Failed to connect to tracker.\n" );
            return error;
        }

        return error;
    }

    tobii_error_t calibrate( tobii_device_t* device )
    {
        tobii_error_t error = tobii_calibration_start( device, TOBII_ENABLED_EYE_BOTH );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        // plot an object at (0.1, 0.1, 0.1) that the user will look at
        error = tobii_calibration_collect_data_3d( device, 0.1f, 0.1f, 0.1f );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        // plot an object at (0.9, 0.9, 0.9) that the user will look at
        error = tobii_calibration_collect_data_3d( device, 0.9f, 0.9f, 0.9f );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        // plot an object at (0.5, 0.5, 0.5) that the user will look at
        error = tobii_calibration_collect_data_3d( device, 0.5f, 0.5f, 0.5f );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        error = tobii_calibration_compute_and_apply( device );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        return tobii_calibration_stop( device );
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = create_device( api, &device, url );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to create device!\n" );
            tobii_api_destroy( api );
            return error;
        }

        error = calibrate( device );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "Calibration succeed!\n" );
        }
        else
        {
            printf( "Calibration failed!\n" );
        }

        tobii_device_destroy( device );
        tobii_api_destroy( api );

        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_per_eye_2d( tobii_device_t* device, float x, float y, tobii_enabled_eye_t requested_eyes, tobii_enabled_eye_t* collected_eyes );
@ingroup tobii_config

tobii_calibration_collect_data_per_eye_2d
-----------------------------------------

### Function

Performs data collection for the specified screen coordinate, for the left, right or both eyes.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_collect_data_per_eye_2d( tobii_device_t* device,
        float x, float y, tobii_enabled_eye_t requested_eyes,
        tobii_enabled_eye_t* collected_eyes );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

This function is synchronous and may block for several seconds while the eye tracker collects calibration data.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

*y* the y-coordinate (vertical) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

*requested_eyes* specifies which eye to collect data for: **TOBII_ENABLED_EYE_LEFT**, **TOBII_ENABLED_EYE_RIGHT** or
**TOBII_ENABLED_EYE_BOTH**

*collected_eyes* reports back which eye data was successfully collected for: **TOBII_ENABLED_EYE_LEFT**,
**TOBII_ENABLED_EYE_RIGHT** or **TOBII_ENABLED_EYE_BOTH**


### Return value

If the operation is successful, tobii_calibration_collect_data_per_eye_2d returns **TOBII_ERROR_NO_ERROR**. If the call
fails, tobii_calibration_collect_data_per_eye_2d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL, or *requested_eyes* was passed in as an invalid enum value.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The functionality is not supported on this device.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_OPERATION_FAILED**

    The tracker failed to collect a sufficient amount of data. It is recommended to performing the operation again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_collect_data_per_eye_2d from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)

### Example

@code{.c}

    #include <tobii/tobii_licensing.h>
    #include <tobii/tobii_config.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <memory.h>
    #include <assert.h>
    #include <string.h>

    static size_t read_license_file( uint16_t* license )
    {
        FILE *license_file = fopen( "se_license_key_sample", "rb" );

        if( !license_file )
        {
            printf( "License key could not be found!\n" );
            return 0;
        }

        fseek( license_file, 0, SEEK_END );
        long file_size = ftell( license_file );
        rewind( license_file );

        if( file_size <= 0 )
        {
            fclose( license_file );
            printf( "Unable to read License file!\n" );
            return 0;
        }

        if( license )
        {
            fread( license, sizeof( uint16_t ), file_size / sizeof( uint16_t ), license_file );
        }

        fclose( license_file );
        return (size_t)file_size;
    }

    tobii_error_t create_device( tobii_api_t* api, tobii_device_t** device, char* url )
    {
        tobii_error_t error;

        size_t license_size = read_license_file( 0 );
        assert( license_size > 0 );

        uint16_t* license_key = (uint16_t*)malloc( license_size );
        memset( license_key, 0, license_size );
        read_license_file( license_key );
        tobii_license_key_t license = { license_key, license_size };

        tobii_license_validation_result_t license_result;
        error = tobii_device_create_ex( api, url, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &license, 1, &license_result, device );
        free( license_key );
        license_key = 0;

        if( error == TOBII_ERROR_CONNECTION_FAILED )
        {
            printf( "Failed to connect to tracker.\n" );
            return error;
        }

        return error;
    }

    tobii_error_t calibrate( tobii_device_t* device, tobii_enabled_eye_t requested_eye )
    {
        tobii_error_t error = tobii_calibration_start( device, TOBII_ENABLED_EYE_BOTH );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        printf( "Look at the top left!\n" );
        tobii_enabled_eye_t collected_eyes;
        error = tobii_calibration_collect_data_per_eye_2d( device, 0.1f, 0.1f, requested_eye,
            &collected_eyes );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        printf( "Look at the bottom right!\n" );
        error = tobii_calibration_collect_data_per_eye_2d( device, 0.9f, 0.9f, requested_eye,
            &collected_eyes );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        printf( "Look at the center!\n" );
        error = tobii_calibration_collect_data_per_eye_2d( device, 0.5f, 0.5f, requested_eye,
            &collected_eyes );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        error = tobii_calibration_compute_and_apply_per_eye( device, &collected_eyes );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        error = tobii_calibration_stop( device );
        if( error != TOBII_ERROR_NO_ERROR ) { return error; }

        if( collected_eyes != requested_eye ) { return TOBII_ERROR_OPERATION_FAILED; }

        return TOBII_ERROR_NO_ERROR;
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = create_device( api, &device, url );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to create device!\n" );
            tobii_api_destroy( api );
            return error;
        }

        error = calibrate( device, TOBII_ENABLED_EYE_LEFT );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "Left eye calibration succeed!\n" );
        }
        else
        {
            printf( "Left eye calibration failed!\n" );
        }

        error = calibrate( device, TOBII_ENABLED_EYE_RIGHT );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "Right eye calibration succeed!\n" );
        }
        else
        {
            printf( "Right eye calibration failed!\n" );
        }

        tobii_device_destroy( device );
        tobii_api_destroy( api );

        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_2d( tobii_device_t* device, float x, float y );
@ingroup tobii_config

tobii_calibration_discard_data_2d
---------------------------------

### Function

Discards all data collected for the specified screen coordinate.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_discard_data_2d( tobii_device_t* device,
        float x, float y );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to discard data for, as specified in a prior call to
tobii_calibration_collect_data_2d.

*y* the y-coordinate (vertical) of the point to discard data for, as specified in a prior call to
tobii_calibration_collect_data_2d.


### Return value

If the operation is successful, tobii_calibration_discard_data_2d returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_discard_data_2d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_discard_data_2d from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

See [tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_3d( tobii_device_t* device, float x, float y, float z );
@ingroup tobii_config

tobii_calibration_discard_data_3d
---------------------------------

### Function

Discards all data collected for the specified 3d coordinate.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_discard_data_3d( tobii_device_t* device,
        float x, float y, float z );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to discard data for, as specified in a prior call to
tobii_calibration_collect_data_3d.

*y* the y-coordinate (vertical) of the point to discard data for, as specified in a prior call to
tobii_calibration_collect_data_3d.

*z* the z-coordinate (depth) of the point to discard data for, as specified in a prior call to
tobii_calibration_collect_data_3d.


### Return value

If the operation is successful, tobii_calibration_discard_data_3d returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_discard_data_3d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_discard_data_3d from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

See [tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_per_eye_2d( tobii_device_t* device, float x, float y, tobii_enabled_eye_t eyes );
@ingroup tobii_config

tobii_calibration_discard_data_per_eye_2d
-----------------------------------------

### Function

Discards all data collected by a corresponding call to tobii_calibration_collect_data_per_eye_2d for a given stimuli point.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_discard_data_per_eye_2d( tobii_device_t* device,
        float x, float y, tobii_enabled_eye_t eyes );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to discard data for, as specified in a prior call to
tobii_calibration_collect_data_per_eye_2d.

*y* the y-coordinate (vertical) of the point to discard data for, as specified in a prior call to
tobii_calibration_collect_data_per_eye_2d.

*eyes* specifies which eye to discard data for: **TOBII_ENABLED_EYE_LEFT**, **TOBII_ENABLED_EYE_RIGHT** or
**TOBII_ENABLED_EYE_BOTH**, which should match the value passed in the corresponding
tobii_calibration_collect_data_per_eye_2d call.


### Return value

If the operation is successful, tobii_calibration_discard_data_per_eye_2d returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_discard_data_per_eye_2d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL, *eyes* was passed in as an invalid enum value.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The functionality is not supported on this device.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_discard_data_per_eye_2d from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)

### Example

See [tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_clear( tobii_device_t* device );
@ingroup tobii_config

tobii_calibration_clear
-----------------------

### Function

Resets the calibration to the default state.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_clear( tobii_device_t* device );


### Remarks

Also performed internally when calling tobii_calibration_start.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.


### Return value

If the operation is successful, tobii_calibration_clear returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_clear returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_clear from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

See [tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d) and [tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_compute_and_apply( tobii_device_t* device );
@ingroup tobii_config

tobii_calibration_compute_and_apply
-----------------------------------

### Function

Computes a calibration based on data collected so far and applies the resulting calibration to the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_compute_and_apply( tobii_device_t* device );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.


### Return value

If the operation is successful, tobii_calibration_compute_and_apply returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_compute_and_apply returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_OPERATION_FAILED**

    Not enough data collected to compute calibration.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_compute_and_apply from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.



### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)



### Example

See [tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d) and [tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_compute_and_apply_per_eye( tobii_device_t* device, tobii_enabled_eye_t* calibrated_eyes );
@ingroup tobii_config

tobii_calibration_compute_and_apply_per_eye
-------------------------------------------

### Function

Computes a calibration based on data collected so far and applies the resulting calibration to the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_compute_and_apply_per_eye( tobii_device_t* device,
        tobii_enabled_eye_t* calibrated_eyes );


### Remarks

This function should not be called unless the calibration process has been started by calling tobii_calibration_start().

Computes the calibration given the data gathered by calling tobii_calibration_discard_data_per_eye_2d(). Depending on the
quality of the data gathered it will determine and return the information regarding for which eye the calibration was
successful.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*calibrated_eyes* receives information about which eyes were successfully calibrated: **TOBII_ENABLED_EYE_LEFT**,
**TOBII_ENABLED_EYE_RIGHT** or **TOBII_ENABLED_EYE_BOTH**


### Return value

If the operation is successful, tobii_calibration_compute_and_apply_per_eye returns **TOBII_ERROR_NO_ERROR**. If the
call fails, tobii_calibration_compute_and_apply_per_eye returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_OPERATION_FAILED**

    Not enough data collected to compute calibration.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_compute_and_apply_per_eye from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

See [tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_retrieve( tobii_device_t* device, tobii_data_receiver_t receiver, void* user_data );
@ingroup tobii_config

tobii_calibration_retrieve
--------------------------

### Function

Retrieves the currently applied calibration from the device as a binary blob.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_retrieve( tobii_device_t* device,
        tobii_data_receiver_t receiver, void* user_data );


### Remarks

This function will always return the binary data of the latest applied calibration, this data can be stored and
then be used for re-applying the calibration in case of power cycle or switching user.

This function should be called after tobii_calibration_compute_and_apply()/tobii_calibration_compute_and_apply_per_eye()
in order to retrieve the latest calibration data, otherwise the returned calibration will be the data from previous
calibration.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*receiver* is a function pointer to a function with the prototype:

    void data_receiver( void const* data, size_t size, void* user_data )

This function will be called with the retrieved calibration data. It is called with the following parameters:

-   *data*
    The calibration data read from device. This pointer will be invalid after returning from the function,
    so ensure you make a copy of the data rather than storing the pointer directly.

-   *size*
    The size of the calibration data, in bytes.

-   *user_data*
    This is the custom pointer passed to tobii_calibration_retrieve.

*user_data* custom pointer which will be passed unmodified to the receiver function.


### Return value

If the operation is successful, tobii_calibration_retrieve returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_retrieve returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *receiver* parameter was passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_retrieve from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example

@code{.c}

    #include <tobii/tobii_licensing.h>
    #include <tobii/tobii_config.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <memory.h>
    #include <assert.h>
    #include <string.h>

    static size_t read_license_file( uint16_t* license )
    {
        FILE *license_file = fopen( "se_license_key_sample", "rb" );

        if( !license_file )
        {
            printf( "License key could not be found!\n" );
            return 0;
        }

        fseek( license_file, 0, SEEK_END );
        long file_size = ftell( license_file );
        rewind( license_file );

        if( file_size <= 0 )
        {
            fclose( license_file );
            printf( "Unable to read License file!\n" );
            return 0;
        }

        if( license )
        {
            fread( license, sizeof( uint16_t ), file_size / sizeof( uint16_t ), license_file );
        }

        fclose( license_file );
        return (size_t)file_size;
    }

    tobii_error_t create_device( tobii_api_t* api, tobii_device_t** device, char* url )
    {
        tobii_error_t error;

        size_t license_size = read_license_file( 0 );
        assert( license_size > 0 );

        uint16_t* license_key = (uint16_t*)malloc( license_size );
        memset( license_key, 0, license_size );
        read_license_file( license_key );
        tobii_license_key_t license = { license_key, license_size };

        tobii_license_validation_result_t license_result;
        error = tobii_device_create_ex( api, url, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &license, 1, &license_result, device );
        free( license_key );
        license_key = 0;

        if( error == TOBII_ERROR_CONNECTION_FAILED )
        {
            printf( "Failed to connect to tracker.\n" );
            return error;
        }

        return error;
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    typedef struct
    {
        size_t size;
        uint8_t* data;
    } calibration_blob_t;

    void calibration_retrieve_callback( void const* data, size_t size, void* user_data )
    {
        calibration_blob_t* blob = (calibration_blob_t*)user_data;
        blob->data = (uint8_t*)malloc( size * sizeof( blob->data ) );
        blob->size = size;
        memcpy(blob->data, data, size );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = create_device( api, &device, url );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to create device!\n" );
            tobii_api_destroy( api );
            return error;
        }

        // Performed the calibration process and apply as the current calibration
        // with tobii_compute_and_apply();

        calibration_blob_t blob;
        error = tobii_calibration_retrieve( device, calibration_retrieve_callback, &blob );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            FILE* fp = fopen("calibration_blob.bin", "w+");
            fwrite( blob.data, sizeof(blob.data), blob.size, fp );
            fclose( fp );
            printf( "Calibration successfully stored on disk!\n" );
        }
        else
        {
            printf( "Calibration retrieval failed!\n" );
        }

        if( blob.data ) free( blob.data );

        tobii_device_destroy( device );
        tobii_api_destroy( api );

        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_parse( tobii_api_t* api, void const* data, size_t data_size, tobii_calibration_point_data_receiver_t receiver, void* user_data );
@ingroup tobii_config

tobii_calibration_parse
-----------------------

### Function

Extracts data about calibration points from the specified calibration.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_parse( tobii_api_t* api, void const* data,
        size_t data_size, tobii_calibration_point_data_receiver_t receiver,
        void* user_data );


### Remarks

Query which stimuli points where used in a specific calibration and gives information regarding if the stimuli point is
valid and whether it was used in the calibration or not. Also contains information about the left and right eye focus
point for each stimuli point.

*api* must be a pointer to a valid tobii_api_t instance as created by calling tobii_api_create.

*data* is the calibration data retrieved by tobii_calibration_retrieve().

*data_size* is the size of the data retrieved by tobii_calibration_retrieve().

*receiver* is a function pointer to a function with the prototype:

    void receiver( tobii_calibration_point_data_t const* point_data, void* user_data )

This function will be called for each parsed point from the calibration. It is called with the following parameters:

-   *point_data*
    A pointer to a struct containing all the data related to a calibration point.

    -   *point_xy*

        x and y coordinates of the requested calibration stimuli point.

    -   *left_status*

        Indicates the left eye calibration status of the stimuli point: **TOBII_CALIBRATION_POINT_STATUS_FAILED_OR_INVALID**,
        **TOBII_CALIBRATION_POINT_STATUS_VALID_BUT_NOT_USED_IN_CALIBRATION** or **TOBII_CALIBRATION_POINT_STATUS_VALID_AND_USED_IN_CALIBRATION**

    -   *left_mapping_xy*

        The left eye focus point captured during calibration.

    -   *right_status*

        Indicates the right eye calibration status of the stimuli point: **TOBII_CALIBRATION_POINT_STATUS_FAILED_OR_INVALID**,
        **TOBII_CALIBRATION_POINT_STATUS_VALID_BUT_NOT_USED_IN_CALIBRATION** or **TOBII_CALIBRATION_POINT_STATUS_VALID_AND_USED_IN_CALIBRATION**

    -   *right_mapping_xy*

        The right eye focus point captured during calibration.


-   *user_data*
    This is the custom pointer sent in to tobii_calibration_parse.

*user_data* custom pointer which will be passed unmodified to the receiver function.


### Return value

If the operation is successful, tobii_calibration_parse returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_parse returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *api*, *data* or *receiver* parameters were passed in as NULL, or *data_size* parameter was less than 8.

-   **TOBII_OPERATION_FAILED**

    The data being parsed was not a valid calibration.


### See also

[tobii_calibration_start()](#tobii_calibration_start),
[tobii_calibration_stop()](#tobii_calibration_stop),
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_collect_data_2d()](#tobii_calibration_collect_data_2d),
[tobii_calibration_collect_data_3d()](#tobii_calibration_collect_data_3d),
[tobii_calibration_collect_data_per_eye_2d()](#tobii_calibration_collect_data_per_eye_2d),
[tobii_calibration_discard_data_2d()](#tobii_calibration_discard_data_2d),
[tobii_calibration_discard_data_3d()](#tobii_calibration_discard_data_3d),
[tobii_calibration_discard_data_per_eye_2d()](#tobii_calibration_discard_data_per_eye_2d),
[tobii_calibration_compute_and_apply()](#tobii_calibration_compute_and_apply),
[tobii_calibration_compute_and_apply_per_eye()](#tobii_calibration_compute_and_apply_per_eye),
[tobii_calibration_apply()](#tobii_calibration_apply),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),


### Example

@code{.c}

    #include <tobii/tobii_licensing.h>
    #include <tobii/tobii_config.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <memory.h>
    #include <assert.h>

    void point_receiver( tobii_calibration_point_data_t const* point_data, void* user_data )
    {
        (void)user_data;
        printf( "Target point : [%f, %f] ",
            point_data->point_xy[ 0 ], point_data->point_xy[ 1 ] );
        printf( "Left eye focus point : [%f, %f] ",
            point_data->left_mapping_xy[ 0 ], point_data->left_mapping_xy[ 1 ] );
        printf( "Right eye focus point : [%f, %f] \n",
            point_data->right_mapping_xy[ 0 ], point_data->right_mapping_xy[ 1 ] );
    }

    void calibration_retrieve_callback( void const* data, size_t size, void* user_data )
    {
        tobii_api_t* api = (tobii_api_t*)user_data;

        tobii_error_t error = tobii_calibration_parse( api, data, size, point_receiver, NULL );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Parsing calibration failed!" );
            return;
        }
    }

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy(),
        // tobii_licensing.h on how to create a device using tobii_device_create_ex()
        // and tobii.h on how to destroy a device using tobii_device_destroy().
        tobii_api_t* api = 0; // Dummy for demo purposes
        tobii_device_t* device = 0; // Dummy for demo purposes

        // Make the calibration and apply as the current calibration with tobii_compute_and_apply();

        tobii_error_t error = tobii_calibration_retrieve( device, calibration_retrieve_callback, api );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Retrieving calibration failed!" );
        }

        // destroy api and device, omitted here for brevity
        return error;
    }
@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_apply( tobii_device_t* device, void const* data, size_t size );
@ingroup tobii_config

tobii_calibration_apply
-----------------------

### Function

Applies the specified calibration binary blob to the device, making it the current calibration.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_apply( tobii_device_t* device,
        void const* data, size_t size );


### Remarks

This function is used in conjunction with the tobii_calibration_retrieve() function to re-apply a previously retrieved
binary data calibration blob.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*data* is the calibration data which has previously been retrieved by calling tobii_calibration_retrieve()

*size* is the size of the calibration data which has previously been retrieved by calling tobii_calibration_retrieve()


### Return value

If the operation is successful, tobii_calibration_apply returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_apply returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *data* parameters were passed in as NULL, or the *data_size* parameter was not greater than 0.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The provided calibration could not be applied to the device.

-   **TOBII_ERROR_CALIBRATION_BUSY**

    The device is currently being calibrated. tobii_calibration_apply can not be called while calibrating the device.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_apply from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also
[tobii_calibration_clear()](#tobii_calibration_clear),
[tobii_calibration_retrieve()](#tobii_calibration_retrieve),
[tobii_calibration_parse()](#tobii_calibration_parse)


### Example
@code{.c}

    #include <tobii/tobii_licensing.h>
    #include <tobii/tobii_config.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <memory.h>
    #include <assert.h>
    #include <string.h>

    static size_t read_file( uint16_t* buffer, char* file_path )
    {
        FILE *fp = fopen( file_path, "rb" );

        if( !fp )
        {
            printf( "File could not be found!\n" );
            return 0;
        }

        fseek( fp, 0, SEEK_END );
        long file_size = ftell( fp );
        rewind( fp );

        if( file_size <= 0 )
        {
            fclose( fp );
            printf( "Unable to read file!\n" );
            return 0;
        }

        if( buffer )
        {
            fread( buffer, sizeof( uint16_t ), file_size / sizeof( uint16_t ), fp );
        }

        fclose( fp );
        return (size_t)file_size;
    }

    tobii_error_t create_device( tobii_api_t* api, tobii_device_t** device, char* url )
    {
        tobii_error_t error;

        size_t license_size = read_file( 0, "se_license_key_sample");
        assert( license_size > 0 );

        uint16_t* license_key = (uint16_t*)malloc( license_size );
        memset( license_key, 0, license_size );
        read_file( license_key, "se_license_key_sample" );
        tobii_license_key_t license = { license_key, license_size };

        tobii_license_validation_result_t license_result;
        error = tobii_device_create_ex( api, url, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &license, 1, &license_result, device );
        free( license_key );
        license_key = 0;

        if( error == TOBII_ERROR_CONNECTION_FAILED )
        {
            printf( "Failed to connect to tracker.\n" );
            return error;
        }

        return error;
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = create_device( api, &device, url );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to create device!\n" );
            tobii_api_destroy( api );
            return error;
        }

        // See example for tobii_calibration_retrieve() on how to fetch and store the binary calibration blob

        size_t calibration_size = read_file( 0, "calibration_blob.bin" );
        assert( calibration_size > 0 );
        uint16_t* calibration_data = (uint16_t*)malloc( calibration_size );
        memset( calibration_data, 0, calibration_size );
        read_file( calibration_data, "calibration_blob.bin" );

        error = tobii_calibration_apply( device, calibration_data, calibration_size );
        free( calibration_data );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "Calibration was successfully applied!\n" );
        }
        else
        {
            printf( "Calibration apply failed!\n" );
        }

        tobii_device_destroy( device );
        tobii_api_destroy( api );

        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_geometry_mounting( tobii_device_t* device, tobii_geometry_mounting_t* geometry_mounting );
@ingroup tobii_config

tobii_get_geometry_mounting
---------------------------

### Function

Queries the mounting geometry of a remote eye tracker


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_geometry_mounting( tobii_device_t* device,
        tobii_geometry_mounting_t* geometry_mounting );


### Remarks

This information is needed when calculating the display area screen plane coordinates.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*geometry_mounting* must be a valid pointer to a tobii_geometry_mounting_t instance which will receive the result. The
tobii_geometry_mounting_t contains the following values:

-   *guides*

    Integer value containing the number of display alignment guide lines on a remote trackers. These guides should be
    duplicated in the display setup UI so they can be aligned with the physical guide grooves on the tracker.

-   *width_mm*

    Floating point value containing the width of a remote eye tracker in mm. If *guides* > 1, then the value contains
    the distance between two guides.

-   *angle_deg*

    Floating point value containing the tilt angle of a remote tracker in degrees.

-   *external_offset_mm_xyz*

    Floating point array containing the 3d-coordinate of the offset between the bottom center of the screen and the
    point at the top front (x = 0) of the eye tracker enclosure

-   *internal_offset_mm_xyz*

    Floating point array containing the 3d-coordinates of the offset along the y- and z-axis from the eye tracker center
    point (origin in the eye tracker coordinate system) to a point at the top front of the eye tracker enclosure, at the
    intersection of the centerline (x = 0).


### Return value

If the operation is successful, tobii_get_geometry_mounting returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_get_geometry_mounting returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *geometry_mounting* parameters were passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_geometry_mounting from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_calculate_display_area_basic()](#tobii_calculate_display_area_basic)


### Example

See [tobii_set_display_area()](#tobii_set_display_area).

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_display_area( tobii_device_t* device, tobii_display_area_t* display_area );
@ingroup tobii_config

tobii_get_display_area
----------------------

### Function

Retrieves the current display area coordinates from the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_display_area( tobii_device_t* device,
        tobii_display_area_t* display_area );


### Remarks

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*display_area* must be a valid pointer to a tobii_display_area_t instance which will receive the result.
tobii_display_area_t contains the following values:

-   *top_left_mm_xyz*

    Floating point array containing the 3d-coordinate of the top left screen plane.

-   *top_right_mm_xyz*

    Floating point array containing the 3d-coordinate of the top right screen plane.

-   *bottom_left_mm_xyz*

    Floating point array containing the 3d-coordinate of the bottom left screen plane.

### Return value

If the operation is successful, tobii_get_display_area returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_get_display_area returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *display_area* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The operation could not be performed at this time. Please wait a while and try again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_display_area from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_set_display_area()](#tobii_set_display_area),
[tobii_calculate_display_area_basic()](#tobii_calculate_display_area_basic)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy()
        // and on how to create/destroy a device using tobii_device_create/destroy().
        tobii_device_t* device = 0; // Dummy for demo purposes

        tobii_display_area_t display_area;
        tobii_error_t error = tobii_get_display_area( device, &display_area );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to retrieve display area!\n" );
            // destroy api and device, omitted here for brevity
            return error;
        }

        printf( "Currently configured display area:\n" );
        printf( "bottom left = [%f, %f, %f]\n",
            display_area.bottom_left_mm_xyz[0], display_area.bottom_left_mm_xyz[1], display_area.bottom_left_mm_xyz[2] );
        printf( "top left = [%f, %f, %f]\n",
            display_area.top_left_mm_xyz[0], display_area.top_left_mm_xyz[1], display_area.top_left_mm_xyz[2] );
        printf( "top right = [%f, %f, %f]\n",
            display_area.top_right_mm_xyz[0], display_area.top_right_mm_xyz[1], display_area.top_right_mm_xyz[2] );

        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_display_area( tobii_device_t* device, tobii_display_area_t const* display_area );
@ingroup tobii_config

tobii_set_display_area
----------------------

### Function

Configures the display area of the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_set_display_area( tobii_device_t* device,
        tobii_display_area_t const* display_area );


### Remarks

Configures the display area of the current eye tracker/monitor setup. It is defined by specifying the
three corner coordinates that makes up the screen plane, top left, lower left and top right. The helper function
tobii_calculate_display_area_basic() provides a convenient way of calculating the display area for basic eye
tracker/monitor setups.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*display_area* must be a valid pointer to a tobii_display_area_t which is correctly initialize, for example by calling
tobii_calculate_display_area_basic(). tobii_display_area_t contains the following values:

-   *top_left_mm_xyz*

    Floating point array containing the 3d-coordinate of the top left screen plane.

-   *top_right_mm_xyz*

    Floating point array containing the 3d-coordinate of the top right screen plane.

-   *bottom_left_mm_xyz*

    Floating point array containing the 3d-coordinate of the bottom left screen plane.

### Return value

If the operation is successful, tobii_set_display_area returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_set_display_area returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *display_area* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The operation could not be performed at this time. Please wait a while and try again.

-   **TOBII_ERROR_CALIBRATION_BUSY**

    The device is currently being calibrated. tobii_set_display_area can not be called while calibrating the device.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_set_display_area from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_get_display_area()](#tobii_get_display_area),
[tobii_calculate_display_area_basic()](#tobii_calculate_display_area_basic)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy(),
        // tobii_licensing.h on how to create a device using tobii_device_create_ex()
        // and tobii.h on how to destroy a device using tobii_device_destroy().
        tobii_api_t* api = 0; // Dummy for demo purposes
        tobii_device_t* device = 0; // Dummy for demo purposes

        tobii_geometry_mounting_t geometry_mounting;
        tobii_error_t error = tobii_get_geometry_mounting( device, &geometry_mounting );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to retrieve geometry mounting!\n" );
            // destroy api and device, omitted here for brevity
            return error;
        }

        // Screen geometry, usually obtained via a UI application
        float width_mm = 300.0f;
        float height_mm = 200.0f;
        float offset_x_mm = 10.0f;

        tobii_display_area_t display_area;
        error = tobii_calculate_display_area_basic( api, width_mm, height_mm, offset_x_mm, &geometry_mounting, &display_area );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to calculate display area!\n" );
            // destroy api and device, omitted here for brevity
            return error;
        }

        error = tobii_set_display_area( device, &display_area );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            printf( "Failed to calculate display area!\n" );
            // destroy api and device, omitted here for brevity
            return error;
        }

        printf( "Display area successfully configured!\n" );

        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calculate_display_area_basic( tobii_api_t* api, float width_mm, float height_mm, float offset_x_mm, tobii_geometry_mounting_t const* geometry_mounting, tobii_display_area_t* display_area );
@ingroup tobii_config

tobii_calculate_display_area_basic
----------------------------------

### Function

Calculates a basic display area configuration based on screen size and geometry mounting.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calculate_display_area_basic( tobii_api_t* api,
        float width_mm, float height_mm, float offset_x_mm,
        tobii_geometry_mounting_t const* geometry_mounting,
        tobii_display_area_t* display_area );


### Remarks

Calculate the display area for a basic eye tracker/monitor setup, where the eye tracker is situated just beneath the
screen plane, only offset from center screen along the x-axis. The resulting tobii_display_area_t struct can be used as
an input parameter to tobii_set_display_area().

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*width_mm* is the width of the display device in millimeters.

*height_mm* is the height of the display device in millimeters.

*offset_x* is the offset of the eye tracker from the center of the display device in the x-axis, in millimeters.

*geometry_mounting* is the geometry mounting information as received by tobii_get_geometry_mounting()

*display_area* must be a valid pointer to a tobii_display_area_t instance which will receive the result.


### Return value

If the operation is successful, tobii_calculate_display_area_basic returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calculate_display_area_basic returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *api*, *geometry_mounting* or *display_area* parameters was passed in as NULL.


### See also

[tobii_set_display_area()](#tobii_set_display_area),
[tobii_get_geometry_mounting()](#tobii_get_geometry_mounting)


### Example

See [tobii_set_display_area()](#tobii_set_display_area)

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_device_name( tobii_device_t* device, tobii_device_name_t* device_name );
@ingroup tobii_config

tobii_get_device_name
---------------------

### Function

Retrieves the custom name for a given eye tracker device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_device_name( tobii_device_t* device,
        tobii_device_name_t* device_name );


### Remarks

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*device_name*  must be a valid pointer to a tobii_device_name_t instance which will receive the result.


### Return value

If the operation is successful, tobii_get_device_name returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_get_device_name returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *device_name* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The operation could not be performed at this time. Please wait a while and try again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_device_name from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_set_device_name()](#tobii_set_device_name)


### Example

@code{.c}

    #include <tobii/tobii_config.h>
    #include <stdio.h>
    #include <assert.h>
    #include <string.h>

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = tobii_device_create( api, url, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &device );
        assert( error == TOBII_ERROR_NO_ERROR );

        tobii_device_name_t device_name;
        error = tobii_get_device_name( device, &device_name );
        if( error == TOBII_ERROR_NOT_SUPPORTED )
        {
            printf( "Get device name is not supported on this tracker \n" );
        }
        else
        {
            assert( error == TOBII_ERROR_NO_ERROR );
            printf( "Name of the device is: %s \n", device_name );
        }

        error = tobii_device_destroy( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_api_destroy( api );
        assert( error == TOBII_ERROR_NO_ERROR );
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_device_name( tobii_device_t* device, tobii_device_name_t const device_name );
@ingroup tobii_config

tobii_set_device_name
---------------------

### Function

Configures a custom name for a given eye tracker device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_set_device_name( tobii_device_t* device,
        tobii_device_name_t const device_name );


### Remarks

Configures a custom name for a given eye tracker device, to help distinguish between different eye trackers, i.e. in a
lab environment with multiple connected eye trackers.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*device_name* a tobii_device_name_t containing the name to be applied.


### Return value

If the operation is successful, tobii_set_device_name returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_set_device_name returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *device_name* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The operation could not be performed at this time. Please wait a while and try again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_set_device_name from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_get_device_name()](#tobii_get_device_name)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy(),
        // tobii_licensing.h on how to create a device using tobii_device_create_ex()
        // and tobii.h on how to destroy a device using tobii_device_destroy().
        tobii_device_t* device = 0; // Dummy for demo purposes

        tobii_device_name_t name = "Tobii Tracker";
        tobii_error_t error = tobii_set_device_name( device, name );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "Device name has been set!\n" );
        }
        else
        {
            printf( "Device name cold not be set!\n" );
        }

        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_enumerate_output_frequencies( tobii_device_t* device, tobii_output_frequency_receiver_t receiver, void* user_data );
@ingroup tobii_config

tobii_enumerate_output_frequencies
----------------------------------

### Function

Lists all valid output frequencies for the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_enumerate_output_frequencies( tobii_device_t* device,
        tobii_output_frequency_receiver_t receiver, void* user_data );


### Remarks

List all output frequencies supported by the advanced gaze data stream, which can then be used as input to
tobii_set_output_frequency().

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*receiver* is a function pointer to a function with the prototype:

    void receiver( ( float output_frequency, void* user_data ) )

This function will be called for each available output frequency. It is called with the following parameters:

-   *output_frequency*
    The frequency in Hz.

-   *user_data*
    This is the custom pointer sent in to tobii_enumerate_output_frequencies.

*user_data* custom pointer which will be passed unmodified to the receiver function.


### Return value

If the operation is successful, tobii_enumerate_output_frequencies returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_enumerate_output_frequencies returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *receiver* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_geometry_mounting from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_set_output_frequency()](#tobii_set_output_frequency),
[tobii_get_output_frequency()](#tobii_get_output_frequency)


### Example

@code{.c}

    #include <stdio.h>
    #include <stdlib.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    typedef struct output_frequency_context_t
    {
        float output_frequencies[ 10 ];
        size_t count;
    } output_frequency_context_t;

    void output_frequency_receiver( float output_frequency, void* user_data )
    {
        if ( user_data == NULL ) return;

        output_frequency_context_t* output_frequency_context = (output_frequency_context_t*)user_data;
        output_frequency_context->output_frequencies[ output_frequency_context->count++ ] = output_frequency;
    }

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy()
        // and on how to create/destroy a device using tobii_device_create/destroy().
        tobii_device_t* device = 0; // Dummy for demo purposes

        output_frequency_context_t output_frequency_context = {0};
        tobii_error_t error = tobii_enumerate_output_frequencies( device, output_frequency_receiver,
            &output_frequency_context );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "* This tracker supports %zu output frequencies: ", output_frequency_context.count );

            for( size_t i = 0; i < output_frequency_context.count; i++ )
            {
                printf( "%f%c " , output_frequency_context.output_frequencies [ i ],
                    i + 1 < output_frequency_context.count ? ',' : '.' );
            }
        }
        else
        {
            printf( "* Failed to get supported output frequencies for this tracker.\n" );
        }

        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_output_frequency( tobii_device_t* device, float output_frequency );
@ingroup tobii_config

tobii_set_output_frequency
--------------------------

### Function

Configures the output frequency of the advanced gaze data stream.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_set_output_frequency( tobii_device_t* device,
        float output_frequency );


### Remarks

Configures the output frequency of the advanced gaze data stream, i.e. how many gaze data samples will be sent to the
client per second, the supported output frequencies can be listed by calling tobii_enumerate_output_frequency().

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*output_frequency* the frequency to apply.


### Return value

If the operation is successful, tobii_set_output_frequency returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_set_output_frequency returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameters were passed in as NULL, or *output_frequency* is not valid.
    To list all valid output frequencies for the device, use the tobii_enumerate_output_frequencies() function.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The function failed because of the one of following reasons:
    - the function was called while the device was in calibration mode;
    - the function was called while some user of the device was subscribed to a stream that does not support the new
    output frequency.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_set_output_frequency from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_get_output_frequency()](#tobii_get_output_frequency),
[tobii_enumerate_output_frequencies()](#tobii_enumerate_output_frequencies)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy(),
        // tobii_licensing.h on how to create a device using tobii_device_create_ex()
        // and tobii.h on how to destroy a device using tobii_device_destroy().
        tobii_device_t* device = 0; // Dummy for demo purposes

        float output_frequency = 33.0f;
        tobii_error_t error = tobii_set_output_frequency( device, output_frequency );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "Output frequency set!\n" );
        }
        else
        {
            printf( "Setting output frequency failed\n" );
        }

        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_output_frequency( tobii_device_t* device, float* output_frequency );
@ingroup tobii_config

tobii_get_output_frequency
--------------------------

### Function

Query the current output frequency of the advanced gaze data stream.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_output_frequency( tobii_device_t* device,
        float* output_frequency );


### Remarks

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*output_frequency* is a valid pointer to a float which will receive the current output frequency.

### Return value

If the operation is successful, tobii_get_output_frequency returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_get_output_frequency returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *output_frequency* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The operation could not be performed at this time. Please wait a while and try again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_output_frequency from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_set_output_frequency()](#tobii_set_output_frequency),
[tobii_enumerate_output_frequencies()](#tobii_enumerate_output_frequencies)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy()
        // and on how to create/destroy a device using tobii_device_create/destroy().
        tobii_device_t* device = 0; // Dummy for demo purposes

        float output_frequency = 0.0f;
        tobii_error_t error = tobii_get_output_frequency( device, &output_frequency );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "* Current output frequency is %f Hz\n", output_frequency );
        }
        else
        {
            printf( "Get output frequency failed\n" );
        }

        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_output_frequency_subscribe( tobii_device_t* device, tobii_output_frequency_receiver_t callback, void* user_data );
@ingroup tobii_config

tobii_output_frequency_subscribe
--------------------------------

### Function

Start listening for changes to the output frequency property.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_output_frequency_subscribe( tobii_device_t* device,
        tobii_output_frequency_receiver_t callback, void* user_data );


### Remarks

This subscription is for receiving notifications on changes to the output frequency property.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*callback* is a function pointer to a function with the prototype:

    void tobii_output_frequency_receiver( const float output_frequency, void* user_data )

This function will be called when the output frequency property has changed. It is called with the following parameters:

-   *output_frequency*

    This is float data containing the output frequency.

-   *user_data*
    This is a pointer to custom user data sent in when registering the callback in the call to
    tobii_output_frequency_subscribe().

*user_data* pointer to custom user data, which will be passed unmodified to the callback function.


### Return value

If the operation is successful, tobii_output_frequency_subscribe returns **TOBII_ERROR_NO_ERROR**. If the call
fails, tobii_output_frequency_subscribe returns one of the following error codes:

-   **TOBII_ERROR_INVALID_PARAMETER**

    One or more of the *device* and *callback* parameters were passed in as NULL. *device* must be a valid
    tobii_device_t pointer as created by tobii_device_create, and *callback* must be a valid pointer to a
    tobii_output_frequency_receiver_t function.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not valid, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The functionality is not supported on this device.

-   **TOBII_ERROR_ALREADY_SUBSCRIBED**

    A subscription for output frequency property change notifications were already made. There can only be one callback
    registered at a time. To change to another callback, first call tobii_output_frequency_unsubscribe().

-   **TOBII_ERROR_OPERATION_FAILED**

    The output frequency property change notification subscribe operation failed.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    This function is called from a tobii call back function which is not allowed.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_output_frequency_unsubscribe()](#tobii_output_frequency_unsubscribe),
[tobii_wait_for_callbacks()](#tobii_wait_for_callbacks),
[tobii_device_process_callbacks()](#tobii_device_process_callbacks)


### Example

@code{.c}

    #include <tobii/tobii_streams.h>
    #include <tobii/tobii_config.h>
    #include <stdio.h>
    #include <assert.h>
    #include <string.h>

    void output_frequency_callback( float output_frequency, void* user_data )
    {
        (void)user_data;
        printf( "Output frequency: %f\n", output_frequency );
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = tobii_device_create( api, url, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_output_frequency_subscribe( device, output_frequency_callback, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        int is_running = 1000; // in this sample, exit after some iterations
        while( --is_running > 0 )
        {
            error = tobii_wait_for_callbacks( 1, &device );
            assert( error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT );

            error = tobii_device_process_callbacks( device );
            assert( error == TOBII_ERROR_NO_ERROR );
        }

        error = tobii_output_frequency_unsubscribe( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_device_destroy( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_api_destroy( api );
        assert( error == TOBII_ERROR_NO_ERROR );
        return 0;
    }

@endcode
*/


/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_output_frequency_unsubscribe( tobii_device_t* device );
@ingroup tobii_config

tobii_output_frequency_unsubscribe
----------------------------------

### Function

Stops listening to notifications on changes to output frequency property that was subscribed to by a call to tobii_output_frequency_subscribe()


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_output_frequency_unsubscribe( tobii_device_t* device );


### Remarks

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.


### Return value

If the operation is successful, tobii_output_frequency_unsubscribe returns **TOBII_ERROR_NO_ERROR**. If the call
fails, tobii_output_frequency_unsubscribe returns an error code specific to the device.

### See also

[tobii_output_frequency_subscribe()](#tobii_output_frequency_subscribe)


### Example

See [tobii_output_frequency_subscribe()](#tobii_output_frequency_subscribe)

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_display_id( tobii_device_t* device, tobii_display_id_t* display_id );
@ingroup tobii_config

tobii_get_display_id
----------------------

### Function

Retrieves the current display id from the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_display_id( tobii_device_t* device,
        tobii_display_id_t* display_id );


### Remarks

The format of the display id is application specific and transparent to stream engine.
Stream engine does not impose that any specific format be used.
Hence, encoding and decoding the display id is up to the producer and the consumer to decide.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*display_id* must be a valid pointer to a tobii_display_id_t instance which will receive the result.
The maximum size of the returned tobii_display_id is 256 characters and it is '\0'-terminated.
The default value is all '\0' characters, meaning that the display id has not yet been set.


### Return value

If the operation is successful, tobii_get_display_id returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_get_display_id returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *display_id* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The operation could not be performed at this time. Please wait a while and try again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_display_id from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_set_display_id()](#tobii_set_display_id)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy()
        // and on how to create/destroy a device using tobii_device_create/destroy().
        tobii_device_t* device = 0; // Dummy for demo purposes

        tobii_display_id_t display_id;
        tobii_error_t error = tobii_get_display_id( device, &display_id );
        if( error != TOBII_ERROR_NO_ERROR )
        {
            // destroy api and device, omitted here for brevity
            printf( "Retrieving display id failed!\n" );
            return error;
        }
        printf( "Display ID = %s\n", display_id );
        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_display_id( tobii_device_t* device, tobii_display_id_t const display_id );
@ingroup tobii_config

tobii_set_display_id
----------------------

### Function

Applies the specified display area setting to the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_set_display_id( tobii_device_t* device,
        tobii_display_id_t const* display_id );


### Remarks

The format of the display id is application specific and transparent to stream engine.
Stream engine does not impose that any specific format be used.
Hence, encoding and decoding the display id is up to the producer and the consumer to decide.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*display_id* a valid tobii_display_id_t containing the display id to be applied.
The maximum size of the device id is 256 characters and it shall be '\0'-terminated.


### Return value

If the operation is successful, tobii_set_display_id returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_set_display_id returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *display_id* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The function failed because the operation is not supported by the connected tracker.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The operation could not be performed at this time. Please wait a while and try again.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_set_display_id from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_get_display_id()](#tobii_get_display_id)


### Example

@code{.c}

    #include <stdio.h>
    #include <tobii/tobii.h>
    #include <tobii/tobii_config.h>

    int main()
    {
        // See tobii.h for examples on how to create/destroy an api using tobii_api_create/destroy(),
        // tobii_licensing.h on how to create a device using tobii_device_create_ex()
        // and tobii.h on how to destroy a device using tobii_device_destroy().
        tobii_device_t* device = 0; // Dummy for demo purposes

        tobii_display_id_t display_id = "DUMMY_ID_123";
        tobii_error_t error = tobii_set_display_id( device, display_id );
        if( error == TOBII_ERROR_NO_ERROR )
        {
            printf( "Display ID has been set!\n" );
        }
        else
        {
            printf( "Display ID cold not be set!\n" );
        }

        // destroy api and device, omitted here for brevity
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_display_id_subscribe( tobii_device_t* device, tobii_display_id_callback_t callback, void* user_data );
@ingroup tobii_config

tobii_display_id_subscribe
--------------------------

### Function

Start listening for changes to the display ID property.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_display_id_subscribe( tobii_device_t* device,
        tobii_display_id_callback_t callback, void* user_data );


### Remarks

This subscription is for receiving notifications on changes to the display ID property.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*callback* is a function pointer to a function with the prototype:

    void tobii_display_id_callback( tobii_display_id_t const* display_id, void* user_data )

This function will be called when the display id property has changed. It is called with the following parameters:

-   *display_id*

    This is a pointer to a char array of size 256 containing the '\0'-terminated device id.

-   *user_data*
    This is a pointer to custom user data sent in when registering the callback in the call to tobii_display_id_subscribe().

*user_data* pointer to custom user data, which will be passed unmodified to the callback function.


### Return value

If the operation is successful, tobii_display_id_subscribe returns **TOBII_ERROR_NO_ERROR**. If the call
fails, tobii_display_id_subscribe returns one of the following error codes:

-   **TOBII_ERROR_INVALID_PARAMETER**

    One or more of the *device* and *callback* parameters were passed in as NULL. *device* must be a valid
    tobii_device_t pointer as created by tobii_device_create, and *callback* must be a valid pointer to a
    tobii_display_id_callback_t function.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not valid, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The functionality is not supported on this device.

-   **TOBII_ERROR_ALREADY_SUBSCRIBED**

    A subscription for display id property change notifications were already made. There can only be one callback registered at a time.
    To change to another callback, first call tobii_display_id_unsubscribe().

-   **TOBII_ERROR_OPERATION_FAILED**

    The display id property change notification subscribe operation failed.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    This function is called from a tobii call back function which is not allowed.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### See also

[tobii_display_id_unsubscribe()](#tobii_display_id_unsubscribe),
[tobii_wait_for_callbacks()](#tobii_wait_for_callbacks),
[tobii_device_process_callbacks()](#tobii_device_process_callbacks)


### Example

@code{.c}

    #include <tobii/tobii_streams.h>
    #include <tobii/tobii_config.h>
    #include <stdio.h>
    #include <assert.h>
    #include <string.h>

    void display_id_callback( tobii_display_id_t const display_id, void* user_data )
    {
        (void)user_data;
        printf( "Display id: %s\n", display_id );
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = tobii_device_create( api, url, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_display_id_subscribe( device, display_id_callback, 0 );
        assert( error == TOBII_ERROR_NO_ERROR );

        int is_running = 1000; // in this sample, exit after some iterations
        while( --is_running > 0 )
        {
            error = tobii_wait_for_callbacks( 1, &device );
            assert( error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT );

            error = tobii_device_process_callbacks( device );
            assert( error == TOBII_ERROR_NO_ERROR );
        }

        error = tobii_display_id_unsubscribe( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_device_destroy( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_api_destroy( api );
        assert( error == TOBII_ERROR_NO_ERROR );
        return 0;
    }

@endcode

*/


/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_display_id_unsubscribe( tobii_device_t* device );
@ingroup tobii_config

tobii_display_id_unsubscribe
----------------------------

### Function

Stops listening to notifications on changes to display id property that was subscribed to by a call to tobii_display_id_subscribe()


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_display_id_unsubscribe( tobii_device_t* device );


### Remarks

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.


### Return value

If the operation is successful, tobii_display_id_unsubscribe returns **TOBII_ERROR_NO_ERROR**. If the call
fails, tobii_display_id_unsubscribe returns an error code specific to the device.

### See also

[tobii_display_id_subscribe()](#tobii_display_id_subscribe)


### Example

See [tobii_display_id_subscribe()](#tobii_display_id_subscribe)

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_stimulus_points_get( tobii_device_t* device,tobii_calibration_stimulus_points_t* stimulus_points );
@ingroup tobii_config

tobii_calibration_stimulus_points_get
--------------------------

### Function

Retrieves information about the stimulus points used to compute the personal calibration currently in use.

### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_stimulus_points_get( tobii_device_t* device,
        tobii_calibration_stimulus_points_t* stimulus_points  );


### Remarks

A personal calibration is based on gaze data collected for a number of fixed stimuli points with a known position on the display plane (remote eye trackers) or in space (wearable eye trackers).
This function can be used to retrieve information about the personal calibration currently applied on the eye tracker. The information includes information about what stimuli points that were
used when doing the personal calibration, as well both an offset in each point and the precision of the gaze data in each point. The values can be used as an indication how well the eye tracker
was able to track the user in the area around the stimuli point position.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create with a valid config level license.

*stimulus_points* is a valid pointer to a tobii_calibration_stimulus_points_t instance to receive the information. It constains the following fields:

   -   *stimulus_point_count* is the number of tobii_calibration_stimulus_point_data_t.

   -   *stimulus_points* one array of tobii_calibration_stimulus_point_data_t, which contains the following information:

       -  *point_xyz*
          It is the calibration stimulus point coordinate specified during calibration, for wearable trackers this coordinate relates to a point in 3d space,
          for remote trackers the coordinate relates to a point where the x and y coordinates are on the screen plane, where z-axis is the offset from the screen plane in
          the direction of the normal vector.

       -  *left_status/right_status*
           The status of this stimuli point, includes information whether this stimulus point was used for setting the personal calibration and the type of the problem that
           may be the cause for it not to be used. It is one of tobii_calibration_stimulus_point_status_t enum value:

            **TOBII_CALIBRATION_STIMULUS_POINT_STATUS_FAILED_OR_INVALID**
              No gaze data could be calculated for this stimulus point.

            **TOBII_CALIBRATION_STIMULUS_POINT_STATUS_VALID_NOT_USED**
              Gaze data could be calculated for this stimulus point, but the data in this point was not used for personal calibration. This can happen for example if the user is
              not fixating at the point or if the user is looking at some point of the screen.

            **TOBII_CALIBRATION_STIMULUS_POINT_STATUS_VALID_USED*
              This stimuli point is used in the calibration.

       -  *left_bias/right_bias*
          The distance between stimulus point and mean of the gaze point that was collected during personal calibration. For a remote eye tracker this will be a value in
          mm in the display plane, for wearable trackers it will be an angle in degrees.

       -  *left_precision/right_precision*
          The standard deviation of the difference between the gaze vectors and the mean gaze. For remote trackers this the distance in mm in the display will be used and
          for wearable trackers, angular differences will be used to calculate the standard deviation.



### Return value

If the operation is successful, tobii_calibration_stimulus_points_get returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_calibration_stimulus_points_get returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *stimulus_points* parameters were passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blocklisted.

-   **TOBII_ERROR_NOT_SUPPORTED**

    Getting calibration stimulus points property is not supported by the device.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_stimulus_points_get from within a callback function is not supported.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.


### Example

@code{.c}

    #include "tobii/tobii.h"
    #include <tobii/tobii_config.h>
    #include <assert.h>
    #include <stdio.h>

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );
        tobii_device_t* device;
        error = tobii_device_create( api, NULL, TOBII_FIELD_OF_USE_STORE_OR_TRANSFER_FALSE, &device );
        assert( error == TOBII_ERROR_NO_ERROR );

        tobii_calibration_stimulus_points_t stimulus_points;

        error = tobii_calibration_stimulus_points_get( device, &stimulus_points );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_device_destroy( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_api_destroy( api );
        assert( error == TOBII_ERROR_NO_ERROR );
        return 0;

    }

@endcode
*/


#include <stdlib.h>
#include <string.h>
#include <pcilib.h>
#include "uca.h"
#include "uca-cam.h"
#include "uca-grabber.h"

#define set_void(p, type, value) { *((type *) p) = value; }

static uint32_t uca_ipe_acquire_image(struct uca_camera_t *cam, void *buffer)
{
    return UCA_NO_ERROR;
}

static uint32_t uca_ipe_set_property(struct uca_camera_t *cam, enum uca_property_ids property, void *data)
{
    return UCA_NO_ERROR;
}

static uint32_t uca_ipe_get_property(struct uca_camera_t *cam, enum uca_property_ids property, void *data)
{
    return UCA_NO_ERROR;
}

uint32_t uca_ipe_start_recording(struct uca_camera_t *cam)
{
    return UCA_NO_ERROR;
}

uint32_t uca_ipe_stop_recording(struct uca_camera_t *cam)
{
    return UCA_NO_ERROR;
}

uint32_t uca_ipe_grab(struct uca_camera_t *cam, char *buffer)
{
    return UCA_NO_ERROR;
}

static uint32_t uca_ipe_destroy(struct uca_camera_t *cam)
{
    return UCA_NO_ERROR;
}

uint32_t uca_ipe_init(struct uca_camera_t **cam, struct uca_grabber_t *grabber)
{
    /* Camera found, set function pointers... */
    uca->destroy = &uca_ipe_destroy;
    uca->set_property = &uca_ipe_set_property;
    uca->get_property = &uca_ipe_get_property;
    uca->start_recording = &uca_ipe_start_recording;
    uca->stop_recording = &uca_ipe_stop_recording;
    uca->grab = &uca_ipe_grab;

    uca->state = UCA_CAM_CONFIGURABLE;
    *cam = uca;

    return UCA_NO_ERROR;
}

/* Copyright (C) 2011, 2012 Matthias Vogelgesang <matthias.vogelgesang@kit.edu>
   (Karlsruhe Institute of Technology)

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by the
   Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
   details.

   You should have received a copy of the GNU Lesser General Public License along
   with this library; if not, write to the Free Software Foundation, Inc., 51
   Franklin St, Fifth Floor, Boston, MA 02110, USA */

#include <glib.h>
#include "uca-camera.h"

#define UCA_CAMERA_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), UCA_TYPE_CAMERA, UcaCameraPrivate))

G_DEFINE_TYPE(UcaCamera, uca_camera, G_TYPE_OBJECT)

/**
 * UcaCameraError:
 * @UCA_CAMERA_ERROR_RECORDING: Camera is already recording
 * @UCA_CAMERA_ERROR_NOT_RECORDING: Camera is not recording
 */
GQuark uca_camera_error_quark()
{
    return g_quark_from_static_string("uca-camera-error-quark");
}

enum {
    PROP_0 = 0,
    PROP_SENSOR_WIDTH,
    PROP_SENSOR_HEIGHT,
    PROP_SENSOR_BITDEPTH,
    PROP_SENSOR_HORIZONTAL_BINNING,
    PROP_SENSOR_VERTICAL_BINNING,
    N_PROPERTIES
};

struct _UcaCameraPrivate {
    gboolean recording;
};

static GParamSpec *uca_camera_properties[N_PROPERTIES] = { NULL, };

static void uca_camera_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
}

static void uca_camera_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
}

static void uca_camera_class_init(UcaCameraClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->set_property = uca_camera_set_property;
    gobject_class->get_property = uca_camera_get_property;

    klass->start_recording = NULL;
    klass->stop_recording = NULL;
    klass->grab = NULL;

    uca_camera_properties[PROP_SENSOR_WIDTH] = 
        g_param_spec_uint("sensor-width",
            "Width of sensor",
            "Width of the sensor in pixels",
            1, G_MAXUINT, 1,
            G_PARAM_READABLE);

    uca_camera_properties[PROP_SENSOR_HEIGHT] = 
        g_param_spec_uint("sensor-height",
            "Height of sensor",
            "Height of the sensor in pixels",
            1, G_MAXUINT, 1,
            G_PARAM_READABLE);

    uca_camera_properties[PROP_SENSOR_BITDEPTH] = 
        g_param_spec_uint("sensor-bitdepth",
            "Number of bits per pixel",
            "Number of bits per pixel",
            1, 32, 1,
            G_PARAM_READABLE);

    uca_camera_properties[PROP_SENSOR_HORIZONTAL_BINNING] = 
        g_param_spec_uint("sensor-horizontal-binning",
            "Horizontal binning",
            "Number of sensor ADCs that are combined to one pixel in horizontal direction",
            1, G_MAXUINT, 1,
            G_PARAM_READWRITE);

    uca_camera_properties[PROP_SENSOR_VERTICAL_BINNING] = 
        g_param_spec_uint("sensor-vertical-binning",
            "Vertical binning",
            "Number of sensor ADCs that are combined to one pixel in vertical direction",
            1, G_MAXUINT, 1,
            G_PARAM_READWRITE);

    for (guint id = PROP_0 + 1; id < N_PROPERTIES; id++)
        g_object_class_install_property(gobject_class, id, uca_camera_properties[id]);

    g_type_class_add_private(klass, sizeof(UcaCameraPrivate));
}

static void uca_camera_init(UcaCamera *camera)
{
    camera->priv = UCA_CAMERA_GET_PRIVATE(camera);
    camera->priv->recording = FALSE;
}

void uca_camera_start_recording(UcaCamera *camera, GError **error)
{
    g_return_if_fail(UCA_IS_CAMERA(camera));

    UcaCameraClass *klass = UCA_CAMERA_GET_CLASS(camera);

    g_return_if_fail(klass != NULL);
    g_return_if_fail(klass->start_recording != NULL);

    if (camera->priv->recording) {
        g_set_error(error, UCA_CAMERA_ERROR, UCA_CAMERA_ERROR_RECORDING,
                "Camera is already recording");
        return;
    }

    camera->priv->recording = TRUE;
    (*klass->start_recording)(camera, error);
}

void uca_camera_stop_recording(UcaCamera *camera, GError **error)
{
    g_return_if_fail(UCA_IS_CAMERA(camera));

    UcaCameraClass *klass = UCA_CAMERA_GET_CLASS(camera);

    g_return_if_fail(klass != NULL);
    g_return_if_fail(klass->stop_recording != NULL);

    if (!camera->priv->recording) {
        g_set_error(error, UCA_CAMERA_ERROR, UCA_CAMERA_ERROR_NOT_RECORDING,
                "Camera is not recording");
        return;
    }

    camera->priv->recording = FALSE;
    (*klass->stop_recording)(camera, error);
}

void uca_camera_grab(UcaCamera *camera, gchar *data, GError **error)
{
    g_return_if_fail(UCA_IS_CAMERA(camera));

    UcaCameraClass *klass = UCA_CAMERA_GET_CLASS(camera);

    g_return_if_fail(klass != NULL);
    g_return_if_fail(klass->grab != NULL);

    if (!camera->priv->recording) {
        g_set_error(error, UCA_CAMERA_ERROR, UCA_CAMERA_ERROR_NOT_RECORDING,
                "Camera is not recording");
        return;
    }

    (*klass->grab)(camera, data, error);
}


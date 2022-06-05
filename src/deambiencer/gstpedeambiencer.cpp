/*
 *  Copyright © 2017-2022 Wellington Wallace
 *  Copyright © 2022 Roman Lebedev
 *
 *  This file is part of PulseEffects.
 *
 *  PulseEffects is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PulseEffects is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PulseEffects.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * SECTION:element-gstpedeambiencer
 *
 * The pedeambiencer element is an instantenious gate.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v audiotestsrc ! pedeambiencer ! pulsesink
 * ]|
 * The pedeambiencer element is an instantenious gate.
 * </refsect2>
 */

#include "gstpedeambiencer.hpp"
#include <gst/audio/gstaudiofilter.h>
#include <gst/gst.h>
#include "config.h"
#include "util.hpp"

GST_DEBUG_CATEGORY_STATIC(gst_pedeambiencer_debug_category);
#define GST_CAT_DEFAULT gst_pedeambiencer_debug_category

/* prototypes */

static void gst_pedeambiencer_set_property(GObject* object,
                                           guint property_id,
                                           const GValue* value,
                                           GParamSpec* pspec);

static void gst_pedeambiencer_get_property(GObject* object,
                                           guint property_id,
                                           GValue* value,
                                           GParamSpec* pspec);

static auto gst_pedeambiencer_setup(GstAudioFilter* filter,
                                    const GstAudioInfo* info) -> gboolean;

static auto gst_pedeambiencer_transform_ip(GstBaseTransform* trans,
                                           GstBuffer* buffer) -> GstFlowReturn;

static void gst_pedeambiencer_process(GstPedeambiencer* pedeambiencer,
                                      GstBuffer* buffer);

enum {
  PROP_THRESHOLD_LEVEL = 1,
};

/* pad templates */

static GstStaticPadTemplate gst_pedeambiencer_src_template =
    GST_STATIC_PAD_TEMPLATE(
        "src",
        GST_PAD_SRC,
        GST_PAD_ALWAYS,
        GST_STATIC_CAPS("audio/x-raw,format=F32LE,rate=[1,max],"
                        "channels=2,layout=interleaved"));

static GstStaticPadTemplate gst_pedeambiencer_sink_template =
    GST_STATIC_PAD_TEMPLATE(
        "sink",
        GST_PAD_SINK,
        GST_PAD_ALWAYS,
        GST_STATIC_CAPS("audio/x-raw,format=F32LE,rate=[1,max],"
                        "channels=2,layout=interleaved"));

/* class initialization */

G_DEFINE_TYPE_WITH_CODE(
    GstPedeambiencer,
    gst_pedeambiencer,
    GST_TYPE_AUDIO_FILTER,
    GST_DEBUG_CATEGORY_INIT(gst_pedeambiencer_debug_category,
                            "pedeambiencer",
                            0,
                            "debug category for pedeambiencer element"));

static void gst_pedeambiencer_class_init(GstPedeambiencerClass* klass) {
  GObjectClass* gobject_class = G_OBJECT_CLASS(klass);

  GstBaseTransformClass* base_transform_class = GST_BASE_TRANSFORM_CLASS(klass);

  GstAudioFilterClass* audio_filter_class = GST_AUDIO_FILTER_CLASS(klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */

  gst_element_class_add_static_pad_template(GST_ELEMENT_CLASS(klass),
                                            &gst_pedeambiencer_src_template);
  gst_element_class_add_static_pad_template(GST_ELEMENT_CLASS(klass),
                                            &gst_pedeambiencer_sink_template);

  gst_element_class_set_static_metadata(
      GST_ELEMENT_CLASS(klass), "PulseEffects deambiencer", "Generic",
      "PulseEffects deambiencer", "Roman Lebedev <lebedev.ri@gmail.com>");

  /* define virtual function pointers */

  gobject_class->set_property = gst_pedeambiencer_set_property;
  gobject_class->get_property = gst_pedeambiencer_get_property;

  audio_filter_class->setup = GST_DEBUG_FUNCPTR(gst_pedeambiencer_setup);
  base_transform_class->transform_ip =
      GST_DEBUG_FUNCPTR(gst_pedeambiencer_transform_ip);
  base_transform_class->transform_ip_on_passthrough = false;

  /* define properties */

  g_object_class_install_property(
      gobject_class, PROP_THRESHOLD_LEVEL,
      g_param_spec_float("threshold-level", "Threshold Level",
                         "Threshold ambient level (in dB)", -1528.0F, 0.0F,
                         -60.0F,
                         static_cast<GParamFlags>(G_PARAM_READWRITE |
                                                  G_PARAM_STATIC_STRINGS)));
}

static void gst_pedeambiencer_init(GstPedeambiencer* pedeambiencer) {
  pedeambiencer->bpf = 0;
  pedeambiencer->threshold_level = -60.0F;  // dB

  gst_base_transform_set_in_place(GST_BASE_TRANSFORM(pedeambiencer), true);
}

void gst_pedeambiencer_set_property(GObject* object,
                                    guint property_id,
                                    const GValue* value,
                                    GParamSpec* pspec) {
  GstPedeambiencer* pedeambiencer = GST_PEdeambiencer(object);

  GST_DEBUG_OBJECT(pedeambiencer, "set_property");

  switch (property_id) {
    case PROP_THRESHOLD_LEVEL:
      pedeambiencer->threshold_level = g_value_get_float(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
  }
}

void gst_pedeambiencer_get_property(GObject* object,
                                    guint property_id,
                                    GValue* value,
                                    GParamSpec* pspec) {
  GstPedeambiencer* pedeambiencer = GST_PEdeambiencer(object);

  GST_DEBUG_OBJECT(pedeambiencer, "get_property");

  switch (property_id) {
    case PROP_THRESHOLD_LEVEL:
      g_value_set_float(value, pedeambiencer->threshold_level);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
  }
}

static auto gst_pedeambiencer_setup(GstAudioFilter* filter,
                                    const GstAudioInfo* info) -> gboolean {
  GstPedeambiencer* pedeambiencer = GST_PEdeambiencer(filter);

  GST_DEBUG_OBJECT(pedeambiencer, "setup");

  pedeambiencer->bpf = info->bpf;

  return true;
}

static auto gst_pedeambiencer_transform_ip(GstBaseTransform* trans,
                                           GstBuffer* buffer) -> GstFlowReturn {
  GstPedeambiencer* pedeambiencer = GST_PEdeambiencer(trans);

  GST_DEBUG_OBJECT(pedeambiencer, "transform");

  gst_pedeambiencer_process(pedeambiencer, buffer);

  return GST_FLOW_OK;
}

static void gst_pedeambiencer_process(GstPedeambiencer* pedeambiencer,
                                      GstBuffer* buffer) {
  GstMapInfo map;

  gst_buffer_map(buffer, &map, GST_MAP_READWRITE);

  auto* data = reinterpret_cast<float*>(map.data);

  guint num_samples = map.size / pedeambiencer->bpf;

  const float cutoff_linear =
      util::db_to_linear(pedeambiencer->threshold_level);

  for (unsigned int n = 0U; n < 2U * num_samples; n++) {
    auto& sample = data[n];
    sample = sample < cutoff_linear ? 0 : sample;
  }

  gst_buffer_unmap(buffer, &map);
}

static gboolean plugin_init(GstPlugin* plugin) {
  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register(plugin, "pedeambiencer", GST_RANK_NONE,
                              GST_TYPE_PEdeambiencer);
}

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR,
                  GST_VERSION_MINOR,
                  pedeambiencer,
                  "PulseEffects deambiencer",
                  plugin_init,
                  VERSION,
                  "LGPL",
                  PACKAGE,
                  "https://github.com/wwmm/pulseeffects")

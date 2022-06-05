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

#ifndef GST_PEdeambiencer_H
#define GST_PEdeambiencer_H

#include <gst/audio/gstaudiofilter.h>

G_BEGIN_DECLS

#define GST_TYPE_PEdeambiencer (gst_pedeambiencer_get_type())
#define GST_PEdeambiencer(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_PEdeambiencer, GstPedeambiencer))
#define GST_PEdeambiencer_CLASS(klass)                      \
  (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_PEdeambiencer, \
                           GstPedeambiencerClass))
#define GST_IS_PEdeambiencer(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_PEdeambiencer))
#define GST_IS_PEdeambiencer_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_PEdeambiencer))

struct GstPedeambiencer {
  GstAudioFilter base_pedeambiencer;

  /* properties */

  float threshold_level;  // ambient loudness level (dB)

  /* < private > */

  int bpf;  // bytes per frame : channels * bps
};

struct GstPedeambiencerClass {
  GstAudioFilterClass base_pedeambiencer_class;
};

GType gst_pedeambiencer_get_type(void);

G_END_DECLS

#endif

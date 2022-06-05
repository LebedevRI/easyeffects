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

#include "deambiencer_preset.hpp"

DeAmbiencerPreset::DeAmbiencerPreset()
    : input_settings(Gio::Settings::create(
          "com.github.wwmm.pulseeffects.deambiencer",
          "/com/github/wwmm/pulseeffects/sourceinputs/deambiencer/")),
      output_settings(Gio::Settings::create(
          "com.github.wwmm.pulseeffects.deambiencer",
          "/com/github/wwmm/pulseeffects/sinkinputs/deambiencer/")) {}

void DeAmbiencerPreset::save(boost::property_tree::ptree& root,
                             const std::string& section,
                             const Glib::RefPtr<Gio::Settings>& settings) {
  root.put(section + ".deambiencer.state", settings->get_boolean("state"));

  root.put(section + ".deambiencer.threshold_level",
           settings->get_double("threshold_level"));
}

void DeAmbiencerPreset::load(const boost::property_tree::ptree& root,
                             const std::string& section,
                             const Glib::RefPtr<Gio::Settings>& settings) {
  update_key<bool>(root, settings, "state", section + ".deambiencer.state");

  update_key<double>(root, settings, "threshold_level",
                     section + ".deambiencer.threshold_level");
}

void DeAmbiencerPreset::write(PresetType preset_type,
                              boost::property_tree::ptree& root) {
  switch (preset_type) {
    case PresetType::output:
      save(root, "output", output_settings);
      break;
    case PresetType::input:
      save(root, "input", input_settings);
      break;
  }
}

void DeAmbiencerPreset::read(PresetType preset_type,
                             const boost::property_tree::ptree& root) {
  switch (preset_type) {
    case PresetType::output:
      load(root, "output", output_settings);
      break;
    case PresetType::input:
      load(root, "input", input_settings);
      break;
  }
}

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

#ifndef DEAMBIENCER_PRESET_HPP
#define DEAMBIENCER_PRESET_HPP

#include "plugin_preset_base.hpp"

class DeAmbiencerPreset : public PluginPresetBase {
 public:
  DeAmbiencerPreset();

  void write(PresetType preset_type,
             boost::property_tree::ptree& root) override;
  void read(PresetType preset_type,
            const boost::property_tree::ptree& root) override;

 private:
  Glib::RefPtr<Gio::Settings> input_settings, output_settings;

  void save(boost::property_tree::ptree& root,
            const std::string& section,
            const Glib::RefPtr<Gio::Settings>& settings) override;
  void load(const boost::property_tree::ptree& root,
            const std::string& section,
            const Glib::RefPtr<Gio::Settings>& settings) override;
};

#endif

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

#ifndef DEAMBIENCER_UI_HPP
#define DEAMBIENCER_UI_HPP

#include "plugin_ui_base.hpp"

class DeAmbiencerUi : public Gtk::Grid, public PluginUiBase {
 public:
  DeAmbiencerUi(BaseObjectType* cobject,
                const Glib::RefPtr<Gtk::Builder>& builder,
                const std::string& schema,
                const std::string& schema_path);
  DeAmbiencerUi(const DeAmbiencerUi&) = delete;
  auto operator=(const DeAmbiencerUi&) -> DeAmbiencerUi& = delete;
  DeAmbiencerUi(const DeAmbiencerUi&&) = delete;
  auto operator=(const DeAmbiencerUi&&) -> DeAmbiencerUi& = delete;
  ~DeAmbiencerUi() override;

  void reset() override;

 private:
  Glib::RefPtr<Gtk::Adjustment> input_gain, output_gain, threshold_level;
};

#endif

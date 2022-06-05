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

#ifndef DEAMBIENCER_HPP
#define DEAMBIENCER_HPP

#include "plugin_base.hpp"

class DeAmbiencer : public PluginBase {
 public:
  DeAmbiencer(const std::string& tag,
              const std::string& schema,
              const std::string& schema_path);
  DeAmbiencer(const DeAmbiencer&) = delete;
  auto operator=(const DeAmbiencer&) -> DeAmbiencer& = delete;
  DeAmbiencer(const DeAmbiencer&&) = delete;
  auto operator=(const DeAmbiencer&&) -> DeAmbiencer& = delete;
  ~DeAmbiencer() override;

  GstElement* deambiencer = nullptr;

 private:
  void bind_to_gsettings();
};

#endif

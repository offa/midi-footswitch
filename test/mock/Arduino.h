/*
 * MIDI Footswitch
 *
 * Copyright (C) 2019-2020  offa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstdint>
#include <trompeloeil.hpp>

using boolean = bool;



inline constexpr uint8_t OUTPUT{1};
inline constexpr uint8_t HIGH{1};

void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
void pinMode(uint8_t pin, uint8_t mode);

namespace mock
{

    struct ArduinoMock
    {
        MAKE_MOCK2(pinMode, void(uint8_t, uint8_t));
    };

    extern ArduinoMock arduino;
}

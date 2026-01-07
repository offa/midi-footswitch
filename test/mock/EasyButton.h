/*
 * MIDI Footswitch
 *
 * Copyright (C) 2019-2026  offa
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

class EasyButton
{
public:
    EasyButton(uint8_t pin, uint32_t debounce_time = 35, bool pullup_enable = true, bool invert = true)
    {
    }

    MAKE_MOCK0(begin, void());
    MAKE_MOCK0(read, bool());
    MAKE_MOCK0(wasReleased, bool());
};

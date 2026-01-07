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

#include "MIDIUSB.h"

namespace detail
{
    template <uint8_t type, uint8_t channel>
    void sendEventMessage(uint8_t data0, uint8_t data1) noexcept
    {
        midiEventPacket_t event{(type & 0xf0) >> 4, (type | channel), data0, data1};
        MidiUSB.sendMIDI(event);
        MidiUSB.flush();
    }
}


template <uint8_t channel, uint8_t control, uint8_t data>
struct ControlChangeAction
{
    static void onPressed() noexcept
    {
        detail::sendEventMessage<0xb0, channel>(control, data);
    }
};


template <uint8_t channel, uint8_t control, uint8_t dataA, uint8_t dataB>
struct ControlChangeToggleAction
{
    static void onPressed() noexcept
    {
        detail::sendEventMessage<0xb0, channel>(control, (state ? dataA : dataB));
        state = !state;
    }

private:
    static inline bool state{true};
};


template <uint8_t channel, uint8_t program>
struct ProgramChangeAction
{
    static void onPressed() noexcept
    {
        detail::sendEventMessage<0xc0, channel>(program, 0x00);
    }
};

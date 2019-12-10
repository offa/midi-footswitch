/*
 * MIDI Footswitch
 *
 * Copyright (C) 2019  offa
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

#include <Arduino.h>
#include <EasyButton.h>
#include <MIDIUSB.h>

template <uint8_t channel, uint8_t control, uint8_t data>
struct ControlChangeAction
{
    static void onPressed()
    {
        const midiEventPacket_t event{0x0b, (0xb0 | channel), control, data};
        MidiUSB.sendMIDI(event);
        MidiUSB.flush();
    }
};



template <size_t id, class Command>
class MidiButton
{
public:
    void setup()
    {
        button.begin();
        button.onPressed(Command::onPressed);
    }

    void read()
    {
        button.read();
    }

private:
    EasyButton button{id};
};
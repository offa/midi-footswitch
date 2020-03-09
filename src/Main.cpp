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

#include "MidiButton.h"
#include <Arduino.h>

namespace
{
    namespace pin
    {
        inline constexpr uint8_t d2{2};
        inline constexpr uint8_t d3{3};
        inline constexpr uint8_t d4{4};
        inline constexpr uint8_t d5{5};
        inline constexpr uint8_t d6{6};
        inline constexpr uint8_t d7{7};
    }

    constexpr uint8_t channel{0};
    constexpr uint8_t midiOff{0};
    constexpr uint8_t midiOn{127};

    MidiButton<pin::d2, ControlChangeToggleAction<channel, 0, midiOn, midiOff>> button0;
    MidiButton<pin::d3, ControlChangeToggleAction<channel, 1, midiOn, midiOff>> button1;
    MidiButton<pin::d4, ControlChangeToggleAction<channel, 2, midiOn, midiOff>> button2;
    MidiButton<pin::d5, ControlChangeToggleAction<channel, 3, midiOn, midiOff>> button3;
    MidiButton<pin::d6, ControlChangeToggleAction<channel, 4, midiOn, midiOff>> button4;
    MidiButton<pin::d7, ControlChangeToggleAction<channel, 5, midiOn, midiOff>> button5;


    template <class... Buttons>
    void setupButtons(Buttons&&... buttons)
    {
        (buttons.setup(), ...);
    }

    template <class... Buttons>
    void readButtons(Buttons&&... buttons)
    {
        (buttons.read(), ...);
    }
}


void setup()
{
    setupButtons(button0, button1, button2, button3, button4, button5);
}

void loop()
{
    readButtons(button0, button1, button2, button3, button4, button5);
}

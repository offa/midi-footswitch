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
    constexpr uint8_t midiOff{0};
    constexpr uint8_t midiOn{127};

    MidiButton<2, ProgramChangeAction<0, 13>> button0;
    MidiButton<3, ControlChangeAction<0, 3, midiOn>> button1;
    MidiButton<4, ControlChangeAction<0, 4, midiOn>> button2;
    MidiButton<5, ControlChangeAction<0, 4, midiOff>> button3;
    MidiButton<6, ControlChangeAction<0, 3, midiOff>> button4;
    MidiButton<7, ProgramChangeAction<0, 14>> button5;
}


void setup()
{
    Serial.begin(9600);
    Serial.println("Init");

    button0.setup();
    button1.setup();
    button2.setup();
    button3.setup();
    button4.setup();
    button5.setup();
}

void loop()
{
    button0.read();
    button1.read();
    button2.read();
    button3.read();
    button4.read();
    button5.read();
}

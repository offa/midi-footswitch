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

#include <EasyButton.h>
#include <MIDIUSB.h>


constexpr size_t numberOfButtons{6};

EasyButton button[numberOfButtons] = {
    // First row
    EasyButton{2},
    EasyButton{3},
    EasyButton{4},

    // Second row
    EasyButton{5},
    EasyButton{6},
    EasyButton{7}
};


constexpr uint8_t midiOff{0};
constexpr uint8_t midiOn{127};

constexpr uint8_t data[numberOfButtons] = {
    midiOn,
    midiOn,
    midiOn,

    midiOff,
    midiOff,
    midiOff
};

constexpr uint8_t control[numberOfButtons] = {
    2,
    3,
    4,

    5,
    6,
    7
};


void controlChange(uint8_t channel, uint8_t control, uint8_t value)
{
    constexpr uint8_t eventTypeControlChange{0xb0};
    const midiEventPacket_t event{eventTypeControlChange, static_cast<uint8_t>(eventTypeControlChange | channel), control, value};
    MidiUSB.sendMIDI(event);
    MidiUSB.flush();
}

template<size_t id>
void onPressed()
{
    static_assert(id < numberOfButtons, "Invalid id");

    controlChange(0, control[id], data[id]);
    Serial.println("ControlChange: \t#" + String(id) + "\t" + String(control[id]) + "\t" + String(data[id]));
}

template<size_t id>
void setupButton()
{
    static_assert(id < numberOfButtons, "Invalid id");

    button[id].begin();
    button[id].onPressed(onPressed<id>);
}


void setup()
{
    Serial.begin(9600);
    Serial.println("Init");

    setupButton<0>();
    setupButton<1>();
    setupButton<2>();
    setupButton<3>();
    setupButton<4>();
    setupButton<5>();
}

void loop()
{
    for( auto& b : button )
    {
        b.read();
    }
}

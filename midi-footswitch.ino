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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#include <EasyButton.h>
#pragma GCC diagnostic pop

#include <MIDIUSB.h>


constexpr size_t numberOfButtons{6};

// First row
EasyButton button0{2};
EasyButton button1{3};
EasyButton button2{4};

// Second row
EasyButton button3{5};
EasyButton button4{6};
EasyButton button5{7};


constexpr uint8_t midiOff{0};
constexpr uint8_t midiOn{127};

constexpr uint8_t data[] = {
    midiOn,
    midiOn,
    midiOn,

    midiOff,
    midiOff,
    midiOff
};
constexpr uint8_t control[] = {
    2,
    3,
    4,

    5,
    6,
    7
};


void controlChange(uint8_t channel, uint8_t control, uint8_t value)
{
    const midiEventPacket_t event{0x0Bu, static_cast<uint8_t>(0xB0u | channel), control, value};
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

void setup()
{
    Serial.begin(9600);
    Serial.println("Init");

    button0.begin();
    button0.onPressed(onPressed<0>);

    button1.begin();
    button1.onPressed(onPressed<1>);

    button2.begin();
    button2.onPressed(onPressed<2>);

    button3.begin();
    button3.onPressed(onPressed<3>);

    button4.begin();
    button4.onPressed(onPressed<4>);

    button5.begin();
    button5.onPressed(onPressed<5>);
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

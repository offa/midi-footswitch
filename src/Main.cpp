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

#include <Arduino.h>
#include <EasyButton.h>
#include <MIDIUSB.h>


constexpr uint8_t midiOff{0};
constexpr uint8_t midiOn{127};


void controlChange(uint8_t channel, uint8_t control, uint8_t value)
{
    constexpr uint8_t eventTypeControlChange{0xb0};
    const midiEventPacket_t event{eventTypeControlChange, static_cast<uint8_t>(eventTypeControlChange | channel), control, value};
    MidiUSB.sendMIDI(event);
    MidiUSB.flush();
}

template<size_t id, uint8_t control, uint8_t data>
void onPressed()
{
    controlChange(0, control, data);
    Serial.println("ControlChange: \t#" + String(id) + "\t" + String(control) + "\t" + String(data));
}


template<size_t id, uint8_t control, uint8_t data>
class MidiButton
{
public:
    void setup()
    {
        button.begin();
        button.onPressed(::onPressed<id, control, data>);
    }

    void read()
    {
        button.read();
    }

private:
    EasyButton button{id};
};

MidiButton<2, 2, midiOn> button0;
MidiButton<3, 3, midiOn> button1;
MidiButton<4, 4, midiOn> button2;
MidiButton<5, 5, midiOff> button3;
MidiButton<6, 6, midiOff> button4;
MidiButton<7, 7, midiOff> button5;

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

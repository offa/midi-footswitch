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

#include <Arduino.h>
#include <EasyButton.h>
#include <MIDIUSB.h>

namespace detail
{
    template <uint8_t type, uint8_t channel>
    void sendEventMessage(uint8_t data0, uint8_t data1)
    {
        midiEventPacket_t event{(type & 0xf0) >> 4, (type | channel), data0, data1};
        MidiUSB.sendMIDI(event);
        MidiUSB.flush();
    }
}


template <uint8_t channel, uint8_t control, uint8_t data>
struct ControlChangeAction
{
    static void onPressed()
    {
        detail::sendEventMessage<0xb0, channel>(control, data);
    }
};


template <uint8_t channel, uint8_t control, uint8_t dataA, uint8_t dataB>
struct ControlChangeToggleAction
{
    static void onPressed()
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
    static void onPressed()
    {
        detail::sendEventMessage<0xc0, channel>(program, 0x00);
    }
};

template <uint8_t pin>
class Button
{
public:
    void setup()
    {
        button.begin();
    }

    bool pressed()
    {
        button.read();
        return button.wasReleased();
    }

protected:
    EasyButton button{pin};
};

template <uint8_t pin>
class Led
{
public:
    void setup()
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    void toggle()
    {
        digitalWrite(pin, !digitalRead(pin));
    }
};

template <class Button, class Led, class Action>
class MidiButton
{
public:
    void setup()
    {
        button.setup();
        led.setup();
    }

    void read()
    {
        if (button.pressed())
        {
            Action::onPressed();
            led.toggle();
        }
    }

    Button& getButton()
    {
        return button;
    }

    Led& getLed()
    {
        return led;
    }

private:
    Button button;
    Led led;
};

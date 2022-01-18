/*
 * MIDI Footswitch
 *
 * Copyright (C) 2019-2022  offa
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

#include "MidiAction.h"
#include <Arduino.h>
#include <EasyButton.h>


template <uint8_t pin>
class Button
{
public:
    void setup() noexcept
    {
        button.begin();
    }

    bool pressed() noexcept
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
    void setup() noexcept
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    void toggle() noexcept
    {
        digitalWrite(pin, !digitalRead(pin));
    }
};

template <class Button, class Led, class Action>
class MidiButton
{
public:
    void setup() noexcept
    {
        button.setup();
        led.setup();
    }

    void read() noexcept
    {
        if (button.pressed())
        {
            Action::onPressed();
            led.toggle();
        }
    }

    Button& getButton() noexcept
    {
        return button;
    }

    Led& getLed() noexcept
    {
        return led;
    }

private:
    Button button;
    Led led;
};

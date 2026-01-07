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

#include "MidiButton.h"
#include <trompeloeil.hpp>

namespace mock
{
    template <uint8_t pin>
    struct ButtonSpy : public Button<pin>
    {
        EasyButton& spy()
        {
            return this->button;
        }
    };

    class ButtonMock
    {
    public:
        MAKE_MOCK0(setup, void());
        MAKE_MOCK0(pressed, bool());
    };

    class LedMock
    {
    public:
        MAKE_MOCK0(setup, void());
        MAKE_MOCK0(toggle, void());
    };

    class ActionImpl
    {
    public:
        MAKE_MOCK0(onPressed, void());
    };

    class ActionMock
    {
    public:
        static void onPressed()
        {
            impl.onPressed();
        }

        inline static ActionImpl impl;
    };
}

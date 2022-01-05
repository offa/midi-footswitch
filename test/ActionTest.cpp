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

#include "MidiButton.h"
#include "mock/Mocks.h"
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

namespace
{
    auto midiEventEq(const midiEventPacket_t& event)
    {
        return trompeloeil::make_matcher<midiEventPacket_t>(
            [](const midiEventPacket_t& expected, const midiEventPacket_t& actual) {
                return std::tie(expected.header, expected.byte1, expected.byte2, expected.byte3) ==
                       std::tie(actual.header, actual.byte1, actual.byte2, actual.byte3);
            },
            [](std::ostream& os, const midiEventPacket_t& ev) {
                os << std::hex << std::showbase << std::internal << std::setfill('0')
                   << " midi event {" << std::setw(4) << std::uint32_t{ev.header}
                   << ", " << std::setw(4) << std::uint32_t(ev.byte1)
                   << ", " << std::setw(4) << std::uint32_t(ev.byte2)
                   << ", " << std::setw(4) << std::uint32_t(ev.byte3) << "}";
            },
            event);
    }
}

MIDIMock MidiUSB;


TEST_CASE("Send event message", "[ActionTest]")
{
    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({(0x1a & 0xf0) >> 4, (0x1a | 0x2b), 0xbb, 0xcc})));
    REQUIRE_CALL(MidiUSB, flush());
    detail::sendEventMessage<0x1a, 0x2b>(0xbb, 0xcc);
}

TEST_CASE("Program change action sends packet", "[ActionTest]")
{
    constexpr std::uint8_t channel{0x02};
    constexpr std::uint8_t program{0x11};
    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({0x0c, (0xc0 | channel), program, 0x00})));
    REQUIRE_CALL(MidiUSB, flush());

    ProgramChangeAction<channel, program>::onPressed();
}

TEST_CASE("Control change action sends packet", "[ActionTest]")
{
    constexpr std::uint8_t channel{0x05};
    constexpr std::uint8_t control{0x22};
    constexpr std::uint8_t data{0x33};
    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({0x0b, (0xb0 | channel), control, data})));
    REQUIRE_CALL(MidiUSB, flush());

    ControlChangeAction<channel, control, data>::onPressed();
}

TEST_CASE("Control change toggle action sends packet with toggled values", "[ActionTest]")
{
    constexpr std::uint8_t channel{0x03};
    constexpr std::uint8_t control{0x17};
    constexpr std::uint8_t dataA{0x01};
    constexpr std::uint8_t dataB{0x02};

    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({0x0b, (0xb0 | channel), control, dataA})));
    REQUIRE_CALL(MidiUSB, flush());
    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({0x0b, (0xb0 | channel), control, dataB})));
    REQUIRE_CALL(MidiUSB, flush());

    ControlChangeToggleAction<channel, control, dataA, dataB>::onPressed();
    ControlChangeToggleAction<channel, control, dataA, dataB>::onPressed();
}

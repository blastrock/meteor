// Meteor - A Nintendo Gameboy Advance emulator
// Copyright (C) 2009 Philippe Daouadi
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ameteor.hpp"
#include <string.h>

int main (int argc, char **argv)
{
	if (argc != 2)
		return 1;

	//AMeteor::_memory.LoadBios("/home/blastrock/GBA.BIOS");
	AMeteor::_memory.LoadRom(argv[1]);
	{
		int len = strlen(argv[1]);
		argv[1][len-3] = 's';
		argv[1][len-2] = 'a';
		argv[1][len-1] = 'v';
	}
	if (!AMeteor::_memory.LoadCart(argv[1]))
	{
		puts("cart load fail");
		AMeteor::_memory.SetCartFile(argv[1]);
	}

	if (!AMeteor::LoadState("ss.mst"))
		puts("state load fail");

	AMeteor::_keypad.BindKey('w',            (AMeteor::Keypad::Button)0x001);
	AMeteor::_keypad.BindKey('x',            (AMeteor::Keypad::Button)0x002);
	AMeteor::_keypad.BindKey('z',            (AMeteor::Keypad::Button)0x004);
	AMeteor::_keypad.BindKey('a',            (AMeteor::Keypad::Button)0x008);
	AMeteor::_keypad.BindKey(sf::Key::Right, (AMeteor::Keypad::Button)0x010);
	AMeteor::_keypad.BindKey(sf::Key::Left,  (AMeteor::Keypad::Button)0x020);
	AMeteor::_keypad.BindKey(sf::Key::Up,    (AMeteor::Keypad::Button)0x040);
	AMeteor::_keypad.BindKey(sf::Key::Down,  (AMeteor::Keypad::Button)0x080);
	AMeteor::_keypad.BindKey('s',            (AMeteor::Keypad::Button)0x100);
	AMeteor::_keypad.BindKey('q',            (AMeteor::Keypad::Button)0x200);

	AMeteor::_cpu.Run();

	return 0;
}

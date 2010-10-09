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
#include "ameteor/bios.hpp"
#include <string.h>
#include <stdint.h>
#include <cmath>

void ArcTan (float tan)
{
	int16_t iTan = tan * (1 << 14);
	AMeteor::_cpu.Reg(0) = (int16_t)iTan;
	AMeteor::Bios::ArcTan();
	float alpha = (int16_t)AMeteor::_cpu.Reg(0);
	alpha /= 0x4000;
	alpha *= 90;
	printf("atan(%f) = %f\n", tan, alpha);
}

void ArcTan2 (float x, float y)
{
	int16_t iX = x * (1 << 14);
	int16_t iY = y * (1 << 14);
	AMeteor::_cpu.Reg(0) = iX;
	AMeteor::_cpu.Reg(1) = iY;
	AMeteor::Bios::ArcTan2();
	float alpha = (uint16_t)AMeteor::_cpu.Reg(0);
	alpha /= 0x10000;
	alpha *= 360;
	printf("atan2(%f, %f) = %f\n", x, y, alpha);
}

void ArcTan2 (float ang)
{
	printf("%f - ", ang);
	ang /= 180;
	ang *= M_PI;
	float x = cos(ang);
	float y = sin(ang);
	ArcTan2(x, y);
}

void TestArcTan ()
{
	ArcTan(1.9);
	ArcTan(0.8);
	ArcTan(0.1);
	ArcTan(0);
	ArcTan(-0.1);
	ArcTan(-0.8);
	ArcTan(-1.9);
	ArcTan2(0, 0);
	ArcTan2(1, 0);
	ArcTan2(1);
	ArcTan2(45);
	ArcTan2(70);
	ArcTan2(90);
	ArcTan2(100);
	ArcTan2(170);
	ArcTan2(180);
	ArcTan2(200);
	ArcTan2(260);
	ArcTan2(270);
	ArcTan2(280);
	ArcTan2(350);
}

int main ()
{
	TestArcTan();

	return 0;
}

/*
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "stdafx.h"
#include "StringMisc.h"

StringWords::StringWords(IN std::string str)
{
	this->buffer=str;
}

StringWords::~StringWords()
{
}

std::string StringWords::next()
{
	if (this->done()) return std::string("");

	size_t start = pos;
	size_t end=buffer.find(" ",pos);
	if (end == std::string::npos)
		end = buffer.length();
	pos = end + 1;

	return buffer.substr(start, end - start);
}

bool StringWords::done()
{
	return pos > buffer.length();
}
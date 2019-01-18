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
#pragma once
#include <SubAuth.h>

class SecureString
{
public:
	SecureString(IN PUNICODE_STRING source);
	SecureString(IN const unsigned char * source, size_t length);
	~SecureString();
	const char* c_str();
	size_t length();
	bool icontains(SecureString* search);
	bool contains_lower(const char* search_lower);
	const unsigned char* lower();
private:
	unsigned char* str = NULL;
	unsigned char* str_lower;
	size_t len = 0;
};


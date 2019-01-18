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
#include "SecureString.h"
#include <mbstring.h>

SecureString::SecureString(IN PUNICODE_STRING source)
{

	len = WideCharToMultiByte(CP_UTF8, 0, source->Buffer, (int)wcsnlen_s(source->Buffer,source->Length), NULL, 0, NULL, NULL);
	if (len == 0)
		throw "UTF8 conversion error";
	

	str = new unsigned char[len+1]; //include extra byte for the NULL terminator

	if (WideCharToMultiByte(CP_UTF8, 0, source->Buffer, (int)wcsnlen_s(source->Buffer, source->Length), (LPSTR)str, (int)len, NULL, NULL) == 0)
		throw "UTF8 conversion error";

	str[len] = NULL;
	//p_word = str;

}

SecureString::SecureString(IN const unsigned char* source, size_t length)
{
	len = length;
	str = new unsigned char[len + 1];
	for (size_t i=0; i < len; i++)
		str[i] = source[i];
	str[len] = NULL;
	//p_word = str;
}

SecureString::~SecureString()
{
	if(str) SecureZeroMemory(str, len);
	delete str;
	str = NULL;

	if (str_lower) SecureZeroMemory(str_lower, len);
	delete str_lower;
	str_lower = NULL;
}

const char * SecureString::c_str()
{
	return (const char*)str;
}

size_t SecureString::length()
{
	return len;
}

bool SecureString::icontains(SecureString* search)
{
	return _mbsstr(lower(), search->lower())!=NULL;
}

bool SecureString::contains_lower(const char * search_lower)
{
	return _mbsstr(lower(), (const unsigned char*)search_lower) != NULL;
}

const unsigned char * SecureString::lower()
{
	if (str_lower == NULL) {
		str_lower = new unsigned char[len + 1];
		for (size_t i = 0; i < len; i++) {
			str_lower[i] = tolower(str[i]);
		}
		str_lower[len] = NULL;
	}

	return str_lower;
}
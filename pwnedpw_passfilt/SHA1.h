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
#include <string>
#include <bcrypt.h>
class SHA1
{
public:
	SHA1(IN std::string str);
	SHA1(IN const unsigned char* str);
	~SHA1();
	PBYTE getBytes();
	int getSize();
private:
	void calc_hash(IN const char* str);
	#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)
	#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
	BCRYPT_ALG_HANDLE       hAlg = NULL;
	BCRYPT_HASH_HANDLE      hHash = NULL;
	NTSTATUS                status = STATUS_UNSUCCESSFUL;
	DWORD                   cbData = 0,
		cbHash = 0,
		cbHashObject = 0;
	PBYTE                   pbHashObject = NULL;
	PBYTE                   pbHash = NULL;
};


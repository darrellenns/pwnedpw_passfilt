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
#include "SHA1.h"
#include <stdexcept>


SHA1::SHA1(IN std::string str)
{
	const char* cstr = str.c_str();
	calc_hash((const char*) str.c_str());
}

SHA1::SHA1(IN const unsigned char * str)
{
	calc_hash((const char*)str);
}


SHA1::~SHA1()
{
	if (hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
	if (hHash) BCryptDestroyHash(hHash);

	if (pbHashObject) {
		SecureZeroMemory(pbHashObject, cbHashObject);
		delete pbHashObject;
	}

	if (pbHash) {
		SecureZeroMemory(pbHash, cbHash);
		delete pbHash;
	}
}

PBYTE SHA1::getBytes() {
	return pbHash;
}

int SHA1::getSize() {
	return cbHash;
}

void SHA1::calc_hash(IN const char * cstr)
{
	//open an algorithm handle
	if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA1_ALGORITHM, NULL, 0)))
		throw "BCryptOpenAlgorithmProvider";

	//calculate the size of the buffer to hold the hash object
	if (!NT_SUCCESS(status = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0)))
		throw "BCryptGetProperty buffer size";

	//allocate the hash object on the heap
	pbHashObject = new BYTE[cbHashObject];

	//calculate the length of the hash
	if (!NT_SUCCESS(status = BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0)))
		throw "BCryptGetProperty hash length";

	//make sure we always return 20 bytes
	if (cbHash != 20)
		throw "Internal error - invalid hash size";

	//allocate the hash buffer on the heap
	pbHash = new BYTE[cbHash];

	//create a hash
	if (!NT_SUCCESS(status = BCryptCreateHash(hAlg, &hHash, pbHashObject, cbHashObject, NULL, 0, 0)))
		throw "BCryptCreateHash";

	//hash some data
	if (!NT_SUCCESS(status = BCryptHashData(hHash, (PUCHAR)cstr, (ULONG)strlen(cstr), 0)))
		throw "BCryptHashData";

	//close the hash
	if (!NT_SUCCESS(status = BCryptFinishHash(hHash, pbHash, cbHash, 0)))
		throw "BCryptFinishHash";
}

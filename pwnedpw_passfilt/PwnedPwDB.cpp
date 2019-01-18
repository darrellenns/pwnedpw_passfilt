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
#include "PwnedPwDB.h"
#include <stdexcept>
using namespace std;

const char* PwnedPwDB::magic_header = "PWNP\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

PwnedPwDB::PwnedPwDB(IN const WCHAR* dbPath)
{
	h_file=CreateFile(dbPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	if(h_file==INVALID_HANDLE_VALUE)
		throw std::runtime_error("db CreateFile");
	readChunk();
	if (memcmp(buffer, magic_header, buffer_s) != 0)
		throw std::runtime_error("db header");
}

PBYTE PwnedPwDB::readChunk()
{
	DWORD bytes_read;
	if (ReadFile(h_file, buffer, buffer_s, &bytes_read, NULL)!=TRUE)
		throw std::runtime_error("db read");
	if (bytes_read!=buffer_s)
		throw std::runtime_error("db read");
	return buffer;
}

void PwnedPwDB::seekChunk(UINT64 chunkNum) {
	LARGE_INTEGER pos;
	pos.QuadPart = (chunkNum+1)*buffer_s;

	if(SetFilePointerEx(h_file, pos, NULL, FILE_BEGIN)==0)
		throw std::runtime_error("db seek");	
}

long PwnedPwDB::getTotalChunks()
{
	LARGE_INTEGER s;
	if (GetFileSizeEx(h_file, &s) == 0)
		throw std::runtime_error("get size");
	return (long)(s.QuadPart / 20) - 1;
}

bool PwnedPwDB::searchHash(PBYTE hash) {
	long start = 0;
	long end = getTotalChunks()-1;
	long mid;
	while (TRUE) {
		mid = start + (end - start) / 2;
		seekChunk(mid);
		readChunk();
		int cmp = memcmp(hash, buffer, buffer_s);
		if (cmp == 0) {
			return TRUE;
		}else if(start >= end) {
			return FALSE;
		}else if(cmp > 0) {
			start = mid + 1;
		}else if(cmp < 0) {
			end = mid - 1;
		}
	}
}


PwnedPwDB::~PwnedPwDB()
{
	CloseHandle(h_file);
	SecureZeroMemory(buffer, buffer_s);
}

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

class PwnedPwDB
{
public:
	PwnedPwDB(IN const WCHAR* dbPath);
	~PwnedPwDB();
	bool searchHash(PBYTE hash);
private:
	static const int buffer_s = 20;
	BYTE buffer[buffer_s];
	HANDLE h_file = NULL;
	PBYTE readChunk();
	void seekChunk(UINT64 chunkNum);
	long getTotalChunks();
	static const char* magic_header;
};


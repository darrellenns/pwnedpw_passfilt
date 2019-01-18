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
#include <Windows.h>
#include <SubAuth.h>

extern "C" __declspec(dllexport) BOOLEAN __stdcall PasswordFilter(IN PUNICODE_STRING AccountName, IN PUNICODE_STRING FullName, PUNICODE_STRING Password, IN BOOLEAN SetOperation);
extern "C" __declspec(dllexport) NTSTATUS PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeId, PUNICODE_STRING NewPassword);
extern "C" __declspec(dllexport) BOOLEAN __stdcall InitializeChangeNotify(void);
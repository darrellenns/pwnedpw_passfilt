// pwnedpw_passfilt.cpp : Defines the exported functions for the DLL application.
//

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
#include <SubAuth.h>
#include <stdio.h>
#include <Windows.h>
#include <bcrypt.h>
#include "SHA1.h"
#include "PwnedPwDB.h"
#include <iostream>
#include <fstream>
#include <string>
#include "StringMisc.h"
#include "SecureString.h"
#include <mbstring.h>
#include "Logger.h"



class GetEnv {
public:
	GetEnv(IN const WCHAR* variable) {
		WCHAR envBuffer[32767];
		if (GetEnvironmentVariable(variable, envBuffer, 32767) != 0) {
			str.assign(envBuffer);
		}
	};
	~GetEnv() {};
	std::wstring str=L"";
};

extern "C" __declspec(dllexport) BOOLEAN __stdcall InitializeChangeNotify(void)
{
	Logger::init();
	Logger::log("Initialize", Logger::EVT_MISC, Logger::SEV_SUCCESS);
	return TRUE;
}

extern "C" __declspec(dllexport) NTSTATUS PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeId, PUNICODE_STRING NewPassword)
{
	SecureZeroMemory(NewPassword->Buffer, NewPassword->Length);
	return STATUS_SUCCESS;
}


extern "C" __declspec(dllexport) BOOLEAN __stdcall PasswordFilter(IN PUNICODE_STRING AccountName,IN PUNICODE_STRING FullName,PUNICODE_STRING Password,IN BOOLEAN SetOperation) {
	
	try {

		if (Password->Length > 4096) {
			return FALSE; //Windows should never give us a password this long in the first place - something is wrong if we see one, so just reject it
		}

		SecureString s_AccountName(AccountName);
		SecureString s_FullName(FullName);
		SecureString s_Password(Password);
		SecureZeroMemory(Password->Buffer, Password->Length);

		std::wstring pathRoot;
		pathRoot = GetEnv(L"PwnedPWRoot").str;
		if (pathRoot.length() == 0)
			pathRoot = GetEnv(L"SystemRoot").str;

		SHA1 hash(s_Password.c_str());
		PwnedPwDB db((pathRoot + L"\\pwnedpw.bin").c_str());

		//check if password is in the pwnedpw database
		if (db.searchHash(hash.getBytes())) {
			Logger::log(
				(
					std::string("Rejected password in pwned database.\nUsername: ") +
					std::string(s_AccountName.c_str())
					).c_str(),
				Logger::EVT_REJ_PWNED,
				Logger::SEV_WARNING
			);
			return FALSE;
		}

		//check if password contains the account name
		if (s_AccountName.length()>3 && s_Password.icontains(&s_AccountName)) {
			Logger::log(
				(
					std::string("Rejected password containing account name.\nUsername: ") +
					std::string(s_AccountName.c_str())
					).c_str(),
				Logger::EVT_REJ_ACCTNAME,
				Logger::SEV_WARNING
			);
			return FALSE;
		}


		//check if password contains any word of name (such as first or last name)
		std::string str_FullName(s_FullName.c_str());
		stdstr_tolower(str_FullName);
		StringWords words(str_FullName);
		while (!words.done()) {
			std::string w = words.next();
			if (w.length() > 3 && s_Password.contains_lower(w.c_str())) {
				Logger::log(
					(
						std::string("Rejected password containing one or more words in FullName.\nUsername: ")+
						std::string(s_AccountName.c_str())
					).c_str(),
					Logger::EVT_REJ_NAME,
					Logger::SEV_WARNING
				);
				return FALSE;
			}
		}

		//check if password contains a banned word
		std::ifstream f_banned(pathRoot + L"\\bannedwords.txt", std::ifstream::in);
		if (!f_banned.good()) Logger::log("Error opening banned word file");
		for (std::string line; std::getline(f_banned, line);) {
			stdstr_tolower(line);
			if (s_Password.contains_lower(line.c_str())) {
				Logger::log(
					(
						std::string("Rejected password containing a banned word.\nUsername: ") +
						std::string(s_AccountName.c_str())
					).c_str(),
					Logger::EVT_REJ_BANNED,
					Logger::SEV_WARNING
				);
				return FALSE;
			}
		}

		return TRUE;
	}
	catch (...) {
		Logger::log("Unhandled exception in PasswordFilter()",Logger::EVT_MISC,Logger::SEV_ERROR);
		return FALSE;
	}
}
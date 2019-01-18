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

class Logger
{
public:
	static void init();
	static void log(IN const char* message,DWORD event_id=EVT_MISC,WORD severity=SEV_SUCCESS);
	static const WORD SEV_SUCCESS = EVENTLOG_SUCCESS;
	static const WORD SEV_AUDIT_FAILURE = EVENTLOG_AUDIT_FAILURE;
	static const WORD SEV_AUDIT_SUCCESS = EVENTLOG_AUDIT_SUCCESS;
	static const WORD SEV_ERROR = EVENTLOG_ERROR_TYPE;
	static const WORD SEV_INFORMATION = EVENTLOG_INFORMATION_TYPE;
	static const WORD SEV_WARNING = EVENTLOG_WARNING_TYPE;

	static const DWORD EVT_MISC = 0;
	static const DWORD EVT_REJ_PWNED = 4390;
	static const DWORD EVT_REJ_ACCTNAME = 4391;
	static const DWORD EVT_REJ_NAME = 4392;
	static const DWORD EVT_REJ_BANNED = 4393;


private:
	static HANDLE h_eventlog;
	const char* logname = "PwnedPW";
};


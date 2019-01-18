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
#include "CppUnitTest.h"
#include "../pwnedpw_passfilt/pwnedpw_passfilt.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

wchar_t* new_wstr(const wchar_t *s) {
	wchar_t* ret=new wchar_t[wcslen(s)+1];
	memcpy(ret, s, (wcslen(s)+1) * sizeof(wchar_t));
	ret[wcslen(s)] = NULL;
	return ret;
}

NTSTATUS TestPasswordChangeNotify(wchar_t* s_UserName, ULONG RelativeID, wchar_t* s_NewPassword) {

	UNICODE_STRING userName;
	userName.Buffer = s_UserName;
	userName.Length = (USHORT)wcslen(userName.Buffer) * sizeof(wchar_t);
	userName.MaximumLength = userName.Length + 1;

	UNICODE_STRING password;
	password.Buffer = new_wstr(s_NewPassword);
	password.Length = (USHORT)wcslen(password.Buffer) * sizeof(wchar_t);
	password.MaximumLength = password.Length + 1;

	return PasswordChangeNotify(&userName,RelativeID,&password);
}

bool TestPassFilt(wchar_t* s_accountName, wchar_t* s_realName, wchar_t* s_password) {

	UNICODE_STRING accountName;
	UNICODE_STRING realName;
	UNICODE_STRING password;

	accountName.Buffer = s_accountName;
	accountName.Length = (USHORT)wcslen(accountName.Buffer) * sizeof(wchar_t);
	accountName.MaximumLength = accountName.Length + 1;

	realName.Buffer = s_realName;
	realName.Length = (USHORT)wcslen(realName.Buffer) * sizeof(wchar_t);
	realName.MaximumLength = realName.Length + 1;

	password.Buffer = new_wstr(s_password);
	password.Length = (USHORT)wcslen(password.Buffer) * sizeof(wchar_t);
	password.MaximumLength = password.Length + 1;
	bool ret = PasswordFilter(&accountName, &realName, &password, FALSE);

	delete password.Buffer;

	return ret;
}

namespace test
{		
	TEST_CLASS(passfilt_tests)
	{
	public:
		TEST_METHOD(t_BadPassword) {
			InitializeChangeNotify();
			Assert::IsFalse(TestPassFilt(L"someguy", L"Some Guy", L"badpassword"));
		}

		TEST_METHOD(t_GoodPassword) {
			InitializeChangeNotify();
			Assert::IsTrue(TestPassFilt(L"someguy", L"Some Guy", L"AhX2iv0rofeiw6fu"));
		}

		TEST_METHOD(t_LongPassword) {
			InitializeChangeNotify();
			Assert::IsTrue(TestPassFilt(L"someguy",L"Some Guy",L"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"));
		}
		TEST_METHOD(t_BadShortLastName) {
			InitializeChangeNotify();
			Assert::IsTrue(TestPassFilt(L"someguy", L"Some Guy", L"agh2Aa_gUy_Wa4eel"));
		}
		TEST_METHOD(t_BadLastName) {
			InitializeChangeNotify();
			Assert::IsFalse(TestPassFilt(L"someguy", L"Some Longguy", L"agh2Aa_longgUy_Wa4eel"));
		}
		TEST_METHOD(t_BadFirstName) {
			InitializeChangeNotify();
			Assert::IsFalse(TestPassFilt(L"someguy", L"Some Guy", L"ieso1H_SoMe_Rij2ch"));
		}
		TEST_METHOD(t_BadAccountName) {
			InitializeChangeNotify();
			Assert::IsFalse(TestPassFilt(L"someguy",L"Joe Schmoe",L"ieso1H_SoMeGuY_Rij2ch"));
		}
		TEST_METHOD(t_BannedWord) {
			InitializeChangeNotify();
			Assert::IsFalse(TestPassFilt(L"someguy", L"Some Guy", L"ieso1H_PaSSword_Rij2ch"));
		}
		TEST_METHOD(t_TwoSpaceFullName) {
			InitializeChangeNotify();
			Assert::IsTrue(TestPassFilt(L"someguy", L"Some  Guy", L"AhX2iv0rofeiw6fu"));
		}
		TEST_METHOD(t_TwoSpaceBadFullName) {
			InitializeChangeNotify();
			Assert::IsFalse(TestPassFilt(L"someguy", L"Some  Guy", L"ieso1H_SoMeGuY_Rij2ch"));
		}
		TEST_METHOD(t_RepeatedQuery) {
			InitializeChangeNotify();
			for (DWORD i = 0; i < 1000; i++) {
				Assert::IsTrue(TestPassFilt(L"someguy",L"Some Guy",L"AhX2iv0rofeiw6fu"));
			}
		}
		TEST_METHOD(t_PasswordChangeNotify) {
			InitializeChangeNotify();
			Assert::IsTrue(NT_SUCCESS(TestPasswordChangeNotify(L"someguy", 0, L"aslkdfjwoej")));
		}
	};
}
# PwnedPW Password Filter

PwnedPW Password Filter is an Active Directory password filter that uses the Pwned Passwords database from haveibeenpwned.com.

## Features
* Reject passwords that are listed in the Pwned Passwords database.
* Reject passwords containing the user's username, or any word in the "Full Name" (first/last name).
* Reject passwords containing a word in the list of "banned words".
* No network traffic or services required - only the password filter dll and data files.

## Getting Started

### Prerequisites

Testing has been done on Server 2012 R2 (64 bit) and should work on newer versions as well. Password filters must be installed on all DCs that process password changes to be effective.

### Obtaining and Converting the DB
The Pwned Passwords database can be downloaded from https://haveibeenpwned.com/Passwords. You must use the "SHA-1 ordered by hash" format (the other formats will not work correctly).

Before it can be used with the password filter, the database has to be converted into a binary format. The binary format is used to improve search performance and reduce disk space requirements. The included PwnedDBConvert tool can be used to convert the database. For example: `PwnedDBConvert.exe pwned-passwords-sha1-ordered-by-hash-v4.txt pwnedpw.bin`

### Installing on Domain Controllers

* Copy the Pwned Passwords database to %SystemRoot%\pwnedpw.bin (usually C:\Windows\pwnedpw.bin). It must be the SHA-1 ordered by hash version, converted into PwnedPW_filter's binary format.
* Create a text file %SystemRoot%\bannedwords.txt (usually C:\Windows\pwnedpw.bin) with a list of "banned words" (1 per line) that are not allowed to be used in passwords. If you don't want to use this feature, just create an empty text file.
* Please refer to [Microsoft's Instructions](https://docs.microsoft.com/en-us/windows/desktop/secmgmt/installing-and-registering-a-password-filter-dll) for password filter dll installation. In testing on Server 2012R2, I have found that the registry entry must actually include the full DLL path (contrary to Microsoft's documentation which says to include only the filename without the extension'). If there is an error loading the extension, it should be logged in the Windows event log.

If you want to store pwnedpw.bin and bannedwords.txt in a different location than %SystemRoot%, you can use PwnedPWRoot enviornment variable to a different folder and copy the files there instead.

## Development

### Compiling
The code is developed and compiled with Visual Studio 2017. No additional dependencies are required. Unit tests are included to aid development/testing.

### Binary Database Format
The binary database format is quite simple. All records are a fixed 20 bytes.

There are 2 types of records:
* The first record is the file header. It consists of the ASCII characters "PWNP" followed by 16 NULL bytes. The NULL bytes are reserved for future use (possibly file format versions, etc).
* The remaining records are simply SHA-1 hashes of passwords. The hash records must be sorted in ascending order (the same order as the "ordered by hash" file from haveibeenpwned). The hash bytes are in big endian order (no byte swapping) - same as the original ASCII representation.

### Future Improvements (TODO)
* C++ based tool to convert PwnedPW databases into binary format.
* Event Logging seems to work only in development. This is possibly something to do with the System security context or LSA process that the DLL runs under on an actual DC.
* Add a prevalence filter to DB conversion tool.

## Acknowledgments

* Special thanks to Troy Hunt for created HaveIBeenPwned and the Pwned Passwords database.
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
#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <shlwapi.h>
#include <vector>
#include <filesystem>
#include <iomanip>

char* g_progname = NULL;

const char* MAGIC = "PWNP\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
const int MAGIC_LEN = 20;

void usage() {
	std::cerr << "Usage: " << g_progname << " <input file> <output file>" << std::endl;
}

std::vector<unsigned char> HexToBytes(const std::string& hex) {
	std::vector<unsigned char> bytes;

	for (unsigned int i = 0; i < hex.length(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		unsigned char byte = (char)strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);

		if (byte = 0 && (byteString[0] != '0' || byteString[1] != '0'))
			throw std::exception("invalid hex string");
	}

	return bytes;
}


int main(int argc, char* argv[], char* envp[])
{
	
	g_progname = PathFindFileNameA(argv[0]);
	if (argc != 3) {
		usage();
		return 1;
	}

	char* path_in = argv[1];
	char* path_out = argv[2];

	std::ifstream f_in(path_in, std::ifstream::in);
	if (!f_in.good()) {
		std::cerr << "Input file open error" << std::endl;
		usage();
		return 1;
	}

	if (std::experimental::filesystem::exists(path_out)) {
		std::cerr << "Output file already exists" << std::endl;
		usage();
		return 1;
	}

	std::ofstream f_out(path_out, std::ios::binary|std::ios::trunc); 
	if (!f_out.good()) {
		std::cerr << "Output file open error" << std::endl;
		usage();
		return 1;
	}

	f_out.write(MAGIC,MAGIC_LEN);
	if (!f_out.good()) {
		std::cerr << "File write error" << std::endl;
		usage();
		return 1;
	}

	//get input file size
	f_in.seekg(0, std::ios::end);
	std::streamoff f_in_size = f_in.tellg();
	f_in.seekg(0, std::ios::beg);

	int n = 0;

	for (std::string line; std::getline(f_in, line);) {
		n++;

		if (line[40] != ':') throw std::exception("input file format error");
		std::vector<unsigned char> bytes = HexToBytes(line.substr(0,40));
		f_out.write((const char *)bytes.data(),bytes.size());

		if (n % 10000 == 0) {
			std::streamoff x = f_in.tellg();
			std::cout << "Progress: " << std::fixed << std::setprecision(2) << 100*(double)f_in.tellg()/(double)f_in_size << "%\r" ;
		}

	}
	f_in.close();
	f_out.close();
}
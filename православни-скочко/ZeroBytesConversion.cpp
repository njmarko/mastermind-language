/*
* Author: Marko Njegomir
* Email:  marko.njegomir@uns.ac.rs
* Project: Program that converts the binary code of any file into multiple zerobytes files.
*		   These zerobytes files contain all the binary code in the names of the files in ASCII format.
*		   Because the code is in the name, the file appears to take up 0 bytes of storage (although name itself must be stored)
*		   If these zerobytes files are stored on google drive for example, then it will also report that the filesize is 0 bytes.
*
*		   This is just a prototype that creates files that contain only 160 characters in the generated filenames.
*		   It can be changed so it creates files with up to 32,767 characters in the file name.
*
*/
#include "ZeroBytesConversion.h"

void ConvertToZerobytes(const std::string& in_filename, const std::string& out_folder)
{
	/*
	* Id counter for the zero bytes program
	* it is used when a new zero bytes program is created because it can
	* spread accross multiple files
	*/
	int zbfile_id_cnt = 0;

	/*
	* Type of data that is stored in zerobytes program
	* If it is hexadecimal file then the value should be 1
	* For binary file value should be 0
	*/
	int zb_type = __HEX__;

	std::ifstream in(in_filename, std::ios::binary);

	// clear the temporary directory where zerobytes program will be stored
	std::string base_filename = in_filename.substr(in_filename.find_last_of("/\\") + 1);
	std::string output_folder = out_folder + base_filename + "/";
	std::experimental::filesystem::remove_all(output_folder);
	std::experimental::filesystem::create_directory(out_folder);
	std::experimental::filesystem::create_directory(output_folder);
	std::stringstream stream;
	unsigned char d;
	std::string s; /*= std::string(id_size - std::to_string(zbfile_id_cnt).length(), '0') + std::to_string(zbfile_id_cnt) + "_";*/
	std::string empty = "";
	std::string temp = "";
	while (in >> std::noskipws >> d) {

		if (s.length() > NAMESIZE)
		{
			s = std::string(ZB_ID_SIZE - std::to_string(zbfile_id_cnt).length(), '0') + std::to_string(zbfile_id_cnt) + "_" + s;
			std::ofstream out(output_folder + s);
			out.write((char*)&empty, 0);
			out.close();
			++zbfile_id_cnt;
			// make new string
			s = "";
		}
		if (zb_type == __BIN__)
		{
			s += std::bitset<8>(d).to_string();
		}
		else if (zb_type == __HEX__)
		{
			// using string strim as a way to get hexadecimal representation of the digit
			stream << std::hex << (unsigned int)d;
			temp = stream.str(); // helper variable for making sure all hex numbers have 2 digits
			if (temp.size() < 2)
			{
				temp = "0" + temp; // if the number has only one digit, add a leading zero
			}
			s += temp;
			stream.str(""); // clear the stream
		}
	}
	s = std::string(ZB_ID_SIZE - std::to_string(zbfile_id_cnt).length(), '0') + std::to_string(zbfile_id_cnt) + "_" + s;
	std::ofstream out(output_folder + s);
	out.write((char*)&empty, 0);
	out.close();
	in.close();

}

bool compareFunction (std::string a, std::string b) {return a.substr(0,10)<=b.substr(0,10);} ;

typedef std::basic_string<unsigned char> ustring; 

void ConvertFromZerobytes(const std::string& in_folder, const std::string& out_filename)
{
	std::string base_filename = out_filename.substr(out_filename.find_last_of("/\\") + 1);
	std::string output_folder = "./recreatedFile/" + base_filename;
	std::experimental::filesystem::create_directory("./recreatedFile/");
	std::vector<std::string> fileNames = get_filenames(in_folder + base_filename + "/");

	// sorting is necessary for linux, because the files arent sorted when they are read from the folder
	std::sort(fileNames.begin(), fileNames.end(),compareFunction);
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		fileNames[i] = fileNames[i].substr(ZB_ID_SIZE + 1, fileNames[i].size());
	}
	
	int zb_type =__HEX__;
	int inc = zb_type == __HEX__ ? 2 : 8; // ascii code in hexadeximal is saved using only hexadecimal digits per char
	int base = zb_type == __HEX__ ? 16 : 2; // base for string to integer function
	std::string codeFromFilenames = "";

	/*
	* This way of writing to the file allows me to write new line char without automatically adding
	* carriage return that would corrupt the data.
	* There are other ways to avoid this issue, but this one works for this prototype
	*/
	FILE* out = freopen(output_folder.c_str(),"wb",stdout);
	for (auto& name : fileNames) {
		for (size_t i = 0; i < name.size(); i += inc)
		{
			std::cout << (unsigned char)std::stoi(name.substr(i, inc), 0, base);
		}
	}
	fflush(out);
	fclose(out);
}

std::vector<std::string> get_filenames(Path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> fileNames;

	// http://en.cppreference.com/w/cpp/experimental/fs/directory_iterator
	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		// http://en.cppreference.com/w/cpp/experimental/fs/is_regular_file 
		if (stdfs::is_regular_file(*iter)) // comment out if all names (names of directories tc.) are required
										   // erases the path part of the full file path,leaving only the filename
			fileNames.push_back(iter->path().string().erase(0, path.string().size()));
		// id size is the number of characters in the the identificator; one is added for the separator
	}

	return fileNames;
}

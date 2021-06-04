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


#include <string>
#include "ZeroBytesConversion.h"

int main(int argc, char* argv[]) {

	std::string replay_name;
	std::string operation;
	if (argc == 3)
	{
		replay_name = argv[1];
		operation = argv[2];
	} else{
		return 0;
	}
	

	/*
	* Zerobytes files will be generated based on this file.
	* Change this to test different file
	*/
	// std::string filename("../originalFile/test.txt");
	std::string filename(replay_name);

	/*
	* Folder where zerobytes files will be placed
	*/
	std::string zerobytes_folder("./zeroBytesPrograms/");

	/*
	* Folder where the file will be reconstructed from the zerobytes files
	*/
	std::string reconstructed_folder("./recreatedFiles/");

	if (operation == "encode")
	{
		// first the function that creates zerobytes files is called
		ConvertToZerobytes(filename, zerobytes_folder);
	} else if (operation == "decode"){
		// after that a function that converts the zerobytes files into the original file will be called
		ConvertFromZerobytes(zerobytes_folder, filename);
	}
	


	return 0;
}
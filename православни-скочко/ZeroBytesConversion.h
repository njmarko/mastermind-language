#pragma once
/*
* Author: Marko Njegomir
* Email:  njmarko1991@gmail.com
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
#include <fstream>
#include <iostream>
#include <sstream>
#include <experimental/filesystem> // http://en.cppreference.com/w/cpp/experimental/fs
#include <bitset>
#include <vector>
#include <regex>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

/**
* Filenames used by the experimental library filesystem
*/
typedef std::experimental::filesystem::path Path;

// used for generating binary ascii characters in  filenames
#define __BIN__  0

// used for generating hexadecimal ascii characters in filenames
#define __HEX__  1

/*
* Defines now many characters will be written into the name of the file. Windows limit is around 260 chars
* but it can be configured to be 32,767 characters
*/
#define NAMESIZE 150
/*
* Size of the identificator for zerobytes files
*/
#define ZB_ID_SIZE 10

/**
* This function loads the binary file and converts it to multiple zerobytes files
* that contain the binary code in the name of the file in the ASCII format.
* Each file has an unique identifier that specifies the order in which the files were formed.
* @param in_filename string destination of the file that is about to be converted
* @param out_folder string destination of the folder where zerobytes files will be placed
*/
void ConvertToZerobytes(const std::string& in_filename, const std::string& out_folder);

/**
* This function loads the binary ASCII code from the names of zerobytes files in the specified folder
* and creates the original file.
* Each file has an unique identifier that specifies the order in which the files were formed.
* @param in_folder string destination of the folder that contains zerobytes files that will be loaded
* @param out_filename string destination and name of the file that will be created from the loaded code
*/
void ConvertFromZerobytes(const std::string& in_folder, const std::string& out_filename);

/**
* Function that reads the filenames in the folder path and adds them to the vector of strings
* Solution found at: http://www.cplusplus.com/forum/windows/189681/
* @param path name
* @returns vector of strings that contain the filenames
*/
std::vector<std::string> get_filenames(Path path);

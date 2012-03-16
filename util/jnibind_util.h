/*                                                                                                                                        
* Copyright (c) 2012, Eunhyuk Kim(gunoodaddy) 
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of Redis nor the names of its contributors may be used
*     to endorse or promote products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <stdexcept> 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <regex.h>

std::string ProcessExec(const char *task, ...) {
	char process[4096];
	va_list args;
	va_start(args, task);
	vsprintf(process, task, args);
	va_end(args);

	int len_format = strlen(process);
	if(process[len_format - 1] == '\n')
		process[len_format - 1] = '\0';
	
	FILE *fp = popen(process, "r");
	printf("EXEC : %s\n", process);
	if(!fp) {
		printf("unittest compile internal error!\n");
		return "";
	}

	std::string result;
	#define BUFF_SIZE 1024
	char  buff[BUFF_SIZE];
	while( fgets( buff, BUFF_SIZE, fp) ) {
		result += buff;	
	}
	return result;
}


size_t FileRead(const std::string &path, std::string &output) {
	std::string contents;
	FILE *temp = fopen(path.c_str(), "r");
	if(!temp)
		return 0;
	fseek( temp, 0, SEEK_END ); 
	long size = ftell ( temp ); 
	fseek( temp, 0, SEEK_SET ); 

	char *temp_buffer = new char[size+1]; 
	long readlen = fread ( temp_buffer, sizeof ( char ), (int)size, temp ); 
	temp_buffer[readlen] = '\0'; 

	output = temp_buffer;
	delete [] temp_buffer;

	fclose(temp);

	return size;
}

size_t FileWrite(const std::string &path, const std::string &contents) {
	FILE *temp = fopen(path.c_str(), "w+");
	if(!temp)
		return 0;

	size_t size = fwrite( contents.c_str(), 1, contents.size(), temp );
	fclose(temp);
	return size;
}

int StringReplaceAll(std::string &src, const std::string& findstr, const std::string& target) {
	int nReplaceCnt = 0;
	size_t offset = 0;  
	while( true )   
	{   
		offset = src.find(findstr, offset);
		if( std::string::npos == offset )
			break;   

		src.replace( offset, findstr.length(), target ); 
		nReplaceCnt++;
	}   
	return nReplaceCnt;
}

bool RegMatch(std::string &src, const std::string& pattern) {
	regmatch_t pmatch[1];
	int status;
	int eflag = 0;
	regex_t re;
	if((status = regcomp( &re, pattern.c_str(), REG_EXTENDED)) == 0) {
		eflag = 0;
		if (status = regexec( &re, src.c_str(), 1, pmatch, eflag) == 0) {
			return true;
		} 
	}
	return false;
}

int RegReplace(std::string &src, const std::string& pattern, const std::string &target) {
#define MAX_MATCH_COUNT 100
	regmatch_t pmatch[MAX_MATCH_COUNT];
	int status;
	int eflag = 0;
	regex_t re;
	if((status = regcomp( &re, pattern.c_str(), REG_EXTENDED)) == 0) {
		eflag = 0;
		if (status = regexec( &re, src.c_str(), MAX_MATCH_COUNT, pmatch, eflag) == 0) {
			size_t i = 0;
			for(i = 0; i < MAX_MATCH_COUNT; i++) {
				if(pmatch[i].rm_so < 0)
					break;
				src.replace( pmatch[i].rm_so, pmatch[i].rm_eo, target );
			}
			return i;
		} 
	}
	return 0;
}

std::string &StringTrimLeft( std::string &s ) {
	s.erase( s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace))) );
	return s;
}

std::string &StringTrimRight (std::string &s ) {
	s.erase( std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end() );
	return s;
}

std::string &StringTrim(std::string &s) {
	return StringTrimLeft( StringTrimRight( s ) );
}

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------

std::string deleteHungrianExpression(const::std::string &name) {
	std::string newname = name;

	if(RegMatch(newname, "^str[A-Z]"))
		RegReplace(newname, "^str", "");
	if( RegMatch( newname, "^dw[A-Z]" ) )
		RegReplace( newname, "^dw", "" );
	if( RegMatch( newname, "^n[A-Z]" ) )
		RegReplace( newname, "^n", "" );
	if( RegMatch( newname, "^b[A-Z]" ) )
		RegReplace( newname, "^b", "" );
	if( RegMatch( newname, "^sz[A-Z]" ) )
		RegReplace( newname, "^sz", "" );
	if( RegMatch( newname, "^e[A-Z]" ) )
		RegReplace( newname, "^e", "" );
	return newname;
}

bool isSTLStringType(const std::string &ctype) {
	if( ctype == "string" )
		return true;
	if( ctype == "std::string" )
		return true;
	return false;
}

bool isStringType(const std::string &ctype) {
	if( ctype == "string_array" )
		return true;
	if( ctype == "string_class" )
		return true;
	if( ctype == "string_pointer" )
		return true;
	return false;
}

std::string getAvailableCType(const std::string &ctype) {
	if( ctype == "string_array" )
		return "char";
	if( ctype == "string_class" )
		return "std::string";
	if( ctype == "string_pointer" )
		return "char *";
	return ctype;
}

std::string getJavaTypeFromCType(const std::string &ctype) {
	if( isStringType(ctype) ) 
		return "String";
	return ctype;
}

std::string chageCommonFieldName(const std::string &name) {
	std::string newname = name;

	StringReplaceAll(newname, "ID", "Id");

	// add other rules..
	// HERE
	return newname;
}

std::string getFieldNameFirstUpper(const std::string &name) {
	std::string newname = chageCommonFieldName(name);
	newname[0] = toupper(newname[0]);
	return newname;
}

std::string getFieldNameFirstLower(const std::string &name) {
	std::string newname = chageCommonFieldName(name);
	if( isupper(newname[0]) && (newname.size() <= 1 || islower(newname[1])) )
		newname[0] = tolower(newname[0]);
	return newname;
}

std::string getFieldSigniture(const std::string &ctype) {
	if( isStringType(ctype) )
		return "Ljava/lang/String;";
	if( ctype == "int" )
		return "I";
	if( ctype == "char" )
		return "C";
	if( ctype == "bool" )
		return "Z";
	if( ctype == "short" )
		return "S";
	if( ctype == "double" )
		return "D";
	if( ctype == "float" )
		return "F";

	throw std::runtime_error("unknown type..");
}

std::string getJNIFieldMethodTypeName(const std::string &sigField) {
	if( sigField == "I" )
		return "Int";
	if( sigField == "C" )
		return "Char";
	if( sigField == "Z" )
		return "Boolean";
	if( sigField == "S" )
		return "Short";
	if( sigField == "D" )
		return "Double";
	if( sigField == "F" )
		return "Float";
	if( sigField == "Ljava/lang/String;" )
		return "Float";
	return "Object";
}


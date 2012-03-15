# jnibind
jnibind generates c++ & java jni binding class for your own c structure.

c structure parse tool : flex, bison

## Usage

`jnibind` \<input header file\> \<bind name\>

If you have some user defined type(of typedef) in input header file, you must add your new type in jnibind.l file.

jnibind's output files are 

* Bind\<name\>.cpp
* Bind\<name\>.h
* Bind\<name\>.java
* Bind\<name\>_test.cpp

you use thease file to your androind project.

Also `jnibind` perform making test program and running it to verify that output files are generated correctly.

## Sample

###	Input file
	typedef struct tagSampleData
	{
		int nID;
		bool bFlag;
		GTUserStatusType eStatus;
		GTCHAR strUserID[256];      /* comment test */
		char *ptr;
		// TEST
		std::string string1;
		/* TEST */
		string string2;
		// TEST
		char strCustomData[MAX_CUSTOM_DATA_SIZE];
		/* TEST */
		float testFloat;      
		size_t nCustomDataSize;
		bool bMobileUsageFlag;
		DWORD dwProfileImageCRC;
	}SAMPLE_DATA;




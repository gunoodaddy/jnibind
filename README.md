# jnibind
`jnibind` generates c++ & java jni binding classes for your own c structure.

c language parse tool : `flex`, `bison`

## Usage

`jnibind` \<input header file\> \<bind name\>

jnibind's output files are 

* Bind\<name\>.cpp
* Bind\<name\>.h
* Bind\<name\>.java
* Bind\<name\>_test.cpp

just use these files for your androind project.

Also `jnibind` perform making test program and running it to verify that output files are generated correctly. (by using `dummy_jni.h`, removed ndk-dependancy) 

see `(Autmatically generated) Cpp Test file`

## If new types exist in input file..

If you have some user defined types(of typedef) in input header file, you must add your new types in `jnibind.l` file.

see `DWORD` in jnibind.l 


## How to use Bind<name> Class

Here is a sample code..

	SAMPLE_DATA data;
	data.nID = 1;
	// ... assignment
	
	CBindSample *bindobj = new CBindSample(jniEnv, NULL, &data);
	bindobj->makeObject();
    	
	// Toss to java by JVM
	// ...
    	
	delete bindobj;
    	
    	
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


### (Autmatically generated) Cpp Header file
	// THIS FILE IS AUTOMATICALLY GENERATED BY jnibind
	// SO YOU SHOULD NOT EDIT THIS FILE
	// BY gunoodaddy, 18:55:30 Mar 15 2012
	#pragma once

	#include "bindutil.h"

	class CBindSample
	{
		public:

		CBindSample(JNIEnv *env, jobject obj, const SAMPLE_DATA *data = NULL) : object(obj)
		{
			if(data)
				rawData = const_cast<SAMPLE_DATA *>(data);
			else
				rawData = NULL;

			setEnv(env);
		}

		~CBindSample()
		{
			if(object)
				envJni->DeleteLocalRef(object);
		}

		static void initialize(JNIEnv *env)
		{
			classRef = env->FindClass("com/nhn/gametalk/api/type/BindSample");
			classRef = (jclass)env->NewGlobalRef(classRef);

			fidId = env->GetFieldID(classRef, "id", "I");
			fidFlag = env->GetFieldID(classRef, "flag", "Z");
			fidStatus = env->GetFieldID(classRef, "status", "I");
			fidUserId = env->GetFieldID(classRef, "userId", "Ljava/lang/String;");
			fidPtr = env->GetFieldID(classRef, "ptr", "Ljava/lang/String;");
			fidString1 = env->GetFieldID(classRef, "string1", "Ljava/lang/String;");
			fidString2 = env->GetFieldID(classRef, "string2", "Ljava/lang/String;");
			fidCustomData = env->GetFieldID(classRef, "customData", "Ljava/lang/String;");
			fidTestFloat = env->GetFieldID(classRef, "testFloat", "F");
			fidCustomDataSize = env->GetFieldID(classRef, "customDataSize", "I");
			fidMobileUsageFlag = env->GetFieldID(classRef, "mobileUsageFlag", "Z");
			fidProfileImageCRC = env->GetFieldID(classRef, "profileImageCRC", "I");

			midInit = env->GetMethodID(classRef, "<init>", "()V");
		}

		void setEnv(JNIEnv *env)
		{
			this->envJni = env;
		}

		jobject makeObject(bool fillEmptyData = false)
		{
			if(object)
				return object;

			object = envJni->NewObject(classRef, midInit);

			if(false == fillEmptyData)
			{
				setId(rawData->nID);
				setFlag(rawData->bFlag);
				setStatus(rawData->eStatus);
				setUserId(rawData->strUserID);
				setPtr(rawData->ptr);
				setString1(rawData->string1);
				setString2(rawData->string2);
				setCustomData(rawData->strCustomData);
				setTestFloat(rawData->testFloat);
				setCustomDataSize(rawData->nCustomDataSize);
				setMobileUsageFlag(rawData->bMobileUsageFlag);
				setProfileImageCRC(rawData->dwProfileImageCRC);
			}
			return object;
		}

		int id(void)
		{
			return envJni->GetIntField(object, fidId);
		}
		bool flag(void)
		{
			return envJni->GetBooleanField(object, fidFlag);
		}
		int status(void)
		{
			return envJni->GetIntField(object, fidStatus);
		}
		std::string userId(void)
		{
			return CBindUtil::getStringFromField(envJni, object, fidUserId);
		}
		std::string ptr(void)
		{
			return CBindUtil::getStringFromField(envJni, object, fidPtr);
		}
		std::string string1(void)
		{
			return CBindUtil::getStringFromField(envJni, object, fidString1);
		}
		std::string string2(void)
		{
			return CBindUtil::getStringFromField(envJni, object, fidString2);
		}
		std::string customData(void)
		{
			return CBindUtil::getStringFromField(envJni, object, fidCustomData);
		}
		float testFloat(void)
		{
			return envJni->GetFloatField(object, fidTestFloat);
		}
		int customDataSize(void)
		{
			return envJni->GetIntField(object, fidCustomDataSize);
		}
		bool mobileUsageFlag(void)
		{
			return envJni->GetBooleanField(object, fidMobileUsageFlag);
		}
		int profileImageCRC(void)
		{
			return envJni->GetIntField(object, fidProfileImageCRC);
		}
		void setId(int value)
		{
			envJni->SetIntField(object, fidId, value);
		}
		void setFlag(bool value)
		{
			envJni->SetBooleanField(object, fidFlag, value);
		}
		void setStatus(int value)
		{
			envJni->SetIntField(object, fidStatus, value);
		}
		void setUserId(const std::string &value)
		{
			CBindUtil::setStringToField(envJni, object, fidUserId, value);
		}
		void setPtr(const std::string &value)
		{
			CBindUtil::setStringToField(envJni, object, fidPtr, value);
		}
		void setString1(const std::string &value)
		{
			CBindUtil::setStringToField(envJni, object, fidString1, value);
		}
		void setString2(const std::string &value)
		{
			CBindUtil::setStringToField(envJni, object, fidString2, value);
		}
		void setCustomData(const std::string &value)
		{
			CBindUtil::setStringToField(envJni, object, fidCustomData, value);
		}
		void setTestFloat(float value)
		{
			envJni->SetFloatField(object, fidTestFloat, value);
		}
		void setCustomDataSize(int value)
		{
			envJni->SetIntField(object, fidCustomDataSize, value);
		}
		void setMobileUsageFlag(bool value)
		{
			envJni->SetBooleanField(object, fidMobileUsageFlag, value);
		}
		void setProfileImageCRC(int value)
		{
			envJni->SetIntField(object, fidProfileImageCRC, value);
		}
		void convertToData(void)
		{
			rawData->nID = id();
			rawData->bFlag = flag();
			rawData->eStatus = status();
			BIND_STR_COPY_SAFE(rawData->strUserID, userId().c_str());
			rawData->ptr = const_cast<char *>(ptr().c_str());
			rawData->string1 = string1();
			rawData->string2 = string2();
			BIND_STR_COPY_SAFE(rawData->strCustomData, customData().c_str());
			rawData->testFloat = testFloat();
			rawData->nCustomDataSize = customDataSize();
			rawData->bMobileUsageFlag = mobileUsageFlag();
			rawData->dwProfileImageCRC = profileImageCRC();
		}


	public:
		static jclass classRef;

		static jfieldID fidId;
		static jfieldID fidFlag;
		static jfieldID fidStatus;
		static jfieldID fidUserId;
		static jfieldID fidPtr;
		static jfieldID fidString1;
		static jfieldID fidString2;
		static jfieldID fidCustomData;
		static jfieldID fidTestFloat;
		static jfieldID fidCustomDataSize;
		static jfieldID fidMobileUsageFlag;
		static jfieldID fidProfileImageCRC;

		static jmethodID midInit;
	private:
		jobject object;
		JNIEnv *envJni;
		SAMPLE_DATA *rawData;
	};

### (Autmatically generated) Java file
	// THIS FILE IS AUTOMATICALLY GENERATED BY jnibind
	// SO YOU SHOULD NOT EDIT THIS FILE
	// BY gunoodaddy, 18:55:30 Mar 15 2012

	public class BindSample {
		public int id;
		public bool flag;
		public int status;
		public String userId;
		public String ptr;
		public String string1;
		public String string2;
		public String customData;
		public float testFloat;
		public int customDataSize;
		public bool mobileUsageFlag;
		public int profileImageCRC;
	}


	
### (Autmatically generated) Cpp Test file

	// THIS FILE IS AUTOMATICALLY GENERATED BY jnibind                                                                                        
	// SO YOU SHOULD NOT EDIT THIS FILE
	// BY gunoodaddy, 19:01:41 Mar 15 2012
 
	#include <cstdio>
	#include <string>
	#include "dummy_jni.h"
	 
	using namespace std;
	 
	#define MAX_CUSTOM_DATA_SIZE 0
	 
	typedef int GTUserStatusType;
	typedef char GTCHAR;
	typedef int DWORD;
	 
	 
	#include "sample.h"
	#include "BindSample.h"
	#include "BindSample.cpp"
	 
	int main() {
		CBindSample *bindobj = new CBindSample(NULL, NULL, NULL);
		bindobj->makeObject(true);
	 
		delete bindobj;
	 
		printf("CBindSample test ok!\n");
		return 0;	
	}
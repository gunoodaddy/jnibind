#pragma once

typedef char jbyte;
typedef int* jobject;
typedef int* jfieldID;
typedef int* jmethodID;
typedef int* jclass;
typedef int* jstring;

class JNIEnv {
public:
	jclass FindClass(const char *) { return 0; }	
	jobject NewGlobalRef(jobject) { return 0; }
	jobject NewObject(jclass clazz, jmethodID methodID, ...) { return 0; }

	jfieldID GetFieldID(jclass clazz, const char *name, const char *sig) { return 0; }
	jmethodID GetMethodID(jclass clazz, const char *name, const char *sig) { return 0; }

	jobject GetObjectField(jobject, jfieldID) { return 0; }
	int GetIntField(jobject obj, jfieldID fieldID) { return 0;}
	bool GetBooleanField(jobject obj, jfieldID fieldID) { return false; }
	jbyte GetByteField(jobject obj, jfieldID fieldID) { return 0;}
	char GetCharField(jobject obj, jfieldID fieldID) { return 0;}
	short GetShortField(jobject obj, jfieldID fieldID) { return 0;}
	long GetLongField(jobject obj, jfieldID fieldID) { return 0;}
	float GetFloatField(jobject obj, jfieldID fieldID) { return 0.0f;}
	double GetDoubleField(jobject obj, jfieldID fieldID) { return 0.0f;}

	void SetObjectField(jobject obj, jfieldID fieldID, jobject value) { }
	void SetIntField(jobject obj, jfieldID fieldID, int value) { }
	void SetFloatField(jobject obj, jfieldID fieldID, float value) { }
	void SetBooleanField(jobject obj, jfieldID fieldID, bool value) { }
	void SetByteField(jobject obj, jfieldID fieldID, jbyte value) { } 
	void SetCharField(jobject obj, jfieldID fieldID, char value) { }
	void SetShortField(jobject obj, jfieldID fieldID, short value) { }
	void SetLongField(jobject obj, jfieldID fieldID, long value) { }
	void SetDoubleField(jobject obj, jfieldID fieldID, double value) { }

	const char* GetStringUTFChars(jstring, bool *) { return 0; }
	jstring NewStringUTF(const char *) { return 0; }
	void ReleaseStringUTFChars(jstring, const char *) { }
	void DeleteLocalRef(jobject) { }
	void DeleteGlobalRef(jobject) { }

};



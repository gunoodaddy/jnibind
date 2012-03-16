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



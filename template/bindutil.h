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

#define BIND_STR_COPY_SAFE(DST, SRC) strncpy(DST, SRC, sizeof(DST))

class CBindUtil
{
public:
	static jobject newJObject(JNIEnv *env, jclass clazz, jmethodID midInit, void *bindKey, bool global = true) 
	{
		jobject tempObjectBinder = env->NewObject(clazz, midInit, bindKey);
		if(!global) {
			return tempObjectBinder;
		}

		jobject objectBinder = env->NewGlobalRef(tempObjectBinder);
		env->DeleteLocalRef(tempObjectBinder);
		return objectBinder;
	} 

	static void releaseJObject(JNIEnv *env, jobject jobj, bool global = true) 
	{
		if(NULL == jobj)
			return;

		if(global)
			env->DeleteGlobalRef(jobj);
		else
			env->DeleteLocalRef(jobj);
	}

	static std::string getStringFromField(JNIEnv *env, jobject obj, jfieldID fid)
	{
		jstring jstr = (jstring)env->GetObjectField(obj, fid);

		std::string str = getStringFromJString(env, jstr);

		env->DeleteLocalRef(jstr);
		return str;
	}

	static std::string getStringFromJString(JNIEnv *env, jstring jstr)
	{
		if(NULL == jstr)
			return "";

		const char *tname = env->GetStringUTFChars(jstr, NULL);
		std::string result = tname;
		env->ReleaseStringUTFChars(jstr, tname);
		return result;
	}

	static void setStringToField(JNIEnv *env, jobject obj, jfieldID fid, const std::string &string)
	{
		jstring strName = env->NewStringUTF(string.c_str());
		env->SetObjectField(obj, fid, strName);
		env->DeleteLocalRef((jobject)strName);
	}


	static jstring newJString(JNIEnv *env, const char *str, bool global = true) {
		jstring jstr = env->NewStringUTF(str);
		if(global) {
			jstring globalStr = (jstring)env->NewGlobalRef(jstr);
			env->DeleteLocalRef(jstr);
			return globalStr;
		} else {
			return jstr;
		}
	}

	static void releaseJString(JNIEnv *env, jstring jstr, bool global = true) {
		if(NULL == jstr)
			return;

		if(global)
			env->DeleteGlobalRef(jstr);
		else
			env->DeleteLocalRef(jstr);
	}
};


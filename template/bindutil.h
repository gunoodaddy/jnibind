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


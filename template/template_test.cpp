// THIS FILE IS AUTOMATICALLY GENERATED BY $$KEH_BIND_PROGRAM$$
// SO YOU SHOULD NOT EDIT THIS FILE
// BY gunoodaddy, $$KEH_DATE$$

#include <cstdio>
#include <string>
#include "dummy_jni.h"

using namespace std;

$$KEH_UNKNOWN_DEF_REGION$$
$$KEH_NEW_TYPE_DEF_REGION$$

#include "$$KEH_INPUT_FILE$$"
#include "$$KEH_CLASS_HEADER$$"
#include "$$KEH_CLASS_SOURCE$$"

int main() {
	$$KEH_CLASSNAME$$ *bindobj = new $$KEH_CLASSNAME$$(NULL, NULL, NULL);
	bindobj->makeObject(true);

	delete bindobj;

	printf("$$KEH_CLASSNAME$$ test ok!\n");
	return 0;
}



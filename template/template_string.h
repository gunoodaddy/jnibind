#pragma once

const char *getterTepmplete = 
"\t$$TYPE$$ $$FIRST_LOWER_NAME$$(void)\n"
"\t{\n"
"\t\treturn envJni->$$JNI_GET_FIELD_METHOD_NAME$$(object, $$FID$$);\n"
"\t}\n";

const char *setterTepmplete = 
"\tvoid set$$FIRST_UPPER_NAME$$($$TYPE$$ value)\n"
"\t{\n"
"\t\tenvJni->$$JNI_SET_FIELD_METHOD_NAME$$(object, $$FID$$, value);\n"
"\t}\n";

const char *getterTepmpleteForString = 
"\tstd::string $$FIRST_LOWER_NAME$$(void)\n"
"\t{\n"
"\t\treturn CBindUtil::getStringFromField(envJni, object, $$FID$$);\n"
"\t}\n";

const char *setterTepmpleteForString = 
"\tvoid set$$FIRST_UPPER_NAME$$(const std::string &value)\n"
"\t{\n"
"\t\tCBindUtil::setStringToField(envJni, object, $$FID$$, value);\n"
"\t}\n";

const char *covertFuncTemplete =
"\tvoid convertToData(void)\n"
"\t{\n"
"$$CONVERT_DATA_FUNCTION_REGION$$\n"
"\t}\n";


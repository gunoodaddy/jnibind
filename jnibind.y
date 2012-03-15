%{
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include "template/template_string.h"
#include "util/jnibind_util.h"
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

extern char yytext[];
extern int column;
     
void yyerror(const char *s);

//=====================================================
typedef std::vector<std::string> TYPES;
typedef std::vector<std::string> IDENTIFIERS;
std::set<std::string> unknown_defines_;
TYPES result_types_;	// parsed result type
TYPES real_types_;		// parsed real type
TYPES org_types_;		// original input type
IDENTIFIERS identifiers_;
std::string curr_identifier_;
std::string curr_constant_identifier_;
std::string curr_type_;
std::string curr_struct_name_;
std::string curr_org_type_;
int currStructureCount = 0;

bool struct_decl_mode = false;
bool pointerflag = false;
bool arrayflag = false;
bool errorflag = false;
//=====================================================

%}

%union {
	char *sval;
}

%token CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID BOOL STRING_POINTER STRING_CLASS
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token<sval> IDENTIFIER

%start translation_unit
%%

primary_expression
	: IDENTIFIER {
		curr_constant_identifier_ = $1;
	}
	| CONSTANT
	| STRING_LITERAL
	| '(' expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']' 
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression 
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';' {
		curr_struct_name_ = curr_identifier_;
		if(++currStructureCount > 1) {
			// error!!
			// current spec is for only one structure.
			printf("\n\nyour input file must have only one structure..\n");
			exit(1);
		}
	}
	;

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier
	| type_qualifier declaration_specifiers
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator
	| declarator '=' initializer
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID
	| STRING_CLASS {
		curr_type_ = "string_class";
	}
	| STRING_POINTER {
		curr_type_ = "string_pointer";
	}
	| CHAR {
		curr_type_ = "char";
	}
	| SHORT {
		curr_type_ = "short";
	}
	| INT {
		curr_type_ = "int";
	}
	| LONG {
		curr_type_ = "long";
	}
	| FLOAT {
		curr_type_ = "float";
	}
	| DOUBLE {
		curr_type_ = "double";
	}
	| SIGNED
	| UNSIGNED
	| BOOL {
		curr_type_ = "bool";
	}
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}' {
		curr_struct_name_ = $2
	}
	| struct_or_union '{' struct_declaration_list '}' 
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' {

		real_types_.push_back(curr_type_);

		if(curr_type_ == "char") {
			if( arrayflag ) 
				curr_type_ = "string_array";
			else if( pointerflag)
				curr_type_ = "string_pointer";
		}
		result_types_.push_back(curr_type_);
		org_types_.push_back(curr_org_type_);
		identifiers_.push_back(curr_identifier_);

		// MUST BE false HERE for the next parsing
		pointerflag = false;
		arrayflag = false;
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list 
	| type_specifier 
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;

type_qualifier
	: CONST
	| VOLATILE
	;

declarator
	: pointer direct_declarator
	| direct_declarator
	;

direct_declarator
	: IDENTIFIER
	{
		curr_identifier_ = $1;
	}

	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']' {
		if( !curr_constant_identifier_.empty() ) {
			unknown_defines_.insert(curr_constant_identifier_);
			curr_constant_identifier_.clear();
		}
		arrayflag = true;
	}
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	;

pointer
	: '*' {
		pointerflag = true;
	}
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator {
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: declaration
	;

%%
     
void yyerror(const char *str)
{   
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, str);
	errorflag = true;
}   

int yywrap()
{   
	return 1;   
}  

int main(int argc, char** argv) {

	if (argc < 3) {
		printf("usage : %s <input file> <output name>\n", argv[0]);
		return -1;
	}

	std::string inputName = argv[1];

	yyin = fopen(inputName.c_str(), "r");
	yyparse();
	fclose(yyin);                       

	if(errorflag) {
		return 1;
	}

	const char *		TEMPLATE_CPP_HEADER = "./template/template.h";
	const char *		TEMPLATE_CPP_SOURCE = "./template/template.cpp";
	const char *		TEMPLATE_CPP_TEST	= "./template/template_test.cpp";
	const char *		TEMPLATE_JAVA 		= "./template/template.java";
	const std::string 	PREFIX 				= "Bind";

	std::string template_cpp_header;
	std::string template_cpp_source;
	std::string template_cpp_test;
	std::string template_java;
	
	// read template files
	FileRead(TEMPLATE_CPP_HEADER, 	template_cpp_header);
	FileRead(TEMPLATE_CPP_SOURCE, 	template_cpp_source);
	FileRead(TEMPLATE_CPP_TEST, 	template_cpp_test);
	FileRead(TEMPLATE_JAVA, 		template_java);

	std::string dateString = __TIME__;
	dateString += " ";
	dateString += __DATE__;
	std::string bindProgramName = argv[0];
	std::string outputName = argv[2];
	std::string className = "C" + PREFIX+ outputName;
	std::string javaClassName = PREFIX + outputName;
	std::string structName = curr_struct_name_;

	// output file name
	std::string outputHeaderFileName = PREFIX+outputName+".h";
	std::string outputSourceFileName = PREFIX+outputName+".cpp";
	std::string outputTestSourceFileName = PREFIX+outputName+"_test.cpp";
	std::string outputJavaFileName = PREFIX+outputName+".java";

	// declare output string variables
	std::string dummyNewTypeDeclareRegion;
	std::string dummyUnknownDeclareRegion;
	std::string fidDeclareRegion;
	std::string fidDeclareInCppRegion;
	std::string fidAssignRegion;
	std::string getterMethodRegion;
	std::string setterMethodRegion;
	std::string callSetterMethodRegion;
	std::string convertDataRegion;
	std::string javaDeclareRegion;

	// temporary
	std::string temp;
	std::set<std::string> newtypes;

	// generate output string
	for(size_t i = 0; i < identifiers_.size(); i++) {

		std::string &org_type = org_types_[i];
		std::string &type = result_types_[i];
		std::string &name = identifiers_[i];

		temp = deleteHungrianExpression(name);

		std::string javaType = getJavaTypeFromCType(type);
		std::string capName = getFieldNameFirstUpper(temp);
		std::string smallName = getFieldNameFirstLower(temp);

		std::string fidSig = getFieldSigniture(type);
		std::string fidName = "fid" + capName;
		std::string setMethodName = "set" + capName;
		std::string jniGetFieldMethodName = "Get"+getJNIFieldMethodTypeName(fidSig)+"Field";
		std::string jniSetFieldMethodName = "Set"+getJNIFieldMethodTypeName(fidSig)+"Field";

		//printf(">>>>>>>>>> %s %s %s\n", type.c_str(), fidSig.c_str(), jniGetFieldMethodName.c_str());

		// Make Declaration of jfieldIDs Region in header..
		{
			temp = "\tstatic jfieldID "+fidName+";";
			fidDeclareRegion += temp+"\n";
		}
		// Make Declaration of jfieldIDs Region in cpp..
		{
			temp = "jfieldID "+className+"::"+fidName+";";
			fidDeclareInCppRegion += temp+"\n";
		}
	 	// Make Assign of jfieldIDs Region..
		{
			temp = "\t\t"+fidName+" = env->GetFieldID(classRef, \""+smallName+"\", \""+fidSig+"\");";
			fidAssignRegion += temp+"\n";
		}
		// Make getter Method Region..
		{
			if( isStringType(type) ) {
				temp = getterTepmpleteForString;
				StringReplaceAll(temp, "$$FIRST_LOWER_NAME$$", smallName);
				StringReplaceAll(temp, "$$FID$$", fidName);
			} else {
				temp = getterTepmplete;
				StringReplaceAll(temp, "$$TYPE$$", type);
				StringReplaceAll(temp, "$$FIRST_LOWER_NAME$$", smallName);
				StringReplaceAll(temp, "$$FID$$", fidName);
				StringReplaceAll(temp, "$$JNI_GET_FIELD_METHOD_NAME$$", jniGetFieldMethodName);
			}
			getterMethodRegion += temp;
		}
		// Make setter Method Region..
		{
			if( isStringType(type) ) {
				temp = setterTepmpleteForString;
				StringReplaceAll(temp, "$$FIRST_UPPER_NAME$$", capName);
				StringReplaceAll(temp, "$$FID$$", fidName);
			} else {
				temp = setterTepmplete;
				StringReplaceAll(temp, "$$TYPE$$", type);
				StringReplaceAll(temp, "$$FIRST_UPPER_NAME$$", capName);
				StringReplaceAll(temp, "$$FID$$", fidName);
				StringReplaceAll(temp, "$$JNI_SET_FIELD_METHOD_NAME$$", jniSetFieldMethodName);
			}
			setterMethodRegion += temp;
		}
		// Make call setter Method Region..
		{
			callSetterMethodRegion += "\t\t\t"+setMethodName+"(rawData->"+name+");\n";
		}
		// Make convertToData Region..
		{
			if( isStringType(type) ) {
				if( type == "string_pointer" ) {
					// pointer style assign     
					temp = "\t\trawData->"+name+" = const_cast<char *>("+smallName+"().c_str());";	
				} else if (type == "string_class" ) {
					// stl style assign
					temp = "\t\trawData->"+name+" = "+smallName+"();";
				} else {
					// string_array
					temp = "\t\tBIND_STR_COPY_SAFE(rawData->"+name+", "+smallName+"().c_str());";	// strcpy
				}
			} else {
				temp = "\t\trawData->"+name+" = "+smallName+"();";
			}
			convertDataRegion += temp +"\n";
		}
		// Make java declaration filed Region..
		{
			javaDeclareRegion += "\tpublic "+javaType+" "+smallName+";"+"\n";
		}
		// Make Dummy unit test declare Resgion..
		{
			if(org_type.empty() == false) {
				if(isSTLStringType(org_type) == false) {
					std::string ctype = getAvailableCType(real_types_[i]);
					if(ctype != org_type) {
						std::set<std::string>::iterator it = newtypes.find(org_type);	
						if(it == newtypes.end()) {
							temp = "typedef " + ctype + " " + org_type + ";\n";
							dummyNewTypeDeclareRegion += temp;
							newtypes.insert(org_type);
						}
					}
				}
			}
		}

		//printf("result => %s(%s)(%s) = %s\n", result_types_[i].c_str(), real_types_[i].c_str(), org_types_[i].c_str(), identifiers_[i].c_str());
		//printf("name=%s, javatype=%s, capname=%s, smallname=%s\n", name.c_str(), javaType.c_str(), capName.c_str(), smallName.c_str());
	}
	//printf("%s\n", dummyNewTypeDeclareRegion.c_str());
	//-----------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------
	//------------------------------------[ LOOP END ]-----------------------------------------------
	//-----------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------

	// Make dummy define for unknown definitions
	{
		std::set<std::string>::iterator it = unknown_defines_.begin();
		for(; it != unknown_defines_.end(); it++) {
			temp = "#define " + (*it) + " 0\n";
			dummyUnknownDeclareRegion += temp;
		}
	}

	// last new line trip..
	{
		fidDeclareRegion 		= StringTrimRight( fidDeclareRegion );
		fidDeclareInCppRegion 	= StringTrimRight( fidDeclareInCppRegion );
		fidAssignRegion 		= StringTrimRight( fidAssignRegion );
		getterMethodRegion 		= StringTrimRight( getterMethodRegion );
		setterMethodRegion 		= StringTrimRight( setterMethodRegion );
		convertDataRegion 		= StringTrimRight( convertDataRegion );
		callSetterMethodRegion 	= StringTrimRight( callSetterMethodRegion );
		javaDeclareRegion 		= StringTrimRight( javaDeclareRegion );
	}

	// Make convert function region
	{
		temp = covertFuncTemplete;
		StringReplaceAll(temp, "$$CONVERT_DATA_FUNCTION_REGION$$", convertDataRegion);
		convertDataRegion  = temp;
	}

	// generate cpp header 
	{
		StringReplaceAll(template_cpp_header, "$$KEH_BIND_PROGRAM$$", bindProgramName);
		StringReplaceAll(template_cpp_header, "$$KEH_DATE$$", dateString);
		StringReplaceAll(template_cpp_header, "$$KEH_CLASSNAME$$", className);
		StringReplaceAll(template_cpp_header, "$$KEH_STRUCTNAME$$", structName);
		StringReplaceAll(template_cpp_header, "$$KEH_JAVA_CLASSNAME$$", javaClassName);
		StringReplaceAll(template_cpp_header, "$$KEH_DECLARE_JFIILDID_REGION$$", fidDeclareRegion);
		StringReplaceAll(template_cpp_header, "$$KEH_FID_ASSIGN_REGION$$", fidAssignRegion);
		StringReplaceAll(template_cpp_header, "$$KEH_GETTER_METHOD_REGION$$", getterMethodRegion);
		StringReplaceAll(template_cpp_header, "$$KEH_SETTER_METHOD_REGION$$", setterMethodRegion);
		StringReplaceAll(template_cpp_header, "$$KEH_CALL_SETTER_METHOD_REGION$$", callSetterMethodRegion);
		StringReplaceAll(template_cpp_header, "$$KEH_CONVERT_DATA_FUNCTION$$", convertDataRegion);

		FileWrite(outputHeaderFileName, template_cpp_header);
		//printf("%s\n", template_cpp_header.c_str());
	}

	// generate cpp source
	{
		StringReplaceAll(template_cpp_source, "$$KEH_BIND_PROGRAM$$", bindProgramName);
		StringReplaceAll(template_cpp_source, "$$KEH_DATE$$", dateString);
		StringReplaceAll(template_cpp_source, "$$KEH_CLASSNAME$$", className);
		StringReplaceAll(template_cpp_source, "$$KEH_CLASS_HEADER$$", outputHeaderFileName);
		StringReplaceAll(template_cpp_source, "$$KEH_STATIC_DECLARE_JFIILDID_REGION$$", fidDeclareInCppRegion);
		
		FileWrite(outputSourceFileName, template_cpp_source);
		//printf("%s\n", template_cpp_source.c_str());
	}

	// generate cpp unit test source 
	{
		StringReplaceAll(template_cpp_test, "$$KEH_BIND_PROGRAM$$", bindProgramName);
		StringReplaceAll(template_cpp_test, "$$KEH_DATE$$", dateString);
		StringReplaceAll(template_cpp_test, "$$KEH_CLASSNAME$$", className);
		StringReplaceAll(template_cpp_test, "$$KEH_CLASS_HEADER$$", outputHeaderFileName);
		StringReplaceAll(template_cpp_test, "$$KEH_CLASS_SOURCE$$", outputSourceFileName);
		StringReplaceAll(template_cpp_test, "$$KEH_INPUT_FILE$$", inputName);
		StringReplaceAll(template_cpp_test, "$$KEH_UNKNOWN_DEF_REGION$$", dummyUnknownDeclareRegion);
		StringReplaceAll(template_cpp_test, "$$KEH_NEW_TYPE_DEF_REGION$$", dummyNewTypeDeclareRegion);
		
		FileWrite(outputTestSourceFileName, template_cpp_test);
		//printf("%s\n", template_java.c_str());
	}

	// generate java class source 
	{
		StringReplaceAll(template_java, "$$KEH_BIND_PROGRAM$$", bindProgramName);
		StringReplaceAll(template_java, "$$KEH_DATE$$", dateString);
		StringReplaceAll(template_java, "$$KEH_JAVA_CLASSNAME$$", javaClassName);
		StringReplaceAll(template_java, "$$KEH_JAVA_DECLARE_FIELD_REGION$$", javaDeclareRegion);
		
		FileWrite(outputJavaFileName, template_java);
		//printf("%s\n", template_java.c_str());
	}

	// result print
	{
		printf("\n");
		printf("[Result]\n");
		printf("Date            : %s\n", dateString.c_str());
		printf("Struct Name     : %s\n", structName.c_str());
		printf("Class Name      : %s\n", className.c_str());
		printf("Java Class Name : %s\n", javaClassName.c_str());
		printf("CPP Header File : %s\n", outputHeaderFileName.c_str());
		printf("CPP Source File : %s\n", outputSourceFileName.c_str());
		printf("CPP Test File   : %s\n", outputTestSourceFileName.c_str());
		printf("Java File       : %s\n", outputJavaFileName.c_str());
	}

	// compile unit test & run
	{
		printf("\n");
		printf("Creating test program...\n");
		std::string result;
		result = ProcessExec( "g++  -g -Wall -I./template -I./util %s -o testbind 2>&1", outputTestSourceFileName.c_str());
		printf( "Result :\n%s\n", result.c_str());
		if(result.empty() == false) {
			// compile error occured..
			return -1;
		}
		
		printf("\n");
		printf("Running test program...\n");
		result = ProcessExec( "./testbind" );
		printf( "Result :\n%s\n", result.c_str());
	}

	return 0;
}



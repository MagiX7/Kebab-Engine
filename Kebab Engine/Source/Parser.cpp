#include "Parser.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

JSON_Value* Parser::InitValue()
{
	return json_value_init_object();
}

JSON_Value* Parser::InitArray()
{
	return json_value_init_array();
}

void Parser::SetObjectValue(JSON_Object* obj, const char* name, JSON_Value* value)
{
	json_object_set_value(obj, name, value ? value : json_value_init_object());
}

void Parser::SetObjectNumber(JSON_Object* obj, const char* name, const double& number)
{
	json_object_set_number(obj, name, number);
}

void Parser::SetObjectString(JSON_Object* obj, const char* name, const char* text)
{
	json_object_set_string(obj, name, text);
}

void Parser::DotSetObjectValue(JSON_Object* obj, const char* name, JSON_Value* value)
{
	json_object_dotset_value(obj, name, value);
}

void Parser::DotSetObjectString(JSON_Object* obj, const char* name, const char* text)
{
	json_object_dotset_string(obj, name, text);
}

void Parser::DotSetObjectNumber(JSON_Object* obj, const char* name, double number)
{
	json_object_dotset_number(obj, name, number);
}

JSON_Object* Parser::GetObjectByObject(JSON_Object* object, const char* name)
{
	return json_object_get_object(object, name);
}

JSON_Object* Parser::GetObjectByValue(JSON_Value* value)
{
	return json_value_get_object(value);
}

JSON_Object* Parser::GetObjectByName(JSON_Object* obj, const char* name)
{
	return json_object_get_object(obj, name);
}

double Parser::GetNumberFromObject(JSON_Object* obj, const char* name)
{
	return json_object_get_number(obj, name);
}

JSON_Array* Parser::GetArrayByValue(JSON_Value* value)
{
	return json_value_get_array(value);
}

JSON_Array* Parser::GetArrayByObject(JSON_Object* object, const char* name)
{
	return json_object_get_array(object, name);
}

JSON_Value* Parser::GetValueByObject(JSON_Object* obj, const char* name)
{
	return json_object_get_value(obj, name);
}

void Parser::AppendValueToArray(JSON_Array* array, JSON_Value* value)
{
	json_array_append_value(array, value);
}

void Parser::FreeValue(JSON_Value* value)
{
	json_value_free(value);
}

void Parser::GenerateFile(JSON_Value* value, const char* fileName)
{
	json_serialize_to_file_pretty(value, fileName);
}

JSON_Value* Parser::ParseFile(const char* fileName)
{
	return json_parse_file(fileName);
}

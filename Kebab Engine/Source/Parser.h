#pragma once

#include "parson.h"


class Parser
{
public:
	Parser();
	virtual ~Parser();

	static JSON_Value* InitValue();
	static JSON_Value* InitArray();

	static void SetObjectValue(JSON_Object* obj, const char* name, JSON_Value* value = nullptr);
	static void SetObjectNumber(JSON_Object* obj, const char* name, const double& number);
	static void SetObjectString(JSON_Object* obj, const char* name, const char* text);
	static void DotSetObjectValue(JSON_Object* obj, const char* name, JSON_Value* value);
	static void DotSetObjectString(JSON_Object* obj, const char* name, const char* text);
	static void DotSetObjectNumber(JSON_Object* obj, const char* name, double number);

	static JSON_Object* GetObjectByObject(JSON_Object* object, const char* name);
	static JSON_Object* GetObjectByValue(JSON_Value* value);
	static JSON_Object* GetObjectByName(JSON_Object* obj, const char* name);
	static double GetNumberFromObject(JSON_Object* obj, const char* name);
	static JSON_Array* GetArrayByValue(JSON_Value* value);
	static JSON_Array* GetArrayByObject(JSON_Object* object, const char* name);
	static JSON_Value* GetValueByObject(JSON_Object* value, const char* name);
	

	static void AppendValueToArray(JSON_Array* array, JSON_Value* value);

	static void FreeValue(JSON_Value* value);
	static void GenerateFile(JSON_Value* value, const char* fileName);
	static JSON_Value* ParseFile(const char* fileName);
};
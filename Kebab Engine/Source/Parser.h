#pragma once

#include "parson/src/parson.h"

#include <string>

class Parser
{
public:
	void SaveFile()
	{
		JSON_Value* data = json_value_init_object();
		std::string name = "PEPE";
		json_object_set_string(json_object(data), "Name", name.c_str());
		json_serialize_to_file(data, "FILE.json");
	}
	void ReadFile()
	{

	}

};
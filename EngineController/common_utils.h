
#pragma once

#include <stdio.h>
#include <string>

#include "json/json.h"

// #include "rapidjson/rapidjson.h"
// #include "rapidjson/document.h"
// #include "rapidjson/reader.h"
// #include "rapidjson/writer.h"
// #include "rapidjson/stringbuffer.h"

// using namespace rapidjson;

int GetJsonDataInt(const Json::Value& root, const std::string key, int& data);
int GetJsonDataString(const Json::Value& root, const std::string key, std::string& data);
int GetJsonDataFloat(const Json::Value& root, const std::string key, float& data);


#pragma once

#include <stdio.h>
#include <string>


#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

int GetJsonDataInt(const rapidjson::Document& doc, const std::string key, int& data);
int GetJsonDataString(const rapidjson::Document& doc, const std::string key, std::string& data);
int GetJsonDataFloat(const rapidjson::Document& doc, const std::string key, float& data);

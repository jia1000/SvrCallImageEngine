
#pragma once

#include <stdio.h>
#include <string>
#include "json/json.h"

int GetJsonDataInt(const Json::Value& root, const std::string key, int& data);
int GetJsonDataString(const Json::Value& root, const std::string key, std::string& data);
int GetJsonDataFloat(const Json::Value& root, const std::string key, float& data);


#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include "json/json.h"

int GetJsonDataInt(const Json::Value& root, const std::string key, int& data);
int GetJsonDataString(const Json::Value& root, const std::string key, std::string& data);
int GetJsonDataFloat(const Json::Value& root, const std::string key, float& data);

void SplitString(const std::string& src, std::vector<std::string>& v, const std::string& c);
void TryCreateDir(const std::string& dir);
	

int ListDir(std::string path, std::vector<std::string>& files);
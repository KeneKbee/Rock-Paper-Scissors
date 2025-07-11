#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

namespace ini
{
	std::unordered_map<std::string, std::string> readFile(const std::string path)
	{
		std::unordered_map<std::string, std::string> data;

		std::ifstream iniFile(path);

		std::string line = "";

		while (std::getline(iniFile, line))
		{
			if (line == "")
			{
				continue;
			}

			std::string text = "";
			for (char c : line)
			{
				if (c != ' ')
				{
					text += c;
				}
			}

			if (text == "")
			{
				continue;
			}

			std::string first = "";
			std::string second = "";

			bool isPartOfFirst = true;

			for (char c : text)
			{
				if (c == ':')
				{
					isPartOfFirst = false;
					continue;
				}

				if (isPartOfFirst)
				{
					first += c;
				}
				else {
					second += c;
				}
			}

			if (first == "" || second == "")
			{
				throw std::runtime_error("Ivalid ini file syntax detected!");
			}
			else {
				data[first] = second;
			}
		}

		iniFile.close();

		return data;
	}

	void saveFile(const std::string path, const std::unordered_map<std::string, std::string> data)
	{
		std::ofstream iniFile(path);

		std::string dataPair = "";

		for (auto& pair : data)
		{
			if (pair.first.find(' ') != 0 || pair.first.find(':') != 0 || pair.second.find(' ') != 0 || pair.second.find(':') != 0)
			{
				throw std::runtime_error("Invalid ini syntax detected!");
			}

			dataPair += pair.first + ": " + pair.second + "\n";
		}

		iniFile << dataPair;

		iniFile.close();
	}
}
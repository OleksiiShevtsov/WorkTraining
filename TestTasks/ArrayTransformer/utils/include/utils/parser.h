#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

namespace at
{
    template <typename Container>
    class FileArrayReader
    {
    public:
        explicit FileArrayReader(const std::string& filename)
            : m_filename(filename)
        {}

        Container readArrays()
        {
            Container arrays;
            std::ifstream file(m_filename);

            if (!file.is_open())
            {
                throw std::runtime_error("Error: Failed to open file " + m_filename);
            }

            std::string line;

            while (std::getline(file, line))
            {
                arrays.insert(arrays.end(), parseLine(line));
            }

            file.close();
            return arrays;
        }

    private:

        typename Container::value_type parseLine(const std::string& line)
        {
            typename Container::value_type result;
            std::stringstream ss;

            for (char ch : line)
            {
                if (std::isdigit(ch) || ch == '-')
                {
                    ss << ch;
                }
                else if (!ss.str().empty())
                {
                    result.insert(result.end(), std::stoi(ss.str()));
                    ss.str("");
                    ss.clear();
                }
            }

            if (!ss.str().empty())
            {
                result.insert(result.end(), std::stoi(ss.str()));
            }

            return result;
        }

        std::string m_filename;
    };
}

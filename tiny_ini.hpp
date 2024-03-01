#pragma once
#ifndef TINY_INI_HPP
#define TINY_INI_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iostream>

/*
 * ini_parser.hpp
 *
 * This is a simple C++ INI file parser.
 *
 * Example .ini file format:
 *
 * ;workspace
 * [Profile]
 * name=""
 * username=""
 * age=""
 *
 * [Profile.Data]
 * size=100
 */

struct IniProperty {
    std::string name;
    std::string value;
};

struct IniSection {
    friend class IniParser;
    std::vector<IniProperty> properties;
    std::string name;

    std::string& operator[](const std::string& name);

    // Add a property to this section
    void setProperty(const std::string& name, const std::string& value);

    // Remove a property from this section
    void removeProperty(const std::string& name);
};

class IniParser {
public:
    IniParser() : m_globalSection(std::make_shared<IniSection>()) {}

    // Serialize the INI into string
    std::string serialize() const;

    // Deserialize content from an INI string
    void deserialize(const std::string& content);

    // Get a section from the INI data
    std::shared_ptr<IniSection> getSection(const std::string& section_name = "") const;

    // Add a new section to the INI
    void addSection(const std::string& name);

    // Remove a section and its properties
    void removeSection(const std::string& section_name);

    // Add or update a property in a section (global section by default)
    void setProperty(const std::string& name, const std::string& value, const std::string& section_name = "");

    // Remove a property from a section (global section by default)
    void removeProperty(const std::string& name, const std::string& section_name = "");

public:
    IniSection& operator[](const std::string& section_name);
private:
    // Parse the content of the INI file
    void parse(const std::string& content);

    // Reset the parser state
    void reset();

private:
    std::unordered_map<std::string, std::shared_ptr<IniSection>> m_sections;
    std::shared_ptr<IniSection> m_globalSection;
};

#endif // !TINY_INI_HPP

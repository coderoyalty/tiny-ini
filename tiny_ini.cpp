#include "tiny_ini.hpp"

namespace Utils {
	static std::string trim(const std::string& str) {
		size_t first = str.find_first_not_of(" \t\r\n");
		size_t last = str.find_last_not_of(" \t\r\n");

		if (first == std::string::npos) {
			return "";
		}

		return str.substr(first, last - first + 1);
	}
};


void IniParser::parse(const std::string& content)
{
	reset();

	std::string currentSection;

	std::stringstream ss(content);
	std::string line;

	while (std::getline(ss, line)) {
		line = Utils::trim(line);
		if (line.empty() || line[0] == ';') continue;

		// get section
		if (line[0] == '[' && line[line.length() - 1] == ']') {
			currentSection = line.substr(1, (line.length() - 1) - 1);
			continue;
		}

		std::stringstream property_ss(line);
		std::string name,value;
		std::getline(property_ss, name, '=');
		std::getline(property_ss, value);

		name = Utils::trim(name);
		value = Utils::trim(value);

		// add property
		if (currentSection.empty()) {
			this->m_globalSection->properties.push_back({name, value});
		}
		else {
			if (!m_sections[currentSection]) {
				m_sections[currentSection] = std::make_shared<IniSection>();
			}
			auto& section = m_sections[currentSection];
			section->properties.push_back({ name, value });
			section->name = currentSection;
		}
	}
}


void IniParser::reset()
{
	m_sections.clear();
	m_globalSection.reset();
	m_globalSection = std::make_shared<IniSection>();
}

std::string IniParser::serialize() const
{
	std::stringstream ss;
	ss << "; generated using: https://github.com/coderoyalty/tiny_ini" << std::endl;
	// serialize global section properties first
	for (const auto& property : m_globalSection->properties) {
		ss << property.name << "=" << property.value << std::endl;
	}

	ss << std::endl;

	for (const auto& section_pair : m_sections) {
		ss << "[" << section_pair.first << "]" << std::endl;

		for (const auto& property : section_pair.second->properties) {
			ss << property.name << "=" << property.value << std::endl;
		}
		ss << std::endl;
	}

	return ss.str();
}

void IniParser::deserialize(const std::string& content)
{
	parse(content);
}

std::shared_ptr<IniSection> IniParser::getSection(const std::string& section_name) const
{
	if (section_name.empty()) {
		return m_globalSection;
	}

	auto it = m_sections.find(section_name);
	if (it != m_sections.end()) {
		return it->second;
	}
	else {
		return m_globalSection;
	}
}

void IniParser::addSection(const std::string& name)
{
	if (name.empty() || m_sections[name]) {
		return;
	}

	m_sections[name] = std::make_shared<IniSection>();
}

void IniParser::removeSection(const std::string& section_name)
{
	m_sections.erase(section_name);
}

void IniParser::setProperty(const std::string& name, const std::string& value, const std::string& section_name)
{
	if (section_name.empty()) {
		m_globalSection->setProperty(name, value);
	}
	else {
		auto it = m_sections.find(section_name);

		if (it != m_sections.end()) {
			it->second->setProperty(name, value);
		}
	}
}

void IniParser::removeProperty(const std::string& name, const std::string& section_name)
{
	if (section_name.empty()) {
		m_globalSection->removeProperty(name);
	}
	else {
		auto it = m_sections.find(section_name);

		if (it != m_sections.end()) {
			it->second->removeProperty(name);
		}
	}
}

IniSection& IniParser::operator[](const std::string& name)
{
	std::string section_name = Utils::trim(name);
	addSection(section_name); // add the section incase it's not existing
	if (section_name.empty()) {
		return *m_globalSection;
	}

	return *m_sections[section_name];
}

std::string& IniSection::operator[](const std::string& name)
{
	std::string propertyName = Utils::trim(name);
	for (auto& section : this->properties) {
		if (section.name == propertyName) {
			return section.value;
		}
	}

	this->properties.push_back({ propertyName, "" });
	return properties.back().value;
}

void IniSection::setProperty(const std::string& name, const std::string& value)
{
	std::string propertyName = Utils::trim(name);
	if (propertyName.empty()) {
		return;
	}

	for (auto& prop : this->properties) {
		if (prop.name == propertyName) {
			prop.value = value;
			return;
		}
	}

	this->properties.push_back({ propertyName, value });
}

void IniSection::removeProperty(const std::string& name)
{
	std::string propertyName = Utils::trim(name);
	auto it = std::find_if(this->properties.begin(), this->properties.end(),
		[&propertyName](IniProperty& property) {
			return property.name == propertyName;
		});

	if (it != this->properties.end()) {
		this->properties.erase(it);
	}
}

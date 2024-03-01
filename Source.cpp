#include "tiny_ini.hpp"


static void printIniSection(const std::shared_ptr<IniSection>& section) {
	if (!section->name.empty()) {
		std::cout << "[" << section->name << "]" << std::endl;
	}
	for (auto& prop : section->properties) {
		std::cout << prop.name << "=" << prop.value << std::endl;
	}
}

int main() {
	std::string content = "version=0.05\n[content]\n\nname=Emmanuel\nclass=lvl100\nname=Emmanuel\n";
	IniParser parser;
	parser.deserialize(content);

	parser.addSection("window");
	parser.addProperty("width", "300px", "window");
	parser.addProperty("height", "600px", "window");
	parser.addProperty("vSync", "false", "window");
	printIniSection(parser.getSection());
	printIniSection(parser.getSection("window"));
	std::cout << parser.serialize() << std::endl;
}

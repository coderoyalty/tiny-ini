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
	parser["window"]["width"] = "600";
	parser["window"]["height do"] = "800";
	parser.setProperty("vSync", "false", "window");
	printIniSection(parser.getSection());
	printIniSection(parser.getSection("window"));
	//parser["section"][""];
	std::cout << parser.serialize() << std::endl;
}

#pragma once

#include "BaseNode.h"
#include <string>

class ImportNode : public BaseNode {
public:
    std::string type; // @Chtl, @Html, @Style, @JavaScript, [Custom] @Element, etc.
    std::string item_name; // Optional: the name of the item to import
    std::string file_path;
    std::string as_name; // Optional: the alias to import as
};

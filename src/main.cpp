#define ZAML_IMPLEMENTATION
#include "Zaml.h"

#include "json.hpp"
using nlohmann::json;

#include "ScopeProfiler.h"

#if __EMSCRIPTEN__
#include <emscripten.h>
#endif

void display_text(const std::string& text)
{
#if __EMSCRIPTEN__
    const std::string add_to_dom_script = 
        "var tag = document.createElement(\"p\");"
        "var text = document.createTextNode(\""+ text +"\");"
        "tag.appendChild(text);"
        "var element = document.getElementById(\"testResults\");"
        "element.appendChild(tag);";
    
    emscripten_run_script(add_to_dom_script.c_str());
#else
    std::cout << text << std::endl;
#endif
}

void display_lines(const std::string& text)
{
    std::string line;
    std::istringstream lines(text);
    while(std::getline(lines, line))
    {
        display_text(line);
    }
}

int main()
{
    Zaml::Node config = Zaml::LoadFile(ASSETS_PATH + std::string("config.yaml"));
    
    display_text(config["window"]["title"].as<std::string>());
    display_lines(Zaml::ParseNode(config).str());
    
    
    if (config["test"].as<bool>())
    {
        for (int i = 0; i < config["test"].size(); i++)
        {
            if (config["test"][i].as<bool>())
            {
                if (config["test"][i]["title"].as<std::string>() != "~")
                {
                    display_text(config["test"][i]["title"].as<std::string>());
                }
                
                {
                    ScopeProfiler timer(config["test"][i]["title"].as<std::string>());
                    auto test = Zaml::LoadFile(ASSETS_PATH + config["test"][i]["file"].as<std::string>());
                    
                    display_text(timer.end());
                    display_lines( Zaml::ParseNode(test).str());
                }
            }
        }
    }
    return 0;
}

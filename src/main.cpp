#define NYML_IMPLEMENTATION
#define NYML_EXCEPTIONS
#include "Nyml.h"

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
    using namespace std::string_literals;
    nyml::Node config = nyml::LoadFile(ASSETS_PATH + std::string("config.nyml"));
    
    display_text(config["window"]["title"].as<std::string>());
    display_lines(nyml::Dump(config).str());
    
    display_text("\n\n");


    std::string test_data = 
        "test: ~\n"
        "  hello: world!\n";

    nyml::Node test_node = nyml::Parse(test_data);
    test_node["test"]["string"]["greeting"] = "How are you?"s;
    test_node["test"]["string"]["answer"] = "Good"s;
    test_node["test"]["bool"]["cpp_style_t"] = true;
    test_node["test"]["bool"]["cpp_style_f"] = false;


    display_lines(nyml::Dump(test_node).str());
    
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
                    auto test = nyml::LoadFile(ASSETS_PATH + config["test"][i]["file"].as<std::string>());
                    
                    display_text(timer.end());
                    display_text("---");
                    display_lines(nyml::Dump(test).str());
                    display_text("\n\n");
                }
            }
        }
    }
    return 0;
}

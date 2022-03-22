#define ZAML_IMPLEMENTATION
#include "Zaml.h"

#if __EMSCRIPTEN__
#include <emscripten.h>

void add_text_to_dom(const std::string& text)
{
    const std::string add_to_dom_script = 
        "var tag = document.createElement(\"p\");"
        "var text = document.createTextNode(\""+ text +"\");"
        "tag.appendChild(text);"
        "var element = document.getElementById(\"testResults\");"
        "element.appendChild(tag);";
    
    emscripten_run_script(add_to_dom_script.c_str());
}

#endif



int main()
{
    Zaml::Node test;
    test["hello"]["world"];
    
    Zaml::Node file_test = Zaml::LoadFile(ASSETS_PATH + std::string("config.yaml"));
    Zaml::Node ninja_test = Zaml::LoadFile(ASSETS_PATH + std::string("ninjato.zaml"));
    
    
    std::cout << Zaml::ParseNode(test).str() << std::endl;
    std::cout << Zaml::ParseNode(file_test).str() << std::endl;
    
    
#if __EMSCRIPTEN__
    std::string test_string = file_test["window"]["title"].as<std::string>();
    
    add_text_to_dom(test_string);
    add_text_to_dom( file_test["window"]["title"].as<std::string>());
    add_text_to_dom( ninja_test["meta"]["image"].as<std::string>());
    add_text_to_dom( Zaml::ParseNode(ninja_test).str());
    
#endif
    
    return 0;
}

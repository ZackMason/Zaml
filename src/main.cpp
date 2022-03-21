#define ZAML_IMPLEMENTATION
#include "Zaml.h"

int main()
{
    Zaml::Node test;
    test["hello"]["world"];
    
    std::cout << Zaml::ParseNode(test).str() << std::endl;
    
    return 0;
}

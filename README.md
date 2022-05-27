# Zaml
A simple file format similar to Yaml

---

A basic Zaml config file looks something like this

```
---
# Comments must start with a '#'
version: 0.1.1
window: ~
	width: 640
	height: 480
	title: Zaml is great
	font: ~
		file: Fonts/NotoMono-Regular.ttf
		size: 12

credits: ~
	-: ~
		name: Bill Gates
		role: Programmer
	-: ~
		name: Pablo Picasso
		role: Artist
		
# Any of these work as bools
settings: ~
	fullscreen: false
	mute: True
	vsync: off
	uncappedFPS: On
	
tests: true
	-: on
		title: test1
		file: test1.zaml
	-: off
		title: bench_test
		file: bench_test.zaml
  ```

## Code Example

```c++
#define ZAML_IMPLEMENTATION
#define ZAML_EXCEPTIONS
#include "Zaml.h"

int main()
{
	using namespace std::string_literals;
	// load from a file
	Zaml::Node config = Zaml::LoadFile(ASSETS_PATH + std::string("config.zaml"));

	std::cout << Zaml::Dump(config).str() << std::endl;

	// load from string
	std::string test_data = 
		"test: ~\n"
		"  hello: world!\n";

	Zaml::Node test_node = Zaml::Parse(test_data);
	test_node["test"]["bool"] = true;
	test_node["test"]["bool"] = false;

	try {
		test_node["test"]["bool"].as<std::string>(); // throws exception with ZAML_EXCEPTIONS
	} 
	catch(Zaml::ZamlInvalidAccessException & e)
	{
		std::cerr << e.pretty_print() << std::endl;
	}
	test_node["test"]["bool"].as<bool>(); // valid
	test_node["test"]["bool"] = "false"s;
	test_node["test"]["bool"].as<std::string>(); // valid
	test_node["test"]["bool"].as<bool>(); // invalid
	

	return 0;
}
```


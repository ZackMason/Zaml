# nyml - nyml not yaml
A simple file format similar to Yaml

---

A basic nyml config file looks something like this

```
---
# Comments must start with a '#'
version: 0.1.1
window: ~
	width: 640
	height: 480
	title: nyml is great
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
		file: test1.nyml
	-: off
		title: bench_test
		file: bench_test.nyml
  ```

## Code Example

```c++
#define NYML_IMPLEMENTATION
#define NYML_EXCEPTIONS
#include "Nyml.h"

int main()
{
	using namespace std::string_literals;
	// load from a file
	nyml::Node config = nyml::LoadFile(ASSETS_PATH + std::string("config.nyml"));

	std::cout << nyml::Dump(config).str() << std::endl;

	// load from string
	std::string test_data = 
		"test: ~\n"
		"  hello: world!\n";

	nyml::Node test_node = nyml::Parse(test_data);
	test_node["test"]["bool"] = true;
	test_node["test"]["bool"] = false;

	try {
		test_node["test"]["bool"].as<std::string>(); // throws exception with NYML_EXCEPTIONS
	} 
	catch(nyml::NymlInvalidAccessException & e)
	{
		std::cerr << e.pretty_print() << std::endl;
	}
	// text such as "on" and "Off" is only valid as a bool when using Parse
	test_node["test"]["bool"].as<bool>(); // valid
	test_node["test"]["bool"] = "false"s;
	test_node["test"]["bool"].as<std::string>(); // valid
	test_node["test"]["bool"].as<bool>(); // invalid
	

	return 0;
}
```


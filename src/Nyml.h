#pragma once
// Nyml.h - A simplefied Yaml parser
// add #define NYML_IMPLEMENTATION to a cpp file
// add #define NYML_EXCEPTIONS to add exceptions

#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <type_traits>
#include <cassert>

namespace nyml
{
#ifdef NYML_EXCEPTIONS
#undef NYML_EXCEPTIONS
#define NYML_EXCEPTIONS 1
#endif

#if NYML_EXCEPTIONS && !__EMSCRIPTEN__
	struct NymlInvalidAccessException : public std::exception
	{
		const char* file;
		int line;
		const char* func;
		const char* info;
		NymlInvalidAccessException(
			const char* msg, 
			const char* file_, 
			int line_, 
			const char* func_, 
			const char* info_ = "") : std::exception(msg),
        	file (file_),
        	line (line_),
        	func (func_),
        	info (info_)
        {
        }

		std::string pretty_print() const {
			std::stringstream ss;
			ss << what() << " - " << get_file() << ":" << get_line() << "::" << get_func() << std::endl;
			return ss.str();
		}
	
        const char* get_file() const { return file; }
        int get_line() const { return line; }
		const char* get_func() const { return func; }
        const char* get_info() const { return info; }
	};
#define NYML_THROW_INVALID_ACCESS throw NymlInvalidAccessException("nyml::Invalid Access", __FILE__, __LINE__, __func__);
#else
	struct NymlInvalidAccessException : public std::exception
	{
		std::string pretty_print() const {
			return "idk how you got here please report";
		}
	};
#define NYML_THROW_INVALID_ACCESS  
#endif


    struct Node;
	std::stringstream Dump(Node& root, int indent_level = 0);
	void SaveFile(Node& root, const std::string& filename);
	Node LoadFile(const std::string& filename);
	Node Parse(const std::string& str);
	
	enum class ValueType {
		NONE_TYPE,
		BOOL_TYPE,
		INT_TYPE,
		FLOAT_TYPE,
		STR_TYPE,
		SIZE,
		//ARRAY_TYPE,
	};

    struct Node {
		std::string key;
		
		union Value
		{
			std::string_view string;
			bool boolean;
			int number_integer;
			//uint32_t number_unsigned;
			float number_float;

			Value() = default;
			Value(bool v) noexcept : boolean(v) {}
			Value(int v) noexcept : number_integer(v) {}
			//json_value(number_unsigned_t v) noexcept : number_unsigned(v) {}
			Value(float v) noexcept : number_float(v) {}
			Value(ValueType t)
			{
				switch (t)
				{
					case ValueType::STR_TYPE:
					{
						string = "";
						break;
					}
					case ValueType::BOOL_TYPE:
					{
						boolean = false;
						break;
					}
					case ValueType::INT_TYPE:
					{
						number_integer = 0;
						break;
					}

					case ValueType::FLOAT_TYPE:
					{
						number_float = 0.0f;
						break;
					}
				}
			}
		};
		ValueType _type;
		Value _value;

		std::string value;
        
		std::unordered_map<std::string, Node> children;
        
        Node(const Node& other) 
            : key(other.key), 
			value(other.value), 
			children(other.children), 
			_value(other._value), _type(other._type)
        {
            
        }
        
		Node() : Node("root")
		{
		}

		Node(const std::string& p_key, const std::string& p_value = "~") 
			: key(p_key), value(p_value), _value(0)
		{
            
		}
        
		Node& get_path(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end)
		{
			if (begin != end)
			{
				return children[*begin].get_path(begin + 1, end);
			}
			else
			{
				return *this;
			}
		}
        
		size_t size() const
		{
			return children.size();
		}
        
		auto begin()
		{
			return children.begin();
		}
        
		auto end()
		{
			return children.end();
		}
        
		void operator=(const bool x)
		{
			_type = ValueType::BOOL_TYPE;
			_value.boolean = x;
		}
		
		void operator=(const std::string& x)
		{
			_type = ValueType::STR_TYPE;
			value = x;
		}

		void operator=(const int x)
		{
			_type = ValueType::INT_TYPE;
			_value.number_integer = x;
		}
        
		void operator=(const float x)
		{
			_type = ValueType::FLOAT_TYPE;
			_value.number_float = x;
		}
        
		Node& operator[](const int index) 
		{
			return (*this)[std::to_string(index)];
		}
        
		Node& operator[](const std::string& p_key) 
		{
			if (children.count(p_key) == 0)
			{
				children[p_key] = Node(p_key);
			}
			return children[p_key];
		}
        
		template <typename T>
            T& as() 
		{
			// TODO
			// idk what to do in this case yet
			NYML_THROW_INVALID_ACCESS;
			static T dummy;
			return dummy;
		}
        
		template <>
            int& as<int>() 
		{
			static int dummy;
			if(_type == ValueType::INT_TYPE)
				return _value.number_integer;
			NYML_THROW_INVALID_ACCESS;
			return dummy;
		}
        
		template <>
            std::string_view& as<std::string_view>()
		{
			static std::string_view dummy{"~"};
			if(_type == ValueType::STR_TYPE)
				return _value.string;
			NYML_THROW_INVALID_ACCESS;
			return dummy;
		}

		template <>
            std::string& as<std::string>()
		{
			static std::string dummy{"~"};
			if(_type == ValueType::STR_TYPE)
				return value;
			NYML_THROW_INVALID_ACCESS;
			return dummy;
		}
        
		template <>
            bool& as<bool>() 
		{
			static bool dummy;
			if(_type == ValueType::BOOL_TYPE)
				return _value.boolean;
			NYML_THROW_INVALID_ACCESS;
			return dummy;
		}
        
		template <>
            float& as<float>() 
		{
			static float dummy;
			if(_type == ValueType::FLOAT_TYPE)
				return _value.number_float;
			NYML_THROW_INVALID_ACCESS;
			return dummy;
		}
	};
    
#ifdef NYML_IMPLEMENTATION
	bool is_number(const std::string& s)
	{
		return !s.empty() && std::find_if(s.begin(), 
			s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
	}
	bool is_float(const std::string& str ) {
		std::istringstream iss(str);
		float f;
		iss >> std::noskipws >> f; 
		return iss.eof() && !iss.fail(); 
	}
	bool is_bool(const std::string& value){
		if (value == "true") return true;
		if (value == "false") return true;
		if (value == "True") return true;
		if (value == "False") return true;
		if (value == "on") return true;
		if (value == "off") return true;
		if (value == "On") return true;
		if (value == "Off") return true;
		return false;
	}
	bool as_bool(const std::string& value){
		if (value == "true") return true;
		if (value == "false") return false;
		if (value == "True") return true;
		if (value == "False") return false;
		if (value == "on") return true;
		if (value == "off") return false;
		if (value == "On") return true;
		if (value == "Off") return false;
		return false;
	}

	std::stringstream Dump(Node& root, int indent_level)
	{
		std::stringstream ss;
		//if (indent_level == 0)
		//	ss << "---\n";
        
		for (auto& [name, child] : root)
		{
			for (int i = 0; i < indent_level; i++)
			{
#if __EMSCRIPTEN__
                ss << "_";
#else
                ss << " ";
#endif
			}
			switch (child._type)
			{
				case ValueType::STR_TYPE:
					ss << name << ": " << child.value << '\n';
					break;
				case ValueType::BOOL_TYPE:
					ss << name << ": " << (child._value.boolean ? "true" : "false") << '\n';
					break;
				case ValueType::INT_TYPE:
					ss << name << ": " << child._value.number_integer << '\n';
					break;
				case ValueType::FLOAT_TYPE:
					ss << name << ": " << child._value.number_float << '\n';
					break;
				default:
					ss << name << ": ~\n";
					break;
			}
			//ss << name << ": " << child._value << '\n';
			
			if (child.size() > 0)
			{
				ss << Dump(child, indent_level + 4).str();
			}
		}
        
		return ss;
	}
    
	void SaveFile(Node& root, const std::string& filename)
	{
		auto ss = Dump(root);
        
		std::ofstream file(filename);
        
		file << ss.str();
        
#if _DEBUG
		std::cout << "nyml::Write file: " << filename << " ->:\n" << ss.str() << std::endl;
#endif
	}
    
    auto& xplat_getline(std::istream& stream, std::string& string)
    {
#if __EMSCRIPTEN__
        auto& ret = std::getline(stream, string);
        
        if(!string.empty())
            string.pop_back();
        return ret;
#endif
        return std::getline(stream, string);
    }
    
	Node LoadFile(const std::string& filename)
	{
#if _DEBUG || __EMSCRIPTEN__
		std::cout << "nyml::Opening file: " << filename << std::endl;
#endif
		std::ifstream file(filename);
        
		if (!file.is_open())
		{
			std::cerr << "nyml::Failed to open file: " << filename << std::endl;
			return Node{"root"};
		}
        
		std::stringstream ss;
		ss << file.rdbuf();
		std::string str = ss.str();
		return Parse(str);
	}
	Node Parse(const std::string& data)
	{
		Node root = Node("root");
		std::stringstream ss(data);
		std::string str = ss.str();

#if __EMSCRIPTEN__
		static const std::regex r(R"rgx((\s*)(\w+|-)\s*:\s*(.*))rgx");
#else
		static const std::regex r(R"rgx((\s*)(\w+|-)\s*:\s*(.*))rgx");
#endif
		std::smatch m;
        
		size_t indent_level = 0;
		
		std::vector<std::string> indent_keys; 
		indent_keys.push_back(root.key);
        
		std::string last_key = root.key;
		
		std::vector<size_t> last_indent; 
        
		//std::cout << "nyml::Executing regex on: " << str << std::endl;
        
        while(xplat_getline(ss, str))
        {
			if (str[0] == '#') continue;
		    if (std::regex_match(str, m, r))
			{
				std::string spaces = m[1].str();
				spaces.erase(std::remove(spaces.begin(), spaces.end(), '\n'), spaces.end());
				
				indent_level = spaces.size();
				if(last_indent.empty())
					last_indent.push_back(indent_level);
				
				if (indent_level < last_indent.back())
				{
					do
					{
						last_indent.pop_back();
						indent_keys.pop_back();
						
					} while (last_indent.size() > 1 && indent_level < last_indent.back());
				}
				else if (indent_level > last_indent.back())
				{
					last_indent.push_back(indent_level);
					indent_keys.push_back(last_key);
				}
				
				Node& curr_node = root.get_path(indent_keys.begin()+1, indent_keys.end());
				
				auto key = m[2].str();
				key.erase(std::remove(key.begin(),key.end(), '\n'), key.end());
				
				
				if (key == "-")
				{
					key = std::to_string(curr_node.children.size());
				}
				
				
				curr_node[key].key = key;

				auto read_value = m[3].str();
				if (is_float(read_value))
				{
					curr_node[key]._value.number_float = std::stof(read_value);
					curr_node[key]._type = ValueType::FLOAT_TYPE;
				}
				else if (is_number(read_value))
				{
					curr_node[key]._value.number_integer = std::stoi(read_value);
					curr_node[key]._type = ValueType::INT_TYPE;
				}
				else if (is_bool(read_value))
				{
					curr_node[key]._value.boolean = as_bool(read_value);
					curr_node[key]._type = ValueType::BOOL_TYPE;
				}
				else if (key == "~")
				{
					curr_node[key]._value.string = "~";
					curr_node[key]._type = ValueType::NONE_TYPE;
				}
				else 
				{
					curr_node[key].value = read_value;
					curr_node[key]._value.string = std::string_view{curr_node[key].value};
					curr_node[key]._type = ValueType::STR_TYPE;
				}
				//curr_node[key].value = m[3].str();
				
				last_key = std::string(key);
				
				str = m.suffix().str();
			}
		}
#if _DEBUG || __EMSCRIPTEN__
		std::cout << "nyml::Done parsing: " << std::endl << Dump(root).str() << std::endl;
#endif
        
		return root;
	}
#endif
};

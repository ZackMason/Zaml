#pragma once
// Zaml.h - A simplefied Yaml parser
// add #define ZAML_IMPLEMENTATION to a cpp file

#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Zaml
{
    
    template<typename T>
        using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
        constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    struct Node;
	std::stringstream ParseNode(Node& root, int indent_level = 0);
	void SaveFile(Node& root, const std::string& filename);
	Node LoadFile(const std::string& filename);
	
    struct Node {
		std::string key;
		std::string value;
        
		std::unordered_map<std::string, Node> children;
        
        Node(const Node& other) 
            : key(other.key), value(other.value),
        children(other.children)
        {
            
        }
        
		Node() : Node("root")
		{
            
		}
		Node(const std::string& p_key, const std::string& p_value = "~") : key(p_key), value(p_value)
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
        
        
		void operator=(const int x)
		{
			value = std::to_string(x);
		}
        
		void operator=(const float x)
		{
			value = std::to_string(x);
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
            T as() const
		{
			return T(value);
		}
        
		template <>
            int as<int>() const
		{
			return std::atoi(value.c_str());
		}
        
		template <>
            std::string as<std::string>() const
		{
			return value;
		}
        
		template <>
            bool as<bool>() const
		{
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
        
		template <>
            float as<float>() const
		{
			return std::atof(value.c_str());
		}
	};
    
#ifdef ZAML_IMPLEMENTATION
	std::stringstream ParseNode(Node& root, int indent_level)
	{
		std::stringstream ss;
		if (indent_level == 0)
			ss << "---\n";
        
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
			ss << name << ": " << child.value << '\n';
			
			if (child.size() > 0)
			{
				ss << ParseNode(child, indent_level + 4).str();
			}
		}
        
		return ss;
	}
    
	void SaveFile(Node& root, const std::string& filename)
	{
		auto ss = ParseNode(root);
        
		std::ofstream file(filename);
        
		file << ss.str();
        
#if _DEBUG
		std::cout << "Zaml::Write file: " << filename << " ->:\n" << ss.str() << std::endl;
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
		std::cout << "Zaml::Opening file: " << filename << std::endl;
#endif
		std::ifstream file(filename);
		Node root = Node("root");
        
		if (!file.is_open())
		{
			std::cerr << "Zaml::Failed to open file: " << filename << std::endl;
			return root;
		}
        
		std::stringstream ss;
		ss << file.rdbuf();
		std::string str = ss.str();
        
        
        
#if __EMSCRIPTEN__
		static const std::regex r(R"rgx((\s*)(\w+|-)\s*:\s*(.*))rgx");
#else
		static const std::regex r(R"rgx((\s*)(\w+|-)\s*:\s*(.*))rgx");
#endif
		std::smatch m;
        
		int indent_level = 0;
		
		std::vector<std::string> indent_keys; 
		indent_keys.push_back(root.key);
        
		std::string last_key = root.key;
		
		std::vector<int> last_indent; 
        
		//std::cout << "Zaml::Executing regex on: " << str << std::endl;
        
        while(xplat_getline(ss, str))
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
			curr_node[key].value = m[3].str();
            
			last_key = std::string(key);
            
			str = m.suffix().str();
		}
#if _DEBUG || __EMSCRIPTEN__
		std::cout << "Zaml::Done parsing: " << ParseNode(root).str() << std::endl;
#endif
        
		return root;
	}
#endif
};

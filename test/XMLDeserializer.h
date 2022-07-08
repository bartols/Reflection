#pragma once

#include "Reflection.h"

#include "string_view.hpp"

#include <string>
#include <stack>

namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
	class XMLAttribute;
}

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning(disable:4251)
#endif

class XMLDeserializer : public reflect::Visitor
{
public:
	// ctor dtor
	XMLDeserializer(const char* xml_string);
	~XMLDeserializer();

private:
	// no default ctr
	XMLDeserializer() {}
	// no copyctr
	XMLDeserializer(const XMLDeserializer&) {}
	// operator =
	XMLDeserializer& operator=(const XMLDeserializer&) { return *this; }

	tinyxml2::XMLDocument* _doc = nullptr;
	tinyxml2::XMLElement* _element = nullptr;
	bool _error = false;

	struct str_tree {
		std::string field_name;
		int vect_elem_tot = 0;
		int vect_elem_pos = 0;
	};

	std::stack<str_tree> _stack_tree;
	std::vector<std::string> _namespaces;

	bool visit_struct_start(const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional) override;
	bool visit_struct_end(const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional) override;
	bool visit_vector_start(const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional) override;
	bool visit_vector_end(const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional) override;
	bool check_optional_field(const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type) override;
	bool visit_field(const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional) override;

	tinyxml2::XMLElement* search_backward(tinyxml2::XMLElement* elem, const char* field_name);
	tinyxml2::XMLElement* search_backward(tinyxml2::XMLElement* elem, const reflect::aliases_t& field_names);

	tinyxml2::XMLElement* find_child_n(tinyxml2::XMLElement* elem, const char* field_name, int nth) const;
	tinyxml2::XMLElement* find_child_n(tinyxml2::XMLElement* elem, const reflect::aliases_t& field_names, int nth) const;

	const tinyxml2::XMLElement* findElement(tinyxml2::XMLElement* elem, const char* field_name) const;
	const tinyxml2::XMLElement* findElement(tinyxml2::XMLElement* elem, const reflect::aliases_t& field_names) const;
	const tinyxml2::XMLAttribute* findAttribute(tinyxml2::XMLElement* elem, const reflect::aliases_t& field_names) const;

	nonstd::string_view removeNamespace(nonstd::string_view value) const;
	nonstd::string_view removeKnownNamespace(nonstd::string_view value) const;
	bool equal(nonstd::string_view name, nonstd::string_view field_name) const;
	bool equal(const std::string& name, const reflect::aliases_t& field_names) const;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

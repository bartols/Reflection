//
// XML serializer
//

#pragma once

#include "Reflection.h"

#include <string>
#include <memory>
#include <list>

namespace tinyxml2 {
	class XMLPrinter;
}

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning(disable:4251)
#endif

class XMLSerializer : public reflect::Visitor
{
public:
	// no default ctr
	XMLSerializer() = delete;
	// no copyctr
	XMLSerializer(const XMLSerializer&) = delete;

	// ctor dtor
	XMLSerializer(const std::string& name, const std::string& namesp = {} );
	~XMLSerializer();

	// set compact serialization
	void set_compact_xml( bool compact )		{ _compact_mode = compact; }
	// serialize empty element
	void set_serialize_empty_elem( bool b )		{ _serialize_empty_elem = b; }

	// xml string
	std::string get_xml_string() const;

	// operator =
	XMLSerializer& operator=(const XMLSerializer&) = delete;

private:
	std::unique_ptr<tinyxml2::XMLPrinter> _printer;
	const std::string _name;
	std::string _namespace;
	std::string _namespace_name;
	std::string _nms_uri;
	bool _compact_mode = false;
	bool _serialize_empty_elem = false;
	std::list<std::string> _names;

	bool visit_struct_start( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	bool visit_struct_end( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	bool visit_vector_start( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	bool visit_vector_end( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	bool check_optional_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type ) override;
	bool visit_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	const char * addNamespace(const char* value);
};

#ifdef _MSC_VER
	#pragma warning( pop )
#endif

//
// Xsd serializer
//

#pragma once

#include "Reflection.h"

#include <memory>
#include <deque>

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning(disable:4251)
#endif

class XsdComposite;
class XsdComponent;

class XSDSerializer : public reflect::Visitor
{
public:
	// ctor dtor
	XSDSerializer( const std::string& name, const std::string& namesp = "" );
	~XSDSerializer();

	// no default ctr
	XSDSerializer() = delete;
	// no copyctr
	XSDSerializer( const XSDSerializer& ) = delete;
	// operator =
	XSDSerializer& operator=( const XSDSerializer& ) = delete;

	// xml string
	std::string get_xsd_string() const;

private:
	std::unique_ptr<XsdComposite> _xsd;
	XsdComponent* _lastComponent = nullptr;
	const std::string _namesp;
	const std::string _name;
	std::deque<std::string> _vector_elem;

	bool visit_struct_start( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	bool visit_struct_end( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	bool visit_vector_start( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	bool visit_vector_end( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
	bool check_optional_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type ) override;
	bool visit_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override;
};

#ifdef _MSC_VER
	#pragma warning( pop )
#endif

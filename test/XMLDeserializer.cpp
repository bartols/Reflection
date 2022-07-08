#include <algorithm>


#include "XMLDeserializer.h"
#include "tinyxml2.h"

// #define DEBUG_VERBOSE

using namespace tinyxml2;

#ifdef DEBUG_VERBOSE
#define DEBUG_DUMP_XML_STRUCT
#include <iostream>
#include <cassert>
#endif

#ifdef _WINDOWS
#else
namespace afcs
{
template< class InputIt, class UnaryPredicate >
constexpr bool any_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if(first, last, p) != last;
}
}
#endif


#ifdef DEBUG_DUMP_XML_STRUCT

#define INDENT_STR	"  "

void dumpToStdout( const XMLAttribute* firstAttr, unsigned int indent )
{
	XMLAttribute* attr;

	for( attr = (XMLAttribute*)firstAttr; attr != 0; attr = (XMLAttribute*)attr->Next() ) {
		for( size_t i = 0; i < indent + 1; i++ ) 
			printf( INDENT_STR );
		printf( "attr %s: %s\n", attr->Name(), attr->Value() );
	}
}

void dumpToStdout( const XMLNode* parent, unsigned int indent = 0 )
{
	if( !parent ) return;

	XMLNode* child;

	XMLDeclaration* decl;
	XMLElement* elem;
	XMLComment* comm;
	XMLText* text;

	for( child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling() )
	{
		for( size_t i = 0; i < indent; i++ )
		{
			printf( INDENT_STR );
		}

		if( decl = child->ToDeclaration() )
		{
			printf( "Declaration: %s\n", child->Value() );
		}
		else if( elem = child->ToElement() )
		{
			printf( "Element: %s\n", child->Value() );
			const XMLAttribute* attr = elem->FirstAttribute();
			if( attr ) 
				dumpToStdout( attr, indent + 1 );
			dumpToStdout( child, indent + 1 );
		}
		else if( comm = child->ToComment() )
		{
			printf( "Comment: %s\n", child->Value() );
		}
		else if( text = child->ToText() )
		{
			printf( " text: %s\n", child->Value() );
		}
	}
}

#endif

XMLDeserializer::XMLDeserializer( const char* xml_string )
	:_doc( new tinyxml2::XMLDocument())
{
	// vts is present in reflection definition
	_namespaces.push_back("vts");
	if( _doc->Parse( xml_string ) != tinyxml2::XML_SUCCESS )
	{
		_error = true;
		set_error_msg( "Parsing error: %s", _doc->ErrorStr() );
	}
	else
	{
#ifdef DEBUG_DUMP_XML_STRUCT
		dumpToStdout( _doc );
#endif
	}
}

XMLDeserializer::~XMLDeserializer()
{
	delete _doc;
}

bool XMLDeserializer::visit_struct_start( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_VERBOSE
	if( field_names.empty() )
	{
		std::cout << "Struct start" << std::endl;
	}
	else
	{
		std::cout << "Struct start [" << field_names[0] << "] ";
	}
#endif

	if( _error )
	{
		return false;
	}

	// main struct
	if( field_names.empty() )
	{
		// search first element and extract namespaces
		for( tinyxml2::XMLNode* child = _doc->FirstChild(); child; child = child->NextSibling() )
		{
			if( (_element = child->ToElement()) != 0 )
			{
				const XMLAttribute* attr = _element->FirstAttribute();
				if (attr != nullptr)
				{
					for(; attr != nullptr; attr = attr->Next())
					{
						const char* p{nullptr};
						const char* namesp = attr->Name();
						if ((p = strchr(namesp, ':')) != nullptr)
						{
							_namespaces.push_back(p+1);
#ifdef DEBUG_VERBOSE
							std::cout << "Namespace: " << p+1 << std::endl;
#endif
						}
					}
				}
				break;
			}
		}
		if( _element == nullptr )
		{
			set_error_msg( "Malformed xml" );
			return false;
		}
	}
	else
	{
		if( _stack_tree.empty() || 
			! equal( _stack_tree.top().field_name, field_names) )
		{
			_element = search_backward( _element, field_names );
#ifdef DEBUG_VERBOSE
			if( _element != nullptr )
			{
				std::cout << "xml element [" << _element->Name() << "]" << std::endl;
			}
#endif
		}
		// if struct inside a vector
		else
		{
			// position xml on the correct child
			_element = find_child_n( _element, field_names, _stack_tree.top().vect_elem_pos++ );
#ifdef DEBUG_VERBOSE
			if( _element != nullptr )
			{
				std::cout << "xml element [" << _element->Name() << "] pos:" << _stack_tree.top().vect_elem_pos - 1 << std::endl;
			}
#endif
		}

		if( _element == nullptr )
		{
#ifdef DEBUG_VERBOSE
			std::cout << "Element not found:" << field_names[0] << std::endl;
#endif
			set_error_msg( "Element not found: %s", field_names[0] );
			return false;
		}
	}

	return true;
}

bool XMLDeserializer::visit_struct_end( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_VERBOSE
	if( field_names.empty() )
	{
		std::cout << "Struct end ";
	}
	else
	{
		std::cout << "Struct end [" << field_names[0] << "] - ";
	}
#endif

	if( _element == nullptr )
	{
#ifdef DEBUG_VERBOSE
		std::cout << "Element null" << std::endl;
#endif
		set_error_msg( "Element null" );
		return false;
	}

	if( field_names.empty() )
	{
#ifdef DEBUG_VERBOSE
		std::cout << "xml element [" << _element->Name() << "]" << std::endl;
#endif		
		return true;
	}

	// jump outside the element
	_element = _element->Parent()->ToElement();

#ifdef DEBUG_VERBOSE
	if( _element == nullptr )
	{
		std::cout << "xml element [null]" << std::endl;
	}
	else
	{
		std::cout << "xml element [" << _element->Name() << "]" << std::endl;
	}
#endif

	return true;
}

bool XMLDeserializer::visit_vector_start( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
	if( field_names.empty() )
	{
		set_error_msg( "No name" );
		return false;
	}

	// element
	if( type == reflect::element )
	{
		// if struct in struc
		_element = search_backward( _element, field_names );

	}
	else if( type == reflect::attribute )
	{
#ifdef DEBUG_VERBOSE
		std::cout << "Vector cannot be an attribute: " << field_names[0] << std::endl;
#endif
		set_error_msg( "Vector cannot be an attribute: %s", field_names[0] );
		return false;
	}

	if( _element == nullptr )
	{
#ifdef DEBUG_VERBOSE
		std::cout << "Element not found in vector: " << field_names[0] << std::endl;
#endif
		set_error_msg( "Element not found in vector: %s", field_names[0] );
		return false;
	}

#ifdef DEBUG_VERBOSE
	std::cout << "Vector start [" << field_names[0] << "] - ";
	std::cout << "xml element  [" << _element->Name() << "]";
#endif
	int vect_elem_tot = 0;

	reflect::TypeDescriptor_StdVector* descrVector = dynamic_cast<reflect::TypeDescriptor_StdVector*>(tdescr);

	nonstd::string_view element_name =
		(descrVector->itemType->node_type == reflect::TypeDescriptor::node_field) ? field_names[0] 
																				  : descrVector->itemType->name;
	// count element
	for( tinyxml2::XMLNode* child = _element->FirstChild(); child; child = child->NextSibling() )
	{
		const tinyxml2::XMLElement* elem = child->ToElement();
		if( elem == nullptr )
			continue;

		if( ! equal(element_name, elem->Name() ) )
		{
			continue;
		}

		// append in vector an empty class
		descrVector->append( descrVector->itemType, field );
		++vect_elem_tot;
	}

#ifdef __GNUC__
	str_tree new_elem;
	new_elem.field_name = element_name;
	new_elem.vect_elem_tot = vect_elem_tot;
	new_elem.vect_elem_pos = 0;
	_stack_tree.push( new_elem );
#else
	_stack_tree.push( { element_name.to_string(), vect_elem_tot, 0 } );
#endif

#ifdef DEBUG_VERBOSE
	std::cout << " resize to " << vect_elem_tot << std::endl;
#endif
	return true;
}

bool XMLDeserializer::visit_vector_end( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_VERBOSE
	std::cout << "Vector end [" << field_names[0] << "] - ";
#endif

	_stack_tree.pop();

	if( type == reflect::direct )
	{
#ifdef DEBUG_VERBOSE
		std::cout << "xml element [" << _element->Name() << "]" << std::endl;
#endif		
		return true;
	}

	// jump outside the element
	_element = _element->Parent()->ToElement();

#ifdef DEBUG_VERBOSE
	if( _element == nullptr )
	{
		std::cout << "xml element [null]" << std::endl;
	}
	else
	{
		std::cout << "xml element [" << _element->Name() << "]" << std::endl;
	}
#endif
	return true;
}

bool XMLDeserializer::check_optional_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type )
{
#ifdef DEBUG_VERBOSE
	const char* p = (type == reflect::element) ? "element" :
					(type == reflect::attribute) ? "attribute" : "direct";
	std::cout << "check_optional_field [" << field_names[0] << "] " << p << " ";
#endif

	if( _element == nullptr )
	{
#ifdef DEBUG_VERBOSE
		std::cout << "Element null" << std::endl;
#endif
		set_error_msg( "Element null" );
		return false;
	}

	reflect::TypeDescriptor_StdOptional* typeOpt = (reflect::TypeDescriptor_StdOptional*)tdescr;

	// attribute
	if( type == reflect::attribute )
	{
		// find attribute
		const tinyxml2::XMLAttribute* attr = findAttribute( _element, field_names );
		if( attr == nullptr )
		{
#ifdef DEBUG_VERBOSE
			std::cout << "Element " << field_names[0] << " not present" << std::endl;
#endif
			set_error_msg( "Element not found: %s", field_names[0] );
			return false;
		}

		// set a default 
		typeOpt->set_value( typeOpt->itemType, field );
	}
	// element
	else if( ( type == reflect::element ) ||
			 ( type == reflect::direct ) )
	{
		const tinyxml2::XMLElement* elem = findElement( _element, field_names );
		if( elem == nullptr )
		{
#ifdef DEBUG_VERBOSE
			std::cout << "Element " << field_names[0] << " not present" << std::endl;
#endif
			set_error_msg( "Element not found: %s", field_names[0] );
			return false;
		}

		// set a default 
		typeOpt->set_value( typeOpt->itemType, field );
	}

#ifdef DEBUG_VERBOSE
	std::cout << " present" << std::endl;
#endif

	return true;
}

bool XMLDeserializer::visit_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_VERBOSE
	const char* p = (type == reflect::element) ? "element" :
					(type == reflect::attribute) ? "attribute" : "direct";
	std::cout << "field " << p << " [" << field_names[0] << "]" << std::endl;
#endif
	if( _element == nullptr )
	{
		set_error_msg( "element null for field: %s", field_names[0] );
		return false;
	}

	// attribute
	if( type == reflect::attribute )
	{
		// find attribute
		const tinyxml2::XMLAttribute* attr = findAttribute( _element, field_names );
		if( attr == nullptr )
		{
			set_error_msg( "Attribute [%s] not found", field_names[0] );
			return false;
		}
#ifdef DEBUG_VERBOSE
		std::cout << "xml element [" << _element->Name() << "].[" << attr->Name() << "] :";
		std::cout << attr->Value() << std::endl;
#endif
		if( !tdescr->from_string( attr->Value(), field ) )
		{
			set_error_msg( "Error from_string attribute [%s] value [%s]", field_names[0], attr->Value() );
			return false;
		}
	}
	// elememt
	else if( type == reflect::element )
	{
		const tinyxml2::XMLElement* elem = findElement( _element, field_names );
		if( elem == nullptr )
		{
			set_error_msg( "Element [%s] not found", field_names[0] );
			return false;
		}
#ifdef DEBUG_VERBOSE
		std::cout << "element value: " << elem->GetText() << std::endl;
#endif
		const char* value = elem->GetText();
		if( value == nullptr )
		{
			return true;
		}

		if( !tdescr->from_string( elem->GetText(), field ) )
		{
			set_error_msg( "Error from_string element [%s] value [%s]", field_names[0], elem->GetText() );
			return false;
		}
	}
	// direct
	else
	{
		set_error_msg( "Field direct: %s", field_names[0] );
		return false;
	}

	return true;
}

/// <summary>
/// search backward in tree
/// </summary>
/// <param name="elem">starting element</param>
/// <param name="field_name">name</param>
/// <returns>element found or null</returns>
tinyxml2::XMLElement* XMLDeserializer::search_backward( tinyxml2::XMLElement* elem, const char* field_name )
{
	if( elem == nullptr )
		return nullptr;

	//tinyxml2::XMLElement* search_elem = elem->FirstChildElement( field_name );
	const tinyxml2::XMLElement* search_elem = findElement(elem, field_name);
	// if not inside 
	if( search_elem == nullptr )
	{
		// search in parent
		return search_backward( elem->Parent()->ToElement(), field_name );
	}
	else
	{
		return const_cast<tinyxml2::XMLElement*>(search_elem);
	}
}

/// <summary>
/// search backward in tree
/// </summary>
/// <param name="elem">starting element</param>
/// <param name="field_names">vector of names</param>
/// <returns>element found or null</returns>
tinyxml2::XMLElement* XMLDeserializer::search_backward( tinyxml2::XMLElement* elem, const reflect::aliases_t& field_names )
{
	tinyxml2::XMLElement* res{ nullptr };

	for( auto field_name : field_names )
	{
		if( (res = search_backward( elem, field_name )) != nullptr )
			return res;
	}

	return res;
}

tinyxml2::XMLElement* XMLDeserializer::find_child_n( tinyxml2::XMLElement* elem, const char* field_name, int nth ) const
{
	if( elem == nullptr )
		return nullptr;

	int position( 0 );
	for( tinyxml2::XMLNode* child = elem->FirstChild(); child; child = child->NextSibling() )
	{
		const tinyxml2::XMLElement* elem = child->ToElement();
		if( elem == nullptr )
			continue;

		if( ! equal( field_name, elem->Name() ) )
			continue;

		if( position == nth )
		{
			return child->ToElement();
		}
		++position;
	}

	return nullptr;
}

tinyxml2::XMLElement* XMLDeserializer::find_child_n( tinyxml2::XMLElement* elem, const reflect::aliases_t& field_names, int nth ) const
{
	tinyxml2::XMLElement* res{ nullptr };

	for( auto field_name : field_names )
	{
		if( (res = find_child_n( elem, field_name, nth )) != nullptr )
			return res;
	}

	return res;
}

const tinyxml2::XMLElement* XMLDeserializer::findElement(tinyxml2::XMLElement* elem, const char* field_name) const
{
	const tinyxml2::XMLElement* res{nullptr};

	for (res = elem->FirstChildElement(); res; res = res->NextSiblingElement())
	{
		if (equal(res->Name(), field_name))
			return res;
	}

	return res;
}

const tinyxml2::XMLElement* XMLDeserializer::findElement( tinyxml2::XMLElement* elem, const reflect::aliases_t& field_names ) const
{
	const tinyxml2::XMLElement* res{ nullptr };

	for (res = elem->FirstChildElement(); res; res = res->NextSiblingElement())
	{
		if( equal( res->Name(), field_names ) )
		   return res;
	}

	return res;
}

const tinyxml2::XMLAttribute* XMLDeserializer::findAttribute( tinyxml2::XMLElement* elem, const reflect::aliases_t& field_names ) const
{
	const tinyxml2::XMLAttribute* res{ nullptr };

	for( auto field_name : field_names )
	{
		if( (res = elem->FindAttribute( field_name )) != nullptr )
			return res;
	}

	return res;
}

/**
 * @brief remove namespace
 * @param value 
 * @return 
*/
nonstd::string_view XMLDeserializer::removeNamespace(nonstd::string_view value) const
{
	size_t pos = value.find(":");
	if (pos != std::string::npos)
	{
		value = value.substr(pos + 1);
	}

	return value;
}

/**
 * @brief remove known namespace
 * @param value 
 * @return 
*/
nonstd::string_view XMLDeserializer::removeKnownNamespace(nonstd::string_view value) const
{
	size_t pos = value.find(":");
	if (pos != std::string::npos)
	{
		nonstd::string_view namesp = value.substr(0, pos);

#ifdef _WINDOWS
		if (std::any_of(_namespaces.begin(), _namespaces.end(), [&namesp](const auto& nm) { return namesp == nm; }))
#else
		if (afcs::any_of(_namespaces.begin(), _namespaces.end(), [&namesp](const auto& nm) { return namesp == nm; }))
#endif
		{
			value = value.substr(pos + 1);
		}
	}

	return value;
}

bool XMLDeserializer::equal(nonstd::string_view name, nonstd::string_view field_name) const
{
	auto name_clear = removeKnownNamespace(name);
	auto field_name_clear = removeNamespace(field_name);

	return (name_clear == field_name_clear);
}

bool XMLDeserializer::equal( const std::string& name, const reflect::aliases_t& field_names ) const
{
	for( auto field_name : field_names )
	{
		if(equal( name, field_name ) )
			return true;
	}

	return false;
}

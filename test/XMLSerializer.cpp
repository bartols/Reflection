//
// XML serializer
//

#include "XMLSerializer.h"
#include "tinyxml2.h"

#include <iostream>
#include <algorithm>

// #define DEBUG_VERBOSE

// ctor dtor
XMLSerializer::XMLSerializer( const std::string& name, const std::string& namesp )
	:_name( name )
{
	if (namesp.empty())
		return;

	std::size_t pos = namesp.find('=');
	if (pos == std::string::npos)
		return;

	_namespace = namesp.substr(0, pos);
	_nms_uri = namesp.substr(pos + 1);

	_nms_uri.erase(std::remove(_nms_uri.begin(), _nms_uri.end(), '\"'), _nms_uri.end());

	pos = _namespace.find(':');
	if (pos == std::string::npos)
		return;

	_namespace_name = _namespace.substr(pos + 1);
}

XMLSerializer::~XMLSerializer()
{
}

// xml string
std::string XMLSerializer::get_xml_string() const
{
	return _printer->CStr();
}

bool XMLSerializer::visit_struct_start( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_VERBOSE
	if( field_names.empty() )
	{
		std::cout << "Struct start" << std::endl;
	}
	else
	{
		std::cout << "Struct start [" << field_names[0] << "]" << std::endl;
	}
#endif

	// first struct
	if( field_names.empty() )
	{
		if( ! _printer )
		{
			_printer = std::make_unique<tinyxml2::XMLPrinter>( (FILE *)0, _compact_mode );
		}
		// start doc
		_printer->PushDeclaration( "xml version=\"1.0\" encoding=\"UTF-8\"" );
		_printer->OpenElement( _name.c_str(), _compact_mode );
		if (!_namespace.empty())
		{
			_printer->PushAttribute(_namespace.c_str(), _nms_uri.c_str() );
		}
	}
	else
	{
		if( type != reflect::direct )
		{
			// a structure is always an element
			auto name = addNamespace( field_names[0] );
			_printer->OpenElement( name, _compact_mode );
		}
	}

	return true;
}

bool XMLSerializer::visit_struct_end( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
	if( type != reflect::direct )
	{
		_printer->CloseElement( _compact_mode );
	}
	return true;
}

bool XMLSerializer::visit_vector_start( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_VERBOSE
	std::cout << "Vector start [" << field_names[0] << "]" << std::endl;
#endif
	reflect::TypeDescriptor_StdVector* descrVector = dynamic_cast<reflect::TypeDescriptor_StdVector*>(tdescr);
	if( descrVector->count( descrVector->itemType, field ) == 0 )
		return true;

	// if element
	if( type == reflect::element )
	{
		auto name = addNamespace(field_names[0]);
		_printer->OpenElement( name, _compact_mode );
	}

	return true;
}

bool XMLSerializer::visit_vector_end( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_VERBOSE
	std::cout << "Vector end [" << field_names[0] << "]" << std::endl;
#endif

	reflect::TypeDescriptor_StdVector* descrVector = dynamic_cast<reflect::TypeDescriptor_StdVector*>(tdescr);
	if( descrVector->count( descrVector->itemType, field ) == 0 )
		return true;

	if( type == reflect::element )
	{
		// end element
		_printer->CloseElement( _compact_mode );
	}

	return true;
}

bool XMLSerializer::check_optional_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type )
{
	reflect::TypeDescriptor_StdOptional* typeOpt = (reflect::TypeDescriptor_StdOptional*)tdescr;
	return typeOpt->has_value( typeOpt->itemType, field );
}

bool XMLSerializer::visit_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
	std::string value = tdescr->to_string( field );

#ifdef DEBUG_VERBOSE
	const char* p = (type == reflect::element) ? "element" :
					(type == reflect::attribute) ? "attribute" : "direct";
	std::cout << "field " << p << " [" << field_names[0] << "] = " << value << std::endl;
#endif

	if( type == reflect::element )
	{
		if( !value.empty() || _serialize_empty_elem )
		{
			auto name = addNamespace(field_names[0]);
			_printer->OpenElement( name, _compact_mode );
			_printer->PushText( value.c_str() );
			_printer->CloseElement( _compact_mode );
		}
	}
	else if( type == reflect::attribute )
	{
		_printer->PushAttribute(field_names[0], value.c_str() );
	}
	else
	{
		set_error_msg( "Field direct: %s", field_names[0] );
		return false;
	}

	return true;
}

// purtroppo la tinixmls lavora con stringe nn in copia, quindi tutto cio' che 
// modifichiamo va tenuto fino alla fine
const char * XMLSerializer::addNamespace( const char* value )
{
	std::string name( value );
	
	// remove namespace if any
	bool present{false};
	size_t pos = name.find(':');
	if (pos != std::string::npos)
	{
		// if no nms declared remove
		if (_namespace_name.empty())
		{
			name = name.substr(pos + 1);
		}
		else
		{
			// todo controlla nms
		}
	}

	// insert in a list

	// si mette in una lista perche' non sposta gli elementi su riallocazione
	// dio mio che vergogna
	_names.push_back( name );
	const auto& vv = _names.back();
	return vv.c_str();;
}

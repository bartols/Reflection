//
// Xsd serializer
//

#include "XSDSerializer.h"
#include "string_view.hpp"
#include <cassert>

//#define DEBUG_VERBOSE

#ifdef DEBUG_VERBOSE
#include <iomanip>
#include <iostream>
#define DEBUG_STRUCT
#endif

#ifdef DEBUG_STRUCT
int ntab{0};
void print_tab()
{
	for (int i = 0; i < ntab; ++i)
	{
		std::cout << " ";
	}
}
#endif

#include <algorithm>
#include <unordered_set>

class XsdComponent {
public:
	// element type
	enum eElementType {
		attribute,
		element
	};

	// type
	enum eType {
		unknown,
		string,
		integer,
		int64,
		decimal,
		boolean,
		date,
		time,
		datetime,
		complex_type,
		enumeration
	};

	// ctr dtr
	XsdComponent() = default;
	virtual ~XsdComponent() = default;

	XsdComponent( const std::string& n, const std::string& type_name, eType p )
		:_type( p )
	{
		set_name( n, type_name );
	}

	XsdComponent( const std::string& n, const std::string& type_name, eElementType t, eType p, int min, int max )
		:_elementType(t), _type(p), _min_occur(min), _max_occur(max) 
	{
		set_name( n, type_name );
	}

	// parent
	void set_parent( XsdComponent* p )		{ _parent = p; }
	XsdComponent* get_parent() const		{ return _parent; }

	// add
	virtual void add( std::unique_ptr<XsdComponent>&& )		
											{}
	// clear
	virtual void clear()					{}

	// is composite
	virtual bool is_composite() const		{ return false; }

	std::string get_name() const			{ return _name; }

	// type name
	std::string get_type_name() const		{ return _type_name; }

	// element type
	void set_element_type( eElementType e ) { _elementType = e; }
	eElementType get_element_type() const	{ return _elementType; }

	// type
	void set_type( eType e )				{ _type = e; }

	// min occur
	int get_min_occur() const				{ return _min_occur; }
	// max occur
	int get_max_occur() const				{ return _max_occur; }

	// self containing
	void set_self_containing(bool b)		{ _self_containing = b; }

	// add restrictions
	void add_restriction( nonstd::string_view r )
											{ _restrictions.push_back( r ); }

#ifdef DEBUG_VERBOSE
	const char* to_string( eElementType e ) const
	{
		switch( e )
		{
		case XsdComponent::attribute:	return "attribute";
		case XsdComponent::element:		return "element";
		default:						return "???";
		}
	}

	const char* to_string( eType e ) const
	{
		switch( e )
		{
		case XsdComponent::unknown:			return "unknown";
		case XsdComponent::string:			return "string";
		case XsdComponent::integer:			return "int";
		case XsdComponent::int64:			return "long";
		case XsdComponent::decimal:			return "decimal";
		case XsdComponent::boolean:			return "boolean";
		case XsdComponent::complex_type:	return "complex_type";
		case XsdComponent::enumeration:		return "enumeration";
		default:							return "???";
		}
	}

	virtual void dump( int indent ) const
	{
		std::cout << std::setw( indent * 2 ) << " ";
		std::cout << "[" << _name << "] " << to_string( _elementType );
		std::cout << " " << to_string( _type );
		if (_type == XsdComponent::complex_type)
		{
			std::cout << ": " << _type_name;
		}
		std::cout << " min:" << _min_occur << " max:" << _max_occur;
		if (_self_containing)
		{
			std::cout << " self_cnt";
		}
		std::cout << "\n";
		if( !_restrictions.empty() )
		{
			++indent;
			for( const auto& restriction : _restrictions )
			{
				std::cout << std::setw( indent * 2 ) << " ";
				std::cout << restriction << std::endl;
			}
		}
	}

#endif
	virtual bool serialize_types( std::string& xsd_string, int indent = 0 ) const
	{
		xsd_string += tab( indent );
		if( _elementType == element )
		{
			xsd_string += "<xs:element name=\"";
			xsd_string += _name;
			xsd_string += "\" type=\"";
			xsd_string += toXsdType();
			xsd_string += "\"";

			if( get_min_occur() == 0 )
			{
				xsd_string += " minOccurs=\"0\"";
			}
			if( get_max_occur() == -1 )
			{
				xsd_string += " maxOccurs=\"unbounded\"";
			}
		}
		else
		{
			xsd_string += "<xs:attribute name=\"";
			xsd_string += _name;
			xsd_string += "\" type=\"";
			xsd_string += toXsdType();
			xsd_string += "\"";

			// if not optional
			if( _min_occur != 0 )
			{
				xsd_string += " use=\"required\"";
			}
		}

		xsd_string += "/>\n";
		return true;
	}

	virtual bool serialize_xsd( std::string& xsd_string, int indent = 0 ) const
	{
		return true;
	}

protected:
	std::string tab( int indent ) const
	{
		return std::string( indent * 2, ' ' );
	}

	std::string toXsdType() const
	{
		switch( _type )
		{
		case XsdComponent::string:			return "xs:string";
		case XsdComponent::integer:			return "xs:int";
		case XsdComponent::int64:			return "xs:long";
		case XsdComponent::decimal:			return "xs:decimal";
		case XsdComponent::boolean:			return "xs:boolean";
		case XsdComponent::enumeration:		return "xs:string";
		case XsdComponent::date:			return "xs:date";
		case XsdComponent::time:			return "xs:time";
		case XsdComponent::datetime:		return "xs:dateTime";
		case XsdComponent::unknown:
		case XsdComponent::complex_type:
		default:							return "???";
		}
	}

	std::string removeNameSpace( const std::string& value ) const
	{
		size_t pos = value.rfind( ':' );
		if( pos == std::string::npos )
			return value;

		return value.substr( pos + 1 );
	}

	// intentionally copied
	std::string normalizeTypeName(const std::string& name, std::string value) const
	{
		size_t pos = value.find("std::vector");
		if (pos != std::string::npos)
		{
			pos = value.find("<", pos) + 1;
			size_t pos_end = value.find(">", pos);
			value = value.substr(pos, pos_end - pos) + "List";
		}
		value = removeNameSpace(value);
		if (name == value)
		{
			value += "Type";
		}

		return value;
	}

	XsdComponent* _parent = nullptr;
	eElementType _elementType = element;
	eType _type = unknown;
	int _min_occur = 1;
	int _max_occur = 1;
	std::string _name;
	std::string _type_name;
	std::vector<nonstd::string_view> _restrictions;
	bool _self_containing = false;
	static std::unordered_set<std::string> _names_type;

	// name
	void set_name(const std::string& name, const std::string& type_name)
	{
		_name = removeNameSpace(name);
		if (_type == complex_type)
		{
			_type_name = normalizeTypeName( _name, type_name );
		}
	}
};

std::unordered_set<std::string> XsdComponent::_names_type;

XsdComponent::eType convertToType( nonstd::string_view refl_type )
{
	     if( refl_type == "int" )			return XsdComponent::integer;
	else if (refl_type == "int64" )			return XsdComponent::int64;
	else if( refl_type == "bool" )			return XsdComponent::boolean;
	else if( refl_type == "std::string" )	return XsdComponent::string;
	else if( refl_type == "blob" )			return XsdComponent::string;
	else if( refl_type == "double" )		return XsdComponent::decimal;
	else if (refl_type == "time")			return XsdComponent::time;
	else if (refl_type == "date")			return XsdComponent::date;
	else if (refl_type == "datetime")		return XsdComponent::datetime;
	else									return XsdComponent::enumeration;
}

// a composite
class XsdComposite : public XsdComponent {
public:
	XsdComposite() = default;
	XsdComposite( const std::string& name, const std::string& type_name)
		:XsdComponent( name, type_name, XsdComponent::complex_type ) {}

	XsdComposite( const std::string& name, const std::string& type_name, int min, int max )
		:XsdComponent( name, type_name, XsdComponent::element, XsdComponent::complex_type, min, max ) {}

	// is composite
	bool is_composite() const override			{ return true; }

	// add
	void add( std::unique_ptr<XsdComponent>&& c ) override
	{ 
		c->set_parent( this );
		_childreen.push_back( std::move( c ) ); 
	}

	// clear
	void clear() override
	{
		std::for_each( _childreen.begin(), _childreen.end(), []( auto& it ) { it->clear(); } );
		_childreen.clear();
		_names_type.clear();
		_typeParsed.clear();
	}

#ifdef DEBUG_VERBOSE
	// dump
	void dump( int indent = 0 ) const override
	{
		XsdComponent::dump( indent );
		std::for_each( _childreen.begin(), _childreen.end(), [&indent]( auto& it ) { it->dump( indent + 1 ); } );
	}
#endif

	bool is_anyElement() const
	{
		for( const auto& child : _childreen )
		{
			if( child->get_element_type() == XsdComponent::element )
			{
				return true;
			}
		}

		return false;
	}

	std::string generateMinMax() const
	{
		std::string result;

		// calculate min e max occur of childreen
		int min_occur = 0;
		int max_occur = 1;
		for( const auto& child : _childreen )
		{
			if( child->get_element_type() == XsdComponent::element )
			{
				min_occur = std::max<int>( min_occur, child->get_min_occur() );
				max_occur = std::min<int>( max_occur, child->get_max_occur() );
			}
		}

		if( min_occur == 0 )
		{
			result += " minOccurs=\"0\"";
		}
		if( max_occur == -1 )
		{
			result += " maxOccurs=\"unbounded\"";
		}

		return result;
	}

	bool serialize_types( std::string& xsd_string, int indent = 0 ) const override
	{
		for( const auto& child : _childreen )
		{
			if( child->is_composite() )
			{
				child->serialize_types( xsd_string, indent );
			}
		}

		if (_self_containing)
		{
			return true;
		}

		std::string type_name = get_type_name();
		if ( std::find(_typeParsed.begin(), _typeParsed.end(), type_name) != _typeParsed.end())
		{
			return true;
		}
		_typeParsed.push_back(type_name);

		// if type of an enum
		if (!_restrictions.empty())
		{
			xsd_string += tab(indent) + "<xs:simpleType name=\"" + type_name + "\">\n";
			xsd_string += tab(++indent);
			xsd_string += "<xs:restriction base=\"xs:string\">\n";

			for (const auto& restriction : _restrictions)
			{
				xsd_string += tab(indent + 1);
				xsd_string += "<xs:enumeration value=\"";
				xsd_string += restriction.data();
				xsd_string += "\"/>\n";
			}

			xsd_string += tab(indent--);
			xsd_string += "</xs:restriction>\n";

			xsd_string += tab(indent--);
			xsd_string += "</xs:simpleType>\n";
			
			return true;
		}

		xsd_string += tab(indent) + "<xs:complexType name=\"" + type_name + "\">\n";

		// elements
		if( is_anyElement() )
		{
			xsd_string += tab( ++indent );
			xsd_string += "<xs:sequence>\n";
			for( const auto& child : _childreen )
			{
				if (child->get_element_type() != XsdComponent::element)
					continue;
				
				if (child->is_composite())
				{
					child->serialize_xsd(xsd_string, indent + 1);
				}
				else
				{
					// serialize
					child->serialize_types(xsd_string, indent + 1);
				}
			}
			xsd_string += tab( indent-- );
			xsd_string += "</xs:sequence>\n";
		}

		// attributes
		for( const auto& child : _childreen )
		{
			if (child->get_element_type() != XsdComponent::attribute)
				continue;

			if (child->is_composite())
			{
				child->serialize_xsd(xsd_string, indent + 1);
			}
			else
			{
				child->serialize_types( xsd_string, indent + 1 );
			}
		}

		xsd_string += tab( indent-- );
		xsd_string += "</xs:complexType>\n";

		return true;
	}

	bool serialize_xsd( std::string& xsd_string, int indent = 0 ) const override
	{
		std::string occurs;
		if( get_min_occur() == 0 )
		{
			occurs += " minOccurs=\"0\"";
		}
		if( get_max_occur() == -1 )
		{
			occurs += " maxOccurs=\"unbounded\"";
		}

		if( _namesp.empty() )
		{
			if (_elementType == element)
			{
				xsd_string += tab(indent) + "<xs:element name=\"";
				xsd_string += get_name();
				xsd_string += "\" type=\"";
				xsd_string += get_type_name();
				xsd_string += "\"";
				xsd_string += occurs + "/>\n";
			}
			else
			{
				xsd_string += tab(indent) + "<xs:attribute name=\"";
				xsd_string += get_name();
				xsd_string += "\" type=\"";
				xsd_string += get_type_name();
				xsd_string += "\"";

				// if not optional
				if (get_min_occur() != 0)
				{
					xsd_string += " use=\"required\"";
				}
				xsd_string += "/>\n";
			}
		}
		else
		{
			xsd_string += tab( indent ) + "<xs:element name=\"" + get_name() + "\" type=\"" + _namesp + ":" + get_type_name() + "\"" + occurs + " xmlns:vts=\"http://www.aep-italia.it/vts/\"/>\n";
		}

		return true;
	}

	// serialize 
	bool serialize( const std::string& namesp, std::string& xsd_string ) const
	{
		_namesp = namesp;
		xsd_string = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		xsd_string += "<xs:schema attributeFormDefault=\"unqualified\" elementFormDefault=\"qualified\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"";
		if( !namesp.empty() )
		{
			xsd_string += " targetNamespace=\"http://www.aep-italia.it/";
			xsd_string += namesp;
			xsd_string += "/\" xmlns:";
			xsd_string += namesp;
			xsd_string += "=\"http://www.aep-italia.it/";
			xsd_string += namesp;
			xsd_string += "/\"";
		}
		else
		{
			xsd_string += " targetNamespace=\"http://www.aep-italia.it/vts/\" xmlns=\"http://www.aep-italia.it/vts/\"";
		}

		xsd_string += ">\n";

		xsd_string += tab( 1 ) + "<!-- Complex types declarations -->\n";

		// serialize type
		if( !serialize_types( xsd_string, 1 ) )
			return false;

		xsd_string += tab( 1 ) + "<!-- Elements -->\n";

		// serialize xsd
		if( !serialize_xsd( xsd_string, 1 ) )
			return false;

		xsd_string += "</xs:schema>\n";

		return true;
	}

private:
	std::vector<std::unique_ptr<XsdComponent>> _childreen;
	static std::vector<std::string> _typeParsed;
	static std::string _namesp;
};

std::string XsdComposite::_namesp;
std::vector<std::string> XsdComposite::_typeParsed;

XSDSerializer::XSDSerializer( const std::string& name, const std::string& namesp )
	:_name(name),_namesp( namesp )
{
}

XSDSerializer::~XSDSerializer()
{
	_xsd->clear();
}

// xml string
std::string XSDSerializer::get_xsd_string() const
{
#ifdef DEBUG_VERBOSE
	_xsd->dump();
#endif
	std::string xsd_string;

	if( !_xsd->serialize( _namesp, xsd_string ) )
		return "";

	return xsd_string;
}

bool XSDSerializer::visit_struct_start( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
	// first struct
	if( field_names.empty() )
	{
#ifdef DEBUG_STRUCT
		print_tab();
		std::cout << "Struct Start" << std::endl;
		++ntab;
#endif
		_xsd = std::make_unique<XsdComposite>(_name, tdescr->getFullName());
		_xsd->clear();	// per le statiche
		_lastComponent = _xsd.get();
	}
	else
	{
#ifdef DEBUG_STRUCT
		print_tab();
		std::cout << "Struct Start: "<< field_names[0] << std::endl;
		++ntab;
#endif
		int min = optional ? 0 : 1;
		int max = 1;
		if ((!_vector_elem.empty()) && _vector_elem.back() == tdescr->name)
		{
			min = 0;
			max = -1;
		}

		std::unique_ptr<XsdComposite> nelem = std::make_unique<XsdComposite>( field_names[0], tdescr->getFullName(), min, max );
		XsdComposite* s = nelem.get();
		_lastComponent->add( std::move( nelem ) );
		_lastComponent = s;
	}

	return true;
}

bool XSDSerializer::visit_struct_end( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_STRUCT
	--ntab;
	print_tab();
	if (field_names.empty())
	{
		std::cout << "Struct End" << std::endl;
	}
	else
	{
		std::cout << "Struct End: " << field_names[0] << std::endl;
	}
#endif

	// back to parent
	_lastComponent = _lastComponent->get_parent();
	return true;
}

bool XSDSerializer::visit_vector_start( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_STRUCT
	print_tab();
	std::cout << "Vector start: " << field_names[0] << std::endl;
	++ntab;
#endif
	reflect::TypeDescriptor_StdVector* descrVector = dynamic_cast<reflect::TypeDescriptor_StdVector*>(tdescr);
	const std::string element_name = descrVector->itemType->name;

	// use this to avoid reenter loop problem
	auto occurences = std::count(_vector_elem.begin(), _vector_elem.end(), element_name);
	if (occurences == 0)
	{
		// add un element to visit the inner type
		descrVector->append(descrVector->itemType, field);
	}
	else if (occurences == 1)
	{
		// self containing
		std::unique_ptr<XsdComposite> nelem = std::make_unique<XsdComposite>(element_name, descrVector->itemType->getFullName(), 0, -1);
		nelem->set_self_containing(true);
		_lastComponent->add(std::move(nelem));
	}
	else
	{
		assert(false);
	}

	_vector_elem.push_back(element_name);

	if( type != reflect::eFieldType::direct )
	{
		std::unique_ptr<XsdComposite> nelem = std::make_unique<XsdComposite>( field_names[0], tdescr->getFullName(), optional ? 0 : 1, 1 );
		XsdComposite* s = nelem.get();
		_lastComponent->add( std::move( nelem ) );
		_lastComponent = s;
	}
	
	return true;
}

bool XSDSerializer::visit_vector_end( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_STRUCT
	--ntab;
	print_tab();
	std::cout << "Vector end: " << field_names[0] << std::endl;
#endif

	_vector_elem.pop_back();

	if( type != reflect::eFieldType::direct )
	{
		// back to parent
		_lastComponent = _lastComponent->get_parent();
	}
	return true;
}

bool XSDSerializer::check_optional_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type )
{
	reflect::TypeDescriptor_StdOptional* typeOpt = (reflect::TypeDescriptor_StdOptional*)tdescr;
	if( ! typeOpt->has_value(typeOpt->itemType, field ) )
	{
		typeOpt->set_value( typeOpt->itemType, field );
	}

	return true;
}

bool XSDSerializer::visit_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional )
{
#ifdef DEBUG_STRUCT
	print_tab();
	std::cout << "Field: " << field_names[0] << std::endl;
#endif
	XsdComponent::eType xsd_type = convertToType( tdescr->getFullName() );
	XsdComponent::eElementType elem_type = (type == reflect::element) || (type == reflect::direct) ? XsdComponent::element : XsdComponent::attribute;

	int min = optional ? 0 : 1;
	int max = 1;

	// if inside a vector
	if ((!_vector_elem.empty()) && _vector_elem.back() == tdescr->name)
	{
		min = 0;
		max = -1;
	}

	// if enum
	if( tdescr->node_type == reflect::TypeDescriptor::node_enum )
	{
		std::unique_ptr<XsdComposite> nelem = std::make_unique<XsdComposite>(field_names[0], tdescr->getFullName(), min, max);
		nelem->set_element_type(elem_type );
		reflect::TypeDescriptor_Enum* typeEnum = (reflect::TypeDescriptor_Enum*)tdescr;
		for( const auto& elem : typeEnum->members )
		{
			nelem->add_restriction( { elem.str, elem.len } );
		}
		_lastComponent->add(std::move(nelem));
	}
	else
	{
		std::unique_ptr<XsdComponent> nelem = std::make_unique<XsdComponent>(field_names[0],
																			 tdescr->getFullName(),
																			 elem_type,
																			 xsd_type,
																			 min, max);
		_lastComponent->add(std::move(nelem));
	}

	return true;
}

#pragma once

#include "Reflection.h"

#include <sstream>
#include <string>

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning(disable:4251)
#endif

class StringSerializer : public reflect::Visitor
{
public:
	StringSerializer()		{ reset(); }
	~StringSerializer()		{}

	void reset()			{ _stream.str( "" );  _stream.clear(); indentLevel = 0; }

	std::string composeName( const reflect::aliases_t& field_names ) const
	{
		std::stringstream ss;
		ss << field_names[0];
		size_t len = field_names.size();
		if( len > 1 )
		{
			ss << ":[";
			for( size_t pos = 1; pos < len; ++pos )
			{
				ss << field_names[pos] << ", ";;
			}
			ss.seekp( -2, ss.cur );
			ss << "]";
		}

		return ss.str();
	}

	bool visit_struct_start( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override
	{
		_stream << std::string( 4 * indentLevel, ' ' );
		if( field_names.empty() )
		{
			_stream << tdescr->name << " {" << std::endl;
		}
		else
		{
			_stream << composeName( field_names ) << " = " << tdescr->name << " {" << std::endl;
		}
		++indentLevel;

		return true;
	}

	bool visit_struct_end( const reflect::aliases_t& field_names, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override
	{
		--indentLevel;
		_stream << std::string( 4 * indentLevel, ' ' ) << "}" << std::endl;

		return true;
	}

	bool visit_vector_start( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override
	{
		reflect::TypeDescriptor_StdVector* typeVector = (reflect::TypeDescriptor_StdVector*)tdescr;
		_stream << std::string( 4 * indentLevel, ' ' );
		if( typeVector->count( typeVector->itemType, field ) == 0 )
		{
			_stream << composeName( field_names ) << " = " << tdescr->getFullName() << "{}";
		}
		else
		{
			_stream << composeName( field_names ) << " = " << tdescr->getFullName() << "{";
		}
		
		if( type == reflect::element )
		{
			_stream << " element" << std::endl;
		}
		else if( type == reflect::attribute )
		{
			_stream << " attribute" << std::endl;
		}
		else
		{
			_stream << " direct" << std::endl;
		}

		++indentLevel;

		return true;
	}

	bool visit_vector_end( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override
	{
		reflect::TypeDescriptor_StdVector* typeVector = (reflect::TypeDescriptor_StdVector*)tdescr;
		--indentLevel;
		if( typeVector->count( typeVector->itemType, field ) != 0 )
		{
			_stream << std::string( 4 * indentLevel, ' ' ) << "}" << std::endl;
		}

		return true;
	}

	bool check_optional_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type ) override
	{
		reflect::TypeDescriptor_StdOptional* typeOpt = (reflect::TypeDescriptor_StdOptional*)tdescr;
		return typeOpt->has_value( typeOpt->itemType, field );
	}

	bool visit_field( const reflect::aliases_t& field_names, void* field, reflect::TypeDescriptor* tdescr, reflect::eFieldType type, bool optional ) override
	{
		_stream << std::string( 4 * indentLevel, ' ' );
		if( (tdescr->getFullName() == "std::string") ||
			(tdescr->getFullName() == "std::optional<std::string>") )
		{
			_stream << composeName( field_names ) << " = " << tdescr->getFullName() << "{\"" << tdescr->to_string( field ) << "\"}";
		}
		else
		{
			_stream << composeName( field_names ) << " = " << tdescr->getFullName() << "{" << tdescr->to_string( field ) << "}";
		}

		if( type == reflect::element )
		{
			_stream << " element";
		}
		else if( type == reflect::attribute )
		{
			_stream << " attribute";
		}
		else
		{
			_stream << " direct";
		}

		if( optional )
		{
			_stream << " optional";
		}

		_stream << std::endl;
		return true;
	}

	std::string get_string() const		{ return _stream.str(); }
private:
	int indentLevel;
	std::stringstream _stream;
};

#ifdef _MSC_VER
	#pragma warning( pop )
#endif

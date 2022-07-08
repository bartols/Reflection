
#pragma once

#include "Reflection.h"
#include "XMLSerializer.h"
#include "XMLDeserializer.h"
#include "XSDSerializer.h"
#include "StringSerializer.h"

#ifdef _DEBUG
//	#define DEBUG_VERBOSE
//	#define DEBUG_VERBOSE_XSD
//	#define DEBUG_STRING
#endif

template<typename T>
std::string serialize_xml(const char* name, T* str_elem, const std::string& namesp = {})
{
	reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<T>::get();

#ifdef DEBUG_STRING
	StringSerializer str_serializer;
	if( typeDesc->accept( str_serializer, str_elem ) )
	{
		printf( "%s\n", str_serializer.get_string().c_str() );
	}
#endif

	XMLSerializer xml_serializer( name, namesp);
	if( !typeDesc->accept( xml_serializer, str_elem ) )
	{
#ifdef DEBUG_VERBOSE
		printf( "Errore :%s\n", xml_serializer.get_error_msg().c_str() );
#endif
	}
	else
	{
#ifdef DEBUG_VERBOSE
		printf( "%s\n", xml_serializer.get_xml_string().c_str() );
#endif
	}

	return xml_serializer.get_xml_string();
}

template<typename T>
std::string serialize_xsd( const char* name, T* str_elem )
{
	reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<T>::get();

	XSDSerializer xsd_serializer( name );
	if( !typeDesc->accept( xsd_serializer, str_elem ) )
	{
#ifdef DEBUG_VERBOSE
		printf( "Errore :%s\n", xsd_serializer.get_error_msg().c_str() );
#endif
	}
	else
	{
#ifdef DEBUG_VERBOSE_XSD
		printf( "%s\n", xsd_serializer.get_xsd_string().c_str() );
#endif
	}

	return xsd_serializer.get_xsd_string();
}

template<typename T>
bool deserialize( const std::string& xml, T* str_elem )
{
	reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<T>::get();

	XMLDeserializer deserializer( xml.c_str() );

	if( !typeDesc->accept( deserializer, str_elem ) )
	{
#ifdef DEBUG_VERBOSE
		printf( "Errore :%s\n", deserializer.get_error_msg().c_str() );
#endif
		return false;
	}
	else
	{
#ifdef DEBUG_STRING
		StringSerializer str_serializer;
		typeDesc->accept( str_serializer, str_elem );

		printf( "%s\n", str_serializer.get_string().c_str() );
#endif

		return true;
	}
}

template<typename T>
bool fromStruct2XML2Strct( const char* name, T* str_elem, std::string & xml, T*str_des )
{
	xml = serialize_xml<T>( name, str_elem );
	if( xml.empty() )
		return false;

	if( !deserialize<T>( xml, str_des ) )
		return false;

	return true;
}
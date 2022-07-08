
#include "Reflection.h"
#include "XMLSerializer.h"
#include "XMLDeserializer.h"
#include "StringSerializer.h"

#include <iostream>

struct TestVectorInt {
	std::string description;
	std::vector<int> int_vector;
};

REFLECT_BEGIN(TestVectorInt)
REFLECT_MEMBER_ELEM(description)
REFLECT_MEMBER_ELEM(int_vector)
REFLECT_END()

template<typename T>
std::string serialize_xml( const char* name, T* str_elem )
{
	reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<T>::get();

	StringSerializer str_serializer;
	if( typeDesc->accept( str_serializer, str_elem ) )
	{
		printf( "%s\n", str_serializer.get_string().c_str() );
	}

	XMLSerializer xml_serializer( name, false );
	if( !typeDesc->accept( xml_serializer, str_elem ) )
	{
		printf( "Errore :%s\n", xml_serializer.get_error_msg().c_str() );
	}
	else
	{
		printf( "%s\n", xml_serializer.get_xml_string().c_str() );
	}

	return xml_serializer.get_xml_string();
}

template<typename T>
void deserialize( const std::string & xml, T* str_elem )
{
	reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<T>::get();

	XMLDeserializer deserializer( xml.c_str() );

	if( !typeDesc->accept( deserializer, str_elem ) )
	{
		printf( "Errore :%s\n", deserializer.get_error_msg().c_str() );
	}
	else
	{
		StringSerializer str_serializer;
		typeDesc->accept( str_serializer, str_elem );

		printf( "%s\n", str_serializer.get_string().c_str() );
	}
}

int test_ReflectionXml()
{
	TestVectorInt ttest;
	ttest.description = "prova";
	ttest.int_vector.push_back(1);
	ttest.int_vector.push_back(2);
	ttest.int_vector.push_back(3);
	ttest.int_vector.push_back(4);

	reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<TestVectorInt>::get();
	std::string xml = serialize_xml<TestVectorInt>("Prova", &ttest);

	return 0;
}



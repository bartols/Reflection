#include <gtest/gtest.h>

#include "test_util.h"

#include "SampleStructs.h"

const char * _str_escaped =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strSimplePODAttr int_value="1" str_value="&lt;ciccio&gt;ue&lt;/ciccio&gt;" bool_value="false" double_value="3.000000"/>
)xml";

TEST( Reflection, simplePODElem )
{
	strSimplePODElem str_test;
	str_test.int_value = 1;
	str_test.str_value = "2";
	str_test.bool_value = false;
	str_test.double_value = 3;

	strSimplePODElem str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strSimplePODElem>( "strSimplePODElem", &str_test, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strSimplePODElem_xml );
	EXPECT_EQ( str_test, str_dest );
}

TEST( Reflection, simplePODAttr )
{
	strSimplePODAttr str_test;
	str_test.int_value = 1;
	str_test.str_value = "2";
	str_test.bool_value = false;
	str_test.double_value = 3;

	strSimplePODAttr str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strSimplePODAttr>( "strSimplePODAttr", &str_test, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strSimplePODAttr_xml );
	EXPECT_EQ( str_test, str_dest );
}

TEST(Reflection, escapeNotXmlChar)
{
	strSimplePODAttr str_test;
	str_test.int_value = 1;
	str_test.str_value = "<ciccio>ue</ciccio>";
	str_test.bool_value = false;
	str_test.double_value = 3;

	std::string xml = serialize_xml<strSimplePODAttr>("strSimplePODAttr", &str_test);
	ASSERT_FALSE(xml.empty());
	EXPECT_EQ(xml, _str_escaped);
}

TEST( Reflection, strSimplePODMix )
{
	strSimplePODMix str_test;
	str_test.int_value = 1;
	str_test.str_value = "2";
	str_test.bool_value = false;
	str_test.double_value = 3;

	strSimplePODMix str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strSimplePODMix>( "strSimplePODMix", &str_test, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strSimplePODMix_xml );
	EXPECT_EQ( str_test, str_dest );
}

TEST( Reflection, strVectorElem )
{
	strVectorElem str_vect;
	str_vect.description = "test";

	{
		strSimplePODElem str_test;
		str_test.int_value = 1;
		str_test.str_value = "2";
		str_test.bool_value = false;
		str_test.double_value = 3;
		str_vect.vect_pod.push_back( str_test );
	}

	{
		strSimplePODElem str_test;
		str_test.int_value = 4;
		str_test.str_value = "5";
		str_test.bool_value = false;
		str_test.double_value = 6;
		str_vect.vect_pod.push_back( str_test );
	}

	strVectorElem str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strVectorElem>( "strVectorElem", &str_vect, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strVectorElem_xml );
	EXPECT_EQ( str_vect.description, str_dest.description );
	EXPECT_EQ( str_vect.vect_pod, str_dest.vect_pod );
}

TEST( Reflection, strVectorAttr )
{
	strVectorAttr str_vect;
	str_vect.description = "test";

	{
		strSimplePODAttr str_test;
		str_test.int_value = 1;
		str_test.str_value = "2";
		str_test.bool_value = false;
		str_test.double_value = 3;
		str_vect.vect_pod.push_back( str_test );
	}

	{
		strSimplePODAttr str_test;
		str_test.int_value = 4;
		str_test.str_value = "5";
		str_test.bool_value = false;
		str_test.double_value = 6;
		str_vect.vect_pod.push_back( str_test );
	}

	strVectorAttr str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strVectorAttr>( "strVectorAttr", &str_vect, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strVectorAttr_xml );
	EXPECT_EQ( str_vect.description, str_dest.description );
	EXPECT_EQ( str_vect.vect_pod, str_dest.vect_pod );
}

TEST( Reflection, strVectorUnamedElem )
{
	strVectorUnamedElem str_vect;
	str_vect.description = "test";

	{
		strSimplePODElem str_test;
		str_test.int_value = 1;
		str_test.str_value = "2";
		str_test.bool_value = false;
		str_test.double_value = 3;
		str_vect.vect_pod.push_back( str_test );
	}

	{
		strSimplePODElem str_test;
		str_test.int_value = 4;
		str_test.str_value = "5";
		str_test.bool_value = false;
		str_test.double_value = 6;
		str_vect.vect_pod.push_back( str_test );
	}

	strVectorUnamedElem str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strVectorUnamedElem>( "strVectorUnamedElem", &str_vect, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strVectorUnamedElem_xml );
	EXPECT_EQ( str_vect.description, str_dest.description );
	EXPECT_EQ( str_vect.vect_pod, str_dest.vect_pod );
}

TEST( Reflection, strVectorUnamedAttr )
{
	strVectorUnamedAttr str_vect;
	str_vect.description = "test";

	{
		strSimplePODAttr str_test;
		str_test.int_value = 1;
		str_test.str_value = "2";
		str_test.bool_value = false;
		str_test.double_value = 3;
		str_vect.vect_pod.push_back( str_test );
	}

	{
		strSimplePODAttr str_test;
		str_test.int_value = 4;
		str_test.str_value = "5";
		str_test.bool_value = false;
		str_test.double_value = 6;
		str_vect.vect_pod.push_back( str_test );
	}

	strVectorUnamedAttr str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strVectorUnamedAttr>( "strVectorUnamedAttr", &str_vect, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strVectorUnamedAttr_xml );
	EXPECT_EQ( str_vect.description, str_dest.description );
	EXPECT_EQ( str_vect.vect_pod, str_dest.vect_pod );
}

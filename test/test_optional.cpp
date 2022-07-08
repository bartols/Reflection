
#include <gtest/gtest.h>

#include "SampleStructs.h"

#include "test_util.h"

TEST( Optional, strSimpleOptWith )
{
	strSimpleOpt elem;
	elem.description = "test";
	elem.age = 50;

	strSimpleOpt elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strSimpleOpt>( "strSimpleOpt", &elem, xml_string, &elem_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _xml_strSimpleOptWith );
	EXPECT_EQ( elem, elem_dest );
}

TEST( Optional, strSimpleOptWithout )
{
	strSimpleOpt elem;
	elem.description = "test";

	strSimpleOpt elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strSimpleOpt>( "strSimpleOpt", &elem, xml_string, &elem_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _xml_strSimpleOptWithout );
	EXPECT_EQ( elem, elem_dest );
}

TEST( Optional, strElementOptWith )
{
	strElementOpt elem;
	elem.description = "test";
	elem.item = {"prova","valore"};

	strElementOpt elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strElementOpt>( "strElementOpt", &elem, xml_string, &elem_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _xml_strElementOptWith );
	EXPECT_EQ( elem, elem_dest );
}

TEST( Optional, strElementOptWithOut )
{
	strElementOpt elem;
	elem.description = "test";

	strElementOpt elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strElementOpt>( "strElementOpt", &elem, xml_string, &elem_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _xml_strElementOptWithout );
	EXPECT_EQ( elem, elem_dest );
}

TEST( Optional, strOptionalInOptional_With )
{
	strOptionalInOptional elem;
	elem.int_value = 11;
	elem.opt_elem = { "test", 12 };

	strOptionalInOptional elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strOptionalInOptional>( "strOptionalInOptional", &elem, xml_string, &elem_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _xml_strOptionalInOptionalWith );
	EXPECT_EQ( elem, elem_dest );
}

TEST( Optional, strOptionalInOptional_WithOut )
{
	strOptionalInOptional elem;
	elem.int_value = 11;

	strOptionalInOptional elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strOptionalInOptional>( "strOptionalInOptional", &elem, xml_string, &elem_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _xml_strOptionalInOptionalWithOut );
	EXPECT_EQ( elem, elem_dest );
}

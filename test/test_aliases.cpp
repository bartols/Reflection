
#include <gtest/gtest.h>

#include "test_util.h"

struct strAliaseAsElement {
	std::string description;
	std::string vtokenUid;
	std::string error;
};

REFLECT_BEGIN_AS( strAliaseAsElement, "strAliaseAsElement" )
REFLECT_MEMBER_ELEM( description )
REFLECT_MEMBER_ELEM_AS( vtokenUid, "VTokenUid", "VTokenUID", "vTokenUid", "tuaSorella" )
REFLECT_MEMBER_ELEM_AS( error, "Ciccio" )
REFLECT_END()

const std::string _xml_str_elem_ = 
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strAliaseAsElement>
    <description>test</description>
    <VTokenUid>777</VTokenUid>
    <Ciccio>2</Ciccio>
</strAliaseAsElement>
)xml";

const std::string _xml_str_elem_1_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strAliaseAsElement>
    <description>test</description>
    <VTokenUID>777</VTokenUID>
    <Ciccio>2</Ciccio>
</strAliaseAsElement>
)xml";

const std::string _xml_str_elem_2_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strAliaseAsElement>
    <description>test</description>
    <vTokenUid>777</vTokenUid>
    <Ciccio>2</Ciccio>
</strAliaseAsElement>
)xml";

const std::string _xml_str_elem_3_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strAliaseAsElement>
    <description>test</description>
    <tuaSorella>777</tuaSorella>
    <Ciccio>2</Ciccio>
</strAliaseAsElement>
)xml";

struct strAliaseAsAttribute {
	std::string description;
	std::string vtokenUid;
};

REFLECT_BEGIN_AS( strAliaseAsAttribute, "strAliaseAsAttribute" )
REFLECT_MEMBER_ATTR_AS( vtokenUid, "VTokenUid", "VTokenUID", "vTokenUid", "tuaSorella" )
REFLECT_MEMBER_ELEM( description )
REFLECT_END()

const std::string _xml_str_attr_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strAliaseAsAttribute VTokenUid="777">
    <description>test</description>
</strAliaseAsAttribute>
)xml";

const std::string _xml_str_attr_1_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strAliaseAsAttribute VTokenUid="777">
    <description>test</description>
</strAliaseAsAttribute>
)xml";

const std::string _xml_str_attr_2_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strAliaseAsAttribute VTokenUid="777">
    <description>test</description>
</strAliaseAsAttribute>
)xml";

const std::string _xml_str_attr_3_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strAliaseAsAttribute VTokenUid="777">
    <description>test</description>
</strAliaseAsAttribute>
)xml";

TEST( Aliases, AliasAsElementBase )
{
	strAliaseAsElement str_elem;
	str_elem.description = "test";
	str_elem.vtokenUid = "777";
	str_elem.error = "2";

	strAliaseAsElement elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strAliaseAsElement>( "strAliaseAsElement", &str_elem, xml_string, &elem_dest );
	ASSERT_TRUE( res );
	ASSERT_EQ( xml_string, _xml_str_elem_ );
}

TEST( Aliases, AliasAsElementDeserializeAlias1 )
{
	strAliaseAsElement str_elem;

	bool res = deserialize<strAliaseAsElement>( _xml_str_elem_1_, &str_elem );
	ASSERT_TRUE( res );

	ASSERT_EQ( str_elem.description, "test" );
	ASSERT_EQ( str_elem.vtokenUid, "777" );
	ASSERT_EQ( str_elem.error, "2" );
}

TEST( Aliases, AliasAsElementDeserializeAlias2 )
{
	strAliaseAsElement str_elem;

	bool res = deserialize<strAliaseAsElement>( _xml_str_elem_2_, &str_elem );
	ASSERT_TRUE( res );

	ASSERT_EQ( str_elem.description, "test" );
	ASSERT_EQ( str_elem.vtokenUid, "777" );
	ASSERT_EQ( str_elem.error, "2" );
}

TEST( Aliases, AliasAsElementDeserializeAlias3 )
{
	strAliaseAsElement str_elem;

	bool res = deserialize<strAliaseAsElement>( _xml_str_elem_3_, &str_elem );
	ASSERT_TRUE( res );

	ASSERT_EQ( str_elem.description, "test" );
	ASSERT_EQ( str_elem.vtokenUid, "777" );
	ASSERT_EQ( str_elem.error, "2" );
}

TEST( Aliases, strAliaseAsAttribute )
{
	strAliaseAsAttribute str_attr;
	str_attr.description = "test";
	str_attr.vtokenUid = "777";

	std::string xml = serialize_xml<strAliaseAsAttribute>( "strAliaseAsAttribute", &str_attr );

	ASSERT_EQ( xml, _xml_str_attr_ );
}

TEST( Aliases, AliaseAsAttributeDeserializeAlias1 )
{
	strAliaseAsAttribute str_attr;
	str_attr.description = "test";
	str_attr.vtokenUid = "777";

	std::string xml = serialize_xml<strAliaseAsAttribute>( "strAliaseAsAttribute", &str_attr );

	ASSERT_EQ( xml, _xml_str_attr_1_ );
}

TEST( Aliases, AliaseAsAttributeDeserializeAlias2 )
{
	strAliaseAsAttribute str_attr;
	str_attr.description = "test";
	str_attr.vtokenUid = "777";

	std::string xml = serialize_xml<strAliaseAsAttribute>( "strAliaseAsAttribute", &str_attr );

	ASSERT_EQ( xml, _xml_str_attr_2_ );
}

TEST( Aliases, AliaseAsAttributeDeserializeAlias3 )
{
	strAliaseAsAttribute str_attr;
	str_attr.description = "test";
	str_attr.vtokenUid = "777";

	std::string xml = serialize_xml<strAliaseAsAttribute>( "strAliaseAsAttribute", &str_attr );

	ASSERT_EQ( xml, _xml_str_attr_3_ );
}

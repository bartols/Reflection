
#include <gtest/gtest.h>

#include "test_util.h"

enum eColor {
	yellow,
	green,
	red,
	white
};
REFLECT_ENUM_BEGIN(eColor)
REFLECT_ENUM_MEMBER_AS(yellow,            "yellow")
REFLECT_ENUM_MEMBER_AS(green,             "green")
REFLECT_ENUM_MEMBER_AS(red,               "red")
REFLECT_ENUM_MEMBER_AS(white,             "light")
REFLECT_ENUM_END()

enum eDirection {
	nord,
	est, 
	sud, 
	ovest
};
REFLECT_ENUM_BEGIN(eDirection)
REFLECT_ENUM_MEMBER(nord)
REFLECT_ENUM_MEMBER(est)
REFLECT_ENUM_MEMBER(sud)
REFLECT_ENUM_MEMBER_AS(ovest, "ovest")
REFLECT_ENUM_END()

enum class eReceiptFormat {
	receiptFormat_NONE,
	receiptFormat_PDF,
	receiptFormat_TXT,
	receiptFormat_XML
};
REFLECT_ENUM_BEGIN(eReceiptFormat)
REFLECT_ENUM_MEMBER_AS(eReceiptFormat::receiptFormat_NONE, "None")
REFLECT_ENUM_MEMBER_AS(eReceiptFormat::receiptFormat_PDF, "PDF")
REFLECT_ENUM_MEMBER_AS(eReceiptFormat::receiptFormat_TXT, "TXT")
REFLECT_ENUM_MEMBER_AS(eReceiptFormat::receiptFormat_XML, "XML")
REFLECT_ENUM_END()

struct strEnumAsElement {
	eColor color;
	eDirection direction;
	std::string description;
};

REFLECT_BEGIN( strEnumAsElement )
REFLECT_MEMBER_ELEM( color )
REFLECT_MEMBER_ELEM( direction )
REFLECT_MEMBER_ELEM( description )
REFLECT_END()

const std::string _str_enum_elem_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strEnumAsElement>
    <color>red</color>
    <direction>sud</direction>
    <description>rosso</description>
</strEnumAsElement>
)xml";

const std::string _str_enum_elem_as_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strEnumAsElement>
    <color>light</color>
    <direction>ovest</direction>
    <description>rosso</description>
</strEnumAsElement>
)xml";

struct strEnumAsAttribute {
	eColor color;
	eDirection direction;
	std::string description;
};

REFLECT_BEGIN( strEnumAsAttribute )
REFLECT_MEMBER_ATTR( color )
REFLECT_MEMBER_ATTR( direction )
REFLECT_MEMBER_ELEM( description )
REFLECT_END()

const std::string _str_enum_attr_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strEnumAsAttribute color="red" direction="sud">
    <description>rosso</description>
</strEnumAsAttribute>
)xml";

TEST(Enums, toString)
{
	std::string t1 = reflect::toString(yellow);
	ASSERT_EQ(t1, "yellow");
	t1 = reflect::toString(red);
	ASSERT_EQ(t1, "red");
	t1 = reflect::toString(white);
	ASSERT_EQ(t1, "light");
}

TEST(Enums, toStringEnumClass)
{
	std::string t1 = reflect::toString(eReceiptFormat::receiptFormat_PDF);
	ASSERT_EQ(t1, "PDF");
}

TEST(Enums, fromString)
{
	eColor color = reflect::fromString<eColor>("yellow");
	ASSERT_EQ(color, yellow);
}

TEST( Enums, EnumAsElement )
{
	strEnumAsElement elem;
	elem.color = red;
	elem.direction = sud;
	elem.description = "rosso";

	strEnumAsElement elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strEnumAsElement>( "strEnumAsElement", &elem, xml_string, &elem_dest );
	ASSERT_TRUE( res );
	ASSERT_EQ( xml_string, _str_enum_elem_ );
}

TEST( Enums, EnumAsElementAs )
{
	strEnumAsElement elem;
	elem.color = white;
	elem.direction = ovest;
	elem.description = "rosso";

	strEnumAsElement elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strEnumAsElement>( "strEnumAsElement", &elem, xml_string, &elem_dest );
	ASSERT_TRUE( res );
	ASSERT_EQ( xml_string, _str_enum_elem_as_ );
}

TEST( Enums, EnumAsAttribute )
{
	strEnumAsAttribute elem;
	elem.color = red;
	elem.direction = sud;
	elem.description = "rosso";

	strEnumAsAttribute elem_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strEnumAsAttribute>( "strEnumAsAttribute", &elem, xml_string, &elem_dest );
	ASSERT_TRUE( res );
	ASSERT_EQ( xml_string, _str_enum_attr_ );
}
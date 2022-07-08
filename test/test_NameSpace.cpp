#include <gtest/gtest.h>

#include "test_util.h"

struct strInnerElement {
	std::string name;
	std::string family_name;
	int age = 0;
};

REFLECT_BEGIN(strInnerElement)
REFLECT_MEMBER_ATTR_AS(name,		"name")
REFLECT_MEMBER_ATTR_AS(family_name, "family_name")
REFLECT_MEMBER_ATTR_AS(age,			"age")
REFLECT_END()

struct strTest {
	std::string description;
	int value = 0;
	strInnerElement data;
};

REFLECT_BEGIN_AS(strTest,			"vts:strTest")
REFLECT_MEMBER_ELEM_AS(description, "vts:description")
REFLECT_MEMBER_ELEM_AS(value,		"vts:value")
REFLECT_MEMBER_ELEM_AS(data,		"vts:Person" )
REFLECT_END()

const std::string _testWithNms_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strTest xmlns:vts="http://www.aep-italia.it/vts/">
    <vts:description>prova</vts:description>
    <vts:value>11</vts:value>
    <vts:Person name="Ciccio" family_name="Vaiano" age="50"/>
</strTest>
)xml";

const std::string _testNoNms_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strTest xmlns:vts="http://www.aep-italia.it/vts/">
    <description>prova</description>
    <value>11</value>
    <Person name="Ciccio" family_name="Vaiano" age="50"/>
</strTest>
)xml";

const std::string _testNoNmsNoDefined_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strTest>
    <description>prova</description>
    <value>11</value>
    <Person name="Ciccio" family_name="Vaiano" age="50"/>
</strTest>
)xml";

const std::string _testWithNmsError_ =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strTest xmlns:vts="http://www.aep-italia.it/vts/">
    <ciccio:description>prova</ciccio:description>
    <vts:value>11</vts:value>
    <vts:Person name="Ciccio" family_name="Vaiano" age="50"/>
</strTest>
)xml";

TEST(NameSpace, deserializeWithNms)
{
	strTest test;

	bool res = deserialize<strTest>(_testWithNms_, &test);
	ASSERT_TRUE(res);
}

TEST(NameSpace, deserializeNohNms)
{
	strTest test;

	bool res = deserialize<strTest>(_testNoNms_, &test);
	ASSERT_TRUE(res);
}

TEST(NameSpace, deserializeNohNmsNoDefined)
{
	strTest test;

	bool res = deserialize<strTest>(_testNoNmsNoDefined_, &test);
	ASSERT_TRUE(res);
}

TEST(NameSpace, deserializeWithNmsError)
{
	strTest test;

	bool res = deserialize<strTest>(_testWithNmsError_, &test);
	ASSERT_FALSE(res);
}

TEST( NameSpace, serializeWith )
{
	strTest test;
	test.description = "prova";
	test.value = 11;
	test.data.age = 50;
	test.data.name = "Ciccio";
	test.data.family_name = "Vaiano";

	std::string xml = serialize_xml<strTest>("strTest", &test, "xmlns:vts=\"http://www.aep-italia.it/vts/\"");
	ASSERT_EQ(xml, _testWithNms_);
}

TEST(NameSpace, serializeWithout)
{
	strTest test;
	test.description = "prova";
	test.value = 11;
	test.data.age = 50;
	test.data.name = "Ciccio";
	test.data.family_name = "Vaiano";

	std::string xml = serialize_xml<strTest>("strTest", &test);
	ASSERT_EQ(xml, _testNoNmsNoDefined_);
}

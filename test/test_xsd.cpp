
#include <gtest/gtest.h>

#include "test_util.h"

#include "SampleStructs.h"

const std::string _xsd_simplePODElem =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimplePODElemType">
    <xs:sequence>
      <xs:element name="int_value" type="xs:int"/>
      <xs:element name="str_value" type="xs:string"/>
      <xs:element name="bool_value" type="xs:boolean"/>
      <xs:element name="double_value" type="xs:decimal"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strSimplePODElem" type="strSimplePODElemType"/>
</xs:schema>
)xml";

const std::string _xsd_simplePODAttr =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimplePODAttrType">
    <xs:attribute name="int_value" type="xs:int" use="required"/>
    <xs:attribute name="str_value" type="xs:string" use="required"/>
    <xs:attribute name="bool_value" type="xs:boolean" use="required"/>
    <xs:attribute name="double_value" type="xs:decimal" use="required"/>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strSimplePODAttr" type="strSimplePODAttrType"/>
</xs:schema>
)xml";

const std::string _xsd_simplePODMix = 
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimplePODMixType">
    <xs:sequence>
      <xs:element name="bool_value" type="xs:boolean"/>
      <xs:element name="double_value" type="xs:decimal"/>
    </xs:sequence>
    <xs:attribute name="int_value" type="xs:int" use="required"/>
    <xs:attribute name="str_value" type="xs:string" use="required"/>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strSimplePODMix" type="strSimplePODMixType"/>
</xs:schema>
)xml";

const std::string _xsd_strVectorElem =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimplePODElemType">
    <xs:sequence>
      <xs:element name="int_value" type="xs:int"/>
      <xs:element name="str_value" type="xs:string"/>
      <xs:element name="bool_value" type="xs:boolean"/>
      <xs:element name="double_value" type="xs:decimal"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strSimplePODElemList">
    <xs:sequence>
      <xs:element name="strSimplePODElem" type="strSimplePODElemType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strVectorElemType">
    <xs:sequence>
      <xs:element name="description" type="xs:string"/>
      <xs:element name="vect_pod" type="strSimplePODElemList"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strVectorElem" type="strVectorElemType"/>
</xs:schema>
)xml";

const std::string _xsd_strVectorAttr =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimplePODAttrType">
    <xs:attribute name="int_value" type="xs:int" use="required"/>
    <xs:attribute name="str_value" type="xs:string" use="required"/>
    <xs:attribute name="bool_value" type="xs:boolean" use="required"/>
    <xs:attribute name="double_value" type="xs:decimal" use="required"/>
  </xs:complexType>
  <xs:complexType name="strSimplePODAttrList">
    <xs:sequence>
      <xs:element name="strSimplePODAttr" type="strSimplePODAttrType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strVectorAttrType">
    <xs:sequence>
      <xs:element name="description" type="xs:string"/>
      <xs:element name="vect_pod" type="strSimplePODAttrList"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strVectorAttr" type="strVectorAttrType"/>
</xs:schema>
)xml";

const std::string _xsd_strVectorUnamedElem =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimplePODElemType">
    <xs:sequence>
      <xs:element name="int_value" type="xs:int"/>
      <xs:element name="str_value" type="xs:string"/>
      <xs:element name="bool_value" type="xs:boolean"/>
      <xs:element name="double_value" type="xs:decimal"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strVectorUnamedElemType">
    <xs:sequence>
      <xs:element name="description" type="xs:string"/>
      <xs:element name="strSimplePODElem" type="strSimplePODElemType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strVectorUnamedElem" type="strVectorUnamedElemType"/>
</xs:schema>
)xml";

const std::string _xsd_strVectorUnamedAttr =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimplePODAttrType">
    <xs:attribute name="int_value" type="xs:int" use="required"/>
    <xs:attribute name="str_value" type="xs:string" use="required"/>
    <xs:attribute name="bool_value" type="xs:boolean" use="required"/>
    <xs:attribute name="double_value" type="xs:decimal" use="required"/>
  </xs:complexType>
  <xs:complexType name="strVectorUnamedAttrType">
    <xs:sequence>
      <xs:element name="description" type="xs:string"/>
      <xs:element name="strSimplePODAttr" type="strSimplePODAttrType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strVectorUnamedAttr" type="strVectorUnamedAttrType"/>
</xs:schema>
)xml";

const std::string _xsd_strSimpleOpt =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimpleOptType">
    <xs:attribute name="description" type="xs:string" use="required"/>
    <xs:attribute name="age" type="xs:int"/>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strSimpleOpt" type="strSimpleOptType"/>
</xs:schema>
)xml";

const std::string _xsd_strOptionalInOptional =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strSimpleOpt">
    <xs:attribute name="description" type="xs:string" use="required"/>
    <xs:attribute name="age" type="xs:int"/>
  </xs:complexType>
  <xs:complexType name="strOptionalInOptionalType">
    <xs:sequence>
      <xs:element name="int_value" type="xs:int"/>
      <xs:element name="opt_elem" type="strSimpleOpt" minOccurs="0"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strOptionalInOptional" type="strOptionalInOptionalType"/>
</xs:schema>
)xml";

enum eAnimal {
    cat,
    dog,
    racoon
};

REFLECT_ENUM_BEGIN( eAnimal, "Animal" )
REFLECT_ENUM_MEMBER( cat, "cat" )
REFLECT_ENUM_MEMBER( dog, "dog" )
REFLECT_ENUM_MEMBER( racoon, "racoon" )
REFLECT_ENUM_END()

struct strAnimal {
    eAnimal type;
    std::string name;
};

REFLECT_BEGIN( strAnimal )
REFLECT_MEMBER_ELEM( type )
REFLECT_MEMBER_ELEM( name )
REFLECT_END()

const std::string _xsd_strEnum =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:simpleType name="eAnimal">
    <xs:restriction base="xs:string">
      <xs:enumeration value="cat"/>
      <xs:enumeration value="dog"/>
      <xs:enumeration value="racoon"/>
    </xs:restriction>
  </xs:simpleType>
  <xs:complexType name="strAnimalType">
    <xs:sequence>
      <xs:element name="type" type="eAnimal"/>
      <xs:element name="name" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strAnimal" type="strAnimalType"/>
</xs:schema>
)xml";

struct strPerson {
    std::string firstname;
    std::string lastname;
};

REFLECT_BEGIN( strPerson )
REFLECT_MEMBER_ELEM( firstname )
REFLECT_MEMBER_ELEM( lastname )
REFLECT_END()

struct strPeople {
    std::string description;
    std::vector<strPerson> _persons;
};

REFLECT_BEGIN( strPeople )
REFLECT_MEMBER_ELEM( description )
REFLECT_MEMBER_UNAMED( _persons )
REFLECT_END()

struct strComplexInComplex {
    strPeople _people;
};

REFLECT_BEGIN( strComplexInComplex )
REFLECT_MEMBER_ELEM( _people )
REFLECT_END()

const std::string _xsd_strComplexInComplex =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strPersonType">
    <xs:sequence>
      <xs:element name="firstname" type="xs:string"/>
      <xs:element name="lastname" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strPeople">
    <xs:sequence>
      <xs:element name="description" type="xs:string"/>
      <xs:element name="strPerson" type="strPersonType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strComplexInComplexType">
    <xs:sequence>
      <xs:element name="_people" type="strPeople"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strComplexInComplex" type="strComplexInComplexType"/>
</xs:schema>
)xml";

struct DevicesInitialization_Req
{
    std::vector<std::string> DeviceId;
};

REFLECT_BEGIN( DevicesInitialization_Req )
REFLECT_MEMBER_UNAMED_AS( DeviceId, "vts:DeviceUid" )
REFLECT_END()


struct testMaxOccurs {
    std::string description;
    nonstd::optional<DevicesInitialization_Req> devicesInitialization_Req;
};

REFLECT_BEGIN_AS( testMaxOccurs, "vts:BodyRequest" )
REFLECT_MEMBER_ELEM_AS( description, "descrition" )
REFLECT_MEMBER_ELEM_AS( devicesInitialization_Req, "vts:DevicesInitialization" )
REFLECT_END()

const std::string _xsd_strMaxOccurs =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="DevicesInitialization_Req">
    <xs:sequence>
      <xs:element name="DeviceUid" type="xs:string" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="BodyRequest">
    <xs:sequence>
      <xs:element name="descrition" type="xs:string"/>
      <xs:element name="DevicesInitialization" type="DevicesInitialization_Req" minOccurs="0"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="testMaxOccurs" type="BodyRequest"/>
</xs:schema>
)xml";

TEST( XSD, testMaxOccurs )
{
    testMaxOccurs str_elem;
    std::string xsd_string = serialize_xsd<testMaxOccurs>( "testMaxOccurs", &str_elem );
    EXPECT_EQ( xsd_string, _xsd_strMaxOccurs);
}

TEST( XSD, complexInComplex )
{
    strComplexInComplex str_elem;
    std::string xsd_string = serialize_xsd<strComplexInComplex>( "strComplexInComplex", &str_elem );
    EXPECT_EQ( xsd_string, _xsd_strComplexInComplex );
}

TEST( XSD, simplePODElem )
{
	strSimplePODElem str_elem;
	std::string xsd_string = serialize_xsd<strSimplePODElem>( "strSimplePODElem", &str_elem );
	EXPECT_EQ( xsd_string, _xsd_simplePODElem );
}

TEST( XSD, simplePODAttr )
{
	strSimplePODAttr str_elem;
	std::string xsd_string = serialize_xsd<strSimplePODAttr>( "strSimplePODAttr", &str_elem );
    EXPECT_EQ( xsd_string, _xsd_simplePODAttr );
}

TEST( XSD, simplePODMix )
{
    strSimplePODMix str_elem;
    std::string xsd_string = serialize_xsd<strSimplePODMix>( "strSimplePODMix", &str_elem );
    EXPECT_EQ( xsd_string, _xsd_simplePODMix );
}

TEST( XSD, strVectorElem )
{
	strVectorElem str_vect;
	std::string xsd_string = serialize_xsd<strVectorElem>( "strVectorElem", &str_vect );
    EXPECT_EQ( xsd_string, _xsd_strVectorElem );
}

TEST( XSD, strVectorAttr )
{
	strVectorAttr str_vect;
	std::string xsd_string = serialize_xsd<strVectorAttr>( "strVectorAttr", &str_vect );
    EXPECT_EQ( xsd_string, _xsd_strVectorAttr );
}

TEST( XSD, strVectorUnamedElem )
{
	strVectorUnamedElem str_vect;
	std::string xsd_string = serialize_xsd<strVectorUnamedElem>( "strVectorUnamedElem", &str_vect );
    EXPECT_EQ( xsd_string, _xsd_strVectorUnamedElem );
}

TEST( XSD, strVectorUnamedAttr )
{
    strVectorUnamedAttr str_vect;
    std::string xsd_string = serialize_xsd<strVectorUnamedAttr>( "strVectorUnamedAttr", &str_vect );
    EXPECT_EQ( xsd_string, _xsd_strVectorUnamedAttr );
}

TEST( XSD, strSimpleOpt )
{
    strSimpleOpt str_opt;
    std::string xsd_string = serialize_xsd<strSimpleOpt>( "strSimpleOpt", &str_opt );
    EXPECT_EQ( xsd_string, _xsd_strSimpleOpt );
}

TEST( XSD, strOptionalInOptional )
{
    strOptionalInOptional str_opt;
    std::string xsd_string = serialize_xsd<strOptionalInOptional>( "strOptionalInOptional", &str_opt );
    EXPECT_EQ( xsd_string, _xsd_strOptionalInOptional );
}

TEST( XSD, strEnum )
{
    strAnimal str_enum;
    std::string xsd_string = serialize_xsd<strAnimal>( "strAnimal", &str_enum );
    EXPECT_EQ( xsd_string, _xsd_strEnum );
}

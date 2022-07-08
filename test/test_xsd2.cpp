
#include <gtest/gtest.h>

#include "SampleStructs.h"

#include "test_util.h"

struct strIntern
{
	std::string xx = "leo";
	int zz = 45;
};
REFLECT_BEGIN_AS(strIntern, "vts:strIntern")
REFLECT_MEMBER_ELEM_AS(xx, "vts:xx")
REFLECT_MEMBER_ELEM_AS(zz, "vts:zz")
REFLECT_END()

struct strExtern
{
	strIntern aa;
	int b = 1;
};
REFLECT_BEGIN_AS(strExtern, "AA")
REFLECT_MEMBER_ELEM_AS(aa, "vts:strIntern")
REFLECT_MEMBER_ELEM_AS(b, "vts:b")
REFLECT_END()

const std::string _xsd_str_internExtern =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strInternType">
    <xs:sequence>
      <xs:element name="xx" type="xs:string"/>
      <xs:element name="zz" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="AA">
    <xs:sequence>
      <xs:element name="strIntern" type="strInternType"/>
      <xs:element name="b" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strExtern" type="AA"/>
</xs:schema>
)xml";

struct strWithVector
{
	strIntern aa;
	std::vector<int> vect;
	int b;
};

REFLECT_BEGIN_AS(strWithVector, "strWithVector")
REFLECT_MEMBER_ELEM_AS(aa, "vts:aa")
REFLECT_MEMBER_UNAMED_AS(vect, "vts:vect_of_a")
REFLECT_MEMBER_ELEM_AS(b, "vts:b")
REFLECT_END()

const std::string _xsd_str_vectorUnnamed =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strIntern">
    <xs:sequence>
      <xs:element name="xx" type="xs:string"/>
      <xs:element name="zz" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strWithVectorType">
    <xs:sequence>
      <xs:element name="aa" type="strIntern"/>
      <xs:element name="vect_of_a" type="xs:int" minOccurs="0" maxOccurs="unbounded"/>
      <xs:element name="b" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strWithVector" type="strWithVectorType"/>
</xs:schema>
)xml";

struct strWithVectorNamed
{
    strIntern aa;
    std::vector<int> vect;
    int b;
};

REFLECT_BEGIN_AS(strWithVectorNamed, "strWithVectorNamed")
REFLECT_MEMBER_ELEM_AS(aa, "vts:aa")
REFLECT_MEMBER_ELEM_AS(vect, "vts:vect_of_a")
REFLECT_MEMBER_ELEM_AS(b, "vts:b")
REFLECT_END()

const std::string _xsd_str_vectorNamed =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strIntern">
    <xs:sequence>
      <xs:element name="xx" type="xs:string"/>
      <xs:element name="zz" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="intList">
    <xs:sequence>
      <xs:element name="vect_of_a" type="xs:int" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strWithVectorNamedType">
    <xs:sequence>
      <xs:element name="aa" type="strIntern"/>
      <xs:element name="vect_of_a" type="intList"/>
      <xs:element name="b" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strWithVectorNamed" type="strWithVectorNamedType"/>
</xs:schema>
)xml";

struct strSelfContained {
    int aa = 1;
    Report _report;
    int cc = 0;
};

REFLECT_BEGIN_AS(strSelfContained, "strSelfContained")
REFLECT_MEMBER_ELEM_AS(aa, "vts:aa")
REFLECT_MEMBER_ELEM_AS(_report, "vts:Report")
REFLECT_MEMBER_ELEM_AS(cc, "vts:cc")
REFLECT_END()

const std::string _xsd_str_selfContained =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="ReportDataType">
    <xs:attribute name="name" type="xs:string" use="required"/>
    <xs:attribute name="value" type="xs:string" use="required"/>
  </xs:complexType>
  <xs:complexType name="ReportSectionType">
    <xs:sequence>
      <xs:element name="ReportData" type="ReportDataType" minOccurs="0" maxOccurs="unbounded"/>
      <xs:element name="ReportSection" type="ReportSectionType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
    <xs:attribute name="name" type="xs:string" use="required"/>
  </xs:complexType>
  <xs:complexType name="ReportType">
    <xs:sequence>
      <xs:element name="ReportSection" type="ReportSectionType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
    <xs:attribute name="type" type="xs:int" use="required"/>
  </xs:complexType>
  <xs:complexType name="strSelfContainedType">
    <xs:sequence>
      <xs:element name="aa" type="xs:int"/>
      <xs:element name="Report" type="ReportType"/>
      <xs:element name="cc" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strSelfContained" type="strSelfContainedType"/>
</xs:schema>
)xml";

struct strInternalOpt
{
    std::string xx = "leo";
    nonstd::optional<int> zz = 45;
    nonstd::optional<int> yy = 32;
};
REFLECT_BEGIN_AS(strInternalOpt, "strInternalOpt")
REFLECT_MEMBER_ELEM_AS(xx, "vts:xx")
REFLECT_MEMBER_ELEM_AS(zz, "vts:zz")
REFLECT_MEMBER_ELEM_AS(yy, "vts:yy")
REFLECT_END()

struct strExternalOpt
{
    strInternalOpt aa;
    std::vector<int> a;
    int b;
};
REFLECT_BEGIN_AS(strExternalOpt, "strExternalOpt")
REFLECT_MEMBER_ELEM_AS(aa, "vts:aa")
REFLECT_MEMBER_UNAMED_AS(a, "vts:aCCCC")
REFLECT_MEMBER_ELEM_AS(b, "vts:b")
REFLECT_END()

const std::string _xsd_str_externalOpt =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="strInternalOpt">
    <xs:sequence>
      <xs:element name="xx" type="xs:string"/>
      <xs:element name="zz" type="xs:int" minOccurs="0"/>
      <xs:element name="yy" type="xs:int" minOccurs="0"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strExternalOptType">
    <xs:sequence>
      <xs:element name="aa" type="strInternalOpt"/>
      <xs:element name="aCCCC" type="xs:int" minOccurs="0" maxOccurs="unbounded"/>
      <xs:element name="b" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strExternalOpt" type="strExternalOptType"/>
</xs:schema>
)xml";

// cash detail
struct CashDetail {
    std::string value;
    int qty = 0;
};

// cash detail
REFLECT_BEGIN_AS(CashDetail, "vts:CashDetail")
REFLECT_MEMBER_ATTR_AS(value, "Value")
REFLECT_MEMBER_ATTR_AS(qty, "Qty")
REFLECT_END()

// cash type
enum class eCashType {
    note,
    coin,
    pos,
    paper
};

// enum cash type
REFLECT_ENUM_BEGIN(eCashType, "Cash type")
REFLECT_ENUM_MEMBER_AS(eCashType::note,  "note",  "payment/change with bank-notes")
REFLECT_ENUM_MEMBER_AS(eCashType::coin,  "coin",  "payment/change with coins")
REFLECT_ENUM_MEMBER_AS(eCashType::pos,   "pos",   "payment/change with POS")
REFLECT_ENUM_MEMBER_AS(eCashType::paper, "paper", "payment/change with reeipt")
REFLECT_ENUM_END()

// equipment type
enum class eEquipmentType {
    DEFAULT = 0,
    BNA = 3,
    cashacceptor = 4,
};

// enum equipment type
REFLECT_ENUM_BEGIN(eEquipmentType, "equipment type")
REFLECT_ENUM_MEMBER_AS(eEquipmentType::DEFAULT,      "DEFAULT",      "default")
REFLECT_ENUM_MEMBER_AS(eEquipmentType::BNA,          "BNA",          "BNA")
REFLECT_ENUM_MEMBER_AS(eEquipmentType::cashacceptor, "cashacceptor", "cashacceptor")
REFLECT_ENUM_END()

// cash part
struct CashPart {
    std::string equipment;
    eEquipmentType type;
    int position = 0;
    std::string serialNum;
    eCashType cash_type;
    std::vector<CashDetail> details;
};

REFLECT_BEGIN_AS(CashPart, "vts:CashPart")
REFLECT_MEMBER_ATTR_AS(equipment, "Eq")
REFLECT_MEMBER_ATTR(type)
REFLECT_MEMBER_ATTR_AS(position, "Pos")
REFLECT_MEMBER_ATTR(serialNum);
REFLECT_MEMBER_ATTR(cash_type);
REFLECT_MEMBER_UNAMED_AS(details, "vts:CashDetail")
REFLECT_END()

struct strParameters {
    int a = 1;
    nonstd::optional<std::vector<CashPart>> cashBoxStatus;
};

REFLECT_BEGIN_AS(strParameters, "strParamenters")
REFLECT_MEMBER_ELEM_AS(a, "vts:AAAA")
REFLECT_MEMBER_ELEM_AS(cashBoxStatus, "vts:CashBoxStatus")
REFLECT_END()

const std::string _xsd_str_vectorOptional =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:simpleType name="eEquipmentType">
    <xs:restriction base="xs:string">
      <xs:enumeration value="DEFAULT"/>
      <xs:enumeration value="BNA"/>
      <xs:enumeration value="cashacceptor"/>
    </xs:restriction>
  </xs:simpleType>
  <xs:simpleType name="eCashType">
    <xs:restriction base="xs:string">
      <xs:enumeration value="note"/>
      <xs:enumeration value="coin"/>
      <xs:enumeration value="pos"/>
      <xs:enumeration value="paper"/>
    </xs:restriction>
  </xs:simpleType>
  <xs:complexType name="CashDetailType">
    <xs:attribute name="Value" type="xs:string" use="required"/>
    <xs:attribute name="Qty" type="xs:int" use="required"/>
  </xs:complexType>
  <xs:complexType name="CashPartType">
    <xs:sequence>
      <xs:element name="CashDetail" type="CashDetailType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
    <xs:attribute name="Eq" type="xs:string" use="required"/>
    <xs:attribute name="type" type="eEquipmentType" use="required"/>
    <xs:attribute name="Pos" type="xs:int" use="required"/>
    <xs:attribute name="serialNum" type="xs:string" use="required"/>
    <xs:attribute name="cash_type" type="eCashType" use="required"/>
  </xs:complexType>
  <xs:complexType name="CashPartList">
    <xs:sequence>
      <xs:element name="CashPart" type="CashPartType" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="strParamenters">
    <xs:sequence>
      <xs:element name="AAAA" type="xs:int"/>
      <xs:element name="CashBoxStatus" type="CashPartList" minOccurs="0"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strParameters" type="strParamenters"/>
</xs:schema>
)xml";

struct strOptional2
{
    nonstd::optional<std::vector<int>> a;
    int b;
};
REFLECT_BEGIN_AS(strOptional2, "AA")
REFLECT_MEMBER_ELEM_AS(a, "vts:aCCCC")
REFLECT_MEMBER_ELEM_AS(b, "vts:b")
REFLECT_END()

const std::string _xsd_str_strOptional2 =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:complexType name="intList">
    <xs:sequence>
      <xs:element name="aCCCC" type="xs:int" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>
  <xs:complexType name="AA">
    <xs:sequence>
      <xs:element name="aCCCC" type="intList" minOccurs="0"/>
      <xs:element name="b" type="xs:int"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strOptional2" type="AA"/>
</xs:schema>
)xml";

enum class eGender {
    male,
    female
};

REFLECT_ENUM_BEGIN(eGender, "Gender")
REFLECT_ENUM_MEMBER_AS(eGender::male,   "male",   "male")
REFLECT_ENUM_MEMBER_AS(eGender::female, "female", "female")
REFLECT_ENUM_END()

enum class eNationality {
    italian,
    french,
    spaniard
};

REFLECT_ENUM_BEGIN(eNationality, "Nationality")
REFLECT_ENUM_MEMBER_AS(eNationality::italian, "Italian",   "Italian")
REFLECT_ENUM_MEMBER_AS(eNationality::french, "French",     "French")
REFLECT_ENUM_MEMBER_AS(eNationality::spaniard, "Spaniard", "Spaniard")
REFLECT_ENUM_END()

struct strDoubleEnum {
    std::string name;
    eNationality nationality;
    eGender gen1_gender;
    eGender gen2_gender;
};

REFLECT_BEGIN_AS(strDoubleEnum, "strDoubleEnum")
REFLECT_MEMBER_ELEM_AS(name, "Name")
REFLECT_MEMBER_ELEM_AS(nationality, "Nationality")
REFLECT_MEMBER_ELEM_AS(gen1_gender, "Genitore1_gender")
REFLECT_MEMBER_ELEM_AS(gen2_gender, "Genitore2_gender")
REFLECT_END()

const std::string _xsd_str_strDoubleEnum =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:simpleType name="eNationality">
    <xs:restriction base="xs:string">
      <xs:enumeration value="Italian"/>
      <xs:enumeration value="French"/>
      <xs:enumeration value="Spaniard"/>
    </xs:restriction>
  </xs:simpleType>
  <xs:simpleType name="eGender">
    <xs:restriction base="xs:string">
      <xs:enumeration value="male"/>
      <xs:enumeration value="female"/>
    </xs:restriction>
  </xs:simpleType>
  <xs:complexType name="strDoubleEnumType">
    <xs:sequence>
      <xs:element name="Name" type="xs:string"/>
      <xs:element name="Nationality" type="eNationality"/>
      <xs:element name="Genitore1_gender" type="eGender"/>
      <xs:element name="Genitore2_gender" type="eGender"/>
    </xs:sequence>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strDoubleEnum" type="strDoubleEnumType"/>
</xs:schema>
)xml";

struct strAttrEnum {
    std::string name;
    eNationality nationality;
};

REFLECT_BEGIN_AS(strAttrEnum, "strAttrEnum")
REFLECT_MEMBER_ELEM_AS(name, "Name")
REFLECT_MEMBER_ATTR_AS(nationality, "Nationality")
REFLECT_END()

const std::string _xsd_str_strAttrEnum =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema" targetNamespace="http://www.aep-italia.it/vts/" xmlns="http://www.aep-italia.it/vts/">
  <!-- Complex types declarations -->
  <xs:simpleType name="eNationality">
    <xs:restriction base="xs:string">
      <xs:enumeration value="Italian"/>
      <xs:enumeration value="French"/>
      <xs:enumeration value="Spaniard"/>
    </xs:restriction>
  </xs:simpleType>
  <xs:complexType name="strAttrEnumType">
    <xs:sequence>
      <xs:element name="Name" type="xs:string"/>
    </xs:sequence>
    <xs:attribute name="Nationality" type="eNationality" use="required"/>
  </xs:complexType>
  <!-- Elements -->
  <xs:element name="strAttrEnum" type="strAttrEnumType"/>
</xs:schema>
)xml";

TEST(XSD2, selfContainedStruct)
{
    strSelfContained str_elem;
    std::string xsd_string = serialize_xsd<strSelfContained>("strSelfContained", &str_elem);
    EXPECT_EQ(xsd_string, _xsd_str_selfContained);
}

TEST(XSD2, internExtern)
{
	strExtern str_elem;
	std::string xsd_string = serialize_xsd<strExtern>("strExtern", &str_elem);
	EXPECT_EQ( xsd_string, _xsd_str_internExtern);
}

TEST(XSD2, vectorUnnamed)
{
	strWithVector str_elem;
	std::string xsd_string = serialize_xsd<strWithVector>("strWithVector", &str_elem);
    EXPECT_EQ(xsd_string, _xsd_str_vectorUnnamed);
}

TEST(XSD2, vectorNamed)
{
    strWithVectorNamed str_elem;
    std::string xsd_string = serialize_xsd<strWithVectorNamed>("strWithVectorNamed", &str_elem);
    EXPECT_EQ(xsd_string, _xsd_str_vectorNamed);
}

TEST(XSD2, structOptional)
{
    strExternalOpt str_elem;
    std::string xsd_string = serialize_xsd<strExternalOpt>("strExternalOpt", &str_elem);
    EXPECT_EQ(xsd_string, _xsd_str_externalOpt);
}

TEST(XSD2, vectorOptional)
{
    strParameters str_elem;
    std::string xsd_string = serialize_xsd<strParameters>("strParameters", &str_elem);
    EXPECT_EQ(xsd_string, _xsd_str_vectorOptional);
}

TEST(XSD2, structOptional2)
{
    strOptional2 str_elem;
    std::string xsd_string = serialize_xsd<strOptional2>("strOptional2", &str_elem);
    EXPECT_EQ(xsd_string, _xsd_str_strOptional2);
}

TEST(XSD2, strDoubleEnum)
{
    strDoubleEnum str_elem;
    std::string xsd_string = serialize_xsd<strDoubleEnum>("strDoubleEnum", &str_elem);
    EXPECT_EQ(xsd_string, _xsd_str_strDoubleEnum);
}

TEST(XSD2, strAttrEnum)
{
    strAttrEnum str_elem;
    std::string xsd_string = serialize_xsd<strAttrEnum>("strAttrEnum", &str_elem);
    EXPECT_EQ(xsd_string, _xsd_str_strAttrEnum);
}

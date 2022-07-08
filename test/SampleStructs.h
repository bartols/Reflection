
#pragma once

#include <string>
#include <vector>
#include "optional.hpp"

struct strSimplePODElem {
	int int_value;
	std::string str_value;
	bool bool_value;
	double double_value;

	bool operator==( const strSimplePODElem& rhs ) const
	{
		return (int_value == rhs.int_value) &&
			(str_value == rhs.str_value) &&
			(bool_value == rhs.bool_value) &&
			(double_value == rhs.double_value);
	}
};

const std::string _strSimplePODElem_xml =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strSimplePODElem>
    <int_value>1</int_value>
    <str_value>2</str_value>
    <bool_value>false</bool_value>
    <double_value>3.000000</double_value>
</strSimplePODElem>
)xml";

struct strSimplePODAttr {
	int int_value;
	std::string str_value;
	bool bool_value;
	double double_value;

	bool operator==( const strSimplePODAttr& rhs ) const
	{
		return (int_value == rhs.int_value) &&
			(str_value == rhs.str_value) &&
			(bool_value == rhs.bool_value) &&
			(double_value == rhs.double_value);
	}
};

const std::string _strSimplePODAttr_xml =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strSimplePODAttr int_value="1" str_value="2" bool_value="false" double_value="3.000000"/>
)xml";

struct strSimplePODMix {
	int int_value;
	std::string str_value;
	bool bool_value;
	double double_value;

	bool operator==( const strSimplePODMix& rhs ) const
	{
		return (int_value == rhs.int_value) &&
			(str_value == rhs.str_value) &&
			(bool_value == rhs.bool_value) &&
			(double_value == rhs.double_value);
	}
};

const std::string _strSimplePODMix_xml =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strSimplePODMix int_value="1" str_value="2">
    <bool_value>false</bool_value>
    <double_value>3.000000</double_value>
</strSimplePODMix>
)xml";

struct strVectorElem {
	std::string description;
	std::vector<strSimplePODElem> vect_pod;
};

const std::string _strVectorElem_xml =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strVectorElem>
    <description>test</description>
    <vect_pod>
        <strSimplePODElem>
            <int_value>1</int_value>
            <str_value>2</str_value>
            <bool_value>false</bool_value>
            <double_value>3.000000</double_value>
        </strSimplePODElem>
        <strSimplePODElem>
            <int_value>4</int_value>
            <str_value>5</str_value>
            <bool_value>false</bool_value>
            <double_value>6.000000</double_value>
        </strSimplePODElem>
    </vect_pod>
</strVectorElem>
)xml";

struct strVectorAttr {
	std::string description;
	std::vector<strSimplePODAttr> vect_pod;
};

const std::string _strVectorAttr_xml =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strVectorAttr>
    <description>test</description>
    <vect_pod>
        <strSimplePODAttr int_value="1" str_value="2" bool_value="false" double_value="3.000000"/>
        <strSimplePODAttr int_value="4" str_value="5" bool_value="false" double_value="6.000000"/>
    </vect_pod>
</strVectorAttr>
)xml";


struct strVectorUnamedElem {
	std::string description;
	std::vector<strSimplePODElem> vect_pod;
};

const std::string _strVectorUnamedElem_xml =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strVectorUnamedElem>
    <description>test</description>
    <strSimplePODElem>
        <int_value>1</int_value>
        <str_value>2</str_value>
        <bool_value>false</bool_value>
        <double_value>3.000000</double_value>
    </strSimplePODElem>
    <strSimplePODElem>
        <int_value>4</int_value>
        <str_value>5</str_value>
        <bool_value>false</bool_value>
        <double_value>6.000000</double_value>
    </strSimplePODElem>
</strVectorUnamedElem>
)xml";


struct strVectorUnamedAttr {
	std::string description;
	std::vector<strSimplePODAttr> vect_pod;
};

const std::string _strVectorUnamedAttr_xml =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strVectorUnamedAttr>
    <description>test</description>
    <strSimplePODAttr int_value="1" str_value="2" bool_value="false" double_value="3.000000"/>
    <strSimplePODAttr int_value="4" str_value="5" bool_value="false" double_value="6.000000"/>
</strVectorUnamedAttr>
)xml";

/////////////////////////////////////////////////////////////////////////////////////////////

struct strSimpleOpt {
	std::string description;
	nonstd::optional<int> age;

	bool operator==( const strSimpleOpt& rhs ) const {
		return description == rhs.description && age == rhs.age;
	}
};

const std::string _xml_strSimpleOptWith =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strSimpleOpt description="test" age="50"/>
)xml";

const std::string _xml_strSimpleOptWithout =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strSimpleOpt description="test"/>
)xml";


struct strItem {
	std::string name;
	std::string value;

	bool operator==( const strItem& rhs ) const {
		return name == rhs.name && value == rhs.value;
	}
};

struct strElementOpt {
	std::string description;
	nonstd::optional<strItem> item;

	bool operator==( const strElementOpt& rhs ) const {
		return (description == rhs.description) &&
			(item == rhs.item);
	}
};

const std::string _xml_strElementOptWith =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strElementOpt>
    <description>test</description>
    <item name="prova" value="valore"/>
</strElementOpt>
)xml";

const std::string _xml_strElementOptWithout =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strElementOpt>
    <description>test</description>
</strElementOpt>
)xml";

struct strOptionalInOptional {
	int int_value;
	nonstd::optional<strSimpleOpt> opt_elem;

	bool operator==( const strOptionalInOptional& rhs ) const {
		return int_value == rhs.int_value &&
			opt_elem == rhs.opt_elem;
	}
};

const std::string _xml_strOptionalInOptionalWith =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strOptionalInOptional>
    <int_value>11</int_value>
    <opt_elem description="test" age="12"/>
</strOptionalInOptional>
)xml";

const std::string _xml_strOptionalInOptionalWithOut =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strOptionalInOptional>
    <int_value>11</int_value>
</strOptionalInOptional>
)xml";


// report data
struct ReportData {
	ReportData(const std::string& n, const std::string& v)
		:name(n), value(v) {}
	ReportData() = default;

	std::string name;
	std::string value;
};

// report section
struct ReportSection {
	ReportSection(const std::string& sec)
		:name(sec) {}
	ReportSection() = default;

	std::string name;
	std::vector<ReportData> datas;
	std::vector<ReportSection> sections;
};

// report
struct Report {
	Report() = default;
	int type;
	std::vector<ReportSection> sections;
};

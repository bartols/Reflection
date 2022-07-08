#include <gtest/gtest.h>

#include "SampleStructs.h"

#include "test_util.h"

struct strProposal {
	std::string str_value1;
	std::string str_value2;

	bool operator==( const strProposal& rhs ) const
	{
		return str_value1 == rhs.str_value1 &&
			str_value2 == rhs.str_value2;
	}
};

REFLECT_BEGIN( strProposal )
REFLECT_MEMBER_ATTR( str_value1 )
REFLECT_MEMBER_ATTR( str_value2 )
REFLECT_END()

struct strContract {
	int int_value1;
	int int_value2;
	std::vector<strProposal> proposals;

	bool operator==( const strContract& rhs ) const
	{
		return int_value1 == rhs.int_value1 &&
			int_value2 == rhs.int_value2 &&
			proposals == rhs.proposals;
	}
};

REFLECT_BEGIN( strContract )
REFLECT_MEMBER_ELEM( int_value1 )
REFLECT_MEMBER_ELEM( int_value2 )
REFLECT_MEMBER_ELEM( proposals )
REFLECT_END()

struct strMessage {
	int version = 1;
	std::vector<strContract> contracts;

	bool operator==(const strMessage& rhs ) const
	{
		return version == rhs.version &&
			contracts == rhs.contracts;
	}
};

REFLECT_BEGIN( strMessage )
REFLECT_MEMBER_ELEM( version )
REFLECT_MEMBER_UNAMED( contracts )
REFLECT_END()

const std::string _strMessage_xml = 
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strMessage>
    <version>1</version>
    <strContract>
        <int_value1>1</int_value1>
        <int_value2>2</int_value2>
        <proposals>
            <strProposal str_value1="uno" str_value2="due"/>
            <strProposal str_value1="tre" str_value2="quattro"/>
            <strProposal str_value1="ccc" str_value2="ddd"/>
        </proposals>
    </strContract>
    <strContract>
        <int_value1>3</int_value1>
        <int_value2>4</int_value2>
        <proposals>
            <strProposal str_value1="xxx" str_value2="yyy"/>
            <strProposal str_value1="zzz" str_value2="www"/>
            <strProposal str_value1="ccc" str_value2="ddd"/>
        </proposals>
    </strContract>
</strMessage>
)xml";

const std::string _strMessage_xml_no_proposal =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strMessage>
    <version>1</version>
    <strContract>
        <int_value1>1</int_value1>
        <int_value2>2</int_value2>
        <proposals/>
    </strContract>
    <strContract>
        <int_value1>3</int_value1>
        <int_value2>4</int_value2>
        <proposals/>
    </strContract>
</strMessage>
)xml";

struct strContractAttr {
	int int_value1;
	int int_value2;
	std::vector<strProposal> proposals;

	bool operator==( const strContractAttr& rhs ) const
	{
		return int_value1 == rhs.int_value1 &&
			int_value2 == rhs.int_value2 &&
			proposals == rhs.proposals;
	}
};

REFLECT_BEGIN( strContractAttr )
REFLECT_MEMBER_ATTR( int_value1 )
REFLECT_MEMBER_ATTR( int_value2 )
REFLECT_MEMBER_ELEM( proposals )
REFLECT_END()

struct strMessageAttr {
	int version = 1;
	std::vector<strContractAttr> contracts;

	bool operator==( const strMessageAttr& rhs ) const
	{
		return version == rhs.version &&
			contracts == rhs.contracts;
	}
};

REFLECT_BEGIN( strMessageAttr )
REFLECT_MEMBER_ELEM( version )
REFLECT_MEMBER_UNAMED( contracts )
REFLECT_END()

const std::string _strMessage_attr_xml =
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<strMessageAttr>
    <version>1</version>
    <strContractAttr int_value1="1" int_value2="2">
        <proposals>
            <strProposal str_value1="uno" str_value2="due"/>
            <strProposal str_value1="tre" str_value2="quattro"/>
            <strProposal str_value1="cinque" str_value2="sei"/>
        </proposals>
    </strContractAttr>
    <strContractAttr int_value1="3" int_value2="4">
        <proposals>
            <strProposal str_value1="xxx" str_value2="yyy"/>
            <strProposal str_value1="zzz" str_value2="www"/>
            <strProposal str_value1="ccc" str_value2="ddd"/>
            <strProposal str_value1="eee" str_value2="fff"/>
        </proposals>
    </strContractAttr>
</strMessageAttr>
)xml";

struct Customer {
	std::string name;
	std::string familyName;
};

REFLECT_BEGIN( Customer )
REFLECT_MEMBER_ATTR( name )
REFLECT_MEMBER_ATTR( familyName )
REFLECT_END()

struct Customers {
	std::vector<Customer> customers;
};

REFLECT_BEGIN( Customers )
REFLECT_MEMBER_ELEM( customers )
REFLECT_END()

struct Message
{
	std::string test;
	std::vector<Customer> customers;
};

REFLECT_BEGIN( Message )
REFLECT_MEMBER_ATTR( test )
REFLECT_MEMBER_ELEM( customers )
REFLECT_END()

const std::string _message_xml = 
R"xml(<?xml version="1.0" encoding="UTF-8"?>
<message test="prova">
    <customers>
        <Customer name="franco" familyName="rosi"/>
        <Customer name="marco" familyName="valdo"/>
    </customers>
</message>
)xml";


struct strReportSelfContained {
	int aa = 1;
	Report _report;
};

REFLECT_BEGIN_AS(strReportSelfContained, "strSelfContained")
REFLECT_MEMBER_ELEM_AS(aa, "vts:aa")
REFLECT_MEMBER_ELEM_AS(_report, "vts:Report")
REFLECT_END()

TEST(ReflectionVect, report)
{
	strReportSelfContained str_test;

	str_test._report.type = 10;
	{
		ReportSection section;
		section.name = "Section1";
		section.datas.push_back({"name", "value"});
		section.datas.push_back({"test_name_1", "test_value_1"});
		section.datas.push_back({"test_name_2", "test_value_2"});
		section.datas.push_back({"test_name_3", "test_value_3"});

		// section in section
		{
			ReportSection section2;
			section2.name = "SectionInside";
			section2.datas.push_back({"name", "value"});
			section2.datas.push_back({"test_name_a", "test_value_a"});
			section2.datas.push_back({"test_name_b", "test_value_b"});
			section2.datas.push_back({"test_name_c", "test_value_c"});

			section.sections.push_back(section2);
		}

		str_test._report.sections.push_back(section);
	}
	{
		ReportSection section;
		section.name = "Section2";
		section.datas.push_back({"name", "value"});
		section.datas.push_back({"test_name_10", "test_value_10"});
		section.datas.push_back({"test_name_20", "test_value_20"});
		section.datas.push_back({"test_name_30", "test_value_30"});

		str_test._report.sections.push_back(section);
	}

	strReportSelfContained str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strReportSelfContained>("strSelfContained", &str_test, xml_string, &str_dest);
	// todo
}

TEST( ReflectionVect, vectorInVectorSerialize )
{
	strMessage str_test;
	
	{
		strContract contract;
		contract.int_value1 = 1;
		contract.int_value2 = 2;

		{
			strProposal proposal;
			proposal.str_value1 = "uno";
			proposal.str_value2 = "due";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "tre";
			proposal.str_value2 = "quattro";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "ccc";
			proposal.str_value2 = "ddd";
			contract.proposals.push_back( proposal );
		}
		str_test.contracts.push_back( contract );
	}

	{
		strContract contract;
		contract.int_value1 = 3;
		contract.int_value2 = 4;

		{
			strProposal proposal;
			proposal.str_value1 = "xxx";
			proposal.str_value2 = "yyy";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "zzz";
			proposal.str_value2 = "www";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "ccc";
			proposal.str_value2 = "ddd";
			contract.proposals.push_back( proposal );
		}
		str_test.contracts.push_back( contract );
	}

	strMessage str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strMessage>( "strMessage", &str_test, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strMessage_xml );
	EXPECT_EQ( str_test, str_dest );
}

TEST( ReflectionVect, vectorInVectorDeserialize )
{
	strMessage elem;
	bool res = deserialize<strMessage>( _strMessage_xml, &elem );
	ASSERT_TRUE( res );
	ASSERT_EQ( elem.contracts.size(), 2 );
	ASSERT_EQ( elem.contracts[0].proposals.size(), 3 );
	ASSERT_EQ( elem.contracts[0].proposals[0].str_value1, "uno" );
	ASSERT_EQ( elem.contracts[0].proposals[0].str_value2, "due" );
	ASSERT_EQ( elem.contracts[0].proposals[1].str_value1, "tre" );
	ASSERT_EQ( elem.contracts[0].proposals[1].str_value2, "quattro" );
	ASSERT_EQ( elem.contracts[0].proposals[2].str_value1, "ccc" );
	ASSERT_EQ( elem.contracts[0].proposals[2].str_value2, "ddd" );
	ASSERT_EQ( elem.contracts[1].proposals.size(), 3 );
	ASSERT_EQ( elem.contracts[1].proposals[0].str_value1, "xxx" );
	ASSERT_EQ( elem.contracts[1].proposals[0].str_value2, "yyy" );
}

TEST( ReflectionVect, vectorInVectorNoProposalDeserialize )
{
	strMessage elem;
	bool res = deserialize<strMessage>( _strMessage_xml_no_proposal, &elem );
	ASSERT_TRUE( res );
	ASSERT_EQ( elem.contracts.size(), 2 );
	ASSERT_EQ( elem.contracts[0].proposals.size(), 0 );
	ASSERT_EQ( elem.contracts[1].proposals.size(), 0 );
}

TEST( ReflectionVect, vectorInVectorAttr )
{
	strMessageAttr str_test;

	{
		strContractAttr contract;
		contract.int_value1 = 1;
		contract.int_value2 = 2;

		{
			strProposal proposal;
			proposal.str_value1 = "uno";
			proposal.str_value2 = "due";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "tre";
			proposal.str_value2 = "quattro";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "cinque";
			proposal.str_value2 = "sei";
			contract.proposals.push_back( proposal );
		}
		str_test.contracts.push_back( contract );
	}

	{
		strContractAttr contract;
		contract.int_value1 = 3;
		contract.int_value2 = 4;

		{
			strProposal proposal;
			proposal.str_value1 = "xxx";
			proposal.str_value2 = "yyy";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "zzz";
			proposal.str_value2 = "www";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "ccc";
			proposal.str_value2 = "ddd";
			contract.proposals.push_back( proposal );
		}
		{
			strProposal proposal;
			proposal.str_value1 = "eee";
			proposal.str_value2 = "fff";
			contract.proposals.push_back( proposal );
		}
		str_test.contracts.push_back( contract );
	}

	strMessageAttr str_dest;
	std::string xml_string;
	bool res = fromStruct2XML2Strct<strMessageAttr>( "strMessageAttr", &str_test, xml_string, &str_dest );

	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _strMessage_attr_xml );
	EXPECT_EQ( str_test, str_dest );
}

TEST( ReflectionVect, direct )
{
	Message msg;
	msg.test = "prova";
	//msg.cust.customers.push_back( { "franco","rosi" } );
	//msg.cust.customers.push_back( { "marco","valdo" } );
	msg.customers.push_back( { "franco","rosi" } );
	msg.customers.push_back( { "marco","valdo" } );

	Message msg2;

	std::string xml_string;
	bool res = fromStruct2XML2Strct<Message>( "message", &msg, xml_string, &msg2 );
	ASSERT_TRUE( res );
	EXPECT_EQ( xml_string, _message_xml );
}

#include "SampleStructs.h"

#include "Reflection.h"

REFLECT_BEGIN( strSimplePODElem )
REFLECT_MEMBER_ELEM( int_value )
REFLECT_MEMBER_ELEM( str_value )
REFLECT_MEMBER_ELEM( bool_value )
REFLECT_MEMBER_ELEM( double_value )
REFLECT_END()

REFLECT_BEGIN( strSimplePODAttr )
REFLECT_MEMBER_ATTR( int_value )
REFLECT_MEMBER_ATTR( str_value )
REFLECT_MEMBER_ATTR( bool_value )
REFLECT_MEMBER_ATTR( double_value )
REFLECT_END()

REFLECT_BEGIN( strSimplePODMix )
REFLECT_MEMBER_ATTR( int_value )
REFLECT_MEMBER_ATTR( str_value )
REFLECT_MEMBER_ELEM( bool_value )
REFLECT_MEMBER_ELEM( double_value )
REFLECT_END()

REFLECT_BEGIN( strVectorElem )
REFLECT_MEMBER_ELEM( description )
REFLECT_MEMBER_ELEM( vect_pod )
REFLECT_END()

REFLECT_BEGIN( strVectorAttr )
REFLECT_MEMBER_ELEM( description )
REFLECT_MEMBER_ELEM( vect_pod )
REFLECT_END()

REFLECT_BEGIN( strVectorUnamedElem )
REFLECT_MEMBER_ELEM( description )
REFLECT_MEMBER_UNAMED_AS( vect_pod, "strSimplePODElem" )
REFLECT_END()

REFLECT_BEGIN( strVectorUnamedAttr )
REFLECT_MEMBER_ELEM( description )
REFLECT_MEMBER_UNAMED_AS( vect_pod, "strSimplePODAttr" )
REFLECT_END()

REFLECT_BEGIN( strSimpleOpt )
REFLECT_MEMBER_ATTR( description )
REFLECT_MEMBER_ATTR( age )
REFLECT_END()

REFLECT_BEGIN( strItem )
REFLECT_MEMBER_ATTR( name )
REFLECT_MEMBER_ATTR( value )
REFLECT_END()

REFLECT_BEGIN( strElementOpt )
REFLECT_MEMBER_ELEM( description )
REFLECT_MEMBER_ELEM( item )
REFLECT_END()

REFLECT_BEGIN( strOptionalInOptional )
REFLECT_MEMBER_ELEM( int_value )
REFLECT_MEMBER_ELEM( opt_elem )
REFLECT_END()

// report data
REFLECT_BEGIN_AS(ReportData, "vts:ReportData")
REFLECT_MEMBER_ATTR_AS(name, "name")
REFLECT_MEMBER_ATTR_AS(value, "value")
REFLECT_END()

// report section
REFLECT_BEGIN_AS(ReportSection, "vts:ReportSection")
REFLECT_MEMBER_ATTR_AS(name, "name")
REFLECT_MEMBER_UNAMED(datas)
REFLECT_MEMBER_UNAMED(sections)
REFLECT_END()

// report 
REFLECT_BEGIN_AS(Report, "vts:Report")
REFLECT_MEMBER_ATTR_AS(type, "type")
REFLECT_MEMBER_UNAMED(sections)
REFLECT_END()

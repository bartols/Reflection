#include "Reflection.h"

#ifdef CPP98
#include <sstream>
#include <string.h>
#include <exception>

namespace reflect {
    template< typename T >
    std::string to_string( const T& a )
    {
        std::ostringstream str;
        str << a;
        return str.str();
    }

    template< typename T >
    T from_string( const std::string & str )
    {
        std::istringstream convert(str);
        
        T res;
        if( !(convert >> res) )
        {
            throw std::invalid_argument("invalid string to convert");
        }

        return res;
    }
}

#define _stricmp strcasecmp

#endif

namespace reflect {

    void Visitor::set_error_msg( const char* format, ... )
    {
        char buffer[256];

        va_list arglist;
        va_start( arglist, format );

        if( vsnprintf( buffer, sizeof( buffer ) - 1, format, arglist ) > 0 )
        {
            _last_error_msg = buffer;
        }

        va_end( arglist );
    }

    //--------------------------------------------------------
    // A type descriptor for int
    //--------------------------------------------------------
    struct TypeDescriptor_Int : TypeDescriptor 
    {
        TypeDescriptor_Int() 
            :TypeDescriptor( "int", sizeof(int), node_field ) {}
        
        // to string
        std::string to_string( const void* field ) const override
        {
#ifdef CPP98
            return reflect::to_string<int>( *reinterpret_cast<const int*>(field) );
#else
            return std::to_string( *reinterpret_cast<const int*>(field) );
#endif
        }

        // from string
        bool from_string( const std::string& str, const void* field ) override
        {
            try {
#ifdef CPP98                
                *(reinterpret_cast<int*>(const_cast<void*>( field ))) = reflect::from_string<int>( str );
#else
                *(reinterpret_cast<int*>(const_cast<void*>( field ))) = std::stoi( str );
#endif
            }
            catch( ... )
            {
                return false;
            }

            return true;
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<int>() 
    {
        static TypeDescriptor_Int typeDesc;
        return &typeDesc;
    }

    //--------------------------------------------------------
// A type descriptor for int
//--------------------------------------------------------
    struct TypeDescriptor_Int64 : TypeDescriptor
    {
        TypeDescriptor_Int64()
            :TypeDescriptor("int64", sizeof(int64_t), node_field) {}

        // to string
        std::string to_string(const void* field) const override
        {
#ifdef CPP98
            return reflect::to_string<int>(*reinterpret_cast<const int*>(field));
#else
            return std::to_string(*reinterpret_cast<const int64_t*>(field));
#endif
        }

        // from string
        bool from_string(const std::string& str, const void* field) override
        {
            try {
#ifdef CPP98                
                * (reinterpret_cast<int64_t*>(const_cast<void*>(field))) = reflect::from_string<int64_t>(str);
#else
                * (reinterpret_cast<int64_t*>(const_cast<void*>(field))) = std::stoll(str);
#endif
            }
            catch (...)
            {
                return false;
            }

            return true;
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<int64_t>()
    {
        static TypeDescriptor_Int64 typeDesc;
        return &typeDesc;
    }

    //--------------------------------------------------------
    // A type descriptor for bool
    //--------------------------------------------------------
    struct TypeDescriptor_Bool : TypeDescriptor
    {
        TypeDescriptor_Bool() 
            :TypeDescriptor( "bool", sizeof( bool ), node_field ) {}

        // to string
        std::string to_string( const void* field ) const override
        {
            return (*reinterpret_cast<const bool*>(field)) ? "true" : "false";
        }

        // from string
        bool from_string( const std::string& str, const void* field ) override
        {
            try 
            {
                if( _stricmp( str.c_str(), "true" ) == 0 )
                {
                    *(reinterpret_cast<bool*>(const_cast<void*>(field))) = true;
                }
                else if( _stricmp( str.c_str(), "false" ) == 0 )
                {
                    *(reinterpret_cast<bool*>(const_cast<void*>(field))) = false;
                }
                else
                {
#ifdef CPP98                
                    *(reinterpret_cast<bool*>(const_cast<void*>(field))) = reflect::from_string<bool>( str );
#else
                    *(reinterpret_cast<bool*>(const_cast<void*>(field))) = std::stoi( str ) == 0 ? false : true;
#endif
                }
            }
            catch( ... )
            {
                return false;
            }

            return true;
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<bool>()
    {
        static TypeDescriptor_Bool typeDesc;
        return &typeDesc;
    }

    //--------------------------------------------------------
    // A type descriptor for std::string
    //--------------------------------------------------------
    struct TypeDescriptor_StdString : TypeDescriptor 
    {
        TypeDescriptor_StdString() 
            :TypeDescriptor( "std::string", sizeof(std::string), node_field ) {}

        // to string
        std::string to_string( const void* field ) const override
        {
            return (*reinterpret_cast<const std::string*>(field));
        }

        // from string
        bool from_string( const std::string& str, const void* field ) override
        {
            *(reinterpret_cast<std::string*>(const_cast<void *>(field))) = str;
            return true;
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<std::string>() 
    {
        static TypeDescriptor_StdString typeDesc;
        return &typeDesc;
    }

    //--------------------------------------------------------
    // A type descriptor for double
    //--------------------------------------------------------

    struct TypeDescriptor_Double : TypeDescriptor 
    {
        TypeDescriptor_Double() 
            :TypeDescriptor( "double", sizeof(double), node_field ) {}
        
        // to string
        std::string to_string( const void* field ) const override
        {
#ifdef CPP98
            return reflect::to_string<double>( *reinterpret_cast<const double*>(field) );
#else
            return std::to_string( *reinterpret_cast<const double*>(field) );
#endif
        }

        // from string
        bool from_string( const std::string& str, const void* field ) override
        {
            try {
#ifdef CPP98                
                *(reinterpret_cast<double*>(const_cast<void*>( field ))) = reflect::from_string<double>( str );
#else
                *(reinterpret_cast<double*>(const_cast<void *>(field))) = std::stod( str );
#endif
            }
            catch( ... )
            {
                return false;
            }

            return true;
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<double>() 
    {
        static TypeDescriptor_Double typeDesc;
        return &typeDesc;
    }
/*
    //--------------------------------------------------------
    // A type descriptor for Blob
    //--------------------------------------------------------

    struct TypeDescriptor_Blob : TypeDescriptor
    {
        TypeDescriptor_Blob()
            :TypeDescriptor( "blob", sizeof( Blob ), node_field ) {}

        // to string
        std::string to_string( const void* field ) const override
        {
            return reinterpret_cast<const Blob*>(field)->encode_to_string();
        }

        // from string
        bool from_string( const std::string& str, const void* field ) override
        {
            return reinterpret_cast<Blob*>(const_cast<void*>(field))->decode_from_string( str );
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<Blob>()
    {
        static TypeDescriptor_Blob typeDesc;
        return &typeDesc;
    }

    //--------------------------------------------------------
    // A type descriptor for Date
    //--------------------------------------------------------

    struct TypeDescriptor_Date : TypeDescriptor
    {
        TypeDescriptor_Date()
            :TypeDescriptor("date", sizeof(Date), node_field) {}

        // to string
        std::string to_string(const void* field) const override
        {
            return reinterpret_cast<const Date*>(field)->to_string();
        }

        // from string
        bool from_string(const std::string& str, const void* field) override
        {
            return reinterpret_cast<Date*>(const_cast<void*>(field))->from_string(str);
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<Date>()
    {
        static TypeDescriptor_Date typeDesc;
        return &typeDesc;
    }

    //--------------------------------------------------------
    // A type descriptor for Time
    //--------------------------------------------------------

    struct TypeDescriptor_Time : TypeDescriptor
    {
        TypeDescriptor_Time()
            :TypeDescriptor("time", sizeof(Time), node_field) {}

        // to string
        std::string to_string(const void* field) const override
        {
            return reinterpret_cast<const Time*>(field)->to_string();
        }

        // from string
        bool from_string(const std::string& str, const void* field) override
        {
            return reinterpret_cast<Time*>(const_cast<void*>(field))->from_string(str);
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<Time>()
    {
        static TypeDescriptor_Time typeDesc;
        return &typeDesc;
    }

    //--------------------------------------------------------
    // A type descriptor for DateTime
    //--------------------------------------------------------

    struct TypeDescriptor_DateTime : TypeDescriptor
    {
        TypeDescriptor_DateTime()
            :TypeDescriptor("datetime", sizeof(DateTime), node_field) {}

        // to string
        std::string to_string(const void* field) const override
        {
            return reinterpret_cast<const DateTime*>(field)->to_string();
        }

        // from string
        bool from_string(const std::string& str, const void* field) override
        {
            return reinterpret_cast<DateTime*>(const_cast<void*>(field))->from_string(str);
        }
    };

    template <>
    TypeDescriptor* getPrimitiveDescriptor<DateTime>()
    {
        static TypeDescriptor_DateTime typeDesc;
        return &typeDesc;
    }
    */
} // namespace reflect

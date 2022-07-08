#pragma once

#ifndef _REFLECTION_H_
#define _REFLECTION_H_

#include <vector>
#include <string>
#include <string.h>
#include <stdarg.h>
#include "optional.hpp"

namespace reflect {

	using aliases_t = std::vector<const char*>;

	struct TypeDescriptor;

	enum eFieldType {
		attribute,
		element,
		direct
	};

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning(disable:4251)
#endif
	// reflection types visitor
	class Visitor
	{
	public:
		virtual ~Visitor() {}
		virtual bool visit_struct_start( const aliases_t& field_names, TypeDescriptor* tdescr, eFieldType type, bool optional ) = 0;
		virtual bool visit_struct_end( const aliases_t& field_names, TypeDescriptor* tdescr, eFieldType type, bool optional ) = 0;
		virtual bool visit_vector_start( const aliases_t& field_names, void* field, TypeDescriptor* tdescr, eFieldType type, bool optional ) = 0;
		virtual bool visit_vector_end( const aliases_t& field_names, void* field, TypeDescriptor* tdescr, eFieldType type, bool optional ) = 0;
		virtual bool check_optional_field( const aliases_t& field_names, void* field, TypeDescriptor* tdescr, eFieldType type ) = 0;
		virtual bool visit_field( const aliases_t& field_names, void* field, TypeDescriptor* tdescr, eFieldType type, bool optional ) = 0;

		void set_error_msg( const char* format, ... );
		const std::string& get_error_msg() const		{ return _last_error_msg; }
	private:
		std::string _last_error_msg;
	};
#ifdef _MSC_VER
	#pragma warning( pop )
#endif

	//--------------------------------------------------------
	// Base class of all type descriptors
	//--------------------------------------------------------
	struct TypeDescriptor
	{
		enum eNodeType {
			node_struct,
			node_vector,
			node_field,
			node_enum
		};

		const char* name;
		size_t size;
		eNodeType node_type;

		TypeDescriptor( const char* name, size_t size, eNodeType type )
			: name( name ), size( size ), node_type( type ) {}

		virtual ~TypeDescriptor() {}

		virtual std::string getFullName() const { return name; }

		// accept visitor
		virtual bool accept( Visitor& v, void* obj, const aliases_t& field_names = {}, eFieldType type = element, bool optional = false )
		{
			return v.visit_field( field_names, obj, this, type, optional );
		}

		// to string
		virtual std::string to_string( const void* field ) const = 0;
		// from string
		virtual bool from_string( const std::string& str, const void* field ) = 0;
		
		// is optional
		virtual bool is_optional() const	{ return false; }
	};

	//--------------------------------------------------------
	// Finding type descriptors
	//--------------------------------------------------------

	// Declare the function template that handles primitive types such as int, std::string, etc.:
	template <typename T>
	TypeDescriptor* getPrimitiveDescriptor();

	// int specialization declaration
	template <>
	TypeDescriptor* getPrimitiveDescriptor<int>();

	// int64 specialization declaration
	template <>
	TypeDescriptor* getPrimitiveDescriptor<int64_t>();

	// bool
	template <>
	TypeDescriptor* getPrimitiveDescriptor<bool>();

	// string
	template <>
	TypeDescriptor* getPrimitiveDescriptor<std::string>();

	// double
	template <>
	TypeDescriptor* getPrimitiveDescriptor<double>();

	/*
	// blob
	template <>
	TypeDescriptor* getPrimitiveDescriptor<Blob>();

	// date
	template <>
	TypeDescriptor* getPrimitiveDescriptor<Date>();

	// time
	template <>
	TypeDescriptor* getPrimitiveDescriptor<Time>();

	// Date time
	template <>
	TypeDescriptor* getPrimitiveDescriptor<DateTime>();
	*/

	// This is the primary class template for finding all TypeDescriptors:
	template <typename T>
	struct TypeResolver {
		static TypeDescriptor* get() {
			return getPrimitiveDescriptor<T>();
		}
	};

	//--------------------------------------------------------
	// Type descriptors for user-defined structs/classes
	//--------------------------------------------------------
	struct TypeDescriptor_Struct : TypeDescriptor 
	{
		// struct member
		struct Member {
			aliases_t field_names;
			eFieldType field_type;
			size_t offset;
			TypeDescriptor* type_descr;
			bool optional;
		};

		// members
		std::vector<Member> members;

		TypeDescriptor_Struct()
			: TypeDescriptor( nullptr, 0, node_struct )
		{
		}

		void initialize( void (*init)(TypeDescriptor_Struct*) )
		{
			init( this );
		}

		// accept visitor
		bool accept( Visitor& v, void* obj, const aliases_t& field_names, eFieldType type, bool optional ) override
		{
			if( !v.visit_struct_start( field_names, this, type, optional ) )
				return false;

			char* curr = reinterpret_cast<char*>(obj);
			std::vector<Member>::const_iterator it_end = members.end();
			for( std::vector<Member>::const_iterator it = members.begin(); it != it_end; ++it )
			{
				const Member& member = (*it);
				if( !member.type_descr->accept( v,
												curr + member.offset, 
												member.field_names, 
												member.field_type,
												member.optional ) )
					return false;
			}

			return v.visit_struct_end( field_names, this, type, optional );
		}

		// to string for struct return the name
		std::string to_string( const void* field ) const override
		{
			return getFullName();
		}

		// from string
		bool from_string( const std::string& str, const void* field ) override
		{
			// only for pod value
			return true;
		}
	};

	// generic init reflection for struct
	template<typename T>
	void initReflection( TypeDescriptor_Struct* );

	//--------------------------------------------------------
	// Type descriptors for enum
	//--------------------------------------------------------
	struct TypeDescriptor_Enum : TypeDescriptor
	{
		// enum member
		struct EMember {
			const char* str;
			size_t len;
			int val;
		};

		std::vector<EMember> members;

		TypeDescriptor_Enum()
			: TypeDescriptor( nullptr, 0, node_enum )
		{
		}

		void initialize( void (*init)(TypeDescriptor_Enum*) )
		{
			init( this );
		}

		// to string for struct return the name
		std::string to_string( const void* field ) const override
		{
			int enum_value = *reinterpret_cast<const int*>(field);
			for( const auto& memb : members )
			{
				if( enum_value == memb.val )
				{
					return memb.str;
				}
			}
			return "???";
		}

		const char * to_string(int enum_value) const
		{
			for (const auto& memb : members)
			{
				if (enum_value == memb.val)
				{
					return memb.str;
				}
			}
			return "???";
		}

		// from string
		bool from_string( const std::string& str, const void* field ) override
		{
			for( const auto& memb : members )
			{
				if( strncmp( str.c_str(), memb.str, memb.len ) == 0 )
				{
					*(reinterpret_cast<int*>(const_cast<void*>(field))) = memb.val;
					return true;
				}
			}
			return false;
		}

		int from_string(const char * str)
		{
			for (const auto& memb : members)
			{
				if (strncmp(str, memb.str, memb.len) == 0)
				{
					return memb.val;
				}
			}
			// better an exception
			return 0;
		}
	};

	template<typename T>
	const char * toString(T value)
	{
		reflect::TypeDescriptor_Enum* td = reinterpret_cast<reflect::TypeDescriptor_Enum*>(getPrimitiveDescriptor<T>());
		return td->to_string(static_cast<int>(value));
	}

	template<typename T>
	T fromString(const char * value)
	{
		reflect::TypeDescriptor_Enum* td = reinterpret_cast<reflect::TypeDescriptor_Enum*>(getPrimitiveDescriptor<T>());
		return static_cast<T>(td->from_string(value));
	}

	// generic init reflection for enum
	template<typename T>
	void initReflection( TypeDescriptor_Enum* );

#define REFLECT_BEGIN_AS(type, as_name)                                      \
	namespace reflect {                                                      \
	template<typename T>                                                     \
	TypeDescriptor* selectResolver( T type::* ) {                            \
		return TypeResolver<T>::get();                                       \
	}                                                                        \
	template<> void initReflection<type>( TypeDescriptor_Struct* typeDesc ); \
	template<> TypeDescriptor* getPrimitiveDescriptor<type>()                \
	{                                                                        \
		static TypeDescriptor_Struct typeDesc;                               \
		static bool initialized = false;                                     \
		if( !initialized )                                                   \
		{                                                                    \
			initialized = true;                                              \
			typeDesc.initialize( initReflection<type> );                     \
		}                                                                    \
		return &typeDesc;                                                    \
	}                                                                        \
	template<>                                                               \
    void initReflection<type>(TypeDescriptor_Struct* typeDesc) {             \
        typedef type T;                                                      \
        typeDesc->name = as_name;                                            \
        typeDesc->size = sizeof(type);

#define __REFLECT_MEMBER_AS( fld_type, field, name, ... )                    \
		{                                                                    \
			TypeDescriptor_Struct::Member memb;                              \
			memb.field_names.push_back( name );                              \
			memb.field_names.insert( memb.field_names.end(), { __VA_ARGS__ } ); \
			memb.offset = offsetof( T, field );                              \
			memb.type_descr = selectResolver(&T::field);                     \
			memb.field_type = fld_type;                                      \
			memb.optional = memb.type_descr->is_optional();                  \
			typeDesc->members.push_back( memb );                             \
		}
#define __REFLECT_MEMBER( fld_type, field )                                  \
		{                                                                    \
			TypeDescriptor_Struct::Member memb;                              \
			memb.field_names.push_back( #field );                            \
			memb.offset = offsetof( T, field );                              \
			memb.type_descr = selectResolver(&T::field);                     \
			memb.field_type = fld_type;                                      \
			memb.optional = memb.type_descr->is_optional();                  \
			typeDesc->members.push_back( memb );                             \
		}

#define REFLECT_BEGIN( type )						REFLECT_BEGIN_AS( type, #type )	

#define REFLECT_MEMBER_ELEM( type )					__REFLECT_MEMBER( element, type )
#define REFLECT_MEMBER_ELEM_AS( type, name, ... )	__REFLECT_MEMBER_AS( element, type, name, __VA_ARGS__ )

#define REFLECT_MEMBER_ATTR( type )					__REFLECT_MEMBER( attribute, type )
#define REFLECT_MEMBER_ATTR_AS( type, name, ... )	__REFLECT_MEMBER_AS( attribute, type, name, __VA_ARGS__ )

#define REFLECT_MEMBER_UNAMED( type )				__REFLECT_MEMBER( direct, type )
#define REFLECT_MEMBER_UNAMED_AS( type, name, ... )	__REFLECT_MEMBER_AS( direct, type, name, __VA_ARGS__ )

#define REFLECT_END()                                                        \
		}                                                                    \
    }

#define __REFLECT_ENUM_BEGIN( type, as_name )                                \
	namespace reflect {                                                      \
	template<> void initReflection<type>( TypeDescriptor_Enum* typeDesc );   \
	template<> TypeDescriptor* getPrimitiveDescriptor<type>()        \
	{                                                                        \
		static TypeDescriptor_Enum typeDesc;                                 \
		static bool initialized = false;                                     \
		if( !initialized )                                                   \
		{                                                                    \
			initialized = true;                                              \
			typeDesc.initialize( initReflection<type> );                     \
		}                                                                    \
		return &typeDesc;                                                    \
	}                                                                        \
	template<>                                                               \
	void initReflection<type>( TypeDescriptor_Enum* typeDesc ) {             \
		typedef type T;                                                      \
		typeDesc->name = as_name;											 \
		typeDesc->size = sizeof(type);

#define __REFLECT_ENUM_MEMBER( field, name )                                 \
		{                                                                    \
			TypeDescriptor_Enum::EMember memb;                               \
			memb.str = name;                                                 \
			memb.len = sizeof( name ) - 1;                                   \
			memb.val = (int)field;                                           \
			typeDesc->members.push_back( memb );                             \
		}

#define REFLECT_ENUM_BEGIN( type )					__REFLECT_ENUM_BEGIN( type, #type )
#define REFLECT_ENUM_BEGIN_AS( type, name )			__REFLECT_ENUM_BEGIN( type, name )

#define REFLECT_ENUM_MEMBER( type )					__REFLECT_ENUM_MEMBER( type, #type )
#define REFLECT_ENUM_MEMBER_AS( type, name )		__REFLECT_ENUM_MEMBER( type, name )

#define REFLECT_ENUM_END()                                                   \
		}                                                                    \
	}
#define REFLECT_EXPORT( decoration, type )	template <> decoration reflect::TypeDescriptor* reflect::getPrimitiveDescriptor<type>();

	//--------------------------------------------------------
	// Type descriptors for std::vector
	//--------------------------------------------------------
	struct TypeDescriptor_StdVector : TypeDescriptor
	{
		TypeDescriptor_StdVector()
			: TypeDescriptor( "std::vector<>", 0, node_vector ),
			itemType( nullptr ), resize( nullptr ), count( nullptr ), 
			at( nullptr ), append( nullptr ) {}

		template <typename ItemType>
		void intialize( ItemType* )
		{
			size = sizeof( std::vector<ItemType> );
			itemType = TypeResolver<ItemType>::get();
			resize = &resizeTemplated<ItemType>;
			count = &countTemplated<ItemType>;
			at = &atTemplated<ItemType>;
			append = &appendTemplated<ItemType>;
		}

		virtual std::string getFullName() const override
		{
			return std::string( "std::vector<" ) + itemType->getFullName() + ">";
		}

		// accept visitor
		bool accept( Visitor& v, void* obj, const aliases_t& field_names, eFieldType type, bool optional ) override
		{
			if( !v.visit_vector_start( field_names, obj, this, type, optional ) )
				return false;

			aliases_t tmp_field_names;
			if(itemType->node_type == node_struct)
			{
				tmp_field_names.push_back( itemType->name );
			}
			else
			{
				tmp_field_names = field_names;
			}

			size_t numItems = count( itemType, obj );
			for( size_t index = 0; index < numItems; ++index )
			{
				// element of vector can be only element and no optional (the vector itself could be optional)
				if( !itemType->accept( v, at( itemType, obj, index ), tmp_field_names, eFieldType::element, false ) )
					return false;
			}

			return v.visit_vector_end( field_names, obj, this, type, optional );
		}

		// to string for struct return the name
		std::string to_string( const void* field ) const override
		{
			return getFullName();
		}

		// from string
		bool from_string( const std::string& str, const void* field ) override
		{
			// only for pod value
			return false;
		}

		// vector method
		void (*resize)(const TypeDescriptor* type, void*, size_t);
		size_t( *count )(const TypeDescriptor* type, const void*);
		void* (*at)(const TypeDescriptor* type, const void*, size_t);
		void(*append)(const TypeDescriptor* type, void*);

		TypeDescriptor* itemType;

	private:
		// assign function pointer

		// resize
		template<typename ItemType>
		static void resizeTemplated( const TypeDescriptor* type, void* vecptr, size_t size )
		{
			(*reinterpret_cast<std::vector<ItemType>*>(vecptr)).resize( size );
		}

		// count
		template<typename ItemType>
		static size_t countTemplated( const TypeDescriptor* type, const void* vecptr )
		{
			return (*reinterpret_cast<const std::vector<ItemType>*>(vecptr)).size();
		}

		// at
		template<typename ItemType>
		static void * atTemplated( const TypeDescriptor* type, const void* vecptr, size_t pos )
		{
			return &((*(std::vector<ItemType>*)vecptr).at( pos ));
		}

		// append
		template<typename ItemType>
		static void appendTemplated( const TypeDescriptor* type, void* vecptr )
		{
			(*reinterpret_cast<std::vector<ItemType>*>(vecptr)).push_back( ItemType() );
		}
	};

	// Partially specialize TypeResolver<> for std::vectors:
	template <typename T>
	class TypeResolver< std::vector<T> > {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_StdVector typeDesc;
			static bool initialized = false;
			if( !initialized )
			{
				initialized = true;
				typeDesc.intialize( (T*) nullptr );
			}
			
			return &typeDesc;
		}
	};

	//--------------------------------------------------------
	// Type descriptors for nonstd::optional
	//--------------------------------------------------------
	struct TypeDescriptor_StdOptional : TypeDescriptor
	{
		template <typename ItemType>
		TypeDescriptor_StdOptional( ItemType* )
			: TypeDescriptor( "std::optional<>", sizeof( nonstd::optional<ItemType> ), node_field ),
			itemType( TypeResolver<ItemType>::get() )
		{
			has_value = &hasvalueTemplated<ItemType>;
			value = &valueTemplated<ItemType>;
			set_value= &setvalueTemplated<ItemType>;
			// type is underlinetype
			node_type = itemType->node_type;
		}

		virtual std::string getFullName() const override
		{
			return std::string( "std::optional<" ) + itemType->getFullName() + ">";
		}

		// accept visitor
		bool accept( Visitor& v, void* obj, const aliases_t& field_names, eFieldType type, bool optional ) override
		{
			if( v.check_optional_field( field_names, obj, this, type ) )
			{
				try {
					// accept visitor
					return itemType->accept( v, value( itemType, obj ), field_names, type, true );
				}
				catch( const nonstd::bad_optional_access & e )
				{
					v.set_error_msg( "Exception: %s", e.what() );
					return false;
				}
			}
			return true;
		}

		// to string use underlying type
		std::string to_string( const void* field ) const override
		{
			// only for pod value
			return itemType->to_string( field );
		}

		// from string
		bool from_string( const std::string& str, const void* field ) override
		{
			// only for pod value
			return itemType->from_string( str, field );
		}

		// is optional
		bool is_optional() const override		{ return true; }


		TypeDescriptor* itemType;

		// optional method
		bool (*has_value)(const TypeDescriptor* type, const void*);
		void*(*value)(const TypeDescriptor* type, void*);
		void(*set_value)(const TypeDescriptor* type, void*);
	private:
		// assign function pointer

		// has value
		template<typename ItemType>
		static bool hasvalueTemplated( const TypeDescriptor* type, const void* optptr )
		{
			return (*reinterpret_cast<const nonstd::optional<ItemType>*>(optptr)).has_value();
		}

		// value
		template<typename ItemType>
		static void * valueTemplated( const TypeDescriptor* type, void* optptr )
		{
			return &((*(nonstd::optional<ItemType>*)optptr).value());
		}

		// set value
		template<typename ItemType>
		static void setvalueTemplated( const TypeDescriptor* type, void* optptr )
		{
			(*(nonstd::optional<ItemType>*)optptr).operator=( ItemType() );
		}
	};

	// Partially specialize TypeResolver<> for std::optional:
	template <typename T>
	class TypeResolver< nonstd::optional<T> > {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_StdOptional typeDesc( (T*) nullptr );
			return &typeDesc;
		}
	};

} // namespace reflect

#endif // _REFLECTION_H_

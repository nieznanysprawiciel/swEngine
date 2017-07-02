#pragma once
/**
@file BasicTypesWrappers.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//				IntPropertyWrapper
//====================================================================================//

/**@brief Property typy in.*/
public ref class IntPropertyWrapper : PropertyWrapper
{
public:
	IntPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyInt, prop, prop.get_name().to_string().c_str() )
	{}


	property int		Value
	{
		int			get ();
		void		set	( int value );
	}

};


//====================================================================================//
//				UIntPropertyWrapper
//====================================================================================//


/**@brief Property typy unsinged int.*/
public ref class UIntPropertyWrapper : PropertyWrapper
{
public:
	UIntPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyUInt, prop, prop.get_name().to_string().c_str() )
	{}


	property uint32		Value
	{
		uint32		get ();
		void		set	( uint32 value );
	}

};


//====================================================================================//
//				BoolPropertyWrapper
//====================================================================================//



/**@brief Property typu bool.*/
public ref class BoolPropertyWrapper : PropertyWrapper
{
public:
	BoolPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyBool, prop, prop.get_name().to_string().c_str() )
	{}

	property bool		Value
	{
		bool		get ();
		void		set ( bool value );
	}

};

//====================================================================================//
//				FloatPropertyWrapper
//====================================================================================//


/**@brief Property typu float.*/
public ref class FloatPropertyWrapper : PropertyWrapper
{
public:
	FloatPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyFloat, prop, prop.get_name().to_string().c_str() )
	{}

	property float		Value
	{
		float		get ();
		void		set ( float value );
	}

};


//====================================================================================//
//				DoublePropertyWrapper
//====================================================================================//


/**@brief Property typu double.*/
public ref class DoublePropertyWrapper : PropertyWrapper
{
public:
	DoublePropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyDouble, prop, prop.get_name().to_string().c_str() )
	{}

public:

	property double		Value
	{
		double		get	();
		void		set	( double value );
	}

};



}
}	// sw

//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Reflection/BsIReflectable.h"

namespace bs
{
	namespace impl
	{
		/** @addtogroup Implementation
		 *  @{
		 */

		/** Union of possible primitive types a settings value field can take on. */
		union SettingsPrimitiveValue
		{
			SettingsPrimitiveValue() = default;

			SettingsPrimitiveValue(float floatVal)
				:floatVal(floatVal)
			{ }

			SettingsPrimitiveValue(INT32 intVal)
				:intVal(intVal)
			{ }

			SettingsPrimitiveValue(bool boolVal)
				:boolVal(boolVal)
			{ }

			float floatVal;
			INT32 intVal;
			bool boolVal;
		};

		/** Types of possible values stored in the settings. */
		enum class SettingsValueType
		{
			Float, Int, Bool, String, Object
		};

		/** Information about where to find a value for a key. */
		struct SettingsKeyInfo
		{
			SettingsKeyInfo() = default;
			SettingsKeyInfo(UINT32 index, SettingsValueType type)
				: index(index), type(type)
			{ }

			/** Index of the value in the relevant value array. */
			UINT32 index;

			/** Type of the value, determining which array is the value stored in. */
			SettingsValueType type;
		};

		/**	Contains an object and corresponding key in the settings list. */
		struct SettingsObjectValue : IReflectable
		{
			SettingsObjectValue() = default;
			SettingsObjectValue(const String& key, const SPtr<IReflectable>& value)
				: key(key), value(value)
			{ }

			/** Key used to look up the value. */
			String key;

			/** Stored value. */
			SPtr<IReflectable> value;

			/************************************************************************/
			/* 								RTTI		                     		*/
			/************************************************************************/
		public:
			friend class SettingsObjectValueRTTI;
			static RTTITypeBase* getRTTIStatic();
			RTTITypeBase* getRTTI() const override;
		};

		/** Information about a value attached to a key. */
		template<class T>
		struct TSettingsValue
		{
			TSettingsValue() = default;
			TSettingsValue(const String& key, const T& value)
				: key(key), value(value)
			{ }

			/** Key used to look up the value. */
			String key;

			/** Stored value. */
			T value;
		};

		/** @} */
	}

	/** @addtogroup Settings
	 *  @{
	 */

	/**	Contains a serializable set of generic key-value pairs. */
	class BS_ED_EXPORT Settings : public IReflectable
	{
	public:
		Settings() = default;
		virtual ~Settings() = default;

		/**	Adds or updates a property key/value pair with a floating point value. */
		void setFloat(const String& name, float value);

		/**	Adds or updates a property key/value pair with a signed integer value. */
		void setInt(const String& name, INT32 value);

		/**	Adds or updates a property key/value pair with a boolean value. */
		void setBool(const String& name, bool value);

		/**	Adds or updates a property key/value pair with a string value. */
		void setString(const String& name, const String& value);

		/**	Adds or updates a property key/value pair with a serializable object. */
		void setObject(const String& name, const SPtr<IReflectable>& value);

		/** Returns the floating point value of the specified key, or the default value if such key cannot be found. */
		float getFloat(const String& name, float defaultValue = 0.0f);

		/** Returns the integer point value of the specified key, or the default value if such key cannot be found. */
		INT32 getInt(const String& name, INT32 defaultValue = 0);

		/** Returns the boolean point value of the specified key, or the default value if such key cannot be found. */
		bool getBool(const String& name, bool defaultValue = false);

		/** Returns the string point value of the specified key, or the default value if such key cannot be found. */
		String getString(const String& name, const String& defaultValue = StringUtil::BLANK);

		/**	Returns the object value of the specified key, or null if such key cannot be found. */
		SPtr<IReflectable> getObject(const String& name);

		/**	Returns true if the key with the specified name exists. */
		bool hasKey(const String& name);

		/**	Deletes a key with the specified name. */
		void deleteKey(const String& name);

		/**	Deletes all key/value pairs. */
		void deleteAllKeys();

		/**	Returns a hash value that may be used for checking if any internal settings were modified. */
		UINT32 getHash() const { return mHash; }

	protected:
		/**	Marks the object as dirty so that outside objects know when to update. */
		void markAsDirty() const { mHash++; }

		UnorderedMap<String, impl::SettingsKeyInfo> mKeyLookup;

		Vector<impl::TSettingsValue<impl::SettingsPrimitiveValue>> mPrimitives;
		Vector<impl::TSettingsValue<String>> mStrings;
		Vector<impl::SettingsObjectValue> mObjects;

		mutable UINT32 mHash = 0;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class SettingsRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;
	};

	/** @} */
}
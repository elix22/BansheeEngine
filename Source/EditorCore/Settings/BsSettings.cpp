//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Settings/BsSettings.h"
#include "Private/RTTI/BsSettingsRTTI.h"

namespace bs
{
	using namespace impl;

	RTTITypeBase* SettingsObjectValue::getRTTIStatic()
	{
		return SettingsObjectValueRTTI::instance();
	}

	RTTITypeBase* SettingsObjectValue::getRTTI() const
	{
		return getRTTIStatic();
	}

	void Settings::setFloat(const String& name, float value)
	{
		deleteKey(name);

		UINT32 keyIdx = (UINT32)mPrimitives.size();
		mKeyLookup[name] = SettingsKeyInfo(keyIdx, SettingsValueType::Float);
		mPrimitives.push_back(TSettingsValue<SettingsPrimitiveValue>(name, SettingsPrimitiveValue(value)));

		markAsDirty();
	}

	void Settings::setInt(const String& name, INT32 value)
	{
		deleteKey(name);

		UINT32 keyIdx = (UINT32)mPrimitives.size();
		mKeyLookup[name] = SettingsKeyInfo(keyIdx, SettingsValueType::Int);
		mPrimitives.push_back(TSettingsValue<SettingsPrimitiveValue>(name, SettingsPrimitiveValue(value)));

		markAsDirty();
	}

	void Settings::setBool(const String& name, bool value)
	{
		deleteKey(name);

		UINT32 keyIdx = (UINT32)mPrimitives.size();
		mKeyLookup[name] = SettingsKeyInfo(keyIdx, SettingsValueType::Bool);
		mPrimitives.push_back(TSettingsValue<SettingsPrimitiveValue>(name, SettingsPrimitiveValue(value)));

		markAsDirty();
	}

	void Settings::setString(const String& name, const String& value)
	{
		deleteKey(name);

		UINT32 keyIdx = (UINT32)mStrings.size();
		mKeyLookup[name] = SettingsKeyInfo(keyIdx, SettingsValueType::String);
		mStrings.push_back(TSettingsValue<String>(name, value));

		markAsDirty();
	}

	void Settings::setObject(const String& name, const SPtr<IReflectable>& value)
	{
		deleteKey(name);

		UINT32 keyIdx = (UINT32)mObjects.size();
		mKeyLookup[name] = SettingsKeyInfo(keyIdx, SettingsValueType::Object);
		mObjects.push_back(SettingsObjectValue(name, value));

		markAsDirty();
	}

	float Settings::getFloat(const String& name, float defaultValue)
	{
		auto iterFind = mKeyLookup.find(name);
		if(iterFind == mKeyLookup.end())
			return defaultValue;

		const SettingsKeyInfo& valueInfo = iterFind->second;
		if(valueInfo.type != SettingsValueType::Float)
			return defaultValue;

		return mPrimitives[valueInfo.index].value.floatVal;
	}

	INT32 Settings::getInt(const String& name, INT32 defaultValue)
	{
		auto iterFind = mKeyLookup.find(name);
		if(iterFind == mKeyLookup.end())
			return defaultValue;

		const SettingsKeyInfo& valueInfo = iterFind->second;
		if(valueInfo.type != SettingsValueType::Int)
			return defaultValue;

		return mPrimitives[valueInfo.index].value.intVal;
	}

	bool Settings::getBool(const String& name, bool defaultValue)
	{
		auto iterFind = mKeyLookup.find(name);
		if(iterFind == mKeyLookup.end())
			return defaultValue;

		const SettingsKeyInfo& valueInfo = iterFind->second;
		if(valueInfo.type != SettingsValueType::Bool)
			return defaultValue;

		return mPrimitives[valueInfo.index].value.boolVal;
	}

	String Settings::getString(const String& name, const String& defaultValue)
	{
		auto iterFind = mKeyLookup.find(name);
		if(iterFind == mKeyLookup.end())
			return defaultValue;

		const SettingsKeyInfo& valueInfo = iterFind->second;
		return mStrings[valueInfo.index].value;
	}

	SPtr<IReflectable> Settings::getObject(const String& name)
	{
		auto iterFind = mKeyLookup.find(name);
		if(iterFind == mKeyLookup.end())
			return nullptr;

		const SettingsKeyInfo& valueInfo = iterFind->second;
		return mObjects[valueInfo.index].value;
	}

	bool Settings::hasKey(const String& name)
	{
		return mKeyLookup.find(name) != mKeyLookup.end();
	}

	void Settings::deleteKey(const String& name)
	{
		auto iterFind = mKeyLookup.find(name);
		if(iterFind == mKeyLookup.end())
			return;

		const SettingsKeyInfo& valueInfo = iterFind->second;
		size_t curIdx = (size_t)valueInfo.index;
		switch(valueInfo.type)
		{
		case SettingsValueType::Float:
		case SettingsValueType::Int:
		case SettingsValueType::Bool:
			{
				if(bs_swap_and_erase(mPrimitives, mPrimitives.begin() + curIdx))
				{
					SettingsKeyInfo& swappedValue = mKeyLookup[mPrimitives[curIdx].key];
					swappedValue.index = (UINT32)curIdx;
				}
			}
			break;
		case SettingsValueType::String:
			{
				if(bs_swap_and_erase(mStrings, mStrings.begin() + curIdx))
				{
					SettingsKeyInfo& swappedValue = mKeyLookup[mStrings[curIdx].key];
					swappedValue.index = (UINT32)curIdx;
				}
			}
			break;
		case SettingsValueType::Object:
			{
				if(bs_swap_and_erase(mObjects, mObjects.begin() + curIdx))
				{
					SettingsKeyInfo& swappedValue = mKeyLookup[mObjects[curIdx].key];
					swappedValue.index = (UINT32)curIdx;
				}
			}
			break;
		}

		mKeyLookup.erase(iterFind);

		markAsDirty();
	}

	void Settings::deleteAllKeys()
	{
		mKeyLookup.clear();
		mPrimitives.clear();
		mStrings.clear();
		mObjects.clear();

		markAsDirty();
	}

	RTTITypeBase* Settings::getRTTIStatic()
	{
		return SettingsRTTI::instance();
	}

	RTTITypeBase* Settings::getRTTI() const
	{
		return getRTTIStatic();
	}
}
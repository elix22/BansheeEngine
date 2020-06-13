//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptBrowseDialog.h"
#include "Utility/BsEditorUtility.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "Platform/BsPlatform.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptBrowseDialog::ScriptBrowseDialog(MonoObject* instance)
			:ScriptObject(instance)
	{ }

	void ScriptBrowseDialog::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_OpenFile", (void*)&ScriptBrowseDialog::internal_OpenFile);
		metaData.scriptClass->addInternalCall("Internal_OpenFolder", (void*)&ScriptBrowseDialog::internal_OpenFolder);
		metaData.scriptClass->addInternalCall("Internal_SaveFile", (void*)&ScriptBrowseDialog::internal_SaveFile);
	}

	bool ScriptBrowseDialog::internal_OpenFile(MonoString* defaultFolder, MonoString* filterList, bool allowMultiselect,
											   MonoArray** outPaths)
	{
		Path defaultFolderNative = MonoUtil::monoToString(defaultFolder);
		String filterListNative = MonoUtil::monoToString(filterList);
		
		FileDialogType type = (FileDialogType)((UINT32)FileDialogType::OpenFile | (UINT32)FileDialogType::Multiselect);

		Vector<Path> paths;
		if (EditorUtility::openBrowseDialog(type, defaultFolderNative, filterListNative, paths))
		{
			ScriptArray pathArray(MonoUtil::getStringClass(), (UINT32)paths.size());

			for (UINT32 i = 0; i < (UINT32)paths.size(); i++)
			{
				MonoString* monoString = MonoUtil::stringToMono(paths[i].toString());
				pathArray.set(i, monoString);
			}

			MonoUtil::referenceCopy(outPaths, (MonoObject*)pathArray.getInternal());
			return true;
		}
		else
		{
			*outPaths = nullptr;
			return false;
		}
	}

	bool ScriptBrowseDialog::internal_OpenFolder(MonoString* defaultFolder, MonoString** outPath)
	{
		Path defaultFolderNative = MonoUtil::monoToString(defaultFolder);

		FileDialogType type = FileDialogType::OpenFolder;

		Vector<Path> paths;
		if (EditorUtility::openBrowseDialog(type, defaultFolderNative, "", paths))
		{
			if (paths.size() > 0)
			{
				MonoString* path = MonoUtil::stringToMono(paths[0].toString());
				MonoUtil::referenceCopy(outPath, (MonoObject*)path);
			}
			else
				*outPath = nullptr;

			return true;
		}
		else
		{
			*outPath = nullptr;
			return false;
		}
	}

	bool ScriptBrowseDialog::internal_SaveFile(MonoString* defaultFolder, MonoString* filterList, MonoString** outPath)
	{
		Path defaultFolderNative = MonoUtil::monoToString(defaultFolder);
		String filterListNative = MonoUtil::monoToString(filterList);

		FileDialogType type = FileDialogType::Save;

		Vector<Path> paths;
		if (EditorUtility::openBrowseDialog(type, defaultFolderNative, filterListNative, paths))
		{
			if (paths.size() > 0)
			{
				MonoString* path = MonoUtil::stringToMono(paths[0].toString());
				MonoUtil::referenceCopy(outPath, (MonoObject*)path);
			}
			else
				*outPath = nullptr;

			return true;
		}
		else
		{
			*outPath = nullptr;
			return false;
		}
	}
}
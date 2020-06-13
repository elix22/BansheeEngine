//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "../../bsf/Source/Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "../../bsf/Source/Foundation/bsfCore/Localization/BsHString.h"
#include "../../bsf/Source/Foundation/bsfCore/Particles/BsParticleDistribution.h"

namespace bs { class GUIColorDistributionField; }
namespace bs { struct __GUIContentInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUIColorDistributionField : public TScriptGUIElement<ScriptGUIColorDistributionField>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIColorDistributionField")

		ScriptGUIColorDistributionField(MonoObject* managedInstance, GUIColorDistributionField* value);

	private:
		void onMinClicked();
		void onMaxClicked();

		typedef void(BS_THUNKCALL *onMinClickedThunkDef) (MonoObject*, MonoException**);
		static onMinClickedThunkDef onMinClickedThunk;
		typedef void(BS_THUNKCALL *onMaxClickedThunkDef) (MonoObject*, MonoException**);
		static onMaxClickedThunkDef onMaxClickedThunk;

		static MonoObject* Internal_getValue(ScriptGUIColorDistributionField* thisPtr);
		static void Internal_setValue(ScriptGUIColorDistributionField* thisPtr, MonoObject* value);
		static PropertyDistributionType Internal_getType(ScriptGUIColorDistributionField* thisPtr);
		static void Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style);
		static void Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style);
		static void Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style);
		static void Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style);
		static void Internal_create3(MonoObject* managedInstance, MonoString* style);
	};
}

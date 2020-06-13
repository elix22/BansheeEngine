//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptHandleDrawing.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "Handles/BsHandleManager.h"
#include "Handles/BsHandleDrawManager.h"
#include "BsMonoUtil.h"

#include "Generated/BsScriptFont.generated.h"

namespace bs
{
	ScriptHandleDrawing::ScriptHandleDrawing(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptHandleDrawing::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_SetColor", (void*)&ScriptHandleDrawing::internal_SetColor);
		metaData.scriptClass->addInternalCall("Internal_SetTransform", (void*)&ScriptHandleDrawing::internal_SetTransform);
		metaData.scriptClass->addInternalCall("Internal_SetLayer", (void*)&ScriptHandleDrawing::internal_SetLayer);
		metaData.scriptClass->addInternalCall("Internal_DrawCube", (void*)&ScriptHandleDrawing::internal_DrawCube);
		metaData.scriptClass->addInternalCall("Internal_DrawSphere", (void*)&ScriptHandleDrawing::internal_DrawSphere);
		metaData.scriptClass->addInternalCall("Internal_DrawWireCube", (void*)&ScriptHandleDrawing::internal_DrawWireCube);
		metaData.scriptClass->addInternalCall("Internal_DrawWireSphere", (void*)&ScriptHandleDrawing::internal_DrawWireSphere);
		metaData.scriptClass->addInternalCall("Internal_DrawLine", (void*)&ScriptHandleDrawing::internal_DrawLine);
		metaData.scriptClass->addInternalCall("Internal_DrawCone", (void*)&ScriptHandleDrawing::internal_DrawCone);
		metaData.scriptClass->addInternalCall("Internal_DrawDisc", (void*)&ScriptHandleDrawing::internal_DrawDisc);
		metaData.scriptClass->addInternalCall("Internal_DrawWireDisc", (void*)&ScriptHandleDrawing::internal_DrawWireDisc);
		metaData.scriptClass->addInternalCall("Internal_DrawArc", (void*)&ScriptHandleDrawing::internal_DrawArc);
		metaData.scriptClass->addInternalCall("Internal_DrawWireArc", (void*)&ScriptHandleDrawing::internal_DrawWireArc);
		metaData.scriptClass->addInternalCall("Internal_DrawRect", (void*)&ScriptHandleDrawing::internal_DrawRect);
		metaData.scriptClass->addInternalCall("Internal_DrawText", (void*)&ScriptHandleDrawing::internal_DrawText);
	}

	void ScriptHandleDrawing::internal_SetColor(Color* color)
	{
		HandleManager::instance().getDrawManager().setColor(*color);
	}

	void ScriptHandleDrawing::internal_SetTransform(Matrix4* transform)
	{
		HandleManager::instance().getDrawManager().setTransform(*transform);
	}

	void ScriptHandleDrawing::internal_SetLayer(UINT64 layer)
	{
		HandleManager::instance().getDrawManager().setLayer(layer);
	}

	void ScriptHandleDrawing::internal_DrawCube(Vector3* position, Vector3* extents, float size)
	{
		HandleManager::instance().getDrawManager().drawCube(*position, *extents, size);
	}

	void ScriptHandleDrawing::internal_DrawSphere(Vector3* position, float radius, float size)
	{
		HandleManager::instance().getDrawManager().drawSphere(*position, radius, size);
	}

	void ScriptHandleDrawing::internal_DrawWireCube(Vector3* position, Vector3* extents, float size)
	{
		HandleManager::instance().getDrawManager().drawWireCube(*position, *extents, size);
	}

	void ScriptHandleDrawing::internal_DrawWireSphere(Vector3* position, float radius, float size)
	{
		HandleManager::instance().getDrawManager().drawWireSphere(*position, radius, size);
	}

	void ScriptHandleDrawing::internal_DrawLine(Vector3* start, Vector3* end, float size)
	{
		HandleManager::instance().getDrawManager().drawLine(*start, *end, size);
	}

	void ScriptHandleDrawing::internal_DrawCone(Vector3* coneBase, Vector3* normal, float height, float radius, float size)
	{
		HandleManager::instance().getDrawManager().drawCone(*coneBase, *normal, height, radius, size);
	}

	void ScriptHandleDrawing::internal_DrawDisc(Vector3* position, Vector3* normal, float radius, float size)
	{
		HandleManager::instance().getDrawManager().drawDisc(*position, *normal, radius, size);
	}

	void ScriptHandleDrawing::internal_DrawWireDisc(Vector3* position, Vector3* normal, float radius, float size)
	{
		HandleManager::instance().getDrawManager().drawWireDisc(*position, *normal, radius, size);
	}

	void ScriptHandleDrawing::internal_DrawArc(Vector3* position, Vector3* normal, float radius, Degree* startAngle, Degree* amountAngle, float size)
	{
		HandleManager::instance().getDrawManager().drawArc(*position, *normal, radius, *startAngle, *amountAngle, size);
	}

	void ScriptHandleDrawing::internal_DrawWireArc(Vector3* position, Vector3* normal, float radius, Degree* startAngle, Degree* amountAngle, float size)
	{
		HandleManager::instance().getDrawManager().drawWireArc(*position, *normal, radius, *startAngle, *amountAngle, size);
	}

	void ScriptHandleDrawing::internal_DrawRect(Vector3* center, Vector3* horzAxis, Vector3* vertAxis, float extentH, float extentV, float size)
	{
		std::array<Vector3, 2> axes = { { *horzAxis, *vertAxis } };
		std::array<float, 2> extents = { { extentH, extentV } };

		Rect3 area(*center, axes, extents);
		HandleManager::instance().getDrawManager().drawRect(area, size);
	}

	void ScriptHandleDrawing::internal_DrawText(Vector3* position, MonoString* text, ScriptFont* font, int fontSize)
	{
		String nativeText = MonoUtil::monoToString(text);

		HFont fontHandle;
		if (font != nullptr)
			fontHandle = font->getHandle();

		HandleManager::instance().getDrawManager().drawText(*position, nativeText, fontHandle, fontSize);
	}
}
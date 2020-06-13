//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "BsGUIGameObjectField.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIDropButton.h"
#include "GUI/BsGUIButton.h"
#include "Resources/BsBuiltinResources.h"
#include "GUI/BsGUISceneTreeView.h"
#include "Scene/BsGameObjectManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsMonoClass.h"
#include "Scene/BsSceneObject.h"
#include "BsManagedComponent.h"
#include "BsMonoManager.h"
#include "Utility/BsBuiltinEditorResources.h"
#include "Scene/BsComponent.h"
#include "Scene/BsSelection.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUIGameObjectField::DEFAULT_LABEL_WIDTH = 100;

	GUIGameObjectField::GUIGameObjectField(const PrivatelyConstruct& dummy, const String& typeNamespace, const String& type, const GUIContent& labelContent, UINT32 labelWidth,
		const String& style, const GUIDimensions& dimensions, bool withLabel)
		: GUIElementContainer(dimensions, style), mLabel(nullptr), mDropButton(nullptr), mClearButton(nullptr), mType(type)
		, mNamespace(typeNamespace), mInstanceId(0)
	{
		mLayout = GUILayoutX::create();
		_registerChildElement(mLayout);

		if(withLabel)
		{
			mLabel = GUILabel::create(labelContent, GUIOptions(GUIOption::fixedWidth(labelWidth)), getSubStyleName(BuiltinEditorResources::ObjectFieldLabelStyleName));
			mLayout->addElement(mLabel);
		}

		mDropButton = GUIDropButton::create((UINT32)DragAndDropType::SceneObject, GUIOptions(GUIOption::flexibleWidth()), getSubStyleName(BuiltinEditorResources::ObjectFieldDropBtnStyleName));
		mClearButton = GUIButton::create(HString(""), getSubStyleName(BuiltinEditorResources::ObjectFieldClearBtnStyleName));
		mClearButton->onClick.connect(std::bind(&GUIGameObjectField::onClearButtonClicked, this));

		mLayout->addElement(mDropButton);
		mLayout->addElement(mClearButton);

		mDropButton->onDataDropped.connect(std::bind(&GUIGameObjectField::dataDropped, this, _1));
		mDropButton->onClick.connect(std::bind(&GUIGameObjectField::onDropButtonClicked, this));
		mDropButton->setContent(GUIContent(HString("None (" + mType + ")")));
	}

	GUIGameObjectField::~GUIGameObjectField()
	{

	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const GUIContent& labelContent, UINT32 labelWidth, const GUIOptions& options,
		const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, labelContent, labelWidth, *curStyle,
			GUIDimensions::create(options), true);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const GUIContent& labelContent, const GUIOptions& options,
		const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, labelContent, DEFAULT_LABEL_WIDTH, *curStyle,
			GUIDimensions::create(options), true);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const HString& labelText, UINT32 labelWidth, const GUIOptions& options,
		const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, GUIContent(labelText), labelWidth, *curStyle,
			GUIDimensions::create(options), true);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const HString& labelText, const GUIOptions& options,
		const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, GUIContent(labelText), DEFAULT_LABEL_WIDTH, *curStyle,
			GUIDimensions::create(options), true);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const GUIOptions& options, const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, GUIContent(), 0, *curStyle,
			GUIDimensions::create(options), false);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const GUIContent& labelContent, UINT32 labelWidth,
		const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, labelContent, labelWidth, *curStyle,
			GUIDimensions::create(), true);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const GUIContent& labelContent,
		const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, labelContent, DEFAULT_LABEL_WIDTH, *curStyle,
			GUIDimensions::create(), true);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const HString& labelText, UINT32 labelWidth,
		const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, GUIContent(labelText), labelWidth, *curStyle,
			GUIDimensions::create(), true);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const HString& labelText,
		const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, GUIContent(labelText), DEFAULT_LABEL_WIDTH, *curStyle,
			GUIDimensions::create(), true);
	}

	GUIGameObjectField* GUIGameObjectField::create(const String& typeNamespace, const String& type, const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::ObjectFieldStyleName;

		return bs_new<GUIGameObjectField>(PrivatelyConstruct(), typeNamespace, type, GUIContent(), 0, *curStyle,
			GUIDimensions::create(), false);
	}

	HGameObject GUIGameObjectField::getValue() const
	{
		HGameObject obj;

		if(mInstanceId != 0)
			GameObjectManager::instance().tryGetObject(mInstanceId, obj);

		return obj;
	}

	void GUIGameObjectField::setValue(const HGameObject& value)
	{
		setValue(value, false);
	}

	void GUIGameObjectField::setValue(const HGameObject& value, bool triggerEvent)
	{
		if (value)
		{
			if (mInstanceId == value.getInstanceId())
				return;

			mInstanceId = value->getInstanceId();

			if(rtti_is_of_type<SceneObject>(value.get()))
				mDropButton->setContent(GUIContent(HString(value->getName() + " (" + mType + ")")));
			else
			{
				HComponent component = static_object_cast<Component>(value);
				mDropButton->setContent(GUIContent(HString(component->SO()->getName() + " (" + mType + ")")));
			}
		}
		else
		{
			if (mInstanceId == 0)
				return;

			mInstanceId = 0;
			mDropButton->setContent(GUIContent(HString("None (" + mType + ")")));
		}

		if (triggerEvent)
			onValueChanged(value);
	}

	void GUIGameObjectField::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mDropButton->setTint(color);
		mClearButton->setTint(color);
	}

	void GUIGameObjectField::_updateLayoutInternal(const GUILayoutData& data)
	{
		mLayout->_setLayoutData(data);
		mLayout->_updateLayoutInternal(data);
	}

	Vector2I GUIGameObjectField::_getOptimalSize() const
	{
		return mLayout->_getOptimalSize();
	}

	void GUIGameObjectField::onDropButtonClicked()
	{
		if (mInstanceId == 0)
			return;

		HGameObject go;
		if (GameObjectManager::instance().tryGetObject(mInstanceId, go))
		{
			HSceneObject so;
			if (rtti_is_of_type<SceneObject>(go.get()))
			{
				so = static_object_cast<SceneObject>(go);
			}
			else if(rtti_is_subclass<Component>(go.get()))
			{
				HComponent component = static_object_cast<Component>(go);
				so = component->SO();
			}

			Selection::instance().ping(so);
		}
	}

	void GUIGameObjectField::dataDropped(void* data)
	{
		DraggedSceneObjects* draggedSceneObjects = reinterpret_cast<DraggedSceneObjects*>(data);

		if (draggedSceneObjects->numObjects <= 0)
			return;

		MonoClass* sceneObjectClass = ScriptAssemblyManager::instance().getBuiltinClasses().sceneObjectClass;

		if (mType == sceneObjectClass->getFullName()) // A scene object
		{
			setValue(static_object_cast<GameObject>(draggedSceneObjects->objects[0]), true);
		}
		else // A component
		{
			MonoClass* acceptedClass = MonoManager::instance().findClass(mNamespace, mType);

			ScriptAssemblyManager& sam = ScriptAssemblyManager::instance();

			for (UINT32 i = 0; i < draggedSceneObjects->numObjects; i++)
			{
				HSceneObject so = draggedSceneObjects->objects[i];

				const Vector<HComponent>& components = so->getComponents();
				for (auto& component : components)
				{
					if (component->getTypeId() == TID_ManagedComponent)
					{
						HManagedComponent managedComponent = static_object_cast<ManagedComponent>(component);

						MonoClass* providedClass = MonoManager::instance().findClass(
							managedComponent->getManagedNamespace(), managedComponent->getManagedTypeName());

						if (acceptedClass != nullptr && providedClass != nullptr)
						{
							if (providedClass->isSubClassOf(acceptedClass))
							{
								setValue(static_object_cast<GameObject>(managedComponent), true);
							}
						}
					}
					else
					{
						BuiltinComponentInfo* info = sam.getBuiltinComponentInfo(component->getRTTI()->getRTTIId());
						if (info == nullptr)
							continue;

						if (info->monoClass->isSubClassOf(acceptedClass))
							setValue(static_object_cast<GameObject>(component), true);
					}
				}
			}
		}
	}

	void GUIGameObjectField::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(getSubStyleName(BuiltinEditorResources::ObjectFieldLabelStyleName));

		mDropButton->setStyle(getSubStyleName(BuiltinEditorResources::ObjectFieldDropBtnStyleName));
		mClearButton->setStyle(getSubStyleName(BuiltinEditorResources::ObjectFieldClearBtnStyleName));
	}

	void GUIGameObjectField::onClearButtonClicked()
	{
		setValue(HGameObject(), true);
	}

	const String& GUIGameObjectField::getGUITypeName()
	{
		static String typeName = "GUIGameObjectField";
		return typeName;
	}
}
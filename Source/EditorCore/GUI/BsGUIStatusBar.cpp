//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIStatusBar.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUITexture.h"
#include "GUI/BsGUIPanel.h"
#include "GUI/BsGUISpace.h"
#include "Debug/BsDebug.h"
#include "Utility/BsBuiltinEditorResources.h"
#include "GUI/BsGUIProgressBar.h"

using namespace std::placeholders;

namespace bs
{
	const Color GUIStatusBar::COLOR_INFO = Color(0.7f, 0.7f, 0.7f);
	const Color GUIStatusBar::COLOR_WARNING = Color(192 / 255.0f, 176 / 255.0f, 0.0f);
	const Color GUIStatusBar::COLOR_ERROR = Color(192 / 255.0f, 36 / 255.0f, 0.0f);

	GUIStatusBar::GUIStatusBar(const PrivatelyConstruct& dummy, const String& style, const GUIDimensions& dimensions)
		:GUIElementContainer(dimensions, style)
	{
		mPanel = GUIPanel::create();
		mBgPanel = GUIPanel::create(1);
		_registerChildElement(mPanel);
		_registerChildElement(mBgPanel);

		mBackground = GUITexture::create(GUIOptions(GUIOption::flexibleWidth()), getSubStyleName(getGUIBackgroundTypeName()));
		mMessage = GUIButton::create(HEString(""), GUIOptions(GUIOption::flexibleWidth()), getSubStyleName(getGUIMessageTypeName()));
		mScene = GUILabel::create(HEString("Scene: Unnamed"), GUIOptions(GUIOption::fixedWidth(150)));
		mProject = GUILabel::create(HEString("Project: None"), GUIOptions(GUIOption::fixedWidth(200)));
		mCompiling = GUILabel::create(HEString("Compiling..."), GUIOptions(GUIOption::fixedWidth(60)));
		mImporting = GUILabel::create(HEString("Importing..."), GUIOptions(GUIOption::fixedWidth(60)));
		mImportProgressBar = GUIProgressBar::create(GUIOptions(GUIOption::fixedWidth(100)), "ProgressBarSmall");

		GUIElementOptions msgBtnOptions = mMessage->getOptionFlags();
		msgBtnOptions.unset(GUIElementOption::AcceptsKeyFocus);
		mMessage->setOptionFlags(msgBtnOptions);

		GUILayoutY* vertLayout = mPanel->addNewElement<GUILayoutY>();
		vertLayout->addNewElement<GUIFixedSpace>(3);
		GUILayoutX* horzLayout = vertLayout->addNewElement<GUILayoutX>();

		horzLayout->addNewElement<GUIFixedSpace>(10);
		horzLayout->addElement(mMessage);
		horzLayout->addNewElement<GUIFlexibleSpace>();
		horzLayout->addElement(mScene);
		horzLayout->addNewElement<GUIFixedSpace>(10);
		horzLayout->addElement(mProject);
		horzLayout->addNewElement<GUIFixedSpace>(10);
		horzLayout->addElement(mCompiling);
		mCompilingSpace = horzLayout->addNewElement<GUIFixedSpace>(10);

		horzLayout->addElement(mImporting);
		mImportLayout = horzLayout->addNewElement<GUILayoutY>();
		mImportLayout->addNewElement<GUIFixedSpace>(2);
		mImportLayout->addElement(mImportProgressBar);

		mImportSpace = horzLayout->addNewElement<GUIFixedSpace>(10);

		mBgPanel->addElement(mBackground);

		setIsCompiling(false);
		setIsImporting(false, 0.0f);

		mLogEntryAddedConn = gDebug().onLogModified.connect(std::bind(&GUIStatusBar::logModified, this));
		mMessageBtnPressedConn = mMessage->onClick.connect(std::bind(&GUIStatusBar::messageBtnClicked, this));
	}

	GUIStatusBar::~GUIStatusBar()
	{
		mLogEntryAddedConn.disconnect();
		mMessageBtnPressedConn.disconnect();
	}

	GUIStatusBar* GUIStatusBar::create(const GUIOptions& options, const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &getGUITypeName();

		return bs_new<GUIStatusBar>(PrivatelyConstruct(), *curStyle, GUIDimensions::create(options));
	}

	GUIStatusBar* GUIStatusBar::create(const String& style)
	{
		const String* curStyle = &style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &getGUITypeName();

		return bs_new<GUIStatusBar>(PrivatelyConstruct(), *curStyle, GUIDimensions::create());
	}

	void GUIStatusBar::setProject(const String& name, bool modified)
	{
		StringStream content;
		content << "Project: ";

		if (name.size() > 20)
			content << name.substr(0, 20) << "...";
		else
			content << name;

		if (modified)
			content << "*";

		mProject->setContent(HString(content.str()));
	}

	void GUIStatusBar::setScene(const String& name, bool modified)
	{
		StringStream content;
		content << "Scene: ";

		if (name.size() > 15)
			content << name.substr(0, 15) << "...";
		else
			content << name;

		if (modified)
			content << "*";

		mScene->setContent(HString(content.str()));
	}

	void GUIStatusBar::setIsCompiling(bool compiling)
	{
		mCompiling->setActive(compiling);
		mCompilingSpace->setActive(compiling);
	}

	void GUIStatusBar::setIsImporting(bool importing, float percentage)
	{
		mImporting->setActive(importing);
		mImportLayout->setActive(importing);
		mImportSpace->setActive(importing);
		mImportProgressBar->setPercent(percentage);
	}
	
	void GUIStatusBar::setTint(const Color& color)
	{
		mBackground->setTint(color);
		mMessage->setTint(color);
	}

	void GUIStatusBar::_updateLayoutInternal(const GUILayoutData& data)
	{
		mPanel->_setLayoutData(data);
		mPanel->_updateLayoutInternal(data);

		mBgPanel->_setLayoutData(data);
		mBgPanel->_updateLayoutInternal(data);
	}

	Vector2I GUIStatusBar::_getOptimalSize() const
	{
		return mBgPanel->_getOptimalSize();
	}
	
	void GUIStatusBar::styleUpdated()
	{
		mBackground->setStyle(getSubStyleName(getGUIBackgroundTypeName()));
		mMessage->setStyle(getSubStyleName(getGUIMessageTypeName()));
	}

	void GUIStatusBar::logModified()
	{
		LogEntry entry;
		if(!gDebug().getLog().getLastEntry(entry))
		{
			GUIContent messageContent(HString(""));
			mMessage->setContent(messageContent);

			return;
		}

		HSpriteTexture iconTexture;
		Color textColor = COLOR_INFO;

		LogVerbosity verbosity = entry.getVerbosity();
		switch (verbosity)
		{
		case LogVerbosity::VeryVerbose:
		case LogVerbosity::Verbose:
		case LogVerbosity::Info:
			iconTexture = BuiltinEditorResources::instance().getLogMessageIcon(LogMessageIcon::Info, 16, false);
			break;
		case LogVerbosity::Warning:
			iconTexture = BuiltinEditorResources::instance().getLogMessageIcon(LogMessageIcon::Warning, 16, false);
			textColor = COLOR_WARNING;
			break;
		case LogVerbosity::Error:
		case LogVerbosity::Fatal:
			iconTexture = BuiltinEditorResources::instance().getLogMessageIcon(LogMessageIcon::Error, 16, false);
			textColor = COLOR_ERROR;
			break;
		}

		String message = entry.getMessage();
		size_t lfPos = message.find_first_of('\n');
		size_t crPos = message.find_first_of('\r');
		size_t newlinePos;

		if (lfPos != String::npos)
		{
			if (crPos != String::npos)
				newlinePos = std::min(lfPos, crPos);
			else
				newlinePos = lfPos;
		}
		else if (crPos != String::npos)
			newlinePos = crPos;
		else
			newlinePos = -1;

		if (newlinePos == (UINT32)-1)
		{
			GUIContent messageContent(HString(message), iconTexture);
			mMessage->setContent(messageContent);
			mMessage->setTint(textColor);
		}
		else
		{
			String firstLine = message.substr(0, newlinePos);

			GUIContent messageContent(HString(firstLine), iconTexture);
			mMessage->setContent(messageContent);
			mMessage->setTint(textColor);
		}		
	}

	void GUIStatusBar::messageBtnClicked()
	{
		onMessageClicked();
	}

	const String& GUIStatusBar::getGUITypeName()
	{
		static String TypeName = "GUIStatusBar";
		return TypeName;
	}

	const String& GUIStatusBar::getGUIBackgroundTypeName()
	{
		static String TypeName = "GUIStatusBarBg";
		return TypeName;
	}

	const String& GUIStatusBar::getGUIMessageTypeName()
	{
		static String TypeName = "GUIStatusBarMessage";
		return TypeName;
	}
}
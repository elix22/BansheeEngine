//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "UndoRedo/BsEditorCommand.h"

namespace bs
{
	EditorCommand::EditorCommand(const String& description)
		:mDescription(description), mId(0)
	{ }
}
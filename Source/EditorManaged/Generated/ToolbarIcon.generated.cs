//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/// <summary>Types of icons that may be displayed on the tool bar.</summary>
	public enum ToolbarIcon
	{
		NewCone = 8,
		NewCamera = 0,
		NewRenderable = 1,
		NewPointLight = 2,
		NewDirLight = 3,
		NewSpotLight = 4,
		NewSceneObject = 5,
		NewCube = 6,
		NewSphere = 7,
		NewQuad = 9,
		NewMat = 10,
		NewCSScript = 11,
		NewShader = 12,
		NewSpriteTex = 13,
		Pause = 14,
		Play = 15,
		Step = 16,
		Undo = 17,
		Redo = 18,
		OpenProject = 19,
		SaveProject = 20,
		SaveScene = 21
	}
}

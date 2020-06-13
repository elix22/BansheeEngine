//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/// <summary>Types of valid icons used when viewing the project library.</summary>
	public enum ProjectLibraryIcon
	{
		ShaderInclude = 8,
		Folder = 0,
		Mesh = 1,
		Font = 2,
		Texture = 3,
		PlainText = 4,
		ScriptCode = 5,
		SpriteTexture = 6,
		Shader = 7,
		Material = 9,
		Prefab = 10,
		GUISkin = 11,
		PhysicsMaterial = 12,
		PhysicsMesh = 13,
		AudioClip = 14,
		AnimationClip = 15,
		VectorField = 16
	}
}

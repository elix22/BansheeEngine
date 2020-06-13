//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup Importer
	 *  @{
	 */

	/// <summary>
	/// Contains import options you may use to control how is a file containing script source code importer.
	/// </summary>
	[ShowInInspector]
	public partial class ScriptCodeImportOptions : ImportOptions
	{
		private ScriptCodeImportOptions(bool __dummy0) { }

		/// <summary>Creates a new import options object that allows you to customize how is script code imported.</summary>
		public ScriptCodeImportOptions()
		{
			Internal_create(this);
		}

		/// <summary>Determines whether the script is editor-only or a normal game script.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EditorScript
		{
			get { return Internal_geteditorScript(mCachedPtr); }
			set { Internal_seteditorScript(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_geteditorScript(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_seteditorScript(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create(ScriptCodeImportOptions managedInstance);
	}

	/** @} */
}

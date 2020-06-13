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
	/// Base class for creating import options from. Import options are specific for each importer and control how is data 
	/// imported.
	/// </summary>
	[ShowInInspector]
	public partial class ImportOptions : ScriptObject
	{
		private ImportOptions(bool __dummy0) { }
		protected ImportOptions() { }

	}

	/** @} */
}

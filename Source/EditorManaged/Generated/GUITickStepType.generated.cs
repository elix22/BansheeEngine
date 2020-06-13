//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup GUIEditor
	 *  @{
	 */

	/// <summary>Determines how should ticks reported by &lt;see cref=&quot;GUIGraphTicks&quot;/&gt; be distributed.</summary>
	public enum GUITickStepType
	{
		/// <summary>Ticks represent time values (Multiples of 60).</summary>
		Time = 0,
		/// <summary>Ticks represent generic values (Multiples of 10).</summary>
		Generic = 1
	}

	/** @} */
}

//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup Text
	 *  @{
	 */

	/// <summary>Represents a range of character code.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct CharRange
	{
		/// <summary>Initializes the struct with default values.</summary>
		public static CharRange Default()
		{
			CharRange value = new CharRange();
			value.start = 0;
			value.end = 0;

			return value;
		}

		public CharRange(int start, int end)
		{
			this.start = start;
			this.end = end;
		}

		public int start;
		public int end;
	}

	/** @} */
}

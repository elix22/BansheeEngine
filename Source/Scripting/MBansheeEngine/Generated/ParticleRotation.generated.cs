using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace BansheeEngine
{
	/** @addtogroup Particles
	 *  @{
	 */

	/// <summary>Rotates the particles over the particle lifetime.</summary>
	public partial class ParticleRotation : ParticleEvolver
	{
		private ParticleRotation(bool __dummy0) { }
		protected ParticleRotation() { }

		/// <summary>Options describing the evolver.</summary>
		public ParticleRotationOptions Options
		{
			get
			{
				ParticleRotationOptions temp;
				Internal_getOptions(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_setOptions(mCachedPtr, ref value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setOptions(IntPtr thisPtr, ref ParticleRotationOptions options);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_getOptions(IntPtr thisPtr, out ParticleRotationOptions __output);
	}

	/** @} */
}

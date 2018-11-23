using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace BansheeEngine
{
	/** @addtogroup Particles
	 *  @{
	 */

	/// <summary>Information describing a ParticleEmitterSphereShape.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct ParticleSphereShapeOptions
	{
		/// <summary>Initializes the struct with default values.</summary>
		public static ParticleSphereShapeOptions Default()
		{
			ParticleSphereShapeOptions value = new ParticleSphereShapeOptions();
			value.radius = 0f;
			value.thickness = 0f;

			return value;
		}

		/// <summary>Radius of the sphere.</summary>
		public float radius;
		/// <summary>
		/// Proportion of the volume that can emit particles. Thickness of 0 results in particles being emitted only from the 
		/// edge of the volume, while thickness of 1 results in particles being emitted from the entire volume. In-between values 
		/// will use a part of the volume.
		/// </summary>
		public float thickness;
	}

	/** @} */
}

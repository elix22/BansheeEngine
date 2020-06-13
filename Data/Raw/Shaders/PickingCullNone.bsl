#include "$EDITOR$/PickingCull.bslinc"

shader PickingCullNone
{
	mixin PickingCull;

	raster
	{
		cull = none;
	};
};
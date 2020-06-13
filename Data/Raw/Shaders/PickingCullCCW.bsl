#include "$EDITOR$/PickingCull.bslinc"

shader PickingCullCCW
{
	mixin PickingCull;

	raster
	{
		cull = ccw;
	};
};
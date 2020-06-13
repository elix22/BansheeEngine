#include "$EDITOR$/PickingCull.bslinc"

shader PickingCullCW
{
	mixin PickingCull;
	
	raster
	{
		cull = cw;
	};
};
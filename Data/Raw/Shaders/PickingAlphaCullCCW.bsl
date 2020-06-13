#include "$EDITOR$/PickingAlphaCull.bslinc"

shader PickingAlphaCullCCW
{
	mixin PickingAlphaCull;

	raster
	{
		cull = ccw;
	};
};
#include "$EDITOR$/PickingAlphaCull.bslinc"

shader PickingAlphaCullCW
{
	mixin PickingAlphaCull;

	raster
	{
		cull = cw;
	};
};
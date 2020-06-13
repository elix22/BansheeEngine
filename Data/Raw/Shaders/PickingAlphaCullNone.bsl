#include "$EDITOR$/PickingAlphaCull.bslinc"

shader PickingAlphaCullNone
{
	mixin PickingAlphaCull;

	raster
	{
		cull = none;
	};
};
#include "$EDITOR$\SelectionBase.bslinc"

#define USE_SKELETON
#include "$EDITOR$\SelectionBase.bslinc"
#undef USE_SKELETON

#define USE_BLEND_SHAPES
#include "$EDITOR$\SelectionBase.bslinc"

#define USE_SKELETON
#include "$EDITOR$\SelectionBase.bslinc"

shader Selection1
{ 
	mixin Selection;
};

shader Selection2
{
	mixin SelectionSkinned;

	tags = { "Skinned" };
};

shader Selection3
{
	mixin SelectionMorph;

	tags = { "Morph" };
};

shader Selection4
{
	mixin SelectionSkinnedMorph;

	tags = { "SkinnedMorph" };
};
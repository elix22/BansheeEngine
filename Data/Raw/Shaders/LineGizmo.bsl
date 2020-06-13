#include "$EDITOR$/LineGizmo.bslinc"

shader LineGizmo
{
	mixin LineGizmoBase;

	raster
	{
		multisample = false; // This controls line rendering algorithm
		lineaa = true;
	};
	
	blend
	{
		target
		{
			enabled = true;
			color = { srcA, srcIA, add };
		};
	};
};

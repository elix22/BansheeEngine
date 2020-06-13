#include "$EDITOR$/GizmoCommon.bslinc"

shader TextGizmo
{
	mixin GizmoCommon;

	blend
	{
		target 
		{
			enabled = true;
			color = { srcA, srcIA, add };
		};
	};
	
	depth
	{
		write = false;
	};
	
	code
	{
		void vsmain(
			in float3 inPos : POSITION,
			in float2 uv : TEXCOORD0,
			in float4 color : COLOR0,
			out float4 oPosition : SV_Position,
			out float2 oUv : TEXCOORD0,
			out float4 oColor : COLOR0)
		{
			float tfrmdX = -1.0f + (inPos.x * gInvViewportWidth);
			float tfrmdY = (1.0f - (inPos.y * gInvViewportHeight)) * gViewportYFlip;

			oPosition = float4(tfrmdX, tfrmdY, inPos.z, 1);
			oUv = uv;
			oColor = color;
		}		

		SamplerState gMainTexSamp
		{
			Filter = MIN_MAG_MIP_POINT;
		};
		
		Texture2D gMainTexture;

		float4 fsmain(
			in float4 inPos : SV_Position, 
			float2 uv : TEXCOORD0, 
			float4 color : COLOR0) : SV_Target
		{
			return float4(color.rgb, gMainTexture.Sample(gMainTexSamp, uv).r * color.a);
		}		
	};
};
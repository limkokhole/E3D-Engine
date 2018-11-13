
varying vec3 vPosition;
varying vec3 mCameraPos;

vec4 mixFogColor(vec4 _in_fragColor, vec4 _in_FogColor)
{
	float dist = abs(distance(mCameraPos, vPosition));
	float maxDist = 300.0;
	float minDist = 1.0;
    float fogFactor = (maxDist - dist) / (maxDist - minDist);  
    fogFactor = clamp( fogFactor, 0.0, 1.0 );  
	
    vec4 mColor = mix( _in_FogColor, _in_fragColor, fogFactor );  
  
    return mColor;
}

float getShadowColor(vec4 pos, float bias)
{
	float shadowColor = 1.0;
#ifndef __GLES__
	float textSize = 1.0/4096.0;
 	float depth = texture2D(_e3d_lightDepthTex, pos.xy).r;
	
	if(pos.z - bias > depth) // 
	{
		shadowColor = 0.5;
	}	
	else
	{
		vec4 coord1 = vec4(clamp(pos.xy - textSize, 0.0, 1.0),pos.z, pos.w );		
		vec4 coord2 = vec4(clamp(pos.xy + textSize, 0.0, 1.0),pos.z, pos.w );
		vec4 coord3 = vec4(clamp(pos.x -  textSize, 0.0, 1.0), clamp(pos.y + textSize, 0.0, 1.0) ,pos.z, pos.w );
		vec4 coord4 = vec4(clamp(pos.x +  textSize, 0.0, 1.0), clamp(pos.y - textSize, 0.0, 1.0) ,pos.z, pos.w );
		
		float depth1 =  texture2D(_e3d_lightDepthTex,coord1.xy).r;
		float depth2 =  texture2D(_e3d_lightDepthTex,coord2.xy).r;
		float depth3 =  texture2D(_e3d_lightDepthTex,coord3.xy).r;
		float depth4 =  texture2D(_e3d_lightDepthTex,coord4.xy).r;
		
		depth1 = coord1.z - bias > depth1 ?  0.5 : 1.0; 
		depth2 = coord2.z - bias > depth2 ?  0.5 : 1.0; 
		depth3 = coord3.z - bias > depth3 ?  0.5 : 1.0; 
		depth4 = coord4.z - bias > depth4 ?  0.5 : 1.0; 
		
		float mulColor = depth1* depth2 * depth3 * depth4;
		if (mulColor < 1.0)
		{
			shadowColor = (depth1+ depth2 + depth3 + depth4 ) / 4.0 * 0.9;
		}
	}
#endif
	return shadowColor;
}

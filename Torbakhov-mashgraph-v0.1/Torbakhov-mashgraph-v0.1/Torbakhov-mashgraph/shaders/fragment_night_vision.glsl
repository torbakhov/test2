#version 330 core

in vec3 fragPositionMS;
in vec2 fragTexCoords;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture; //actually noise
uniform float deltaTime;

out vec4 fragColor;

void main ()
{
	vec4 finalColor;
	// Set effectCoverage to 1.0 for normal use. 
	
	vec2 textureSize2d = textureSize(diffuseTexture,0);
	
	vec2 texCoord = vec2(fragTexCoords.x, fragTexCoords.y);
	texCoord -= vec2(0.5, 0.5);
	texCoord.x *= textureSize2d.x/textureSize2d.y;
	if (sqrt(dot(texCoord, texCoord)) < 0.45) 
	{
		vec2 rand_coord = vec2(0.1 * sin(deltaTime*50000), 0.1 * cos(deltaTime*50000));
		vec3 n = texture(normalTexture, texCoord + rand_coord).rgb;
		vec3 c = texture(diffuseTexture, fragTexCoords).rgb;

		float lum = dot(vec3(0.30, 0.59, 0.11), c);
		if (lum < 0.2)
			c *= 4.0;

		vec3 visionColor = vec3(0.1, 0.95, 0.2);
		finalColor.rgb = (c + (n*0.2)) * visionColor;
		if (sqrt(dot(texCoord, texCoord)) > 0.2)
		{
			finalColor.rgb *= (0.45 - sqrt(dot(texCoord, texCoord))) / 0.25;
		}
		finalColor += vec4(0.05,0.05,0.05,1);
	}
	else
	{
		finalColor = vec4(0.05,0.05,0.05,1);
	}
	fragColor.rgb = finalColor.rgb;
	fragColor.a = 1.0;
}

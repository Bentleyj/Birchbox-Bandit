#version 120

uniform sampler2DRect inputTexture;
uniform vec2 resolution;

void main() {
    
    vec2 uv = gl_FragCoord.xy / resolution;
    
    uv.y = 1.0 - uv.y;
    
    vec2 uvAbs = uv * resolution;

    vec3 tc = texture2DRect(inputTexture, uvAbs).rgb;

    float width = 0.2;

   	float d = smoothstep(0.0, width, uv.y);

   	d -= smoothstep(1.0 - width, 1.0, uv.y);

   	tc *= d;
    
    // uvAbs.y += length(tc) * 100.0;
    
    // tc = texture2DRect(inputTexture, uvAbs).rgb;
    
    gl_FragColor = vec4(tc, 1.0);
}
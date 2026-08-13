#version 450 core

out vec4 out_color;

uniform sampler2D g_Texture;                                  

in vec4 gl_FragCoord;
in struct vsout {                                                             
    vec4 position;                                          
    vec4 color;                                                   
    vec2 texcoord;                                             
    vec3 normals;                                                
    float fograte;                                                    
    vec4 pos2;
    vec3 camera_pos;
    vec2 fog_pos;
} VS_OUT;                                                                                 
                                                                                             
layout(std140, binding = 2) uniform WorldLight {                                                  
    vec4 GlobalLightRot;                                                           
    vec4 Ambient;                                                                  
    vec3 FogColor;                                                                 
    vec3 GlobalLightColor;                                                         
    float SpecularPower;
    vec4 CameraPos;
};                                                                                
                                                                                             
void main() {
    vec4 finalcolor = texture2D(g_Texture, VS_OUT.texcoord);   
    vec3 norm = normalize(VS_OUT.normals);                   
    vec4 factor = vec4(vec3(max(dot(norm, normalize(-GlobalLightRot.xyz)), 0.0f)),1.0f);  
    //float specval = pow(max(0.0, dot(normalize(camera_pos - VS_OUT.pos2.xyz), normalize(reflect(-GlobalLightRot.xyz, norm)))), SpecularPower);
    //finalcolor.xyz = finalcolor.xyz + clamp(specval, 0.0, 1.0);
    //finalcolor.xyz *= GlobalLightColor;
    finalcolor *= factor * VS_OUT.color;
    finalcolor += Ambient;
    float fog_factor = (gl_FragDepth - VS_OUT.fog_pos.x)  / (VS_OUT.fog_pos.y - VS_OUT.fog_pos.x);
    fog_factor = clamp(fog_factor, 0.0f, 1.0f);
    finalcolor.rgb = mix(finalcolor.rgb, FogColor, VS_OUT.fograte);              
    out_color = clamp(finalcolor, 0.0f, 1.0f);                                                     
}

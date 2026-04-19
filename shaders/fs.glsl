uniform sampler2D uTexture;
uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vWorldPosition;
void main()
{
    vec3 texColor = texture2D(uTexture, vTexCoords).xyz;
    gl_FragColor = vec4(texColor, 1.0);
}
uniform sampler2D uTexture;
uniform sampler2D uAmbiantTexture;

varying vec2 vTexCoords;
varying vec3 vWorldPosition;
void main()
{
    gl_FragColor = texture2D(uTexture, vTexCoords);
    // gl_FragColor = vec4(vWorldPosition, 1.0);
}
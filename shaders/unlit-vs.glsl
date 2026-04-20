attribute vec3 aVertexPosition;
attribute vec2 aTexCoords;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uWorldMatrix;

varying vec2 vTexCoords;
varying vec3 vWorldPosition;

void main()
{
    gl_Position = uProjectionMatrix*uViewMatrix*uWorldMatrix*vec4(aVertexPosition, 1.0);

    vTexCoords = aTexCoords;
    vTexCoords = vec2(vTexCoords.x, 1-vTexCoords.y);

    vWorldPosition = (uWorldMatrix * vec4(aVertexPosition, 1.0)).xyz;
}
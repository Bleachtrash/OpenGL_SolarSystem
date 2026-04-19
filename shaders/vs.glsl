attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec2 aTexCoords;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uWorldMatrix;
uniform float uTime;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vWorldPosition;
void main()
{

    gl_Position = uProjectionMatrix*uViewMatrix*uWorldMatrix*vec4(aVertexPosition, 1.0);
    vTexCoords = aTexCoords;
    vTexCoords = vec2(vTexCoords.x, 1-vTexCoords.y);
    // gl_Position = uProjectionMatrix*uViewMatrix*uWorldMatrix*vec4(aTexCoords, 0.0, 1.0);

    vNormal = (uWorldMatrix * vec4(aVertexNormal, 0.0)).xyz;
    vWorldPosition = (uWorldMatrix * vec4(aVertexPosition, 1.0)).xyz;
}
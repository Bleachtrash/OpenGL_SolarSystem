attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec2 aTexCoords;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uWorldMatrix;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vWorldPosition;
void main()
{
    float dr = abs(dot(aVertexNormal, vec3(1, 0, 0)));
    float du = abs(dot(aVertexNormal, vec3(0, 1, 0)));
    float df = abs(dot(aVertexNormal, vec3(0, 0, 1)));
    gl_Position = uProjectionMatrix*uViewMatrix*uWorldMatrix*vec4(aVertexPosition, 1.0);

    vTexCoords = aTexCoords;
    vTexCoords = vec2(vTexCoords.x, 1-vTexCoords.y);

    vNormal = (uWorldMatrix * vec4(aVertexNormal, 0.0)).xyz;
    vWorldPosition = (uWorldMatrix * vec4(aVertexPosition, 1.0)).xyz;
    // vWorldPosition = vec3(1.0, 0, 0)*dr + vec3(0, 1, 0)*du + vec3(0, 0, 1)*df;
}
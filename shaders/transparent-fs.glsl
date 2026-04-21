uniform sampler2D uTexture;
uniform vec3 uCameraPosition;
uniform float uTime;
uniform vec3 uLightPos;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vWorldPosition;

vec4 DirectionLight(vec3 LightDirection)
{
    // vec3 lightPosition = vec3(normalize(uCameraPosition).x*20, 3.0, normalize(uCameraPosition).z*20);
    // diffuse contribution
    // todo #1 normalize the light direction and store in a separate variable
    vec3 nLightDirection = normalize(LightDirection);
    // todo #2 normalize the world normal and store in a separate variable
    vec3 nWorldNormal = normalize(vNormal);
    // todo #3 calculate the lambert term
    float lambert = dot(nLightDirection, nWorldNormal);
    // specular contribution
    // todo #4 in world space, calculate the direction from the surface point to the eye (normalized)
    vec3 eyeDirection = normalize(uCameraPosition-vWorldPosition);
    // todo #5 in world space, calculate the reflection vector (normalized)
    vec3 reflectionDirection = -eyeDirection-(2.0*dot(-eyeDirection, nWorldNormal)*nWorldNormal);
    // todo #6 calculate the phong term
    float phongTerm = dot(nLightDirection, reflectionDirection);
    phongTerm = clamp(phongTerm, 0.0, 1.0);
    phongTerm = pow(phongTerm, 64.0);
    // combine
    // todo #7 apply light and material interaction for diffuse value by using the texture color as the material
    vec4 textureColor = texture2D(uTexture, vTexCoords);
    textureColor = vec4(textureColor.xyz, length(textureColor.xyz)/length(vec3(1, 1, 1)));
    vec4 diffuse = vec4(textureColor.xyz*lambert, textureColor.w);
    // todo #8 apply light and material interaction for phong, assume phong material color is (0.3, 0.3, 0.3)
    vec4 specular = vec4(0.3, 0.3, 0.3, 1.0)*phongTerm;

    vec4 albedo = textureColor;

    vec4 ambient = albedo*0.5;

    // todo #9
    // add "diffuseColor" and "specularColor" when ready
    vec4 finalColor = ambient + diffuse;

    return finalColor;
}
void main()
{
    vec4 color = DirectionLight(uLightPos-vWorldPosition);
    gl_FragColor = color;
}
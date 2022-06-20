#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../2022CGFinalProject/texture_mapping.h"


const char* vsCodeBase =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPosition;\n"
    "layout(location = 1) in vec3 aNormal;\n"
    "layout(location = 2) in vec2 aTexCoord;\n"

    "out vec3 fPosition;\n"
    "out vec3 fNormal;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"


    "void main() {\n"
    "    fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
    "    fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
    "    gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
    "}\n";


const char* fsCodeBase =
    "#version 330 core\n"
    "in vec3 fPosition;\n"
    "in vec3 fNormal;\n"
    "out vec4 color;\n"

    // material data structure declaration
    "struct Material {\n"
    "    vec3 ka;\n"
    "    vec3 kd;\n"
    "    vec3 ks;\n"
    "    float ns;\n"
    "};\n"

    // ambient light data structure declaration
    "struct AmbientLight {\n"
    "    vec3 color;\n"
    "    float intensity;\n"
    "};\n"

    // directional light data structure declaration
    "struct DirectionalLight {\n"
    "    vec3 direction;\n"
    "    float intensity;\n"
    "    vec3 color;\n"
    "};\n"

    // spot light data structure declaration
    "struct SpotLight {\n"
    "    vec3 position;\n"
    "    vec3 direction;\n"
    "    float intensity;\n"
    "    vec3 color;\n"
    "    float angle;\n"
    "    float kc;\n"
    "    float kl;\n"
    "    float kq;\n"
    "};\n"

    // uniform variables
    "uniform Material material;\n"
    "uniform AmbientLight ambientLight;\n"
    "uniform DirectionalLight directionalLight;\n"
    "uniform SpotLight spotLight;\n"
    "uniform vec3 viewPos;\n"

    "vec3 calcDirectionalLight(vec3 normal) {\n"
    "    vec3 lightDir = normalize(-directionalLight.direction);\n"
    "    vec3 diffuse = directionalLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;\n"
    "    return directionalLight.intensity * diffuse ;\n"
    "}\n"

    "vec3 calcSpotLight(vec3 normal) {\n"
    "    vec3 lightDir = normalize(spotLight.position - fPosition);\n"
    "    float theta = acos(-dot(lightDir, normalize(spotLight.direction)));\n"
    "    if (theta > spotLight.angle) {\n"
    "        return vec3(0.0f, 0.0f, 0.0f);\n"
    "    }\n"
    "    vec3 diffuse = spotLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;\n"
    "    float distance = length(spotLight.position - fPosition);\n"
    "    float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);\n"
    "    return spotLight.intensity * attenuation * diffuse;\n"
    "}\n"

    "vec3 calcSpotSpecular(vec3 normal) {\n"
    "    vec3 viewDir = normalize(viewPos - fPosition);\n"
    "    vec3 lightDir = normalize(spotLight.position - fPosition);\n"
    "    float theta = acos(-dot(lightDir, normalize(spotLight.direction)));\n"
    "    if (theta > spotLight.angle) {\n"
    "        return vec3(0.0f, 0.0f, 0.0f);\n"
    "    }\n"
    "    vec3 reflectDir = reflect(-lightDir,normal);\n"
    "    float distance = length(spotLight.position - fPosition);\n"
    "    float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);\n"
    "    vec3 specular = pow(max(dot(viewDir, reflectDir), 0),material.ns) * spotLight.color * material.ks;\n"
    "    return specular * spotLight.intensity * attenuation;\n"
    "}\n"

    "vec3 calcDirectionalSpecular(vec3 normal) {\n"
    "    vec3 viewDir = normalize(viewPos - fPosition);\n"
    "    vec3 lightDir = normalize(-directionalLight.direction);\n"
    "    vec3 reflectDir = reflect(-lightDir,normal);\n"
    "    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0f),material.ns) * directionalLight.color * material.ks;\n"
    "    return specular * directionalLight.intensity;\n"
    "}\n"

    "void main() {\n"
    "    vec3 ambient = material.ka * ambientLight.color * ambientLight.intensity;\n"
    "    vec3 normal = normalize(fNormal);\n"
    "    vec3 diffuse = calcDirectionalLight(normal) + calcSpotLight(normal);\n"
    "    vec3 specular = calcSpotSpecular(normal)+calcDirectionalSpecular(normal);\n"

    "    color = vec4(ambient+diffuse+specular, 1.0f);\n"
    "}\n";

//几何变换shader
void TextureMapping::initShapeShader() {
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"uniform mat4 model;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"void main(){\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0);\n"
		"}\n";

	const char* fsCode =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main() {\n"
		"   color = vec4(1.0f, 0.870f, 0.0f, 1.0f);\n"
		"}\n";

	_transformShader.reset(new GLSLProgram);
	_transformShader->attachVertexShader(vsCode);
	_transformShader->attachFragmentShader(fsCode);
	_transformShader->link();
}


void TextureMapping::initPhongShader() {
	_universalPhongShader.reset(new GLSLProgram);
	_universalPhongShader->attachVertexShader(vsCodeBase);
	_universalPhongShader->attachFragmentShader(fsCodeBase);
	_universalPhongShader->link();
}

//混合材质shader
void TextureMapping::initTextureShader() {
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"
		"out vec3 fPosition;\n"
		"out vec3 fNormal;\n"
		"out vec2 fTexCoord;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"

		"void main() {\n"
		"	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	fTexCoord = aTexCoord;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";


	const char* fsCode =
		"#version 330 core\n"
		"in vec3 fPosition;\n"
		"in vec3 fNormal;\n"
		"in vec2 fTexCoord;\n"
		"out vec4 color;\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	vec3 color;\n"
		"	float intensity;\n"
		"};\n"


		"struct Material {\n"
		"	vec3 kds[2];\n"
		"	float blend;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight light;\n"
		"uniform sampler2D mapKds[2];\n"

		"vec3 calcDirectionalLightone(vec3 normal) {\n"
		"	vec3 lightDir = normalize(-light.direction);\n"
		"	vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * material.kds[0];\n"
		"	return light.intensity * diffuse ;\n"
		"}\n"

		"vec3 calcDirectionalLighttwo(vec3 normal) {\n"
		"	vec3 lightDir = normalize(-light.direction);\n"
		"	vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * material.kds[1];\n"
		"	return light.intensity * diffuse ;\n"
		"}\n"

		"void main() {\n"
		"	vec3 normal = normalize(fNormal);\n"
		"	vec3 diffuseone = calcDirectionalLightone(normal); \n"
		"	vec3 diffusetwo = calcDirectionalLighttwo(normal); \n"

		"	color = mix(texture(mapKds[0], fTexCoord) * vec4(diffuseone, 1.0), texture(mapKds[1], fTexCoord) * vec4(diffusetwo, 1.0), material.blend );\n"
		"}\n";

	_textureShader.reset(new GLSLProgram);
	_textureShader->attachVertexShader(vsCode);
	_textureShader->attachFragmentShader(fsCode);
	_textureShader->link();
}

void TextureMapping::initLineShader(){
    const char* lineVsCode =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPosition;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "void main() {\n"
        "    gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
        "}\n";

    const char* lineFsCode =
        "#version 330 core\n"
        "out vec4 color;\n"
        "struct Material {\n"
        "    vec3 color;\n"
        "};\n"
        "uniform Material material;\n"
        "void main() { \n"
        "    color = vec4(material.color, 1.0f);\n"
        "}\n";

    _lineShader.reset(new GLSLProgram);
    _lineShader->attachVertexShader(lineVsCode);
    _lineShader->attachFragmentShader(lineFsCode);
    _lineShader->link();
}


void TextureMapping::initDepthMapShader() {
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"

		"uniform mat4 lightSpaceMatrix;\n"
		"uniform mat4 model;\n"

		"void main()\n"
		"{\n"
		"	gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);\n"
		"}\n";

	const char* fsCode =
		"#version 330 core\n"

		"void main()\n"
		"{\n"
		"	// gl_FragDepth = gl_FragCoord.z;\n"
		"}";

	_depthmapShader.reset(new GLSLProgram);
	_depthmapShader->attachVertexShader(vsCode);
	_depthmapShader->attachFragmentShader(fsCode);
	_depthmapShader->link();
}

void TextureMapping::initShadowShader() {
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 texCoords;\n"

		"out vec2 TexCoords;\n"

		"out VS_OUT{\n"
		"	vec3 FragPos;\n"
		"	vec3 Normal;\n"
		"	vec2 TexCoords;\n"
		"	vec4 FragPosLightSpace;\n"
		"} vs_out;\n"

		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"uniform mat4 lightSpaceMatrix;\n"

		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(position, 1.0f);\n"
		"	vs_out.FragPos = vec3(model * vec4(position, 1.0));\n"
		"	vs_out.Normal = transpose(inverse(mat3(model))) * normal;\n"
		"	vs_out.TexCoords = texCoords;\n"
		"	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);\n"
		"}\n";


	const char* fsCode =
		"#version 330 core\n"
		"out vec4 FragColor;\n"

		"in VS_OUT{\n"
		"	vec3 FragPos;\n"
		"	vec3 Normal;\n"
		"	vec2 TexCoords;\n"
		"	vec4 FragPosLightSpace;\n"
		"} fs_in;\n"

		"uniform sampler2D shadowMap;\n"
		"uniform sampler2D diffuseTexture;\n"

		"uniform vec3 lightPos;\n"
		"uniform vec3 viewPos;\n"

		"float ShadowCalculation(vec4 fragPosLightSpace)\n"
		"{\n"
		"	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;\n"
		"	projCoords = projCoords * 0.5 + 0.5;\n"
		//"	float closestDepth = texture(shadowMap, projCoords.xy).r;\n"
		"	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);"
		"	float currentDepth = projCoords.z;\n"
		"	float shadow = 0.0f;\n"
		//"	float shadow = currentDepth - 0.0001 > closestDepth ? 0.8 : 0.0;\n"
		"	for (int x = -1; x <= 1; ++x)\n"
		"	{\n"
		"		for (int y = -1; y <= 1; ++y)\n"
		"		{\n"
		"			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;\n"
		"			shadow += currentDepth - 0.00012 > pcfDepth ? 0.7 : 0.0;\n"
		"		}\n"
		"	}\n"
		"	shadow /= 9.0;\n"

		"	return shadow;\n"
		"}\n"

		"void main()\n"
		"{\n"
		//"	vec3 color = Texture2D(diffuseTexture, TexCoords);\n"
		"	vec3 color = vec3(1.0);\n"
		"	vec3 normal = normalize(fs_in.Normal);\n"
		"	vec3 lightColor = vec3(1.0);\n"
		"	vec3 ambient = 0.15 * lightColor;\n"
		"	vec3 lightDir = normalize(lightPos - fs_in.FragPos);\n"
		"	float diff = max(dot(lightDir, normal), 0.0);\n"
		"	vec3 diffuse = diff * lightColor;\n"
		"	vec3 viewDir = normalize(viewPos - fs_in.FragPos);\n"
		"	vec3 reflectDir = reflect(-lightDir, normal);\n"
		"	float spec = 0.0;\n"
		"	vec3 halfwayDir = normalize(lightDir + viewDir);\n"
		"	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);\n"
		"	vec3 specular = spec * lightColor;\n"
		"	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);\n"
		"	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;\n"

		"	FragColor = vec4(lighting, 1.0f);\n"
		"}\n";


	_shadowShader.reset(new GLSLProgram);
	_shadowShader->attachVertexShader(vsCode);
	_shadowShader->attachFragmentShader(fsCode);
	_shadowShader->link();
}


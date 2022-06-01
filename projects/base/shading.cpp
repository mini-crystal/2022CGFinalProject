#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../2022CGFinalProject/texture_mapping.h"

//几何变换shader
void TextureMapping::initTransformShader() {
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

//3种材质shader

void TextureMapping::initAmbientShader() {
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";

	const char* fsCode =
		"#version 330 core\n"
		"out vec4 color;\n"

		"// material data structure declaration\n"
		"struct Material {\n"
		"	vec3 ka;\n"
		"};\n"

		"// ambient light data structure declaration\n"
		"struct AmbientLight {\n"
		"	vec3 color;\n"
		"	float intensity;\n"
		"};\n"

		"// uniform variables\n"
		"uniform Material material;\n"
		"uniform AmbientLight ambientLight;\n"

		"void main() {\n"
		"	vec3 ambient = material.ka * ambientLight.color * ambientLight.intensity;\n"
		"	color = vec4(ambient, 1.0f);\n"
		"}\n";

	_ambientShader.reset(new GLSLProgram);
	_ambientShader->attachVertexShader(vsCode);
	_ambientShader->attachFragmentShader(fsCode);
	_ambientShader->link();
}

void TextureMapping::initLambertShader() {
	const char* vsCode =
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
		"	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";

	const char* fsCode =
		"#version 330 core\n"
		"in vec3 fPosition;\n"
		"in vec3 fNormal;\n"
		"out vec4 color;\n"

		"// material data structure declaration\n"
		"struct Material {\n"
		"	vec3 kd;\n"
		"};\n"

		"// directional light data structure declaration\n"
		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"// spot light data structure declaration\n"
		"struct SpotLight {\n"
		"	vec3 position;\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"	float angle;\n"
		"	float kc;\n"
		"	float kl;\n"
		"	float kq;\n"
		"};\n"

		"// uniform variables\n"
		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"
		"uniform SpotLight spotLight;\n"

		"vec3 calcDirectionalLight(vec3 normal) {\n"
		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	vec3 diffuse = directionalLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;\n"
		"	return directionalLight.intensity * diffuse ;\n"
		"}\n"

		"vec3 calcSpotLight(vec3 normal) {\n"
		"	vec3 lightDir = normalize(spotLight.position - fPosition);\n"
		"	float theta = acos(-dot(lightDir, normalize(spotLight.direction)));\n"
		"	if (theta > spotLight.angle) {\n"
		"		return vec3(0.0f, 0.0f, 0.0f);\n"
		"	}\n"
		"	vec3 diffuse = spotLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;\n"
		"	float distance = length(spotLight.position - fPosition);\n"
		"	float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);\n"
		"	return spotLight.intensity * attenuation * diffuse;\n"
		"}\n"

		"void main() {\n"
		"	vec3 normal = normalize(fNormal);\n"
		"	vec3 diffuse = calcDirectionalLight(normal) + calcSpotLight(normal);\n"
		"	color = vec4(diffuse, 1.0f);\n"
		"}\n";

	_lambertShader.reset(new GLSLProgram);
	_lambertShader->attachVertexShader(vsCode);
	_lambertShader->attachFragmentShader(fsCode);
	_lambertShader->link();
}

void TextureMapping::initPhongShader() {
	const char* vsCode =
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
		"	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";


    const char* fsCode =
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

	_phongShader.reset(new GLSLProgram);
	_phongShader->attachVertexShader(vsCode);
	_phongShader->attachFragmentShader(fsCode);
	_phongShader->link();
}


void TextureMapping::initSimpleShader() {
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"
		"out vec2 fTexCoord;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"

		"void main() {\n"
		"	fTexCoord = aTexCoord;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";

	const char* fsCode =
		"#version 330 core\n"
		"in vec2 fTexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D mapKd;\n"
		"void main() {\n"
		"	color = texture(mapKd, fTexCoord);\n"
		"}\n";

	_simpleShader.reset(new GLSLProgram);
	_simpleShader->attachVertexShader(vsCode);
	_simpleShader->attachFragmentShader(fsCode);
	_simpleShader->link();
}

//混合材质shader
void TextureMapping::initBlendShader() {
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

	_blendShader.reset(new GLSLProgram);
	_blendShader->attachVertexShader(vsCode);
	_blendShader->attachFragmentShader(fsCode);
	_blendShader->link();
}

//程序纹理shader
void TextureMapping::initCheckerShader() {
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"
		"out vec2 fTexCoord;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"void main() {\n"
		"	fTexCoord = aTexCoord;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";

	const char* fsCode =
		"#version 330 core\n"
		"in vec2 fTexCoord;\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 colors[2];\n"
		"	int repeat;\n"
		"};\n"

		"uniform Material material;\n"

		"void main() {\n"
		"	float intervel = 1.0 / material.repeat;\n"
		"	int i, j;\n"
		"	int flag;\n"
		"	for(i = 0; i<  material.repeat; i++){\n"
		"		for(j =0; j<  material.repeat; j++){\n"
		"			if(fTexCoord.x >= i*intervel && fTexCoord.x < (i+1)*intervel && fTexCoord.y >= j*intervel && fTexCoord.y < (j+1)*intervel){\n"
		"				flag = i + j;}\n"
		"							}\n"
		"						}\n"

		"	if(flag % 2 == 0){\n"
		"		color = vec4(material.colors[1], 1.0f);}\n"
		"	else{\n"
		"		color = vec4(material.colors[0], 1.0f);}\n"


		"}\n";


	_checkerShader.reset(new GLSLProgram);
	_checkerShader->attachVertexShader(vsCode);
	_checkerShader->attachFragmentShader(fsCode);
	_checkerShader->link();
}

//void TextureMapping::initGroundShader(){
//    _groundShader.reset(new GLSLProgram);
//    _groundShader->link();
//}

void TextureMapping::initWallShader(){
    const char* vsCode =
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


    const char* fsCode =
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

    _wallShader.reset(new GLSLProgram);
    _wallShader->attachVertexShader(vsCode);
    _wallShader->attachFragmentShader(fsCode);
    _wallShader->link();
}

void TextureMapping::initAnimationShader() {
    const char* vsCode =
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
        "	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
        "	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
        "	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
        "}\n";


    const char* fsCode =
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

    _animationShader.reset(new GLSLProgram);
    _animationShader->attachVertexShader(vsCode);
    _animationShader->attachFragmentShader(fsCode);
    _animationShader->link();
}

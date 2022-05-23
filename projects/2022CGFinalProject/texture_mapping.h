#pragma once

#include <memory>
#include <string>
#include <vector>
#include <imgui.h>

#include "../base/application.h"
#include "../base/model.h"
#include "../base/light.h"
#include "../base/glsl_program.h"
#include "../base/texture.h"
#include "../base/camera.h"
#include "../base/skybox.h"
#include "../base/cube.h"
#include "../base/tetrahedron.h"

enum class ShaderRenderMode {
	Ambient, Lambert, Phong
};

// I = ka * Ia
struct AmbientMaterial {
	glm::vec3 ka;
};

// I = sum { kd * Light[i] * cos(theta[i]) }
struct LambertMaterial {
	glm::vec3 kd;
};

// I = ka * Ia + sum { (kd * cos(theta[i]) + ks * cos(theta[i])^ns ) * Light[i] }
struct PhongMaterial {
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	float ns;
};

enum class RenderMode {
	Simple, Blend, Checker
};

struct SimpleMaterial {
	std::shared_ptr<Texture2D> mapKd;
};

struct BlendMaterial {
	glm::vec3 kds[2];
	std::shared_ptr<Texture2D> mapKds[2];
	float blend;
};

struct CheckerMaterial {
	int repeat;
	glm::vec3 colors[2];
};

class TextureMapping : public Application {
public:
	TextureMapping(const Options& options);
	
	~TextureMapping();

private:

	//用obj导入的模型要在这里注册
	std::unique_ptr<Model> _sphere;
	std::unique_ptr<Model> _bunny;
	std::unique_ptr<Model> _bunnycopy;
	std::unique_ptr<Model> _firstcard;
	std::unique_ptr<Model> _firstdeng;
	std::unique_ptr<Model> _firstdesk;
	std::unique_ptr<Model> _firstdeskbottom;
	std::unique_ptr<Model> _firstdisplaybottom;
	std::unique_ptr<Model> _firstmenu;
	std::unique_ptr<Model> _firstfloor;
	std::unique_ptr<Model> _firstupstairs;
	std::unique_ptr<Model> _secondchair;
	std::unique_ptr<Model> _seconddesk;
	std::unique_ptr<Model> _seconddeskbottom;
	std::unique_ptr<Model> _secondfloor;
	std::unique_ptr<Model> _secondscreen;
	std::unique_ptr<Model> _secondsofa;
	






	//———————————几何变换属性————————

	std::unique_ptr<GLSLProgram> _transformShader;

	glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _rotateAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f);

	float _rotateAngles = 0.0f;

	//————————————纹理属性——————————
	
	std::unique_ptr<SimpleMaterial> _simpleMaterial;
	std::unique_ptr<BlendMaterial> _blendMaterial;
	std::unique_ptr<CheckerMaterial> _checkerMaterial;

	

	std::unique_ptr<DirectionalLight> _light;

	std::unique_ptr<GLSLProgram> _simpleShader;
	std::unique_ptr<GLSLProgram> _blendShader;
	std::unique_ptr<GLSLProgram> _checkerShader;

	//顶点绘制图形
	std::unique_ptr<SkyBox> _skybox;
	std::unique_ptr<Cube> _cube;
	std::unique_ptr<Tetrahedron> _tetrahedron;

	enum RenderMode _renderMode = RenderMode::Simple;

	//—————————————材质属性————————————

	// materials
	std::unique_ptr<AmbientMaterial> _ambientMaterial;
	std::unique_ptr<LambertMaterial> _lambertMaterial;
	std::unique_ptr<PhongMaterial> _phongMaterial;

	// shaders
	std::unique_ptr<GLSLProgram> _ambientShader;
	std::unique_ptr<GLSLProgram> _lambertShader;
	std::unique_ptr<GLSLProgram> _phongShader;

	// lights
	std::unique_ptr<AmbientLight> _ambientLight;
	std::unique_ptr<DirectionalLight> _directionalLight;
	std::unique_ptr<SpotLight> _spotLight;

	// camera
	std::unique_ptr<PerspectiveCamera> _camera;

	ShaderRenderMode _shaderrenderMode = ShaderRenderMode::Ambient;

	void initTransformShader();

	// I = ka * albedo
	void initAmbientShader();

	// I = ka * albedo + kd * max(cos<I, n>, 0)
	void initLambertShader();

	// I = ka * albedo + kd * cos<I, n> + ks * (max(cos<R, V>, 0) ^ ns)
	void initPhongShader();

	void initSimpleShader();

	void initBlendShader();

	void initCheckerShader();

	void handleInput() override;

	void renderFrame() override;


};









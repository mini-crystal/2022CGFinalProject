#pragma once

#include <memory>
#include <string>
#include <vector>
#include <imgui.h>

#include "../base/application.h"
#include "../base/model.h"
#include "../base/objmodel.h"
#include "../base/light.h"
#include "../base/glsl_program.h"
#include "../base/texture.h"
#include "../base/camera.h"
#include "../base/skybox.h"
#include "../base/cube.h"
#include "../base/cone.h"
#include "../base/ball.h"
#include "../base/cylinder.h"
#include "../base/prism.h"
#include "../base/prismatictable.h"
#include "../base/tetrahedron.h"
#include "../base/bezier_face.h"
#include "../base/ground.h"

const std::string spheremodelPath = "../../media/sphere.obj";
const std::string bunnymodelpath = "../../media/bunny.obj";
const std::string firstcardPath = "../../media/firstcard.obj";
const std::string firstdengPath = "../../media/firstdeng.obj";
const std::string firstdeskPath = "../../media/firstdesk.obj";
const std::string firstdeskbottomPath = "../../media/firstdeskbottom.obj";
const std::string firstdisplaybottomPath = "../../media/firstdisplaybottom.obj";
const std::string firstfloorPath = "../../media/firstfloor.obj";
const std::string firstupstairsPath = "../../media/firstupstairs.obj";
const std::string firstmenuPath = "../../media/firstmenu.obj";
const std::string seconddeskPath = "../../media/seconddesk.obj";
const std::string seconddeskbottomPath = "../../media/seconddeskbottom.obj";
const std::string secondfloorPath = "../../media/secondfloor.obj";
const std::string secondscreenPath = "../../media/secondscreen.obj";
const std::string secondsofaPath = "../../media/secondsofa.obj";
const std::string secondchairPath = "../../media/secondchair.obj";

const std::string animationPath_1 = "../../media/animation1.obj";
const std::string animationPath_2 = "../../media/animation2.obj";
const std::string animationPath_3 = "../../media/animation3.obj";

//自建加载库加载UNO COFFEE
const std::string unoPath = "../../media/UNOtitle.obj";

const std::string earthTexturePath = "../../media/earthmap.jpg";
const std::string woodTexturePath = "../../media/wood.jpg";
const std::string planetTexturePath = "../../media/planet_Quom1200.png";
const std::string wallTexturePath="../../media/wall.png";

const std::vector<std::string> skyboxTexturePaths = {
    "../../media/starfield/Right_Tex.jpg",
    "../../media/starfield/Left_Tex.jpg",
    "../../media/starfield/Down_Tex.jpg",
    "../../media/starfield/Up_Tex.jpg",
    "../../media/starfield/Front_Tex.jpg",
    "../../media/starfield/Back_Tex.jpg"
};

enum class ShaderRenderMode {
	Ambient, Lambert, Phong
};

enum class ShapeType {
    Tetrahedron, Cube, Ball, Cone, Cylinder, Prism, Prismatictable, BezierFace
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

struct LineMaterial {
    glm::vec3 color;
    float width;
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
    
    //UNO Model Objects
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
	std::unique_ptr<Model> _animation1;
	std::unique_ptr<Model> _animation2;
	std::unique_ptr<Model> _animation3;

	std::unique_ptr<ObjModel> _unotitle;
    
    void InitializeModel();
    void InitScale();
    void InitMaterial();
    void InitLight();
    void InitTexture();
    void InitCamera();
    void HandleMouse();
    void InitAllShader();
    void InitImGui();
    void drawUI();
	void capture();
	//———————————几何变换属性————————

	std::unique_ptr<GLSLProgram> _transformShader;

	glm::vec3 _position = glm::vec3(0.3f, 0.296f, 0.7f);//position of Vertex Shape
	glm::vec3 _rotateAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f);

	float _rotateAngles = 0.0f;
    bool _wireframe = false;// some options related to imGUI
    bool _showBoundingBox = false;
    bool _zoomFit = false;
    float _cameraRotateAngles = 155.0f;
    float _AxisX=0.0f;
    float _AxisY=0.0f;
    float _AxisZ=0.47f;
	int count = 0;
	//————————————纹理属性——————————
	
	std::unique_ptr<SimpleMaterial> _simpleMaterial;
	std::unique_ptr<BlendMaterial> _blendMaterial;
	std::unique_ptr<CheckerMaterial> _checkerMaterial;
    std::unique_ptr<SimpleMaterial> _groundMaterial;

	std::unique_ptr<DirectionalLight> _light;

	std::unique_ptr<GLSLProgram> _simpleShader;
	std::unique_ptr<GLSLProgram> _blendShader;
	std::unique_ptr<GLSLProgram> _checkerShader;

	//顶点绘制图形
	std::unique_ptr<SkyBox> _skybox;
	std::unique_ptr<Cube> _cube;
	std::unique_ptr<Tetrahedron> _tetrahedron;
	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Cone> _cone;
	std::unique_ptr<Cylinder> _cylinder;
	std::unique_ptr<Prism> _prism;
	std::unique_ptr<Prismatictable> _prismatictable;
	std::unique_ptr<BezierFace> _bezierFace;
    std::unique_ptr<Ground> _ground;

	enum RenderMode _renderMode = RenderMode::Blend;
    enum ShapeType _shapeType = ShapeType::Tetrahedron;

	//—————————————材质属性————————————
    // 初始化纹理贴图
    std::shared_ptr<Texture2D> earthTexture = std::make_shared<Texture2D>(earthTexturePath);
    std::shared_ptr<Texture2D> woodTexture = std::make_shared<Texture2D>(woodTexturePath);
    std::shared_ptr<Texture2D> wallTexture = std::make_shared<Texture2D>(wallTexturePath);
    
	// materials
	std::unique_ptr<AmbientMaterial> _ambientMaterial;
	std::unique_ptr<LambertMaterial> _lambertMaterial;
	std::unique_ptr<PhongMaterial> _phongMaterial;
    std::unique_ptr<LineMaterial> _lineMaterial;

	// shaders for mode
	std::unique_ptr<GLSLProgram> _ambientShader;
	std::unique_ptr<GLSLProgram> _lambertShader;
	std::unique_ptr<GLSLProgram> _phongShader;
    
    //shader for boundingBox
    std::unique_ptr<GLSLProgram> _lineShader;
    
    //shaders for objects
    std::unique_ptr<GLSLProgram> _groundShader;
    std::unique_ptr<GLSLProgram> _wallShader;
	std::unique_ptr<GLSLProgram> _animationShader;
    std::unique_ptr<GLSLProgram> _displayShader;
	// lights
	std::unique_ptr<AmbientLight> _ambientLight;
	std::unique_ptr<DirectionalLight> _directionalLight;
	std::unique_ptr<SpotLight> _spotLight;

	// camera
	std::unique_ptr<PerspectiveCamera> _camera;

	ShaderRenderMode _shaderrenderMode = ShaderRenderMode::Phong;

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
    
    void initDisplayShader();
    
    void initWallShader();
    
	void initAnimationShader();

    void initLineShader();
    
    bool CheckBoundingBox(BoundingBox box,glm::mat4 ModelMatrix);
    
	void handleInput() override;

	void renderFrame() override;


};









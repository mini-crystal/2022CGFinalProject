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
const std::string door = "../../media/door.obj";
const std::string unoPath = "../../media/UNOtitle.obj";

const std::string animationPath_1 = "../../media/Moonwalk_0000.obj";
const std::string animationPath_2 = "../../media/Moonwalk_0003.obj";
const std::string animationPath_3 = "../../media/Moonwalk_0006.obj";
const std::string animationPath_4 = "../../media/Moonwalk_0009.obj";
const std::string animationPath_5 = "../../media/Moonwalk_0012.obj";
const std::string animationPath_6 = "../../media/Moonwalk_0015.obj";
const std::string animationPath_7 = "../../media/Moonwalk_0018.obj";
const std::string animationPath_8 = "../../media/Moonwalk_0021.obj";
const std::string animationPath_9 = "../../media/Moonwalk_0024.obj";
const std::string animationPath_10 = "../../media/Moonwalk_0027.obj";
const std::string animationPath_11 = "../../media/Moonwalk_0030.obj";

const std::string earthTexturePath = "../../media/earthmap.jpg";
const std::string woodTexturePath = "../../media/wood.jpg";

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

enum class CameraMode {
    Pan, Orbit, FreeRotation
};

enum class ShapeType {
    Tetrahedron, Cube, Ball, Cone, Cylinder, Prism, Prismatictable, BezierFace
};

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

class TextureMapping : public Application {
public:
	TextureMapping(const Options& options);
	~TextureMapping();

private:
    //UNO Model Objects
    std::unique_ptr<Model> _world;
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
    std::unique_ptr<Model> _door;
    
    struct animation{
        std::unique_ptr<Model> frame[12];
    }danceMan;

    //Obj for output
	std::unique_ptr<ObjModel> _unotitle;

    //properties of Vertex Shape
	glm::vec3 _shapePosition;
	glm::vec3 _shapeRotateAxis;
	glm::vec3 _shapeScale;
    
    //animation frame count
	int animationSwitch = 1;

    //draw mode
    bool _wireframe = false;
    bool _showBoundingBox = false;
    bool _shadow = false;
    
    //camera orbit
    glm::mat4 _projection;
    glm::mat4 _view;
    glm::mat4 _model;
    float _cameraRotateAngles = 155.0f;
    float _AxisX=0.0f;
    float _AxisY=0.0f;
    float _AxisZ=0.47f;
    float _orbitAngle=0.0f;
    
    //capture count
	int count = 0;
    
    //screen lock
    bool _lockMouse=false;
    
	//ptr to Vertex Shape
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

    // ptr to texture
    std::shared_ptr<Texture2D> earthTexture = std::make_shared<Texture2D>(earthTexturePath);
    std::shared_ptr<Texture2D> woodTexture = std::make_shared<Texture2D>(woodTexturePath);
	std::shared_ptr<DepthMap> depthMap = std::make_shared<DepthMap>();
    
	// ptr to materials
	std::unique_ptr<PhongMaterial> _phongMaterial;
    std::unique_ptr<LineMaterial> _lineMaterial;
    std::unique_ptr<BlendMaterial> _woodMaterial;
    std::unique_ptr<SimpleMaterial> _groundMaterial;

	//shaders for mode
	std::unique_ptr<GLSLProgram> _universalPhongShader;

	//shader for DepthMap
	std::unique_ptr<GLSLProgram> _depthmapShader;
	std::unique_ptr<GLSLProgram> _shadowShader;
    
    //shaders for mode
    std::unique_ptr<GLSLProgram> _textureShader;
    std::unique_ptr<GLSLProgram> _transformShader;
    std::unique_ptr<GLSLProgram> _lineShader;
    
	// lights
	std::unique_ptr<AmbientLight> _ambientLight;
	std::unique_ptr<DirectionalLight> _directionalLight;
	std::unique_ptr<SpotLight> _spotLight;
    std::unique_ptr<DirectionalLight> _light;
	std::unique_ptr<DirectionalLight> _shadowLight;

	// camera
	std::unique_ptr<PerspectiveCamera> _camera;

    //mode
	enum ShaderRenderMode _shaderrenderMode = ShaderRenderMode::Phong;
    enum CameraMode _cameraMode = CameraMode::Pan;
    enum RenderMode _renderMode = RenderMode::Blend;
    enum ShapeType _shapeType = ShapeType::Tetrahedron;

    //initial function
    void InitializeModel();
    void InitObject();
    void InitMaterial();
    void InitLight();
    void InitTexture();
    void InitCamera();
    void initShapeShader();
    void initAmbientShader();
    void initLambertShader();
    void initPhongShader();
    void initSimpleShader();
    void initTextureShader();
    void initCheckerShader();
    void initDisplayShader();
    void initWallShader();
	void initDepthMapShader();
	void initShadowShader();
    void initAnimationShader();
    void initDoorShader();
    void initLineShader();
    void InitAllShader();
    void InitImGui();
    
    void drawAllObject();
    void HandleMouse();
    void drawUI();
    void capture();
    bool CheckBoundingBox(BoundingBox box,glm::mat4 ModelMatrix);
	void handleInput() override;
	void renderFrame() override;
	void renderDepthMap();
    void resetPhongShader();
};

inline void TextureMapping::drawAllObject(){
    _firstcard->draw();
    _firstmenu->draw();
    _secondfloor->draw();
    _secondsofa->draw();
    _secondscreen->draw();
    _unotitle->draw();
    _firstdisplaybottom->draw();
    _firstdesk->draw();
    _firstdeng->draw();
    _seconddesk->draw();
    _secondchair->draw();
    _seconddeskbottom->draw();
    _firstfloor->draw();
    _firstupstairs->draw();
    _door->draw();
}

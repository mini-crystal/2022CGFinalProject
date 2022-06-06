#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "texture_mapping.h"

TextureMapping::TextureMapping(const Options& options): Application(options) {
    InitializeModel();
    InitScale();
    InitMaterial();
    InitLight();
    InitTexture();
    InitCamera();
    HandleMouse();
    InitAllShader();
    InitImGui();
}

TextureMapping::~TextureMapping() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void TextureMapping::InitializeModel(){
    // 加载模型
    _sphere.reset(new Model(spheremodelPath));

    _firstcard.reset(new Model(firstcardPath));
    _firstdeng.reset(new Model(firstdengPath));
    _firstdeskbottom.reset(new Model(firstdeskbottomPath));
    _firstdisplaybottom.reset(new Model(firstdisplaybottomPath));
    _firstfloor.reset(new Model(firstfloorPath));
    _firstmenu.reset(new Model(firstmenuPath));
    _firstupstairs.reset(new Model(firstupstairsPath));
    _secondchair.reset(new Model(secondchairPath));
    _seconddesk.reset(new Model(seconddeskPath));
    _seconddeskbottom.reset(new Model(seconddeskbottomPath));
    _secondfloor.reset(new Model(secondfloorPath));
    _secondscreen.reset(new Model(secondscreenPath));
    _secondsofa.reset(new Model(secondsofaPath));
    _firstdesk.reset(new Model(firstdeskPath));
    
    // 顶点模型初始化
    _skybox.reset(new SkyBox(skyboxTexturePaths));
    _cube.reset(new Cube());
    _ground.reset(new Ground());
    _tetrahedron.reset(new Tetrahedron());
	_ball.reset(new Ball());
	_cone.reset(new Cone());
	_cylinder.reset(new Cylinder());
	_prism.reset(new Prism());
	_prismatictable.reset(new Prismatictable());

    _animation1.reset(new Model(animationPath_1));
    _animation2.reset(new Model(animationPath_2));
    _animation3.reset(new Model(animationPath_3));

    _unotitle.reset(new ObjModel(unoPath));
}

void TextureMapping::InitScale(){
    _sphere->scale = glm::vec3(0.3f, 0.3f, 0.3f);
}

void TextureMapping::InitMaterial(){
    _ambientMaterial.reset(new AmbientMaterial);
    _ambientMaterial->ka = glm::vec3(0.03f, 0.03f, 0.03f);

    _lambertMaterial.reset(new LambertMaterial);
    _lambertMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);

    _phongMaterial.reset(new PhongMaterial);
    _phongMaterial->ka = glm::vec3(0.4f,0.34f,0.21f);
    _phongMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);
    _phongMaterial->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    _phongMaterial->ns = 10.0f;
    
    _lineMaterial.reset(new LineMaterial);
    _lineMaterial->color = glm::vec3(0.0f, 1.0f, 0.0f);
    _lineMaterial->width = 1.0f;
}

void TextureMapping::InitLight(){
    _ambientLight.reset(new AmbientLight);
    
    _directionalLight.reset(new DirectionalLight);
    _directionalLight->position = glm::vec3(10.0f, 30.0f, 10.0f);
    _directionalLight->rotation = glm::angleAxis(glm::radians(45.0f), -glm::vec3(1.0f, 1.0f, 1.0f));

    _spotLight.reset(new SpotLight);
    _spotLight->position = glm::vec3(0.0f, 0.0f, 5.0f);
    _spotLight->rotation = glm::vec3(-0.8f, 0.0f, 0.1f);
    
    _light.reset(new DirectionalLight());
    _light->rotation = glm::angleAxis(glm::radians(45.0f), -glm::vec3(1.0f, 1.0f, 1.0f));
}

void TextureMapping::InitTexture(){
    // 初始化材?
    _simpleMaterial.reset(new SimpleMaterial);
    _simpleMaterial->mapKd = wallTexture;

    _blendMaterial.reset(new BlendMaterial);
    _blendMaterial->kds[0] = glm::vec3(1.0f, 1.0f, 1.0f);
    _blendMaterial->kds[1] = glm::vec3(1.0f, 1.0f, 1.0f);
    _blendMaterial->mapKds[0] = woodTexture;
    _blendMaterial->mapKds[1] = earthTexture;
    _blendMaterial->blend = 0.0f;

    _checkerMaterial.reset(new CheckerMaterial);
    _checkerMaterial->repeat = 10;
    _checkerMaterial->colors[0] = glm::vec3(1.0f, 1.0f, 1.0f);
    _checkerMaterial->colors[1] = glm::vec3(0.0f, 0.0f, 0.0f);
}

void TextureMapping::InitCamera(){
    // 初始化摄像机
    _camera.reset(new PerspectiveCamera(
        glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
    _camera->position.y = 0.3f;
    _camera->position.z = 3.0f;
}

void TextureMapping::HandleMouse(){
    //set input mode
    // 这里是把鼠标的输入当作是视角的改变，所以不会有指针出来
    //glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    _mouseInput.move.xOld = _mouseInput.move.xCurrent = 0.5 * _windowWidth;
    _mouseInput.move.yOld = _mouseInput.move.yCurrent = 0.5 * _windowHeight;
    glfwSetCursorPos(_window, _mouseInput.move.xCurrent, _mouseInput.move.yCurrent);
}

void TextureMapping::InitAllShader(){
    //初始化3个控制面板shader   材质+纹理+改变大小
    //几何变换shader
    initTransformShader();

    //材质shader
    initAmbientShader();
    initLambertShader();
    initPhongShader();
    
    //init shader for objects
    initDisplayShader();
    initWallShader();
    initAnimationShader();
    
    //init shader for boundingBox
    initLineShader();
    
    //纹理shader
    initSimpleShader();
    initBlendShader();
    initCheckerShader();
}

void TextureMapping::InitImGui(){
    //初始化imgui面板
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init();
}

bool TextureMapping::CheckBoundingBox(BoundingBox box,glm::mat4 ModelMatrix){
    Camera* camera = _camera.get();
    glm::vec3 BoxMaxPosition=ModelMatrix * glm::vec4(box.max, 0.0f);
    glm::vec3 BoxMinPosition=ModelMatrix * glm::vec4(box.min, 0.0f);
    if(camera->position.x>BoxMaxPosition.x || camera->position.x<BoxMinPosition.x)return false;
    if(camera->position.y>BoxMaxPosition.y || camera->position.y<BoxMinPosition.y)return false;
    if(camera->position.z>BoxMaxPosition.z || camera->position.z<BoxMinPosition.z)return false;
    return true;
}

//接收输入：比如键盘wasd输入
void TextureMapping::handleInput() {
        
    static bool LockMouse=false;
    static bool zoomFitState=false;
    static int onAirFrame=0;
    static float upSpeed=0;
	constexpr float cameraMoveSpeed = 0.5f;
	constexpr float cameraRotateSpeed = 0.2f;
    constexpr float cameraFovySpeed = 0.01f;
    constexpr float gravityFactor = 0.003f;
    Camera* camera = _camera.get();
    glm::vec3 oldPosition=camera->position;
    
    //Zoom-Fit
    if(_zoomFit){
        zoomFitState=true;
        _camera.reset(new PerspectiveCamera(
            glm::radians(80.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
        _camera->position.y = 1.7f;
        _camera->position.z = 3.0f;
                
        glm::vec3 RotationAxis= glm::vec3(_AxisX,_AxisY,_AxisZ);
        float x = RotationAxis.x * sin(glm::radians(_cameraRotateAngles / 2));
        float y = RotationAxis.y * sin(glm::radians(_cameraRotateAngles / 2));
        float z = RotationAxis.z * sin(glm::radians(_cameraRotateAngles / 2));
        float w = cos(glm::radians(_cameraRotateAngles / 2));
        
        _camera->rotation = glm::quat { x,y,z,w };
        return;
    }
    if(!_zoomFit && zoomFitState){
        _cameraRotateAngles = 155.0f;
        _AxisX=0.0f;
        _AxisY=0.0f;
        _AxisZ=0.47f;
        _camera->rotation = glm::quat { 1.0f,0.0f,0.0f,0.0f };
        zoomFitState=false;
    }
    
    //camera fovy
    static float oldScroll = 0.0f;
    float scrollDelta=_mouseInput.scroll.y - oldScroll;
    if(!LockMouse){
        if(_camera->fovy >= glm::radians(20.0f) && _camera->fovy <= glm::radians(70.0f))_camera->fovy -= scrollDelta * cameraFovySpeed;
        if(_camera->fovy <= glm::radians(20.0f))_camera->fovy = glm::radians(20.0f);
        if(_camera->fovy >= glm::radians(70.0f))_camera->fovy = glm::radians(70.0f);
        oldScroll = _mouseInput.scroll.y;
    }
    
    //keyboard
	if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
		glfwSetWindowShouldClose(_window, true);
		return;
	}
	if (_keyboardInput.keyStates[GLFW_KEY_L] == GLFW_PRESS) {
		std::cout << "switch mouse lock state" << std::endl;
        LockMouse=!LockMouse;
		_keyboardInput.keyStates[GLFW_KEY_L] = GLFW_RELEASE;
		return;
	}
    if (_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
        std::cout << "jump" << std::endl;
        upSpeed = 0.1f;
        _keyboardInput.keyStates[GLFW_KEY_SPACE] = GLFW_RELEASE;
        return;
    }
    
	if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) camera->position += cameraMoveSpeed * camera->getFront() *_deltaTime;
	if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) camera->position -= cameraMoveSpeed * camera->getRight() *_deltaTime;
	if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) camera->position -= cameraMoveSpeed * camera->getFront() *_deltaTime;
    if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) camera->position += cameraMoveSpeed * camera->getRight() *_deltaTime;
    
    if (_keyboardInput.keyStates[GLFW_KEY_M] != GLFW_RELEASE) {
        printf("MMM");
        capture();

    }

    //gravity
    onAirFrame++;
    if(onAirFrame)camera->position.y+=
        (upSpeed * onAirFrame - onAirFrame * onAirFrame * gravityFactor)-
        (upSpeed * (onAirFrame-1) - (onAirFrame-1) * (onAirFrame-1) * gravityFactor);
    
    //check if in boundingBox
    if(CheckBoundingBox(_firstdeng->getBoundingBox(), _sphere->getModelMatrix())){
        camera->position=oldPosition;
        onAirFrame=0;
        upSpeed=0;
    }
    //check if touch ground
    if(camera->position.y<=0.2f){
        camera->position.y=0.2f;
        onAirFrame=0;
        upSpeed=0;
    }
    
    //mouse
	if (_mouseInput.move.xCurrent != _mouseInput.move.xOld) {
		//std::cout << "mouse move in x direction" << std::endl;
        static bool firstMouse=true;
        if(firstMouse){
            _mouseInput.move.xOld = _mouseInput.move.xCurrent;
            _mouseInput.move.yOld = _mouseInput.move.yCurrent;
            firstMouse = false;
        }
        float mouse_movement_in_x_direction = (float)_mouseInput.move.xCurrent - (float)_mouseInput.move.xOld;
		const float theta = mouse_movement_in_x_direction * cameraRotateSpeed * _deltaTime;
		if(!LockMouse)camera->rotation = glm::quat(cos(theta * 0.5f), sin(theta * 0.5f) * glm::vec3(0.0f, 1.0f, 0.0f)) * camera->rotation;
		_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	}
	if (_mouseInput.move.yCurrent != _mouseInput.move.yOld) {
		//std::cout << "mouse move in y direction" << std::endl;
        static bool firstMouse=true;
        if(firstMouse){
            _mouseInput.move.xOld = _mouseInput.move.xCurrent;
            _mouseInput.move.yOld = _mouseInput.move.yCurrent;
            firstMouse = false;
        }
        float mouse_movement_in_y_direction = (float)_mouseInput.move.yCurrent - (float)_mouseInput.move.yOld;
		const float theta = mouse_movement_in_y_direction * cameraRotateSpeed * _deltaTime;
        if(!LockMouse)camera->rotation = glm::quat(cos(theta * 0.5f), sin(theta * 0.5f) * camera->getRight()) * camera->rotation;
		_mouseInput.move.yOld = _mouseInput.move.yCurrent;
	}
}

void TextureMapping::drawUI(){
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

    if (!ImGui::Begin("Light Control Panel", nullptr, flags)) {
        ImGui::End();
    }
    else {
        
        ImGui::Text("Adjust Phong rendering:");
        ImGui::Separator();
        ImGui::ColorEdit3("ka##3", (float*)&_phongMaterial->ka);
        ImGui::ColorEdit3("kd##3", (float*)&_phongMaterial->kd);
        ImGui::ColorEdit3("ks##3", (float*)&_phongMaterial->ks);
        ImGui::SliderFloat("ns##3", &_phongMaterial->ns, 1.0f, 50.0f);
        ImGui::NewLine();
        
        ImGui::Text("Adjust light:");
        ImGui::Separator();
        ImGui::Text("ambient light");
        ImGui::SliderFloat("intensity##1", &_ambientLight->intensity, 0.0f, 2.0f);
        ImGui::ColorEdit3("color##1", (float*)&_ambientLight->color);

        ImGui::Text("directional light");
        ImGui::SliderFloat("intensity##2", &_directionalLight->intensity, 0.0f, 1.5f);
        ImGui::ColorEdit3("color##2", (float*)&_directionalLight->color);
        
        
        ImGui::Text("spot light");
        ImGui::SliderFloat("intensity##3", &_spotLight->intensity, 0.0f, 1.5f);
        ImGui::ColorEdit3("color##3", (float*)&_spotLight->color);
        ImGui::SliderFloat("angle##3", (float*)&_spotLight->angle, 0.0f, glm::radians(180.0f), "%f rad");
        ImGui::SliderFloat("positionX##3", &_spotLight->position.x, -10.0, 10.0);
        ImGui::SliderFloat("positionY##3", &_spotLight->position.y, -10.0, 10.0);
        ImGui::SliderFloat("positionZ##3", &_spotLight->position.z, -10.0, 10.0);
        ImGui::NewLine();

        //Choose shape
        ImGui::Text("Choose shape:");
        ImGui::Separator();
        ImGui::RadioButton("Tetrahedron", (int*)&_shapeType, (int)(ShapeType::Tetrahedron));
        ImGui::RadioButton("Cube", (int*)&_shapeType, (int)(ShapeType::Cube));
		ImGui::RadioButton("Ball", (int*)&_shapeType, (int)(ShapeType::Ball));
		ImGui::RadioButton("Cone", (int*)&_shapeType, (int)(ShapeType::Cone));
		ImGui::RadioButton("Cylinder", (int*)&_shapeType, (int)(ShapeType::Cylinder));
		ImGui::RadioButton("Prism", (int*)&_shapeType, (int)(ShapeType::Prism));
		ImGui::RadioButton("Prismatictable", (int*)&_shapeType, (int)(ShapeType::Prismatictable));
        ImGui::Text("Scale");
        ImGui::SliderFloat("transformX", &_scale.x, 0.0, 2.0);
        ImGui::SliderFloat("transformY", &_scale.y, 0.0, 2.0);
        ImGui::SliderFloat("transformZ", &_scale.z, 0.0, 2.0);
        ImGui::Text("Position");
        ImGui::SliderFloat("positionX", &_position.x, -1.0, 1.0);
        ImGui::SliderFloat("positionY", &_position.y, -1.0, 2.0);
        ImGui::SliderFloat("positionZ", &_position.z, -1.0, 1.0);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("rotationX", &_rotateAxis.x, 0.0, 180.0f);
		ImGui::SliderFloat("rotationY", &_rotateAxis.y, 0.0, 180.0f);
		ImGui::SliderFloat("rotationZ", &_rotateAxis.z, 0.0, 180.0f);
        ImGui::NewLine();
        
        //camera
        ImGui::Checkbox("ZoomFitMode", &_zoomFit);
        if(_zoomFit){
            ImGui::Text("Camera:");
            ImGui::Separator();
            ImGui::SliderFloat("Rotate", &_cameraRotateAngles, -180.0, 180.0f);
            ImGui::SliderFloat("X", &_AxisX, -1.0f, 1.0f);
            ImGui::SliderFloat("Y", &_AxisY, -1.0f, 1.0f);
            ImGui::SliderFloat("Z", &_AxisZ, -1.0f, 1.0f);
            ImGui::NewLine();
        }

        //wireframe mode
        ImGui::Checkbox("wireframe", &_wireframe);
        ImGui::Checkbox("boundingBox", &_showBoundingBox);

        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TextureMapping::renderFrame() {

	showFpsInWindowTitle();
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	if (_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//Only PerspectiveCamera available now
    const glm::mat4 projection = _camera->getProjectionMatrix();
    const glm::mat4 view = _camera->getViewMatrix();
    static float scaleDivide=40.0f;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(_position.x, _position.y, _position.z));
    model = glm::rotate(model, glm::radians(_rotateAxis.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(_rotateAxis.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(_rotateAxis.z), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, glm::vec3(_scale.x/scaleDivide, _scale.y/scaleDivide, _scale.z/scaleDivide));

	//draw .obj or Vertex models, but their methods are the same: 1.use shader  2.draw objects
    
    //draw BoundingBox
    if (_showBoundingBox) {
        _lineShader->use();
        _lineShader->setMat4("projection", projection);
        _lineShader->setMat4("view", view);
        _lineShader->setMat4("model", _sphere->getModelMatrix());
        _lineShader->setVec3("material.color", _lineMaterial->color);
        glLineWidth(_lineMaterial->width);
        
        _firstdeng->drawBoundingBox();
        _firstmenu->drawBoundingBox();
        _firstdeng->drawBoundingBox();
        _firstcard->drawBoundingBox();
        _seconddesk->drawBoundingBox();
        _secondsofa->drawBoundingBox();
        _firstfloor->drawBoundingBox();
        _secondfloor->drawBoundingBox();
        _secondchair->drawBoundingBox();
        _secondscreen->drawBoundingBox();
        _firstupstairs->drawBoundingBox();
        _seconddeskbottom->drawBoundingBox();
        
    }
    
    
	//universal shader
    glm::mat4 floorModel = _sphere->getModelMatrix();
    floorModel = glm::translate(floorModel, glm::vec3(0.005f,0.005f, 0.005f));
    _phongShader->use();
    _phongShader->setMat4("projection", _camera->getProjectionMatrix());
    _phongShader->setMat4("view", _camera->getViewMatrix());
    _phongShader->setMat4("model", floorModel);
    _phongShader->setVec3("viewPos", _camera->position);
    _phongShader->setVec3("material.ka", _phongMaterial->ka);
    _phongShader->setVec3("material.kd", _phongMaterial->kd);
    _phongShader->setVec3("material.ks", _phongMaterial->ks);
    _phongShader->setFloat("material.ns", _phongMaterial->ns);
    _phongShader->setVec3("ambientLight.color", _ambientLight->color);
    _phongShader->setFloat("ambientLight.intensity", _ambientLight->intensity);
    _phongShader->setVec3("spotLight.position", _spotLight->position);
    _phongShader->setVec3("spotLight.direction", _spotLight->getFront());
    _phongShader->setFloat("spotLight.intensity", _spotLight->intensity);
    _phongShader->setVec3("spotLight.color", _spotLight->color);
    _phongShader->setFloat("spotLight.angle", _spotLight->angle);
    _phongShader->setFloat("spotLight.kc", _spotLight->kc);
    _phongShader->setFloat("spotLight.kl", _spotLight->kl);
    _phongShader->setFloat("spotLight.kq", _spotLight->kq);
    _phongShader->setVec3("directionalLight.direction", _directionalLight->getFront());
    _phongShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
    _phongShader->setVec3("directionalLight.color", _directionalLight->color);

    _firstcard->draw();
    _firstmenu->draw();
    _secondfloor->draw();
    _secondsofa->draw();
    _secondscreen->draw();
    _unotitle->draw();

    
    //draw display bar
    _displayShader->use();
    _displayShader->setMat4("projection", _camera->getProjectionMatrix());
    _displayShader->setMat4("view", _camera->getViewMatrix());
    _displayShader->setMat4("model", _sphere->getModelMatrix());
    _displayShader->setVec3("viewPos", _camera->position);
    _displayShader->setVec3("material.ka", glm::vec3(0.07f,0.12f,0.31f));
    _displayShader->setVec3("material.kd", glm::vec3(0.12f,0.24f,0.41f));
    _displayShader->setVec3("material.ks", _phongMaterial->ks);
    _displayShader->setFloat("material.ns", _phongMaterial->ns);
    _displayShader->setVec3("ambientLight.color", _ambientLight->color);
    _displayShader->setFloat("ambientLight.intensity", _ambientLight->intensity);
    _displayShader->setVec3("spotLight.position", _spotLight->position);
    _displayShader->setVec3("spotLight.direction", _spotLight->getFront());
    _displayShader->setFloat("spotLight.intensity", _spotLight->intensity);
    _displayShader->setVec3("spotLight.color", _spotLight->color);
    _displayShader->setFloat("spotLight.angle", _spotLight->angle);
    _displayShader->setFloat("spotLight.kc", _spotLight->kc);
    _displayShader->setFloat("spotLight.kl", _spotLight->kl);
    _displayShader->setFloat("spotLight.kq", _spotLight->kq);
    _displayShader->setVec3("directionalLight.direction", _directionalLight->getFront());
    _displayShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
    _displayShader->setVec3("directionalLight.color", _directionalLight->color);
    _firstdisplaybottom->draw();
    
    
    //draw desks
    _blendShader->use();
    _blendShader->setMat4("projection", projection);
    _blendShader->setMat4("view", view);
    _blendShader->setMat4("model", _sphere->getModelMatrix());
    _blendShader->setVec3("light.direction", _directionalLight->getFront());
    _blendShader->setVec3("light.color", _directionalLight->color);
    _blendShader->setFloat("light.intensity", _directionalLight->intensity);
    _blendShader->setVec3("material.kds[0]", _blendMaterial->kds[0]);
    _blendShader->setVec3("material.kds[1]", _blendMaterial->kds[1]);
    _blendShader->setFloat("material.blend", _blendMaterial->blend);
    _blendShader->setInt("mapKds[1]", 1);
    glActiveTexture(GL_TEXTURE0);
    _blendMaterial->mapKds[0]->bind();
    glActiveTexture(GL_TEXTURE1);
    _blendMaterial->mapKds[1]->bind();
	_firstdesk->draw();
    _firstdeng->draw();
    _seconddesk->draw();
    _secondchair->draw();
    _seconddeskbottom->draw();
    
    //draw floor and stairs
    _wallShader->use();
    _wallShader->setMat4("projection", _camera->getProjectionMatrix());
    _wallShader->setMat4("view", _camera->getViewMatrix());
    _wallShader->setMat4("model", _sphere->getModelMatrix());
    _wallShader->setVec3("viewPos", _camera->position);
    _wallShader->setVec3("material.ka", glm::vec3(0.21f,0.21f,0.21f));
    _wallShader->setVec3("material.kd", glm::vec3(0.75f,0.70f,0.44f));
    _wallShader->setVec3("material.ks", glm::vec3(0.76f,0.69f,0.55f));
    _wallShader->setFloat("material.ns", _phongMaterial->ns);
    _wallShader->setVec3("ambientLight.color", _ambientLight->color);
    _wallShader->setFloat("ambientLight.intensity", _ambientLight->intensity);
    _wallShader->setVec3("spotLight.position", _spotLight->position);
    _wallShader->setVec3("spotLight.direction", _spotLight->getFront());
    _wallShader->setFloat("spotLight.intensity", _spotLight->intensity);
    _wallShader->setVec3("spotLight.color", _spotLight->color);
    _wallShader->setFloat("spotLight.angle", _spotLight->angle);
    _wallShader->setFloat("spotLight.kc", _spotLight->kc);
    _wallShader->setFloat("spotLight.kl", _spotLight->kl);
    _wallShader->setFloat("spotLight.kq", _spotLight->kq);
    _wallShader->setVec3("directionalLight.direction", _directionalLight->getFront());
    _wallShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
    _wallShader->setVec3("directionalLight.color", _directionalLight->color);
    
    _firstfloor->draw();
    _firstupstairs->draw();
    
    
    //draw animation
    float transformation = static_cast <float>(sin((double)glfwGetTime()));
    double velocity = 5.0f;
    float trans = transformation * velocity;
    glm::mat4 flowerModel = glm::mat4(1.0f);
    flowerModel = glm::translate(flowerModel, glm::vec3(0.3f, 0.268f, 0.4f));
    flowerModel = glm::scale(flowerModel, glm::vec3(_scale.x/scaleDivide, _scale.y/scaleDivide, _scale.z/scaleDivide));
    
    _animationShader->use();
    _animationShader->setMat4("projection", _camera->getProjectionMatrix());
    _animationShader->setMat4("view", _camera->getViewMatrix());
    _animationShader->setMat4("model", flowerModel);
    _animationShader->setVec3("viewPos", _camera->position);
    _animationShader->setVec3("material.ka", _phongMaterial->ka);
    _animationShader->setVec3("material.kd", _phongMaterial->kd);
    _animationShader->setVec3("material.ks", _phongMaterial->ks);
    _animationShader->setFloat("material.ns", _phongMaterial->ns);
    _animationShader->setVec3("ambientLight.color", _ambientLight->color);
    _animationShader->setFloat("ambientLight.intensity", _ambientLight->intensity);
    _animationShader->setVec3("spotLight.position", _spotLight->position);
    _animationShader->setVec3("spotLight.direction", _spotLight->getFront());
    _animationShader->setFloat("spotLight.intensity", _spotLight->intensity);
    _animationShader->setVec3("spotLight.color", _spotLight->color);
    _animationShader->setFloat("spotLight.angle", _spotLight->angle);
    _animationShader->setFloat("spotLight.kc", _spotLight->kc);
    _animationShader->setFloat("spotLight.kl", _spotLight->kl);
    _animationShader->setFloat("spotLight.kq", _spotLight->kq);
    _animationShader->setVec3("directionalLight.direction", _directionalLight->getFront());
    _animationShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
    _animationShader->setVec3("directionalLight.color", _directionalLight->color);
    
    //tzy change animation
    if (trans <= -3) {
        _animation1->draw();
    }
    else if (trans > -3 && trans < 1) {
        _animation2->draw();
    }
    else {
        _animation3->draw();
    }

    // draw sky box
	_skybox->draw(projection, view);
    
    //draw ground
    _ground->draw(projection, view);
    
    // draw Vertex Shape
    switch (_shapeType){
    case ShapeType::Tetrahedron:
        scaleDivide=40.0f;
        _tetrahedron->draw(model, projection, view);
        break;
    case ShapeType::Cube:
        scaleDivide=25.0f;
        _cube->draw(model,projection, view);
        break;
	case ShapeType::Ball:
		scaleDivide = 25.0f;
		_ball->draw(model, projection, view);
		break;
	case ShapeType::Cone:
		scaleDivide = 25.0f;
		_cone->draw(model, projection, view);
		break;
	case ShapeType::Cylinder:
		scaleDivide = 25.0f;
		_cylinder->draw(model, projection, view);
		break;
	case ShapeType::Prism:
		scaleDivide = 25.0f;
		_prism->draw(model, projection, view);
		break;
	case ShapeType::Prismatictable:
		scaleDivide = 25.0f;
		_prismatictable->draw(model, projection, view);
		break;
    }

    drawUI();

}

void TextureMapping::capture() {
    FILE* pDummyFile;
    FILE* pWritingFile;
    GLubyte* pPixelData;
    GLubyte  BMP_Header[54];
    GLint    i, j;
    GLint    PixelDataLength;

    std::string filename = "uno" + std::to_string(count) + ".bmp";
    count++;
    i = _windowWidth * 3;
    while (i % 4 != 0)
        ++i;
    PixelDataLength = i * _windowHeight;

    pPixelData = (GLubyte*)malloc(PixelDataLength);
    if (pPixelData == 0)
        exit(0);

    pDummyFile = fopen("uno1111.bmp", "rb");
    if (pDummyFile == 0)
        exit(0);

    char* file = (char*)filename.data();
    //printf("%s\n", file);
    pWritingFile = fopen(file, "wb");
    if (pWritingFile == 0)
        exit(0);
    //printf("load new pic\n");
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
    i = _windowWidth;
    j = _windowHeight;
    fwrite(&i, sizeof(i), 1, pWritingFile);
    fwrite(&j, sizeof(j), 1, pWritingFile);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, _windowWidth, _windowHeight,
        GL_BGR, GL_UNSIGNED_BYTE, pPixelData);

    fseek(pWritingFile, 0, SEEK_END);

    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);

    printf("save");
}

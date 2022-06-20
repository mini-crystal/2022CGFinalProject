#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "texture_mapping.h"

TextureMapping::TextureMapping(const Options& options): Application(options) {
    InitializeModel();
    InitObject();
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
    // .obj
    _world.reset(new Model(firstcardPath));
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
    danceMan.frame[1].reset(new Model(animationPath_1));
    danceMan.frame[2].reset(new Model(animationPath_2));
    danceMan.frame[3].reset(new Model(animationPath_3));
    danceMan.frame[4].reset(new Model(animationPath_4));
    danceMan.frame[5].reset(new Model(animationPath_5));
    danceMan.frame[6].reset(new Model(animationPath_6));
    danceMan.frame[7].reset(new Model(animationPath_7));
    danceMan.frame[8].reset(new Model(animationPath_8));
    danceMan.frame[9].reset(new Model(animationPath_9));
    danceMan.frame[10].reset(new Model(animationPath_10));
    danceMan.frame[11].reset(new Model(animationPath_11));
    _door.reset(new Model(door));

    //.obj for output
    _unotitle.reset(new ObjModel(unoPath));
    
    // vertex shape
    _skybox.reset(new SkyBox(skyboxTexturePaths));
    _cube.reset(new Cube());
    _ground.reset(new Ground());
    _tetrahedron.reset(new Tetrahedron());
	_ball.reset(new Ball());
	_cone.reset(new Cone());
	_cylinder.reset(new Cylinder());
	_prism.reset(new Prism());
	_prismatictable.reset(new Prismatictable());
	_bezierFace.reset(new BezierFace());
}

void TextureMapping::InitObject(){
    _world->scale = glm::vec3(0.3f, 0.3f, 0.3f);
    _door->scale = glm::vec3(0.3f,0.3f,0.3f);
    danceMan.frame[1]->scale=glm::vec3(0.005f, 0.005f, 0.005f);
    danceMan.frame[1]->position=glm::vec3(1.5f, 0.0f, 1.0f);
    _firstfloor->scale=glm::vec3(0.3f,0.3f,0.3f);
    _firstfloor->position=glm::vec3(0.005f, 0.005f, 0.005f);
    _shapePosition = glm::vec3(0.3f, 0.296f, 0.7f);
    _shapeRotateAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    _shapeScale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void TextureMapping::InitMaterial(){
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

	_shadowLight.reset(new DirectionalLight);
	_shadowLight->position = glm::vec3(20.0f, 20.0f, 20.0f);
	_shadowLight->rotation = glm::angleAxis(glm::radians(45.0f), -glm::vec3(1.0f, 1.0f, 1.0f));
}

void TextureMapping::InitTexture(){
    _woodMaterial.reset(new BlendMaterial);
    _woodMaterial->kds[0] = glm::vec3(1.0f, 1.0f, 1.0f);
    _woodMaterial->kds[1] = glm::vec3(1.0f, 1.0f, 1.0f);
    _woodMaterial->mapKds[0] = woodTexture;
    _woodMaterial->mapKds[1] = earthTexture;
    _woodMaterial->blend = 0.0f;
}

void TextureMapping::InitCamera(){
    _camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
    _camera->position.y = 0.3f;
    _camera->position.z = 3.0f;
}

void TextureMapping::HandleMouse(){
    //set input mode
    _mouseInput.move.xOld = _mouseInput.move.xCurrent = 0.5 * _windowWidth;
    _mouseInput.move.yOld = _mouseInput.move.yCurrent = 0.5 * _windowHeight;
    glfwSetCursorPos(_window, _mouseInput.move.xCurrent, _mouseInput.move.yCurrent);
}

void TextureMapping::InitAllShader(){
    //init shader for vertex shape
    initShapeShader();

    //init shader for mode
    initPhongShader();
    
    //init shader for boundingBox
    initLineShader();
    
    //init shader for texture
    initTextureShader();

    //init shader for shadow
	initDepthMapShader();
	initShadowShader();
}

void TextureMapping::resetPhongShader(){
    _universalPhongShader->setMat4("projection", _projection);
    _universalPhongShader->setMat4("view", _view);
    _universalPhongShader->setMat4("model", _model);
    _universalPhongShader->setVec3("viewPos", _camera->position);
    _universalPhongShader->setVec3("material.ka", _phongMaterial->ka);
    _universalPhongShader->setVec3("material.kd", _phongMaterial->kd);
    _universalPhongShader->setVec3("material.ks", _phongMaterial->ks);
    _universalPhongShader->setFloat("material.ns", _phongMaterial->ns);
    _universalPhongShader->setVec3("ambientLight.color", _ambientLight->color);
    _universalPhongShader->setFloat("ambientLight.intensity", _ambientLight->intensity);
    _universalPhongShader->setVec3("spotLight.position", _spotLight->position);
    _universalPhongShader->setVec3("spotLight.direction", _spotLight->getFront());
    _universalPhongShader->setFloat("spotLight.intensity", _spotLight->intensity);
    _universalPhongShader->setVec3("spotLight.color", _spotLight->color);
    _universalPhongShader->setFloat("spotLight.angle", _spotLight->angle);
    _universalPhongShader->setFloat("spotLight.kc", _spotLight->kc);
    _universalPhongShader->setFloat("spotLight.kl", _spotLight->kl);
    _universalPhongShader->setFloat("spotLight.kq", _spotLight->kq);
    _universalPhongShader->setVec3("directionalLight.direction", _directionalLight->getFront());
    _universalPhongShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
    _universalPhongShader->setVec3("directionalLight.color", _directionalLight->color);
}

void TextureMapping::InitImGui(){
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

void TextureMapping::handleInput() {
    constexpr float cameraMoveSpeed = 0.5f;
    constexpr float cameraRotateSpeed = 0.2f;
    constexpr float cameraOrbitSpeed = 20.0f;
    constexpr float gravityFactor = 0.003f;
    constexpr float cameraFovySpeed = 0.01f;
    constexpr float orbitRadius = 4.0f;
    static int onAirFrame=0;
    static float upSpeed=0;
    static float orbitAngle=0.0f;
    static enum CameraMode oldCameraMode=CameraMode::Pan;
    Camera* camera = _camera.get();
    glm::vec3 oldPosition=camera->position;
    
    //keyboard: exit
    if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
        glfwSetWindowShouldClose(_window, true);
        return;
    }
    
    //keyboard: capture
    if (_keyboardInput.keyStates[GLFW_KEY_M] == GLFW_PRESS){
        _keyboardInput.keyStates[GLFW_KEY_M] = GLFW_RELEASE;
        capture();
    }
    
    //scroll:zoom
    static float oldScroll = 0.0f;
    float scrollDelta=_mouseInput.scroll.y - oldScroll;
    if(!_lockMouse){
        if(_camera->fovy >= glm::radians(5.0f) && _camera->fovy <= glm::radians(100.0f))_camera->fovy -= scrollDelta * cameraFovySpeed;
        if(_camera->fovy <= glm::radians(5.0f))_camera->fovy = glm::radians(5.0f);
        if(_camera->fovy >= glm::radians(100.0f))_camera->fovy = glm::radians(100.0f);
        oldScroll = _mouseInput.scroll.y;
    }
    
    //keyboard:zoom to fit
    if (_keyboardInput.keyStates[GLFW_KEY_F] == GLFW_PRESS){
        _keyboardInput.keyStates[GLFW_KEY_F] = GLFW_RELEASE;
        _camera->fovy=glm::radians(45.0f);
    }
    
    //Camera Mode:Pan/Orbit/FreeRotation
    switch (_cameraMode) {
        case CameraMode::Pan :
            {
                if(oldCameraMode!=CameraMode::Pan){
                    InitCamera();
                }
                //keyboard: lock camera
                if (_keyboardInput.keyStates[GLFW_KEY_L] == GLFW_PRESS) {
                    std::cout << "switch mouse lock state" << std::endl;
                    _lockMouse=!_lockMouse;
                    _keyboardInput.keyStates[GLFW_KEY_L] = GLFW_RELEASE;
                    return;
                }
                
                //keyboard: jump
                if (_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
                    std::cout << "jump" << std::endl;
                    upSpeed = 0.1f;
                    _keyboardInput.keyStates[GLFW_KEY_SPACE] = GLFW_RELEASE;
                    return;
                }
                
                //keyboard: move
                if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) camera->position += cameraMoveSpeed * camera->getFront() *_deltaTime;
                if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) camera->position -= cameraMoveSpeed * camera->getRight() *_deltaTime;
                if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) camera->position -= cameraMoveSpeed * camera->getFront() *_deltaTime;
                if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) camera->position += cameraMoveSpeed * camera->getRight() *_deltaTime;
                
                //keyboard: open door
                const float openSpeed=0.05f;
                static bool isOpening = false;
                static bool OpenState = false;
                if (_keyboardInput.keyStates[GLFW_KEY_O] == GLFW_PRESS) {
                    _keyboardInput.keyStates[GLFW_KEY_O] = GLFW_RELEASE;
                    isOpening= true;
                }
                if(isOpening){
                    if (!OpenState) {
                        _door->position.x += openSpeed;
                    }
                    else {
                        _door->position.x -= openSpeed;
                    }
                }
                if (_door->position.x >= 2.0f) {
                    _door->position.x = 2.0f;
                    isOpening= false;
                    OpenState = true;
                }
                if (_door->position.x <= 0.0f) {
                    _door->position.x = 0.0f;
                    isOpening= false;
                    OpenState = false;
                }

                //gravity
                onAirFrame++;
                if(onAirFrame)camera->position.y+=
                    (upSpeed * onAirFrame - onAirFrame * onAirFrame * gravityFactor)-
                    (upSpeed * (onAirFrame-1) - (onAirFrame-1) * (onAirFrame-1) * gravityFactor);
                
                //check if in boundingBox
                if (CheckBoundingBox(_door->getBoundingBox(), _model)) {
                    if (OpenState == 0) {
                        camera->position = oldPosition;
                        onAirFrame = 0;
                        upSpeed = 0;
                    }
                }
                
                //check if touch ground
                if(camera->position.y<=0.2f){
                    camera->position.y=0.2f;
                    onAirFrame=0;
                    upSpeed=0;
                }
                
                //mouse:Pan
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
                    if(!_lockMouse)camera->rotation = glm::quat(cos(theta * 0.5f), sin(theta * 0.5f) * glm::vec3(0.0f, 1.0f, 0.0f)) * camera->rotation;
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
                    if(!_lockMouse)camera->rotation = glm::quat(cos(theta * 0.5f), sin(theta * 0.5f) * camera->getRight()) * camera->rotation;
                    _mouseInput.move.yOld = _mouseInput.move.yCurrent;
                }
                oldCameraMode=CameraMode::Pan;
            }
            break;
        case CameraMode::Orbit :
            {
                if(oldCameraMode!=CameraMode::Orbit){
                    camera->position.y=2.5f;
                    _camera->fovy=glm::radians(45.0f);
                }
                camera->position.x=orbitRadius * cos(glm::radians(_orbitAngle));
                camera->position.z=orbitRadius * sin(glm::radians(_orbitAngle));
                
                //keyboard: orbit
                if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) _orbitAngle+= cameraOrbitSpeed * _deltaTime;
                if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) _orbitAngle-= cameraOrbitSpeed * _deltaTime;
                if (_orbitAngle<0.0f)_orbitAngle+=360.0f;
                if (_orbitAngle>360.0f)_orbitAngle-=360.0f;
                
                oldCameraMode=CameraMode::Orbit;
            }
            break;
        case CameraMode::FreeRotation :
            {
                if(oldCameraMode!=CameraMode::FreeRotation){
                    _camera->fovy=glm::radians(80.0f);
                    _camera->position.x = 0.0f;
                    _camera->position.y = 1.7f;
                    _camera->position.z = 3.0f;
                    _cameraRotateAngles = 155.0f;
                    _AxisX=0.0f;
                    _AxisY=0.0f;
                    _AxisZ=0.47f;
                }
                glm::vec3 RotationAxis= glm::vec3(_AxisX,_AxisY,_AxisZ);
                float x = RotationAxis.x * sin(glm::radians(_cameraRotateAngles / 2));
                float y = RotationAxis.y * sin(glm::radians(_cameraRotateAngles / 2));
                float z = RotationAxis.z * sin(glm::radians(_cameraRotateAngles / 2));
                float w = cos(glm::radians(_cameraRotateAngles / 2));
                camera->rotation = glm::quat { x,y,z,w };
                
                oldCameraMode=CameraMode::FreeRotation;
            }
            break;
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
        //camera mode
        ImGui::Text("Camera Mode:");
        ImGui::Separator();
        ImGui::RadioButton("Pan", (int*)&_cameraMode, (int)(CameraMode::Pan));
        ImGui::RadioButton("Orbit", (int*)&_cameraMode, (int)(CameraMode::Orbit));
        if(_cameraMode==CameraMode::Orbit){
            ImGui::SliderFloat("Orbit Angle", &_orbitAngle, 0.0f, 360.0f);
        }
        ImGui::RadioButton("Free Rotation", (int*)&_cameraMode, (int)(CameraMode::FreeRotation));
        if(_cameraMode==CameraMode::FreeRotation){
            ImGui::SliderFloat("Rotate", &_cameraRotateAngles, -180.0, 180.0f);
            ImGui::SliderFloat("X", &_AxisX, -1.0f, 1.0f);
            ImGui::SliderFloat("Y", &_AxisY, -1.0f, 1.0f);
            ImGui::SliderFloat("Z", &_AxisZ, -1.0f, 1.0f);
        }
        ImGui::NewLine();
        
        //Phong render
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

        //Vertex shape
        ImGui::Text("Vertex shape:");
        ImGui::Separator();
        ImGui::RadioButton("Tetrahedron", (int*)&_shapeType, (int)(ShapeType::Tetrahedron));
        ImGui::RadioButton("Cube", (int*)&_shapeType, (int)(ShapeType::Cube));
		ImGui::RadioButton("Ball", (int*)&_shapeType, (int)(ShapeType::Ball));
		ImGui::RadioButton("Cone", (int*)&_shapeType, (int)(ShapeType::Cone));
		ImGui::RadioButton("Cylinder", (int*)&_shapeType, (int)(ShapeType::Cylinder));
		ImGui::RadioButton("Prism", (int*)&_shapeType, (int)(ShapeType::Prism));
		ImGui::RadioButton("Prismatictable", (int*)&_shapeType, (int)(ShapeType::Prismatictable));
		ImGui::RadioButton("BezierFace", (int*)&_shapeType, (int)(ShapeType::BezierFace));
        ImGui::Text("Scale");
        ImGui::SliderFloat("transformX", &_shapeScale.x, 0.0, 2.0);
        ImGui::SliderFloat("transformY", &_shapeScale.y, 0.0, 2.0);
        ImGui::SliderFloat("transformZ", &_shapeScale.z, 0.0, 2.0);
        ImGui::Text("Position");
        ImGui::SliderFloat("positionX", &_shapePosition.x, -1.0, 1.0);
        ImGui::SliderFloat("positionY", &_shapePosition.y, -1.0, 2.0);
        ImGui::SliderFloat("positionZ", &_shapePosition.z, -1.0, 1.0);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("rotationX", &_shapeRotateAxis.x, 0.0, 180.0f);
		ImGui::SliderFloat("rotationY", &_shapeRotateAxis.y, 0.0, 180.0f);
		ImGui::SliderFloat("rotationZ", &_shapeRotateAxis.z, 0.0, 180.0f);
        ImGui::NewLine();
        
        //wireframe mode
        ImGui::Checkbox("wireframe", &_wireframe);
        ImGui::Checkbox("boundingBox", &_showBoundingBox);
		ImGui::Checkbox("shadow", &_shadow);

        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TextureMapping::renderFrame() {

    //window setup
	showFpsInWindowTitle();
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    
    //wireframe mode
	if (_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

    //get projection/view/model matrix
    _projection = _camera->getProjectionMatrix();
    _model= _world->getModelMatrix();
    switch (_cameraMode) {
        case CameraMode::Pan:
            _view = _camera->getViewMatrix();
            break;
        case CameraMode::Orbit:
            _view = glm::lookAt(_camera->position, glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
            break;
        case CameraMode::FreeRotation:
            _view = _camera->getViewMatrix();
            break;
    }
    
    //draw BoundingBox
    if (_showBoundingBox) {
        _lineShader->use();
        _lineShader->setMat4("projection", _projection);
        _lineShader->setMat4("view", _view);
        _lineShader->setMat4("model", _model);
        _lineShader->setVec3("material.color", _lineMaterial->color);
        glLineWidth(_lineMaterial->width);
        
        _firstdeng->drawBoundingBox();
        _firstmenu->drawBoundingBox();
        _firstdesk->drawBoundingBox();
        _firstcard->drawBoundingBox();
        _seconddesk->drawBoundingBox();
        _secondsofa->drawBoundingBox();
        _firstfloor->drawBoundingBox();
        _secondfloor->drawBoundingBox();
        _secondchair->drawBoundingBox();
        _secondscreen->drawBoundingBox();
        _firstupstairs->drawBoundingBox();
        _seconddeskbottom->drawBoundingBox();
        _door->drawBoundingBox();
    }
    
    //draw scene
    
    //draw UNO object
    _universalPhongShader->use();
    resetPhongShader();
    _universalPhongShader->setMat4("model", _firstfloor->getModelMatrix());
    _firstcard->draw();
    _firstmenu->draw();
    _secondfloor->draw();
    _secondsofa->draw();
    _secondscreen->draw();
    _unotitle->draw();

    //draw display bar
    resetPhongShader();
    _universalPhongShader->setVec3("material.ka", glm::vec3(0.07f, 0.12f, 0.31f));
    _universalPhongShader->setVec3("material.kd", glm::vec3(0.12f, 0.24f, 0.41f));
    _firstdisplaybottom->draw();

    //draw floor and stairs
    resetPhongShader();
    _universalPhongShader->setVec3("material.ka", glm::vec3(0.21f, 0.21f, 0.21f));
    _universalPhongShader->setVec3("material.kd", glm::vec3(0.75f, 0.70f, 0.44f));
    _universalPhongShader->setVec3("material.ks", glm::vec3(0.76f, 0.69f, 0.55f));
    _firstfloor->draw();
    _firstupstairs->draw();
    
    //draw door
    resetPhongShader();
    _universalPhongShader->setMat4("model", _door->getModelMatrix());
    _door->draw();
    
    //draw desks
    _textureShader->use();
    _textureShader->setMat4("projection", _projection);
    _textureShader->setMat4("view", _view);
    _textureShader->setMat4("model", _model);
    _textureShader->setVec3("light.direction", _directionalLight->getFront());
    _textureShader->setVec3("light.color", _directionalLight->color);
    _textureShader->setFloat("light.intensity", _directionalLight->intensity);
    _textureShader->setVec3("material.kds[0]", _woodMaterial->kds[0]);
    _textureShader->setVec3("material.kds[1]", _woodMaterial->kds[1]);
    _textureShader->setFloat("material.blend", _woodMaterial->blend);
    _textureShader->setInt("mapKds[1]", 1);
    glActiveTexture(GL_TEXTURE0);
    _woodMaterial->mapKds[0]->bind();
    glActiveTexture(GL_TEXTURE1);
    _woodMaterial->mapKds[1]->bind();
    _firstdesk->draw();
    _firstdeng->draw();
    _seconddesk->draw();
    _secondchair->draw();
    _seconddeskbottom->draw();
    
    //draw shadow
	if(_shadow)renderDepthMap();
    
    //draw animation
    _universalPhongShader->use();
    resetPhongShader();
    _universalPhongShader->setMat4("model", danceMan.frame[1]->getModelMatrix());
    danceMan.frame[(animationSwitch++ % 110)/10 + 1]->draw();

    // draw sky box
	_skybox->draw(_projection, _view);
    
    // draw ground
    _ground->draw(_projection, _view);
    
    // draw Vertex Shape
    static float scaleDivide=25.0f;
    _cube->position = glm::vec3(_shapePosition.x, _shapePosition.y, _shapePosition.z);
    _cube->scale = glm::vec3(_shapeScale.x/scaleDivide, _shapeScale.y/scaleDivide, _shapeScale.z/scaleDivide);
    _cube->rotation = glm::rotate(glm::mat4(1.0f), glm::radians(_shapeRotateAxis.x), glm::vec3(1.0, 0.0, 0.0));
    _cube->rotation = glm::rotate(_cube->rotation, glm::radians(_shapeRotateAxis.y), glm::vec3(0.0, 1.0, 0.0));
    _cube->rotation = glm::rotate(_cube->rotation, glm::radians(_shapeRotateAxis.z), glm::vec3(0.0, 0.0, 1.0));
    
    switch (_shapeType){
    case ShapeType::Tetrahedron:
        _tetrahedron->draw(_cube->getModelMatrix(), _projection, _view);
        break;
    case ShapeType::Cube:
        _cube->draw(_cube->getModelMatrix(),_projection, _view);
        break;
	case ShapeType::Ball:
		_ball->draw(_cube->getModelMatrix(), _projection, _view);
		break;
	case ShapeType::Cone:
		_cone->draw(_cube->getModelMatrix(), _projection, _view);
		break;
	case ShapeType::Cylinder:
		_cylinder->draw(_cube->getModelMatrix(), _projection, _view);
		break;
	case ShapeType::Prism:
		_prism->draw(_cube->getModelMatrix(), _projection, _view);
		break;
	case ShapeType::Prismatictable:
		_prismatictable->draw(_cube->getModelMatrix(), _projection, _view);
		break;
	case ShapeType::BezierFace:
		_bezierFace->draw(_cube->getModelMatrix(), _projection, _view);
		break;
    }

    drawUI();
}

void TextureMapping::renderDepthMap() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //light calculation
    GLfloat near_plane = 0.1f, far_plane = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(_shadowLight->position, glm::vec3(0.0), glm::vec3(-1.0, -1.0, 1.0));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    
    //depth writing
	_depthmapShader->use();
	_depthmapShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	_depthmapShader->setMat4("model", _model);
	depthMap->bindFrameBuffer();
    drawAllObject();
	depthMap->unbindFrameBuffer();

    //shadow writing
	glViewport(0, 0, _windowWidth, _windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shadowShader->use();
	_shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	_shadowShader->setMat4("projection", _projection);
	_shadowShader->setMat4("view", _view);
	_shadowShader->setMat4("model", _model);
	_shadowShader->setVec3("lightPos", _shadowLight->position);
	_shadowShader->setVec3("viewPos", _camera->position);
	glActiveTexture(GL_TEXTURE0);
	depthMap->bind();
	glActiveTexture(GL_TEXTURE1);
	woodTexture->bind();
    drawAllObject();
	depthMap->unbind();
}

void TextureMapping::capture() {
    FILE* pDummyFile;
    FILE* pWritingFile;
    GLubyte* pPixelData;
    GLubyte  BMP_Header[54];
    GLint    width, height;
    GLint    PixelDataLength;

    // data align
    width = _windowWidth * 3;
    while (width % 4)++width;
    PixelDataLength = width * _windowHeight;

    //malloc space
    pPixelData = (GLubyte*)malloc(PixelDataLength);
    if (!pPixelData){
        std::cout<<"Data initialied failed"<<std::endl;
        return;
    };

    //load file
    std::string filename = "../../output/uno" + std::to_string(count++) + ".bmp";
    pDummyFile = fopen("../../media/uno1111.bmp", "rb");
    if (!pDummyFile){
        std::cout<<"path found failed"<<std::endl;
        return;
    };
    char* file = (char*)filename.data();
    pWritingFile = fopen(file, "wb");
    if (!pWritingFile) {
        std::cout<<"path found failed"<<std::endl;
        return;
    };

    //writing picture
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
    width = _windowWidth;
    height = _windowHeight;
    fwrite(&width, sizeof(width), 1, pWritingFile);
    fwrite(&height, sizeof(height), 1, pWritingFile);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, _windowWidth, _windowHeight,GL_BGR, GL_UNSIGNED_BYTE, pPixelData);
    fseek(pWritingFile, 0, SEEK_END);
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);
    
    std::cout << "saved" << std::endl;
    return;
}

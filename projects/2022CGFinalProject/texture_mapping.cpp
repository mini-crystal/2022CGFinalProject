#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "texture_mapping.h"

TextureMapping::TextureMapping(const Options& options): Application(options) {
    HandleMouse();
    InitializeModel();
    InitScale();
    InitMaterial();
    InitLight();
    InitTexture();
    InitCamera();
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
    //_bunny.reset(new Model(bunnymodelpath));
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
}

void TextureMapping::InitScale(){
    _sphere->scale = glm::vec3(0.3f, 0.3f, 0.3f);
    //_bunny->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void TextureMapping::InitMaterial(){
    _ambientMaterial.reset(new AmbientMaterial);
    _ambientMaterial->ka = glm::vec3(0.03f, 0.03f, 0.03f);

    _lambertMaterial.reset(new LambertMaterial);
    _lambertMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);

    _phongMaterial.reset(new PhongMaterial);
    _phongMaterial->ka = glm::vec3(0.03f, 0.03f, 0.03f);
    _phongMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);
    _phongMaterial->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    _phongMaterial->ns = 10.0f;
}
void TextureMapping::InitLight(){
    _ambientLight.reset(new AmbientLight);

    _directionalLight.reset(new DirectionalLight);
    _directionalLight->position = glm::vec3(10.0f, 10.0f, 10.0f);
    _directionalLight->rotation = glm::angleAxis(glm::radians(45.0f), -glm::vec3(1.0f, 1.0f, 1.0f));

    _spotLight.reset(new SpotLight);
    _spotLight->position = glm::vec3(0.0f, 0.0f, 5.0f);
    _spotLight->rotation = glm::vec3(-1.0f, 0.0f, 0.1f);
    
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
    
    //objects
    //_groundMaterial.reset(new SimpleMaterial);
    //_groundMaterial->mapKd = groundTexture;
}
void TextureMapping::InitCamera(){
    // 初始化摄像机
    _camera.reset(new PerspectiveCamera(
        glm::radians(50.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
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
    //initGroundShader();
    initWallShader();
    initAnimationShader();
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

//接收输入：比如键盘wasd输入
void TextureMapping::handleInput() {
        
    static bool LockMouse=false;
	constexpr float cameraMoveSpeed = 0.5f;//change move and rotate speed
	constexpr float cameraRotateSpeed = 0.2f;
    Camera* camera = _camera.get();
    
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
        _keyboardInput.keyStates[GLFW_KEY_SPACE] = GLFW_RELEASE;
        return;
    }
	if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) camera->position += cameraMoveSpeed * camera->getFront() *_deltaTime;
	if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) camera->position -= cameraMoveSpeed * camera->getRight() *_deltaTime;
	if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) camera->position -= cameraMoveSpeed * camera->getFront() *_deltaTime;
    if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) camera->position += cameraMoveSpeed * camera->getRight() *_deltaTime;
    
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
        ImGui::NewLine();

        //纹理shader选择面板
        
//        ImGui::Text("Adjust texture:");
//        ImGui::Separator();
//        ImGui::ColorEdit3("kd1", (float*)&_blendMaterial->kds[0]);
//        ImGui::ColorEdit3("kd2", (float*)&_blendMaterial->kds[1]);
//        ImGui::SliderFloat("blend", &_blendMaterial->blend, 0.0f, 1.0f);
//        ImGui::NewLine();

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
        
        //几何变换shader
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
        //wireframe mode
        ImGui::Checkbox("wireframe", &_wireframe);
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

	//shader Mode
	switch (_shaderrenderMode) {
	case ShaderRenderMode::Ambient:
		_ambientShader->use();
		// 1. transfer mvp matrix to the shader
		_ambientShader->setMat4("projection", _camera->getProjectionMatrix());
		_ambientShader->setMat4("view", _camera->getViewMatrix());
		_ambientShader->setMat4("model", _sphere->getModelMatrix());
		// 2. transfer material attributes to the shader
		_ambientShader->setVec3("material.ka", _ambientMaterial->ka);
		// 3. transfer light attributes to the shader
		_ambientShader->setVec3("ambientLight.color", _ambientLight->color);
		_ambientShader->setFloat("ambientLight.intensity", _ambientLight->intensity);
		break;
	case ShaderRenderMode::Lambert:
		_lambertShader->use();
		// 1. transfer mvp matrix to the shader
		_lambertShader->setMat4("projection", _camera->getProjectionMatrix());
		_lambertShader->setMat4("view", _camera->getViewMatrix());
		_lambertShader->setMat4("model", _sphere->getModelMatrix());
		// 2. transfer material attributes to the shader
		_lambertShader->setVec3("material.kd", _lambertMaterial->kd);
		// 3. transfer light attributes to the shader
		_lambertShader->setVec3("spotLight.position", _spotLight->position);
		_lambertShader->setVec3("spotLight.direction", _spotLight->getFront());
		_lambertShader->setFloat("spotLight.intensity", _spotLight->intensity);
		_lambertShader->setVec3("spotLight.color", _spotLight->color);
		_lambertShader->setFloat("spotLight.angle", _spotLight->angle);
		_lambertShader->setFloat("spotLight.kc", _spotLight->kc);
		_lambertShader->setFloat("spotLight.kl", _spotLight->kl);
		_lambertShader->setFloat("spotLight.kq", _spotLight->kq);
		_lambertShader->setVec3("directionalLight.direction", _directionalLight->getFront());
		_lambertShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
		_lambertShader->setVec3("directionalLight.color", _directionalLight->color);
		break;
	case ShaderRenderMode::Phong:
		_phongShader->use();
		// 1. transfer mvp matrix to the shader
		_phongShader->setMat4("projection", _camera->getProjectionMatrix());
		_phongShader->setMat4("view", _camera->getViewMatrix());
		_phongShader->setMat4("model", _sphere->getModelMatrix());
		// 2. transfer view position to the shader
		_phongShader->setVec3("viewPos", _camera->position);
		// 3. transfer material attributes to the shader
		_phongShader->setVec3("material.ka", _phongMaterial->ka);
		_phongShader->setVec3("material.kd", _phongMaterial->kd);
		_phongShader->setVec3("material.ks", _phongMaterial->ks);
		_phongShader->setFloat("material.ns", _phongMaterial->ns);
		// 4. transfer light attributes to the shader
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
		break;
	}

    _firstdisplaybottom->draw();
    _firstcard->draw();
    _firstmenu->draw();
    _secondfloor->draw();
    //_secondchair->draw();
    _secondsofa->draw();
    _secondscreen->draw();


    {;
        //    //------------------------------------------------
        //
        //    float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        //    _rotateAngles = 3.14f * scaleAmount;
        //
        //    glm::mat4 trans = glm::mat4(1.0f);
        //    glm::mat4 rotation = glm::axisAngleMatrix(_rotateAxis, _rotateAngles);
        //    glm::mat4 translation = glm::translate(trans, _position);
        //    glm::mat4 scale = glm::scale(trans, _scale);
        //    glm::mat4 model = translation * rotation * scale;
        //
        //    _transformShader->use();
        //    _transformShader->setMat4("projection", _camera->getProjectionMatrix());
        //    _transformShader->setMat4("view", _camera->getViewMatrix());
        //    _transformShader->setMat4("model", model);
        //    //------------------------------------------------
    }//test for rotation by time


    {// draw with texture
//	switch (_renderMode) {
//	case RenderMode::Simple:
//		// 1. use the shader
//		_simpleShader->use();
//		// 2. transfer mvp matrices to gpu
//		_simpleShader->setMat4("projection", projection);
//		_simpleShader->setMat4("view", view);
//		_simpleShader->setMat4("model", _sphere->getModelMatrix());
//		// 3. enable textures and transform textures to gpu
//		glActiveTexture(GL_TEXTURE0);
//		_simpleMaterial->mapKd->bind();
//		break;
//	case RenderMode::Blend:
//		// 1. use the shader
//		_blendShader->use();
//		// 2. transfer mvp matrices to gpu
//		_blendShader->setMat4("projection", projection);
//		_blendShader->setMat4("view", view);
//		_blendShader->setMat4("model", _sphere->getModelMatrix());
//		// 3. transfer light attributes to gpu
//		_blendShader->setVec3("light.direction", _directionalLight->getFront());
//		_blendShader->setVec3("light.color", _directionalLight->color);
//		_blendShader->setFloat("light.intensity", _directionalLight->intensity);
//		// 4. transfer materials to gpu
//		// 4.1 transfer simple material attributes
//		_blendShader->setVec3("material.kds[0]", _blendMaterial->kds[0]);
//		_blendShader->setVec3("material.kds[1]", _blendMaterial->kds[1]);
//		// 4.2 transfer blend cofficient to gpu
//		_blendShader->setFloat("material.blend", _blendMaterial->blend);
//		// 4.3 enable textures and transform textures to gpu
//		_blendShader->setInt("mapKds[1]", 1);
//		glActiveTexture(GL_TEXTURE0);
//		_blendMaterial->mapKds[0]->bind();
//		glActiveTexture(GL_TEXTURE1);
//		_blendMaterial->mapKds[1]->bind();
//		break;
//	case RenderMode::Checker:
//		// 1. use the shader
//		_checkerShader->use();
//		// 2. transfer mvp matrices to gpu
//		_checkerShader->setMat4("projection", projection);
//		_checkerShader->setMat4("view", view);
//		_checkerShader->setMat4("model", _sphere->getModelMatrix());
//		// 3. transfer material attributes to gpu
//		_checkerShader->setInt("material.repeat", _checkerMaterial->repeat);
//		_checkerShader->setVec3("material.colors[0]", _checkerMaterial->colors[0]);
//		_checkerShader->setVec3("material.colors[1]", _checkerMaterial->colors[1]);
//		break;
//    }}
    }
    
    
    //    _simpleShader->use();
    //    _simpleShader->setMat4("projection", projection);
    //    _simpleShader->setMat4("view", view);
    //    _simpleShader->setMat4("model", _sphere->getModelMatrix());
    //    glActiveTexture(GL_TEXTURE0);
    //    _simpleMaterial->mapKd->bind();
    
    
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
    
    float transformation = static_cast <float>(sin((double)glfwGetTime()));
    double velocity = 5.0f;
    float trans = transformation * velocity;
    printf("%f\n", trans);
    //glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.289f, 0.268f, 0.0f));
    //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
    model = _animation1->getModelMatrix() * glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    _animationShader->use();

    // 1. transfer mvp matrix to the shader
    _animationShader->setMat4("projection", _camera->getProjectionMatrix());
    _animationShader->setMat4("view", _camera->getViewMatrix());
    _animationShader->setMat4("model", model);
    // 2. transfer view position to the shader
    _animationShader->setVec3("viewPos", _camera->position);
    // 3. transfer material attributes to the shader
    _animationShader->setVec3("material.ka", _phongMaterial->ka);
    _animationShader->setVec3("material.kd", _phongMaterial->kd);
    _animationShader->setVec3("material.ks", _phongMaterial->ks);
    _animationShader->setFloat("material.ns", _phongMaterial->ns);
    // 4. transfer light attributes to the shader
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
    //tzy
    ;
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
    //_groundMaterial->mapKd->bind();
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

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "texture_mapping.h"



//这里可以加载多个模型
//把模型路径放进来就好
//现在是一楼二楼场景中的物体都已经存在
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


const std::string earthTexturePath = "../../media/earthmap.jpg";
const std::string planetTexturePath = "../../media/planet_Quom1200.png";


const std::vector<std::string> skyboxTexturePaths = {
	"../../media/starfield/Right_Tex.jpg",
	"../../media/starfield/Left_Tex.jpg",
	"../../media/starfield/Up_Tex.jpg",
	"../../media/starfield/Down_Tex.jpg",
	"../../media/starfield/Front_Tex.jpg",
	"../../media/starfield/Back_Tex.jpg"
};


TextureMapping::TextureMapping(const Options& options): Application(options) {
	////set input mode
	// 这里是把鼠标的输入当作是视角的改变，所以不会有指针出来
	//glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_mouseInput.move.xOld = _mouseInput.move.xCurrent = 0.5 * _windowWidth;
	_mouseInput.move.yOld = _mouseInput.move.yCurrent = 0.5 * _windowHeight;
	glfwSetCursorPos(_window, _mouseInput.move.xCurrent, _mouseInput.move.yCurrent);
	
	// 加载模型
	_sphere.reset(new Model(spheremodelPath));
	_bunny.reset(new Model(bunnymodelpath));
	_bunnycopy.reset(new Model(bunnymodelpath));
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
	_sphere->scale = glm::vec3(0.3f, 0.3f, 0.3f);
	_bunny->scale = glm::vec3(1.0f, 1.0f, 1.0f);


	// 初始化材质
	_ambientMaterial.reset(new AmbientMaterial);
	_ambientMaterial->ka = glm::vec3(0.03f, 0.03f, 0.03f);

	_lambertMaterial.reset(new LambertMaterial);
	_lambertMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);

	_phongMaterial.reset(new PhongMaterial);
	_phongMaterial->ka = glm::vec3(0.03f, 0.03f, 0.03f);
	_phongMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);
	_phongMaterial->ks = glm::vec3(1.0f, 1.0f, 1.0f);
	_phongMaterial->ns = 10.0f;
	

	// 加载灯光
	_ambientLight.reset(new AmbientLight);

	_directionalLight.reset(new DirectionalLight);
	_directionalLight->position = glm::vec3(10.0f, 10.0f, 10.0f);
	_directionalLight->rotation = glm::angleAxis(glm::radians(45.0f), -glm::vec3(1.0f, 1.0f, 1.0f));

	_spotLight.reset(new SpotLight);
	_spotLight->position = glm::vec3(0.0f, 0.0f, 5.0f);
	_spotLight->rotation = glm::vec3(0.0f, 0.0f, 0.0f);


	// 初始化纹理贴图
	std::shared_ptr<Texture2D> earthTexture = std::make_shared<Texture2D>(earthTexturePath);
	std::shared_ptr<Texture2D> planetTexture = std::make_shared<Texture2D>(planetTexturePath);

	// 初始化材质
	_simpleMaterial.reset(new SimpleMaterial);
	_simpleMaterial->mapKd = planetTexture;

	_blendMaterial.reset(new BlendMaterial);
	_blendMaterial->kds[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	_blendMaterial->kds[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	_blendMaterial->mapKds[0] = planetTexture;
	_blendMaterial->mapKds[1] = earthTexture;
	_blendMaterial->blend = 0.0f;

	_checkerMaterial.reset(new CheckerMaterial);
	_checkerMaterial->repeat = 10;
	_checkerMaterial->colors[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	_checkerMaterial->colors[1] = glm::vec3(0.0f, 0.0f, 0.0f);

	// 顶点模型初始化
	_skybox.reset(new SkyBox(skyboxTexturePaths));
	_cube.reset(new Cube());
	_tetrahedron.reset(new Tetrahedron());

	// 初始化摄像机
	_camera.reset(new PerspectiveCamera(
		glm::radians(50.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
	_camera->position.z = 3.0f;

	// 初始化灯光
	_light.reset(new DirectionalLight());
	_light->rotation = glm::angleAxis(glm::radians(45.0f), -glm::vec3(1.0f, 1.0f, 1.0f));


	
	//初始化3个控制面板shader   材质+纹理+改变大小

	//几何变换shader
	initTransformShader();
	

	//材质shader
	initAmbientShader();
	initLambertShader();
	initPhongShader();


	//纹理shader
	initSimpleShader();
	initBlendShader();
	initCheckerShader();



	//初始化imgui面板
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init();
}

TextureMapping::~TextureMapping() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


//接收输入：比如键盘wasd输入
void TextureMapping::handleInput() {
	const float angluarVelocity = 0.1f;
	const float angle = angluarVelocity * static_cast<float>(_deltaTime);
	const glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
	//_sphere->rotation = glm::angleAxis(angle, axis) * _sphere->rotation;

	//改变视角位置
	constexpr float cameraMoveSpeed = 0.05f;
	constexpr float cameraRotateSpeed = 0.1f;
	const glm::vec3 cameraFront = _camera->getFront();

	if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
		glfwSetWindowShouldClose(_window, true);
		return;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
		std::cout << "switch camera" << std::endl;
		
		_keyboardInput.keyStates[GLFW_KEY_SPACE] = GLFW_RELEASE;
		return;
	}

	Camera* camera = _camera.get();

	if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) {
		std::cout << "W" << std::endl;
		camera->position += cameraMoveSpeed * cameraFront;

	}

	if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
		std::cout << "A" << std::endl;
		camera->position += cameraMoveSpeed * camera->getRight();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
		std::cout << "S" << std::endl;
		camera->position -= cameraMoveSpeed * cameraFront;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
		std::cout << "D" << std::endl;
		camera->position -= cameraMoveSpeed * camera->getRight();
	}

	if (_mouseInput.move.xCurrent != _mouseInput.move.xOld) {
		std::cout << "mouse move in x direction" << std::endl;
	
		float mouse_movement_in_x_direction;
		mouse_movement_in_x_direction = (float)_mouseInput.move.xCurrent - _mouseInput.move.xOld;
		const float theta = mouse_movement_in_x_direction * cameraRotateSpeed * _deltaTime;
		camera->rotation = glm::quat(cos(theta * 0.5f), sin(theta * 0.5f) * glm::vec3(0.0f, 1.0f, 0.0f)) * camera->rotation;
		

		_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	}

	if (_mouseInput.move.yCurrent != _mouseInput.move.yOld) {
		std::cout << "mouse move in y direction" << std::endl;
		
		float mouse_movement_in_y_direction;
		mouse_movement_in_y_direction = (float)_mouseInput.move.yCurrent - _mouseInput.move.yOld;
		const float theta = mouse_movement_in_y_direction * cameraRotateSpeed * _deltaTime;
		camera->rotation = glm::quat(cos(theta * 0.5f), sin(theta * 0.5f) * camera->getRight()) * camera->rotation;
		

		_mouseInput.move.yOld = _mouseInput.move.yCurrent;
	}
}

void TextureMapping::renderFrame() {
	// some options related to imGUI
	static bool wireframe = false;
	
	
	showFpsInWindowTitle();

	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	//现有场景下只有一个相机：透视相机
	//获得该摄像机下的投影矩阵和观察矩阵

	//接下来就是分为两种绘制方式：路径加载模型绘制和顶点模型绘制
	//本质上是一样的，启动一个shader，然后再draw（）

	const glm::mat4 projection = _camera->getProjectionMatrix();
	const glm::mat4 view = _camera->getViewMatrix();




	//样例  给场景中不同的物体上材质


	_phongShader->use();
	// 1. transfer mvp matrix to the shader
	_phongShader->setMat4("projection", _camera->getProjectionMatrix());
	_phongShader->setMat4("view", _camera->getViewMatrix());
	_phongShader->setMat4("model", _sphere->getModelMatrix());

	// 2. transfer view position to the shader

	_phongShader->setVec3("viewPos", _camera->position);
	// ----------------------------------------------------------------

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

	_firstdisplaybottom->draw();




	float scaleAmount = static_cast<float>(sin(glfwGetTime()));
	
	_rotateAngles = 3.14 * scaleAmount;

	glm::mat4 trans = glm::mat4(1.0f);

	glm::mat4 rotation = glm::axisAngleMatrix(_rotateAxis, _rotateAngles);;

	glm::mat4 translation = glm::translate(trans, _position);

	glm::mat4 scale = glm::scale(trans, _scale);

	glm::mat4 model = translation * rotation * scale;

	_transformShader->use();
	_transformShader->setMat4("projection", _camera->getProjectionMatrix());
	_transformShader->setMat4("view", _camera->getViewMatrix());
	_transformShader->setMat4("model", model);

	//_bunny->draw();
	//_firstdisplaybottom->draw();

	//_firstfloor->draw();

	//shader UI
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
		// ----------------------------------------------------------------

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

	_firstdeskbottom->draw();


	// texture UI
	switch (_renderMode) {
	case RenderMode::Simple:
		// 1. use the shader
		_simpleShader->use();
		// 2. transfer mvp matrices to gpu 
		_simpleShader->setMat4("projection", projection);
		_simpleShader->setMat4("view", view);
		_simpleShader->setMat4("model", _sphere->getModelMatrix());
		// 3. enable textures and transform textures to gpu
		glActiveTexture(GL_TEXTURE0);
		_simpleMaterial->mapKd->bind();
		break;
	case RenderMode::Blend:
		// 1. use the shader
		_blendShader->use();
		// 2. transfer mvp matrices to gpu 
		_blendShader->setMat4("projection", projection);
		_blendShader->setMat4("view", view);
		_blendShader->setMat4("model", _sphere->getModelMatrix());
		// 3. transfer light attributes to gpu
		_blendShader->setVec3("light.direction", _light->getFront());
		_blendShader->setVec3("light.color", _light->color);
		_blendShader->setFloat("light.intensity", _light->intensity);
		// 4. transfer materials to gpu
		// 4.1 transfer simple material attributes
		_blendShader->setVec3("material.kds[0]", _blendMaterial->kds[0]);
		_blendShader->setVec3("material.kds[1]", _blendMaterial->kds[1]);
		// 4.2 transfer blend cofficient to gpu
		_blendShader->setFloat("material.blend", _blendMaterial->blend);
		// 4.3 enable textures and transform textures to gpu
		_blendShader->setInt("mapKds[1]", 1);
		glActiveTexture(GL_TEXTURE0);
		_blendMaterial->mapKds[0]->bind();
		glActiveTexture(GL_TEXTURE1);
		_blendMaterial->mapKds[1]->bind();
		

		break;
	case RenderMode::Checker:
		// 1. use the shader
		_checkerShader->use();
		// 2. transfer mvp matrices to gpu 
		_checkerShader->setMat4("projection", projection);
		_checkerShader->setMat4("view", view);
		_checkerShader->setMat4("model", _sphere->getModelMatrix());
		// 3. transfer material attributes to gpu
		_checkerShader->setInt("material.repeat", _checkerMaterial->repeat);
		_checkerShader->setVec3("material.colors[0]", _checkerMaterial->colors[0]);
		_checkerShader->setVec3("material.colors[1]", _checkerMaterial->colors[1]);
		break;
	}



	//_bunny->draw();
	//_sphere->draw();
	// draw skybox
	_firstdesk->draw();

	_skybox->draw(projection, view);
	//_tetrahedron->draw(projection, view);


	//_cube->draw(projection, view);

	// draw ui elements
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const auto flags =
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings;


		if (!ImGui::Begin("Light Control Panel", nullptr, flags)) {
		ImGui::End();
	}
	else {
		ImGui::Text("Render Mode");
		ImGui::Separator();
		ImGui::NewLine();

		//材质shader选择面板
		ImGui::RadioButton("ambient", (int*)&_shaderrenderMode, (int)(ShaderRenderMode::Ambient));
		ImGui::ColorEdit3("ka##1", (float*)&_ambientMaterial->ka);
		ImGui::NewLine();

		ImGui::RadioButton("lambert", (int*)&_shaderrenderMode, (int)(ShaderRenderMode::Lambert));
		ImGui::ColorEdit3("kd##2", (float*)&_lambertMaterial->kd);
		ImGui::NewLine();

		ImGui::RadioButton("phong", (int*)&_shaderrenderMode, (int)(ShaderRenderMode::Phong));
		ImGui::ColorEdit3("ka##3", (float*)&_phongMaterial->ka);
		ImGui::ColorEdit3("kd##3", (float*)&_phongMaterial->kd);
		ImGui::ColorEdit3("ks##3", (float*)&_phongMaterial->ks);
		ImGui::SliderFloat("ns##3", &_phongMaterial->ns, 1.0f, 50.0f);
		ImGui::NewLine();

		ImGui::Text("ambient light");
		ImGui::Separator();
		ImGui::SliderFloat("intensity##1", &_ambientLight->intensity, 0.0f, 2.0f);
		ImGui::ColorEdit3("color##1", (float*)&_ambientLight->color);
		ImGui::NewLine();

		ImGui::Text("directional light");
		ImGui::Separator();
		ImGui::SliderFloat("intensity##2", &_directionalLight->intensity, 0.0f, 1.5f);
		ImGui::ColorEdit3("color##2", (float*)&_directionalLight->color);
		ImGui::NewLine();

		ImGui::Text("spot light");
		ImGui::Separator();
		ImGui::SliderFloat("intensity##3", &_spotLight->intensity, 0.0f, 1.5f);
		ImGui::ColorEdit3("color##3", (float*)&_spotLight->color);
		ImGui::SliderFloat("angle##3", (float*)&_spotLight->angle, 0.0f, glm::radians(180.0f), "%f rad");



		//纹理shader选择面板
		ImGui::RadioButton("Simple Texture Shading", (int*)&_renderMode, (int)(RenderMode::Simple));
		ImGui::NewLine();

		ImGui::RadioButton("Blend Texture Shading", (int*)&_renderMode, (int)(RenderMode::Blend));
		ImGui::ColorEdit3("kd1", (float*)&_blendMaterial->kds[0]);
		ImGui::ColorEdit3("kd2", (float*)&_blendMaterial->kds[1]);
		ImGui::SliderFloat("blend", &_blendMaterial->blend, 0.0f, 1.0f);
		ImGui::NewLine();

		ImGui::RadioButton("Checker Shading", (int*)&_renderMode, (int)(RenderMode::Checker));
		ImGui::SliderInt("repeat", &_checkerMaterial->repeat, 2, 20);
		ImGui::ColorEdit3("color1", (float*)&_checkerMaterial->colors[0]);
		ImGui::ColorEdit3("color2", (float*)&_checkerMaterial->colors[1]);
		ImGui::Checkbox("wireframe", &wireframe);
		ImGui::NewLine();

		ImGui::Text("Directional light");
		ImGui::Separator();
		ImGui::SliderFloat("intensity", &_light->intensity, 0.0f, 2.0f);
		ImGui::ColorEdit3("color", (float*)&_light->color);
		ImGui::NewLine();


		//几何变换shader
		ImGui::Text("Transform panel");
		//scale
		ImGui::Text("Scale");
		ImGui::SliderFloat("transformX", &_scale.x, 0.0, 2.0);
		ImGui::SliderFloat("transformY", &_scale.y, 0.0, 2.0);
		ImGui::SliderFloat("transformZ", &_scale.z, 0.0, 2.0);
		//postion
		ImGui::Text("Position");
		ImGui::SliderFloat("positionX", &_position.x, -8.0, 8.0);
		

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
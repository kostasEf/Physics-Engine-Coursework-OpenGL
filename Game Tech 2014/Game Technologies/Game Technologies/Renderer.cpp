#include "Renderer.h"

Renderer* Renderer::instance = NULL;

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{

	wglSwapIntervalEXT(0);

	frame = time = 0;

	camera = NULL;

	quad = Mesh::GenerateQuad();

	root = new SceneNode();

	simpleShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");

	textShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);


	cubeMap = SOIL_load_OGL_cubemap(
		"../../Textures/rusted_west.jpg", "../../Textures/rusted_east.jpg",
		"../../Textures/rusted_up.jpg", "../../Textures/rusted_down.jpg",
		"../../Textures/rusted_south.jpg", "../../Textures/rusted_north.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
		);

	if (!simpleShader->LinkProgram() || !skyboxShader->LinkProgram() || !textShader->LinkProgram()){
		return;
	}
	if (!cubeMap)
	{
		return;
	}

	instance = this;

	init = true;
}

Renderer::~Renderer(void)	{
	delete root;
	delete simpleShader;
	delete skyboxShader;
	delete basicFont;
	currentShader = NULL;
}

void Renderer::UpdateScene(float msec)
{
	if (camera) {

		camera->UpdateCamera(msec);
	}
	root->Update(msec);

	frame++;
	time += msec;

	if (time > 1000)
	{
		fps = frame;
		time -= 1000;
		frame = 0;
	}
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



	if (camera) {

		DrawSkybox();

		textureMatrix.ToIdentity();
		modelMatrix.ToIdentity();
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
		SetCurrentShader(simpleShader);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
		frameFrustum.FromMatrix(projMatrix * viewMatrix);
		UpdateShaderMatrices();
		//Return to default 'usable' state every frame!

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		BuildNodeLists(root);
		SortNodeLists();

		DrawNodes();


		ClearNodeLists();
		DrawFPS();
	}



	glUseProgram(0);
	SwapBuffers();
}

void Renderer::DrawSkybox() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthMask(GL_FALSE);

	SetCurrentShader(skyboxShader);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	UpdateShaderMatrices();
	quad->Draw();
	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawFPS()
{
	SetCurrentShader(textShader);
	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glDisable(GL_DEPTH_TEST);


	DrawText("Renderer FPS:" + std::to_string(fps), Vector3(0, 0, 0), 16.0f);
	DrawText("Physics FPS:" + std::to_string(PhysicsSystem::GetPhysicsSystem().fps), Vector3(0, 16, 0), 16.0f);
	DrawText("Collision Counter:" + std::to_string(PhysicsSystem::GetPhysicsSystem().counter), Vector3(0, 32, 0), 16.0f);
	DrawText("Score:" + std::to_string(PhysicsSystem::GetPhysicsSystem().score), Vector3(0, 48, 0), 16.0f);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}


void	Renderer::DrawNode(SceneNode*n)	{
	if (n->GetMesh()) {
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&(n->GetWorldTransform()*Matrix4::Scale(n->GetModelScale())));
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());

		n->Draw(*this);
	}
}

void	Renderer::BuildNodeLists(SceneNode* from)	{
	Vector3 direction = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
	from->SetCameraDistance(Vector3::Dot(direction, direction));

	if (frameFrustum.InsideFrustum(*from)) {
		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else{
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void	Renderer::DrawNodes()	 {
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i));
	}

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));
	}
}

void	Renderer::SortNodeLists()	{
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void	Renderer::ClearNodeLists()	{
	transparentNodeList.clear();
	nodeList.clear();
}

void	Renderer::SetCamera(Camera*c) {
	camera = c;
}

void	Renderer::AddNode(SceneNode* n) {
	root->AddChild(n);
}

void	Renderer::RemoveNode(SceneNode* n) {
	root->RemoveChild(n);
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective)	{
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else{
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	delete mesh; //Once it's drawn, we don't need it anymore!
}
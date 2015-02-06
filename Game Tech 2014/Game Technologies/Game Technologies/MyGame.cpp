#include "MyGame.h"
#include "InertialMatrixHelper.h"
#include "Spring.h"
#include "SpringDemo.h"

/*
Creates a really simple scene for our game - A cube robot standing on
a floor. As the module progresses you'll see how to get the robot moving
around in a physically accurate manner, and how to stop it falling
through the floor as gravity is added to the scene. 

You can completely change all of this if you want, it's your game!

*/
MyGame::MyGame()	{
	gameCamera = new Camera(-30.0f,0.0f,Vector3(0,450,850));

	Renderer::GetRenderer().SetCamera(gameCamera);

	CubeRobot::CreateCube();
	Tree::CreateBranch();
	Crate::CreateCrate();

	projSize = 10;
	projSpeed = 1;

	time = 0;
	hasBroken = false;
	speed = Vector3(0, 0, 1.5);

	/*
	We're going to manage the meshes we need in our game in the game class!

	You can do this with textures, too if you want - but you might want to make
	some sort of 'get / load texture' part of the Renderer or OGLRenderer, so as
	to encapsulate the API-specific parts of texture loading behind a class so
	we don't care whether the renderer is OpenGL / Direct3D / using SOIL or 
	something else...
	*/
	cube	= new OBJMesh(MESHDIR"cube3.obj");
	quad	= Mesh::GenerateQuad();
	sphere	= new OBJMesh(MESHDIR"sphere.obj");
	red		= new OBJMesh(MESHDIR"sphere.obj");
	hurt	= new OBJMesh(MESHDIR"sphere.obj");
	cant	= new OBJMesh(MESHDIR"sphere.obj");

	/*
	A more 'robust' system would check the entities vector for duplicates so as
	to not cause problems...why not give it a go?
	*/
	GameEntity* quadEntity = BuildQuadEntity(5000.0f);
	allEntities.push_back(BuildTreeEntity());
	allEntities.push_back(quadEntity);
	

	ball1 = BuildSphereEntity(100.0f, Vector3(-1000, 1000, 1000), Vector3(0, 0, 0));
	ball1->GetPhysicsNode().SetUseGravity(false);
	ball1->GetPhysicsNode().SetResting(false);
	ball1->GetPhysicsNode().SetInverseMass(0);
	ball1->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	allEntities.push_back(ball1);
	cout << allEntities.size() << endl;

	AngryBirds();

	wannaBeSmart = BuildWannaBeSmartEntity(50.0f, Vector3(300, 450, -1100), Vector3(0, 0, 0));
	

	allEntities.push_back(wannaBeSmart);
	
}

MyGame::~MyGame(void)	{
	/*
	We're done with our assets now, so we can delete them
	*/
	delete cube;
	delete quad;
	delete sphere;
	delete red;
	delete hurt;
	delete cant;

	CubeRobot::DeleteCube();
	Tree::DeleteBranch();

	//GameClass destructor will destroy your entities for you...
}

/*
Here's the base 'skeleton' of your game update loop! You will presumably
want your games to have some sort of internal logic to them, and this
logic will be added to this function.
*/
void MyGame::UpdateGame(float msec) {
	if(gameCamera) {
		gameCamera->UpdateCamera(msec);
	}

	time += msec;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P)) { projSize += 10;  }
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_O)) { projSize -= 10;  }
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L)) { projSpeed += 0.5;  }
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_K)) { projSpeed -= 0.5;  }

	if (Window::GetMouse()->KeyTriggered(MOUSE_LEFT))
	{
		Matrix4 viewMatrix = gameCamera->BuildViewMatrix();
		GameEntity* ball10 = BuildProjectileEntity(projSize, gameCamera->GetPosition(), Vector3(-viewMatrix.values[2] * projSpeed, -viewMatrix.values[6] * projSpeed, -viewMatrix.values[10] * projSpeed));
		ball10->GetPhysicsNode().SetProjectile(true);
		ball10->GetPhysicsNode().SetPig(false);
		allEntities.push_back(ball10);
	}
	

	if (!hasBroken && ball1->GetPhysicsNode().GetLinearVelocity() == Vector3(0, 0, 0))
	{
		speed = speed * -1;
		ball1->GetPhysicsNode().SetResting(false);
		ball1->GetPhysicsNode().SetLinearVelocity(speed);
		ball1->GetPhysicsNode().SetResting(false);

	}


	
	if (ball1->GetPhysicsNode().GetCounter() > 5 && hasBroken == false)
	{
		Vector3 pos = ball1->GetPhysicsNode().GetPosition();

		Break(pos);


		ball1->GetPhysicsNode().SetPosition(Vector3(0, -55500, 0));
		ball1->GetPhysicsNode().SetCounter(0);

		hasBroken = true;
		time = 0;
	}


	if (hasBroken && time > 10000)
	{
		ball1->GetPhysicsNode().SetPosition(Vector3(-1000, 1000, 1000));
		hasBroken = false;
	}


	for(vector<GameEntity*>::iterator i = allEntities.begin(); i != allEntities.end(); ++i) {
		(*i)->Update(msec);
	}

	
}

/*
Makes an entity that looks like a CubeRobot! You'll probably want to modify
this so that you can have different sized robots, with different masses and
so on!
*/


GameEntity* MyGame::BuildTreeEntity() {

	Tree* tree = new Tree(0, 0, 0, 200);
	PhysicsNode*p = new PhysicsNode();
	p->SetUseGravity(false);
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetInverseMass(0.0f);
	p->SetPosition(Vector3(0, 0, 0));
	GameEntity*g = new GameEntity(tree, p);
	g->ConnectToSystems();
	return g;
}

/*
Makes a cube. Every game has a crate in it somewhere!
*/
GameEntity* MyGame::BuildCubeEntity(float size, Vector3 pos, int angle) {

	Crate* crate = new Crate(pos.x, pos.y, pos.z, size, angle);
	PhysicsNode*p = new PhysicsNode();
	p->SetResting(true);
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetInverseMass(0.0f);
	p->SetPosition(pos);
	GameEntity*g = new GameEntity(crate, p);
	g->ConnectToSystems();
	return g;

}


/*
Makes a sphere.
*/
GameEntity* MyGame::BuildSphereEntity(float radius, Vector3 pos, Vector3 vel) {
	SceneNode* s = new SceneNode(sphere);
	s->GetMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/wellpig.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	s->SetModelScale(Vector3(radius,radius,radius));
	s->SetBoundingRadius(radius);
	PhysicsNode*p = new PhysicsNode();
	p->SetPosition(pos);
	p->SetResting(true);
	p->SetLinearVelocity(vel);
	p->SetAngularVelocity(Vector3(0,0,0.0f));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, radius));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionSphere(radius));
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	return g;
}

GameEntity* MyGame::BuildHurtPigEntity(float radius, Vector3 pos, Vector3 vel) {
	SceneNode* s = new SceneNode(hurt);
	s->GetMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/hurtpig.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	s->SetModelScale(Vector3(radius, radius, radius));
	s->SetBoundingRadius(radius);
	PhysicsNode*p = new PhysicsNode();
	p->SetPosition(pos);
	p->SetLinearVelocity(vel);
	p->SetAngularVelocity(Vector3(0, 0, 0.0f));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, radius));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionSphere(radius));
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	return g;
}

GameEntity* MyGame::BuildProjectileEntity(float radius, Vector3 pos, Vector3 vel) {
	SceneNode* s = new SceneNode(red);
	s->GetMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/red.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	s->SetModelScale(Vector3(radius, radius, radius));
	s->SetBoundingRadius(radius);
	PhysicsNode*p = new PhysicsNode();
	p->SetPosition(pos);
	p->SetLinearVelocity(vel);
	p->SetAngularVelocity(Vector3(0, 0, 0.0f));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, radius));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionSphere(radius));
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	return g;
}

GameEntity* MyGame::BuildWannaBeSmartEntity(float radius, Vector3 pos, Vector3 vel) {
	SceneNode* s = new SceneNode(cant);
	s->GetMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/cant.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	s->SetModelScale(Vector3(radius, radius, radius));
	s->SetBoundingRadius(radius);
	PhysicsNode*p = new PhysicsNode();
	p->SetPosition(pos);
	p->SetLinearVelocity(vel);
	p->SetAngularVelocity(Vector3(0, 0, 0.0f));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, radius));
	p->SetInverseMass(1);
	p->SetUseGravity(false);
	p->SetResting(false);
	p->SetCollisionVolume(new CollisionSphere(radius * 5));
	p->SetSmart(true);
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	return g;
}

/*
Makes a flat quad, initially oriented such that we can use it as a simple
floor. 
*/
GameEntity* MyGame::BuildQuadEntity(float size) {
	SceneNode* s = new SceneNode(quad);
	s->GetMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/dirt.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	s->SetModelScale(Vector3(size,size,size));
	//Oh if only we had a set texture function...we could make our brick floor again WINK WINK
	s->SetBoundingRadius(size);

	PhysicsNode*p = new PhysicsNode(Quaternion::AxisAngleToQuaterion(Vector3(1,0,0), 90.0f), Vector3());
	p->SetUseGravity(false);
	p->SetInverseMass(0.0f);
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetCollisionVolume(new CollisionPlane(Vector3(0,1,0), 0));
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	return g;
}

void MyGame::Break(Vector3 pos)
{

	{	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x, pos.y + 10, pos.z), Vector3(0, 0.5, 0));
		allEntities.push_back(ball); 	}

	{	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x - 10, pos.y, pos.z), Vector3(-0.5, 0, 0));
		allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x + 10, pos.y, pos.z), Vector3(0.5, 0, 0));
		allEntities.push_back(ball); 	}

	{	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x - 10, pos.y, pos.z + 10), Vector3(-0.5, 0, 0.5));
		allEntities.push_back(ball);    }

	{	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x - 10, pos.y, pos.z - 10), Vector3(-0.5, 0, -0.5));
		allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x, pos.y, pos.z - 10), Vector3(0, 0, -0.5));
		allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x, pos.y, pos.z + 10), Vector3(0, 0, 0.5));
		allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x + 10, pos.y, pos.z + 10), Vector3(0.5, 0, 0.5));
		allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x + 10, pos.y, pos.z - 10), Vector3(0.5, 0, -0.5));
		allEntities.push_back(ball); 	}


	{	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x, pos.y + 10, pos.z), Vector3(0.5, 0.5, 0.5));
	allEntities.push_back(ball); 	}

	{	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x - 10, pos.y, pos.z), Vector3(-0.5, 0.5, 0.5));
	allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x + 10, pos.y, pos.z), Vector3(0.5, 0.5, 0.5));
	allEntities.push_back(ball); 	}

	{	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x - 10, pos.y, pos.z + 10), Vector3(-0.5, 0.5, 0.5));
	allEntities.push_back(ball);    }

	{	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x - 10, pos.y, pos.z - 10), Vector3(-0.5, 0.5, -0.5));
	allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x, pos.y, pos.z - 10), Vector3(0.5, 0.5, -0.5));
	allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x, pos.y, pos.z + 10), Vector3(0.5, 0.5, 0.5));
	allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x + 10, pos.y, pos.z + 10), Vector3(0.5, 0.5, 0.5));
	allEntities.push_back(ball); 	}

	{ 	GameEntity* ball = BuildHurtPigEntity(30.0f, Vector3(pos.x + 10, pos.y, pos.z - 10), Vector3(0.5, 0.5, -0.5));
	allEntities.push_back(ball); 	}
}

void MyGame::AngryBirds()
{
	// Bottom Left
	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 100, 400), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 100, 450), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 100, 350), 0);
	allEntities.push_back(crate); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 350, 750), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 350, 850), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 350, 950), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	//-------------------------------- Bottom Right ----------------------------------------
	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 100, 0), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 100, -50), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 100, -100), 0);
	allEntities.push_back(crate); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 350, 50), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 350, -50), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 350, -150), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	//------------------------------------- Mid -------------------------------------------
	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 200, 250), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 200, 200), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 200, 150), 0);
	allEntities.push_back(crate); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 550, 550), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 550, 450), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 550, 350), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	//--------------------------------- Top Right -----------------------------------------
	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 300, 400), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 300, 450), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 300, 350), 0);
	allEntities.push_back(crate); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 750, 50), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 750, -50), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 750, -150), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	//--------------------------------- Top Left --------------------------------------------
	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 300, 0), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 300, -50), 0);
	allEntities.push_back(crate); }

	{ GameEntity* crate = BuildCubeEntity(100, Vector3(-500, 300, -100), 0);
	allEntities.push_back(crate); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 750, 750), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 750, 850), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	{ GameEntity* pig = BuildSphereEntity(20, Vector3(-950, 750, 950), Vector3(0, 0, 0));
	allEntities.push_back(pig); }

	
}
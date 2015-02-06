#include "Tree.h"

Mesh * Tree::branch = NULL;


Tree::Tree(float x, float y, float z, float scale) {
	// Optional , uncomment if you want a local origin marker !
	//SetMesh ( branch );

	s = scale;
	scale1 = scale2 = 0;
	rate = 100;


	base = new SceneNode(branch, Vector4(1, 0, 0, 1)); // Red !
	base->SetModelScale(Vector3(0, 0, 0));
	base->SetTransform(Matrix4::Translation(Vector3(x, y, z))*
		Matrix4::Rotation(0, Vector3(0, 0, 0)));
	AddChild(base);
	
	test = new SceneNode();
	test->SetTransform(Matrix4::Translation(Vector3(x, y, z))*
		Matrix4::Rotation(0, Vector3(1, 0, 0)));

	p = new PhysicsNode();
	p->SetPosition(Vector3(x + scale/10, y + scale * 0.5  , z - scale/10));
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetInverseMass(0.0f);
	pos = p->GetPosition();
	p->SetOrientation(Quaternion::FromMatrix(test->GetTransform()));

	p->SetCollisionVolume(new CollisionCylinder(scale / 9.5, 0.1));
	p->SetUseGravity(false);
	g = new GameEntity(test, p);
	g->ConnectToSystems();
	
	

	firstBranch = new SceneNode(branch, Vector4(0, 0, 1, 1)); // Blue !
	firstBranch->SetModelScale(base->GetModelScale() *
		Vector3(0, 0, 0));
	firstBranch->SetTransform(base->GetWorldTransform() *
		Matrix4::Translation(Vector3(s / 15, s/2, -(s / 30)))*
		Matrix4::Rotation(50, Vector3(0, 0, 1))*
		Matrix4::Rotation(50, Vector3(0, 1, 0))
		);
	base->AddChild(firstBranch);

	test1 = new SceneNode();
	test1->SetTransform(base->GetWorldTransform() *
		Matrix4::Translation(Vector3(0, 0, 0))*
		Matrix4::Rotation(-50, Vector3(0, 0, 1))
		);
	p1 = new PhysicsNode();
	p1->SetPosition(Vector3(-40, 140, -20));
	p1->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p1->SetInverseMass(0.0f);
	pos1 = p1->GetPosition();
	p1->SetOrientation(Quaternion::FromMatrix(test1->GetTransform()));
	
	p1->SetCollisionVolume(new CollisionCylinder(scale / 19, 0.1));
	p1->SetUseGravity(false);
	g1 = new GameEntity(test1, p1);
	g1->ConnectToSystems();
	
	

	secondBranch = new SceneNode(branch, Vector4(0, 0, 1, 1)); // Blue !
	secondBranch->SetModelScale(base->GetModelScale() *
		Vector3(0, 0, 0));
	secondBranch->SetTransform(base->GetWorldTransform() *
		Matrix4::Translation(Vector3(s / 15, s/2, -(s / 30)))*
		Matrix4::Rotation(-50, Vector3(0, 0, 1))*
		Matrix4::Rotation(50, Vector3(0, 1, 0)));
	base->AddChild(secondBranch);

	test2 = new SceneNode();
	test2->SetTransform(base->GetWorldTransform() *
		Matrix4::Translation(Vector3(0, 0, 0))*
		Matrix4::Rotation(50, Vector3(0, 0, 1))
		);
	p2 = new PhysicsNode();
	p2->SetPosition(Vector3(60, 140, -20));
	p2->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p2->SetInverseMass(0.0f);
	pos2 = p2->GetPosition();
	p2->SetOrientation(Quaternion::FromMatrix(test2->GetTransform()));

	p2->SetCollisionVolume(new CollisionCylinder(scale / 19, 0.1));
	p2->SetUseGravity(false);
	g2 = new GameEntity(test2, p2);
	g2->ConnectToSystems();




	branch->SetTexture(SOIL_load_OGL_texture("../../Textures/wood.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	
}






void Tree::Update(float msec) {

	if (scale1 < s)
	{
		scale1 += (msec / (150 / s * rate));
		base->SetModelScale(Vector3(s / 15, scale1, s / 15));

		CollisionCylinder& v = *(CollisionCylinder*)g->GetPhysicsNode().GetCollisionVolume();
		
		g->GetPhysicsNode().SetPosition(pos * Vector3(1, 0.5 * (scale1/100), 1));
		v.SetHeight(scale1 );
	}

	else if (scale2 <scale1 / 2)
	{
		scale2 += (msec / (150 / s * rate));
		firstBranch->SetModelScale(Vector3(s / 30, scale2, s / 30));
		secondBranch->SetModelScale(Vector3(s / 30, scale2, s / 30));

		CollisionCylinder& v1 = *(CollisionCylinder*)g1->GetPhysicsNode().GetCollisionVolume();
		g1->GetPhysicsNode().SetPosition(pos1 * Vector3(scale2 / 100, 0.1 * ((scale2 + 900) / 100), 1));
		v1.SetHeight(scale2 - 35);

		CollisionCylinder& v2 = *(CollisionCylinder*)g2->GetPhysicsNode().GetCollisionVolume();
		g2->GetPhysicsNode().SetPosition(pos2 * Vector3(scale2 / 100, 0.1 * ((scale2 + 900) / 100), 1));
		v2.SetHeight(scale2 - 35);
		
	}

	
	
	

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_R))
	{
		reset();
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))
	{
		rate += 100;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))
	{
		rate = 100;
	}

	SceneNode::Update(msec);
}



void Tree::reset()
{
	scale1 = 0;
	scale2 = 0;

	firstBranch->SetModelScale(Vector3(0, 0, 0));
	secondBranch->SetModelScale(Vector3(0, 0, 0));
	thirdBranch->SetModelScale(Vector3(0, 0, 0));
	
}
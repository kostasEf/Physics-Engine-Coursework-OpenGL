#include "Crate.h"

Mesh * Crate::crate = NULL;


Crate::Crate(float x, float y, float z, float scale, int angle) {
	// Optional , uncomment if you want a local origin marker !
	//SetMesh ( branch );

	s = scale;
	

	base = new SceneNode(crate, Vector4(1, 0, 0, 1)); // Red !
	base->SetModelScale(Vector3(s, s, s));
	base->SetTransform(Matrix4::Translation(Vector3(x, y, z))*
		Matrix4::Rotation(angle, Vector3(1, 0, 0)));
	AddChild(base);

	test = new SceneNode();
	test->SetTransform(Matrix4::Translation(Vector3(x, y, z))*
		Matrix4::Rotation(angle, Vector3(1, 0, 0))*
		Matrix4::Rotation(90, Vector3(1, 0, 0)));

	p = new PhysicsNode();
	p->SetPosition(Vector3(x*2 + s/2 , y*2 + s/2 , z*2 + s/2 ));
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetInverseMass(0.0f);
	pos = p->GetPosition();
	p->SetOrientation(Quaternion::FromMatrix(test->GetTransform()));

	p->SetCollisionVolume(new CollisionCylinder(scale/2 , 50));
	p->SetUseGravity(false);
	g = new GameEntity(test, p);
	g->ConnectToSystems();



	//firstBranch = new SceneNode(crate, Vector4(0, 0, 1, 1)); // Blue !
	//firstBranch->SetModelScale(base->GetModelScale() *
	//	Vector3(0, 0, 0));
	//firstBranch->SetTransform(base->GetWorldTransform() *
	//	Matrix4::Translation(Vector3(s / 15, s / 2, -(s / 30)))*
	//	Matrix4::Rotation(50, Vector3(0, 0, 1))*
	//	Matrix4::Rotation(50, Vector3(0, 1, 0))
	//	);
	//base->AddChild(firstBranch);

	//test1 = new SceneNode();
	//test1->SetTransform(base->GetWorldTransform() *
	//	Matrix4::Translation(Vector3(0, 0, 0))*
	//	Matrix4::Rotation(-50, Vector3(0, 0, 1))
	//	);
	//p1 = new PhysicsNode();
	//p1->SetPosition(Vector3(-40, 140, -20));
	//p1->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	//p1->SetInverseMass(0.0f);
	//pos1 = p1->GetPosition();
	//p1->SetOrientation(Quaternion::FromMatrix(test1->GetTransform()));

	//p1->SetCollisionVolume(new CollisionCylinder(scale / 19, 0.1));
	//p1->SetUseGravity(false);
	//g1 = new GameEntity(test1, p1);
	//g1->ConnectToSystems();



	//secondBranch = new SceneNode(crate, Vector4(0, 0, 1, 1)); // Blue !
	//secondBranch->SetModelScale(base->GetModelScale() *
	//	Vector3(0, 0, 0));
	//secondBranch->SetTransform(base->GetWorldTransform() *
	//	Matrix4::Translation(Vector3(s / 15, s / 2, -(s / 30)))*
	//	Matrix4::Rotation(-50, Vector3(0, 0, 1))*
	//	Matrix4::Rotation(50, Vector3(0, 1, 0)));
	//base->AddChild(secondBranch);

	//test2 = new SceneNode();
	//test2->SetTransform(base->GetWorldTransform() *
	//	Matrix4::Translation(Vector3(0, 0, 0))*
	//	Matrix4::Rotation(50, Vector3(0, 0, 1))
	//	);
	//p2 = new PhysicsNode();
	//p2->SetPosition(Vector3(60, 140, -20));
	//p2->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	//p2->SetInverseMass(0.0f);
	//pos2 = p2->GetPosition();
	//p2->SetOrientation(Quaternion::FromMatrix(test2->GetTransform()));

	//p2->SetCollisionVolume(new CollisionCylinder(scale / 19, 0.1));
	//p2->SetUseGravity(false);
	//g2 = new GameEntity(test2, p2);
	//g2->ConnectToSystems();




	crate->SetTexture(SOIL_load_OGL_texture("../../Textures/crate.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));


}

void Crate::Update(float msec) {

	
	SceneNode::Update(msec);
}




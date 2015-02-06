#pragma once

#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Mesh.h"
#include "../../nclgl/OBJMesh.h"
#include "PhysicsNode.h"
#include "InertialMatrixHelper.h"
#include "GameEntity.h"
#include "MyGame.h"

class Crate : public SceneNode {
public:
	Crate(float x, float y, float z, float scale, int angle);
	~Crate(void){};
	virtual void Update(float msec);

	static void CreateCrate()
	{
		OBJMesh*m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube3.obj");
		crate = m;
	}


	static void DeleteBranch(){ delete crate; }

protected:

	static Mesh * crate;

	SceneNode * base;

	SceneNode * test;
	SceneNode * test1;
	SceneNode * test2;
	PhysicsNode* p;
	PhysicsNode* p1;
	PhysicsNode* p2;
	GameEntity * g;
	GameEntity * g1;
	GameEntity * g2;

	SceneNode * firstCrate;
	SceneNode * secondCrate;
	SceneNode * thirdCrate;

	float s;

	

	Vector3 pos;
	Vector3 pos1;
	Vector3 pos2;


};

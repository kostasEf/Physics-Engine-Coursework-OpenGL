#pragma once

#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Mesh.h"
#include "PhysicsNode.h"
#include "InertialMatrixHelper.h"
#include "GameEntity.h"
#include "MyGame.h"

class Tree : public SceneNode {
public:
	Tree(float x, float y, float z, float scale);
	~Tree(void){};
	virtual void Update(float msec);

	static void CreateBranch()
	{
		Mesh * m = new Mesh();
		branch = m->GenerateWood();
	}
	
	static void DeleteBranch(){ delete branch; }
	
	void reset();

protected:

	static Mesh * branch;
	
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

	SceneNode * firstBranch;
	SceneNode * secondBranch;
	SceneNode * thirdBranch;
	

	float  scale1;
	float  scale2;
	
	float s;

	int rate;

	Vector3 pos;
	Vector3 pos1;
	Vector3 pos2;

	
};

#include "PhysicsSystem.h"
#include "CollisionHelper.h"

PhysicsSystem* PhysicsSystem::instance = 0;

PhysicsSystem::PhysicsSystem(void)	{
	frame = time = counter = score = 0;
}

PhysicsSystem::~PhysicsSystem(void)	{
	for (int i = 0; i < allSprings.size(); i++) {
		delete allSprings[i];
	}
}

void	PhysicsSystem::Update(float msec) {	
	BroadPhaseCollisions();
	NarrowPhaseCollisions();

	for(vector<Constraint*>::iterator i = allSprings.begin(); i != allSprings.end(); ++i) {
		(*i)->Update(msec);
	}

	for(vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
		(*i)->Update(msec);
	}

	frame++;
	time += msec;

	if (time > 1000)
	{
		fps = frame;
		time -= 1000;
		frame = 0;
	}
}

void	PhysicsSystem::BroadPhaseCollisions() {

}

void	PhysicsSystem::NarrowPhaseCollisions() {
	
	float r2 = -0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5 + 0.5)));
	for (int i = 0; i < allNodes.size(); i++) {
		PhysicsNode& first = *allNodes[i];
		CollisionVolume* fv = first.GetCollisionVolume();
		if (!fv) continue;
		for (int j = i + 1; j < allNodes.size(); j++) {
			PhysicsNode& second = *allNodes[j];
			CollisionVolume* sv = second.GetCollisionVolume();
			if (!sv) continue;

			switch(fv->GetType()) {
			case COLLISION_VOL_SPHERE:
				switch(sv->GetType()) {
				case COLLISION_VOL_SPHERE:
					CollisionData data;
					if (CollisionHelper::SphereSphereCollision(first, second, &data)) {

						if (first.GetSmart() == true )
						{
							first.SetResting(false);
							first.SetForce((first.GetPosition() - second.GetPosition()) * 0.001);
							first.SetResting(false);
							continue;
						}
						if (second.GetSmart() == true)
						{
							second.SetForce((second.GetPosition() - first.GetPosition()) * 0.001);
							continue;
						}

						if (first.GetPig() && second.GetProjectile() || second.GetPig() && first.GetProjectile())
						{
							score += 10 * abs(second.GetLinearVelocity().x + second.GetLinearVelocity().y + second.GetLinearVelocity().z);
						}

						CollisionHelper::AddCollisionImpulse(first, second, data);
						if (first.GetInverseMass() != 0)	first.SetResting(false);
						if (second.GetInverseMass() != 0)	second.SetResting(false);

						first.SetCounter(first.GetCounter() + 1);
						second.SetCounter(second.GetCounter() + 1);

						if (!second.GetResting()) counter++;
						if (!first.GetResting()) counter++;
					}
					continue;

				}
			case COLLISION_VOL_PLANE:
				switch(sv->GetType()) {
				case COLLISION_VOL_SPHERE:
					CollisionData data;
					if (CollisionHelper::PlaneSphereCollision(first, second, &data)) {
						CollisionHelper::AddCollisionImpulse(first, second, data);
					}
					continue;
				}
			case COLLISION_VOL_CYLINDER:
				switch (sv->GetType()) {
				case COLLISION_VOL_SPHERE:
					CollisionData data;
					if (CollisionHelper::SphereCapsuleCollision(first, second, &data)) {
						CollisionHelper::AddCollisionImpulse(second, first, data);
						if (!second.GetResting()) counter++;
					}
					continue;
				}

			}
			
		}
		
	}

}

void	PhysicsSystem::AddNode(PhysicsNode* n) {
	allNodes.push_back(n);
}

void	PhysicsSystem::RemoveNode(PhysicsNode* n) {
	for(vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
		if((*i) == n) {
			allNodes.erase(i);
			return;
		}
	}
}

void	PhysicsSystem::AddConstraint(Constraint* s) {
	allSprings.push_back(s);
}

void	PhysicsSystem::RemoveConstraint(Constraint* c) {
	for(vector<Constraint*>::iterator i = allSprings.begin(); i != allSprings.end(); ++i) {
		if((*i) == c) {
			allSprings.erase(i);
			return;
		}
	}
}

void	PhysicsSystem::AddDebugDraw(DebugDrawer* d) {
	allDebug.push_back(d);
}

void	PhysicsSystem::RemoveDebugDraw(DebugDrawer* d) {
	for(vector<DebugDrawer*>::iterator i = allDebug.begin(); i != allDebug.end(); ++i) {
		if((*i) == d) {
			allDebug.erase(i);
			return;
		}
	}
}

void    PhysicsSystem::DrawDebug() {
	for(vector<DebugDrawer*>::iterator i = allDebug.begin(); i != allDebug.end(); ++i) {
		(*i)->DebugDraw();
	}
}
#pragma once

/**
 * Author: Callum Rhodes <c.g.rhodes@ncl.ac.uk>
 */
#include "../../nclgl/Vector3.h"

enum CollisionVolumeType { COLLISION_VOL_SPHERE, COLLISION_VOL_CYLINDER, COLLISION_VOL_PLANE, COLLISION_VOL_AABB };

class CollisionVolume {
public:

	virtual CollisionVolumeType GetType() const = 0;
};


class CollisionSphere : public CollisionVolume {
public:

	CollisionSphere(float radius): radius(radius) {}

	CollisionVolumeType GetType() const {
		return COLLISION_VOL_SPHERE;
	}

	float GetRadius() const {
		return radius;
	}

private:
	float radius;
};

class CollisionCylinder : public CollisionVolume {
public:

	CollisionCylinder(float radius, float height) : radius(radius), height(height) {}

	CollisionVolumeType GetType() const {
		return COLLISION_VOL_CYLINDER;
	}

	float GetRadius() const { return radius; }

	float GetHeight() const { return height; }

	void SetHeight(float value) { height = value; }

private:
	float radius;
	float height;
};


class CollisionPlane : public CollisionVolume {
public:

	CollisionPlane(Vector3 normal, float distance): distance(distance), normal(normal) {}

	CollisionVolumeType GetType() const {
		return COLLISION_VOL_PLANE;
	}

	Vector3 GetNormal() const {
		return normal;
	}

	float GetDistance() const {
		return distance;
	}

private:
	float distance;
	Vector3 normal;
};

class CollisionAABB : public CollisionVolume {
public:
	CollisionAABB(Vector3 halfDim): halfDim(halfDim) {}

	CollisionVolumeType GetType() const {
		return COLLISION_VOL_AABB;
	}

	Vector3 getHalfDimensions() const { return halfDim; }

private:
	Vector3 halfDim;
};


class CollisionData {
public:
	Vector3 m_point;
	Vector3 m_normal;
	float m_penetration;
};
#pragma once
#include <box2d/box2d.h>
#include <vector>
struct PhysicsDynamicBody {
	b2Body* body;
	b2Fixture* mainFixture;
	std::vector<b2Fixture*> addedFixtures;
	bool bUseFixtureVector;
	bool bIsRotating = true;
	bool bSpawned = false;
	bool bUsesGravity = true;
};

struct PhysicsStaticBody {
	b2Body* body;
	b2Fixture* mainFixture;
	std::vector<b2Fixture*> addedFixtures;
	bool bUseFixtureVector;
	bool bSpawned = false;
};

struct PhysicsDampening {
	float DesiredLinearDamping = .3f;
	float HorizontalDamping = 0;
	b2Vec2 DesiredVelocity = b2Vec2(0, 0);
};
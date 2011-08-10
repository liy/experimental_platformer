#pragma once
#include "acBody.h"
#include "AAnimation.h"

class Scene;

// forward declaration of floating vector2
template<class T> class Vec2;
typedef class Vec2<float> Vec2f;

// TODO: needs extra work

// Abstract class
// The actor is a physical object in the game that has life. AI or user controlled element.
// Since those things are always moving and definitely has a animation. So only animation pointer is needed for graphical representation.
class Actor
{
public:
	Actor(void){};

	Actor(Scene* scene){
		scene_ptr = scene;

		animation_ptr = NULL;
		body_ptr = NULL;
	};

	Actor(Scene* scene, AAnimation* ani, acBody* body){
		scene_ptr = scene;
		animation_ptr = ani;
		body_ptr = body;
	}

	virtual ~Actor(void){};

	// TODO: testing... scene pointer
	Scene* scene_ptr;

	// image pointer, dynamic memory allocation is needed
	AAnimation* animation_ptr;

	// rigid body, dynamic memory allocation is needed
	acBody* body_ptr;

	virtual const Vec2f& position() const{
		//  No  position directly access and modification allowed, since we need to recompute the AABB in the SetPosition function.
		return body_ptr->position;
	};

	virtual void SetPosition(float x, float y){
		body_ptr->position.Set(x, y);
		body_ptr->Synchronize();
	};

	virtual void SetPosition(const Vec2f& p){
		body_ptr->position.Set(p.x, p.y);
		body_ptr->Synchronize();
	};

	// Should be called after your customized update process
	virtual void Update(unsigned short deltaTime){
		// update the animation.
		animation_ptr->Update(deltaTime);

		// update the body's AABB.
		body_ptr->Synchronize();
	};

	virtual void Draw(){
		animation_ptr->Draw(body_ptr->position.x, body_ptr->position.y, 0.0f, body_ptr->rotation);
		body_ptr->DrawAABB(1.0f, 0.3f, 0.1f);
	};
};


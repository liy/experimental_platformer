#include "Actor.h"
#include "Scene.h"
#include "game_settings.h"
#include "AImage.h"
#include "AAnimation.h"
#include "acBody.h"
#include "Tile.h"
#include <GL\glut.h>
#include <GL\GL.h>
#include "acCollision.h"

Actor::Actor(Scene* scene):GamepadEventListener(), _floating(true), _wallGrabbed(false), _floatingTimer(0){
	scene_ptr = scene;

	image_ptr = NULL;
	body_ptr = NULL;
	animation_ptr = NULL;
}

Actor::Actor(Scene* scene, acBody* body): _floating(true), _wallGrabbed(false), _floatingTimer(0){
	scene_ptr = scene;
	body_ptr = body;

	image_ptr = NULL;
	animation_ptr = NULL;
}

Actor::~Actor(void)
{
	if(body_ptr != NULL)
		delete body_ptr;
	if(image_ptr != NULL)
		delete image_ptr;
	if(animation_ptr != NULL)
		delete animation_ptr;
}

void AdjustTOI(const float t, const Vec2f& n, acBody& actorBody, const acBody* tileBody){
	Vec2f actorBodyExtent = actorBody.aabb.GetExtents();
	
	// Adjust the actor position along  Y axis. 
	// Imagine the actor is walking on a flat surface(made by multiple tiles). Although we can fast forward object to the TOI point: position += toi*velocity. This ensure the object never penetrate any tiles.
	// However, over the different update frame, TOI will be vary, any number from [0,  1], which obviously will cause serious floating along Y axis, the actor could penetrate the ground tile little by little. This is because AABB raycast cannot detect 
	// any collisions already penetrated, or overlapped. Touch collision is fine to detect, since TOI will be 0.
	// Therefore we need to readjust the actor's Y position, directly set its Y component to the first collided tile's upper or lower Y position. This step will still suffering floating point error... The main difference is that if surface tiles in perfect mathmaticaly
	// world are laying in the same Y position, their Y floating point number in the computer will also be the same. But the next frame their upper or lower position will be stay relatively the same!
	// By readjusting the Y position, the object can get the correct Y position, and can safely walk through different ground tiles surface.
	//
	// After we adjusted the Y position, the other direction's movement will still needs to be performed. But this time is simple, since only one X component movement is concerend, simply using SAT to resolve any penetration along X axis, you can add some EPSILON to
	// ensure next frame's will never have any penetration.
	if(n.x == 0.0f){
		// hit the ceiling, adjust to the tile's bottom position
		if(n.y < 0.0f){
			actorBody.position.y = tileBody->aabb.lowerBound.y - actorBodyExtent.y;
		}
		// hit the ground, adjust to the tile's top position
		else{
			actorBody.position.y = tileBody->aabb.upperBound.y + actorBodyExtent.y;
		}
	}
	else{
		// hit right of the tile, adjust to tile's right position
		if(n.x > 0.0f){
			//actorBody.position.x = tileBody->aabb.upperBound.x + actorBodyExtent.x;

			// adding 1px for complete seperate, so that the jumping or falling will not hit the bottom or top edge of the tile
			// corresponding SAT seperation will also need 1px as well
			actorBody.position.x = tileBody->aabb.upperBound.x + actorBodyExtent.x + n.x;
		}
		// hit left of the tile, adjust to tile's left position
		else{
			//actorBody.position.x = tileBody->aabb.lowerBound.x - actorBodyExtent.x;

			// adding 1px for complete seperate, so that the jumping or falling will not hit the bottom or top edge of the tile
			// corresponding SAT seperation will also need 1px as well
			actorBody.position.x = tileBody->aabb.lowerBound.x - actorBodyExtent.x + n.x;
		}
	}
}

Vec2f gravity(0.0f, -0.24f);
void Actor::Update(unsigned short deltaTime){
	body_ptr->velocity += gravity;

	Tile* tiles = scene_ptr->tiles;

	////std::cout << "update called\n";
	float tmin = FLT_MAX;
	Vec2f normal;
	bool collide = false;
	Tile* collidedTile;
	for(int i=0; i<NUM_TILES; ++i){
		AABB2f mdAABB = tiles[i].GetBody()->aabb - body_ptr->aabb;

		RayCastInput input;
		input.maxFraction = 1.0f;
		input.p1 = vec2_zero;
		input.p2 = body_ptr->velocity;

		RayCastOutput output;
		if(mdAABB.RayCast(&output, input)){
			collide = true;
			
			if(tmin > output.fraction){
				tmin = output.fraction;
				normal = output.normal;
				collidedTile = &tiles[i];
			}
		}
	}
	//std::cout << "collide " << collide <<"\n";

	if(collide){
		// TODO: change to relative velocity
		// forward the poistion to the positon of TOI
		body_ptr->position += tmin * body_ptr->velocity;

		// Because of the floating point error, we have to readjust the position of the body.
		AdjustTOI(tmin, normal, *body_ptr, collidedTile->GetBody());

		// TODO: you can add collision response code here, dispatch event etc.

		// update the velocity to the velocty after impact
		Vec2f rv = body_ptr->velocity - (1.0f)*(normal * body_ptr->velocity.Dot(normal));

		// Since we only fast foward the object to the time of impact.
		// The object still have amount of time to travel, that is 1-tmin. but now it should use
		// the velocity after the impact
		body_ptr->position += (1.0f-tmin)*rv;

		// TODO: Depending on the contact normal direction, you can specify wether to clamp velocity's two components or not.
		// For example, 
		//  1.  when the contact normal is pointed horizontally which meas we hit the wall. We can multiply original Y velocity by ratio which will reduce the actor's falling speed. This achive a wall grabbing effect.
		//  2.  If the contact normal is pointed downward, the actor hit the ceiling. If we do not update the Y velocity to the velocity after impact: "rv.y". 
		//       The actor will "stick" to the ceiling, until gravity pulls it down. You can also apply a ratio: "body.velocity.y = body.velocity.y * ratio" to allow "partly sticking".
		if(normal.y == -1.0f){
			body_ptr->velocity.y *= 0.9;
		}

		// wall grabbing effect, and also allow jump again
		if(normal.x != 0.0f){
			if(body_ptr->velocity.y < 0.0f){
				body_ptr->velocity.y *= 0.3;

				GrabWall(true);
			}
		}

		// landing
		if(normal.y == 1.0f){
			Land();
		}

		// in order to persit the wall grabbing. We should not update the horizontal velocity.
		// Only when user is not grabbing the walls, eg. grounded and hit the wall, we shall 0 the horizontal velocity.
		// This ensures the wall collision detection is consitent across all updates in the update loops.
		if(!_wallGrabbed){
			body_ptr->velocity.x = rv.x;
		}


		// now since above displacement can cause penetration, we have to check if there is any penetration or not.
		// If there is penetration, use the minimum penetration distance to seperate the object.
		// Since all we use are AABBs, it make sense simply use SAT to detect the penetration and minimum speration vector.
		Vec2f minTransV;
		Vec2f penetrationNormal;
		for(int i=0; i<NUM_TILES; ++i){
			// Using AABB directly to calculate SAT require body's AABB is up to date! Theirfore we need to recompute AABB depending on the current body transform.
			body_ptr->Synchronize();
			if(AABBSAT(tiles[i].GetBody()->aabb, body_ptr->aabb, penetrationNormal, minTransV)){
				//body_ptr->position += sd*penetrationNormal;// + penetrationNormal;
				//std::cout << "New [" << minTransV.x <<", "<< minTransV.y << "]\n";
				// Adding 1px to complete seperate two object, so future time actor will not hitting the edges of the tile.
				body_ptr->position += minTransV + penetrationNormal;
			}
		}
	}
	// actor is floating in the air........... floating mode.
	else{
		// increase the floating timer (counter) since it is in floating mode
		_floatingTimer+=deltaTime;

		//std::cout << "floating \n";
		if(_wallGrabbed){
			body_ptr->velocity.y *= 0.3;
		}
		
		// directly translate the position
		body_ptr->position += body_ptr->velocity;
		
		// The collision is consistence while grabbing the wall, so we can saftly ungrab the wall if no collision is found. (When grounded, _wallGrabbed is always false)
		GrabWall(false);

		// After a short delay, we will set floating mode to true, so actor can not jump anymore.
		// delay to set actor in floating mode
		if(_floatingTimer > FLOATING_TOLERENCE){
			_floating = true;
		}
	}
	
	// TODO: TESTING ONLY screen bottom boundary.
	if(body_ptr->position.y < body_ptr->aabb.GetExtents().y){
		Land();
		body_ptr->position.y = body_ptr->aabb.GetExtents().y;
	}

	// update the animation.
	if(animation_ptr != NULL)
		animation_ptr->Update();
	
	// update the body's AABB.
	body_ptr->Synchronize();
}

void Actor::Land(){
	body_ptr->velocity.y = 0.0f;
	_floating = false;
	_wallGrabbed = false;
	_floatingTimer = 0;
}

void Actor::GrabWall(bool grabbed){
	_wallGrabbed = grabbed;

	// Only if we are grabbing the wall, we set the floating to false.
	// However if we are not grabbing the wall, floating==false will not be changed. we allow a tiny gap before the floating is set to true by update method. The purpose of this is for the actor to do wall jumping.
	// Note that the actor can only jump when:
	//	1. It is NOT GRABBING anything
	//	2. It is NOT FLOATING.
	// Therefore, if we ungrab the wall, for a small amount of time, the floating will still be false, so actor can perform the jump
	if(grabbed){
		_floating = false;
		_floatingTimer = 0;
		//std::cout << "grab \n";
	}
	else{
		//std::cout << "ungrab \n";
	}
}


void Actor::Draw(){
	body_ptr->DrawAABB(1.0f, 0.3f, 0.1f);

	if(image_ptr != NULL)
		image_ptr->Draw(body_ptr->position, body_ptr->rotation); 
	if(animation_ptr != NULL)
		animation_ptr->Draw(body_ptr->position, body_ptr->rotation);

}

void Actor::Move(float xRatio, float yRatio){
	//body_ptr->velocity.Set(4.0f*xRatio, -4.0f*yRatio);
	body_ptr->velocity.x = 3.0f * xRatio;
}

void Actor::Jump(){
	// only when you released the button first, then you can jump agian... do not allow auto button trigger.
	if(_jumpRelased){
		GamepadEventListener::Jump();

		if(!_floating && !_wallGrabbed){
			_wallGrabbed = false;
			_floating = true;
			body_ptr->velocity.y = 7.0f;
		}
	}
}

void Actor::JumpRelease(){
	GamepadEventListener::JumpRelease();

	if(_floating && body_ptr->velocity.y > 0.0f){
		body_ptr->velocity.y *= 0.8f;
	}
}

void Actor::Stop(){

}

const Vec2f& Actor::GetPosition(){
	return body_ptr->position;
}

void Actor::SetPosition(const Vec2f& p){
	body_ptr->position.Set(p.x, p.y);
	// FIXME: remove the position directly access, since we need to recomput the AABB here
	 body_ptr->Synchronize();

}
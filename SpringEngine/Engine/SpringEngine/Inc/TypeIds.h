#pragma once

namespace SpringEngine
{
	enum class ComponentId
	{
		Invalid,                // Invaild component id
		Transform,              // Transform for component, contains positional data 
		Camera,                 // Stationary camera information
		FPSCamera,              // Functionality for a camera to move
		Mesh,					// Renders a MeshBuilt shape
		Model,					// For Render a model
		Animator,               // Controls a Model's animation
		RigidBody,				// Adds a rigidbody to the world
		SoundEffect,            // Adds a single sound effect to an object
		SoundBank,              //Adds an array of sound effects to an object that can be triggred
		UISprite,               //Adds a sprite to the UI
		UIButton,               //Adds a button to the UI
		UIText,                 // Adds a text to the UI

		Count
	};

	enum  class ServiceId
	{
		Invalid,               // invalid service id
		Camera,                // Registers and manages focus camera
		Update,				   // Registers and updates any component that requires an update
		Render,                // Registers and renders any graphical components
		Physics,               // Registers and updates physics based on object
		UIRender,              // Registers and renders UI components

		Count
	};
}

#define SET_TYPE_ID(id)\
static uint32_t StaticGetTypeId(){return static_cast<uint32_t>(id);}\
uint32_t GetTypeId() const override{return StaticGetTypeId();}
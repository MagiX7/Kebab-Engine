#include "Globals.h"
#include "Light.h"
#include "GL/glew.h"
#include <gl/GL.h>
//#include <gl/GLU.h>

OldLight::OldLight() : ref(-1), on(false), position(0.0f, 0.0f, 0.0f)
{}

void OldLight::Init()
{
	glLightfv(ref, GL_AMBIENT, &ambient);
	glLightfv(ref, GL_DIFFUSE, &diffuse);
}

void OldLight::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void OldLight::Render()
{
	if(on)
	{
		float pos[] = {position.x, position.y, position.z, 1.0f};
		glLightfv(ref, GL_POSITION, pos);
	}
}

void OldLight::Active(bool active)
{
	if(on != active)
	{
		on = !on;

		if(on)
			glEnable(ref);
		else
			glDisable(ref);
	}
}
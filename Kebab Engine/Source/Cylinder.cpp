#include "Cylinder.h"

KbCylinder::KbCylinder(math::float3 pos, float baseRadius, float height, float segments, GameObject* owner) : KbGeometry(pos, owner)
{
	/*Vertex vertex;
	for (int i = 0; i < segments; ++i)
	{
		float theta = ((float)i) * 2 * pi;
		float nextTheta = ((float)i + 1) * 2 * pi;

		vertex.position = {}


	}*/

}

KbCylinder::~KbCylinder()
{
}

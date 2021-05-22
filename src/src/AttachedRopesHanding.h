#pragma once

void UpdateRopes();
void addRope(Rope* rope);
bool doesEntityAttachedToRope(Entity entity);
Rope* getClosestRopeWithin(Vector3 position, float radius);

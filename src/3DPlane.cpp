#include "3DPlane.hpp"
#include "Misc/Primitives.h"

void Plane3D::Init(int w16, int h16) {
	// In any case the funny happens
	if(w16 < 1) w16 = 1;
	if(h16 < 1) h16 = 1;

	const float unit = 16.0f;
	float mx = float(w16 * unit) * -0.5f, my = float(h16 * unit) * -0.5f;
	float gtx = 1.0f / float(w16), gty = 1.0f / float(h16);
	float tx = 0.0f, ty = 0.0f;
	TLVertex3D* verts = new TLVertex3D[w16 * h16 * 8 * 6];
	int mi = 0;
	for(int yi = 0; yi < h16; yi++) {
		mx = float(w16 * unit) * -0.5f;
		for(int xi = 0; xi < w16; xi++) {
			verts[mi * 6] =     {mx + unit, my       , 0.0f, 0xffffffff, tx + gtx, ty, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 1] = {mx       , my       , 0.0f, 0xffffffff, tx      , ty, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 2] = {mx + unit, my + unit, 0.0f, 0xffffffff, tx + gtx, ty + gty, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 3] = {mx       , my       , 0.0f, 0xffffffff, tx      , ty, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 4] = {mx + unit, my + unit, 0.0f, 0xffffffff, tx + gtx, ty + gty, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 5] = {mx       , my + unit, 0.0f, 0xffffffff, tx      , ty + gty, 0.0f, 0.0f, 1.0f};
			mx += unit;
			mi++;
		}
		my += unit;
	}
	CreateVertexBuffer(w16 * h16 * 6, verts);
	CreateIndirectDraw(w16 * h16 * 6);
	m_DrawType = GL_TRIANGLES;
	delete[] verts;

	m_Transform.Init();
}

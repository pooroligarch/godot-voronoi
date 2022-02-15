
#include "voronoi.h"

void Voronoi::setup(real_t x_min, real_t x_max, real_t y_min, real_t y_max, real_t z_min, real_t z_max) {
	this->x_min = x_min;
	this->x_max = x_max;
	this->y_min = y_min;
	this->y_max = y_max;
	this->z_min = z_min;
	this->z_max = z_max;
}

void Voronoi::add_point(Vector3 point) {
	points.push_back(point);
}

void Voronoi::voronoi() {

	int nx = 0, ny = 0, nz = 0;
	voro::pre_container pre(x_min, x_max, y_min, y_max, z_min, z_max, false, false, false);
	voro::container con(x_min, x_max, y_min, y_max, z_min, z_max, nx, ny, nz, false, false, false, 8);

	for (int i = 0; i < points.size(); i++) { // Work around voro++ not allowing assignments to containers
		Vector3 &v = points[i];
		pre.put(i, v.x, v.y, v.z);
	}
	// Create a container from the object's pre_container

	pre.setup(con);

	voro::c_loop_all loop(con);
	voro::voronoicell cell;

	if(loop.start()) do if(con.compute_cell(cell, loop)) {	
			int i = 0; // current face

			std::vector<PackedVector3Array> faces; // faces[face[vertex]]

            std::vector<int> fverts;
			std::vector<double> verts;

			cell.face_vertices(fverts);
			cell.vertices(verts);

			for (int j = 0; j < sizeof(fverts); j++) {	
				if (fverts[j] == 0) {
					i++;
				} else {
				// Store the vertex positions as Vector3 into the fragment
				faces[i].push_back(Vector3(verts[fverts[j]*3], verts[fverts[j]*3 + 1], verts[fverts[j]*3 + 2]));
				}
			}

			frags.push_back(faces);

     } while (loop.inc());
	
}

PackedVector3Array Voronoi::get_face(int frag, int face) {
	return frags[frag][face];
}

void Voronoi::_bind_methods() {
	ClassDB::bind_method(D_METHOD("setup", "x_min", "x_max", "y_min", "y_max", "z_min", "z_max"), &Voronoi::setup);
	ClassDB::bind_method(D_METHOD("add_point", "point"), &Voronoi::add_point);
	ClassDB::bind_method(D_METHOD("voronoi"), &Voronoi::voronoi);
	ClassDB::bind_method(D_METHOD("get_face", "fragment", "face"), &Voronoi::get_face);
}

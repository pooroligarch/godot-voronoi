
#include "voronoi.h"

void Voronoi::setup(Vector3 min, Vector3 max) {
	x_min = min.x;
	x_max = max.x;
	y_min = min.y;
	y_max = max.y;
	z_min = min.z;
	z_max = max.z;
}

void Voronoi::add_point(Vector3 point) {
	points.push_back(point);
}

void Voronoi::set_points(PackedVector3Array array) {
	points = array;
}

PackedVector3Array Voronoi::get_points() {
	return points;
}

/** Computes the Voronoi tessellation and stores the results.
 * \param[in] dp a displacement vector to be added to the resulting positions. */
void Voronoi::compute(Vector3 dp) {

	int nx = 0, ny = 0, nz = 0;
	double dx = x_max - x_min, dy = y_max - y_min, dz = z_max - z_min;
	double ilscale = pow(points.size()/(5.6*dx*dy*dz), 1/3.0); // "borrowed" from voro++ because pre_container is broken
	nx = int(dx * ilscale + 1);
	ny = int(dy * ilscale + 1);
	nz = int(dz * ilscale + 1);

	voro::container con(x_min, x_max, y_min, y_max, z_min, z_max, nx, ny, nz, false, false, false, 8);

	for (int i = 0; i < points.size(); i++) { // Work around voro++ not allowing assignments to containers
		con.put(i, points[i].x, points[i].y, points[i].z);
	}

	voro::c_loop_all loop(con);
	voro::voronoicell cell;

	if(loop.start()) do if(con.compute_cell(cell, loop)) {	

			Vector<PackedVector3Array> frag; // frag[face[vertex]]

            std::vector<int> fverts;
			std::vector<double> verts;
			PackedVector3Array face;

			std::vector<std::vector<double>> positions;

			cell.draw_gnuplot(dp.x, dp.y, dp.z, positions);

			int n = fverts[0]; // number of points in face

			//char str[10];

			for (int j = 0; j < positions.size(); j++) {
				for (int k = 0; k < positions[j].size(); k += 3) {
					face.push_back(Vector3(positions[j][k], positions[j][k+1], positions[j][k+2]));
				}
				frag.push_back(face);
			}

			/*for (int j = 1; j < fverts.size(); j++) {	

				if (n == 0) {
					frag.push_back(face);
					n = fverts[j];
					itoa(n, str, 10);
					WARN_PRINT(str);

				} else {
					// Store vertex positions into the face
					face.push_back(Vector3(verts[fverts[j]*3], verts[fverts[j]*3 + 1], verts[fverts[j]*3 + 2]));
				}
				n--;
			}*/

			frags.push_back(frag);

     } while (loop.inc());
	 con.draw_particles("random_points_p.gnu");
	 con.draw_cells_gnuplot("random_points_v.gnu");
	
}

PackedVector3Array Voronoi::get_face(int frag, int face) {
	return frags[frag][face];
} 

/*Vector<PackedVector3Array> Voronoi::get_frag(int frag) {
	return frags[frag];
}

void Voronoi::set_frag(const Vector<PackedVector3Array> &value, int frag) {
	frags[frag] = value;
}*/

int Voronoi::get_num_frags() {
	return frags.size();
}

int Voronoi::get_num_faces(int frag_idx) {
	return frags[frag_idx].size();
}

void Voronoi::_bind_methods() {
	ClassDB::bind_method(D_METHOD("setup", "min", "max"), &Voronoi::setup);
	ClassDB::bind_method(D_METHOD("add_point", "point"), &Voronoi::add_point);
	ClassDB::bind_method(D_METHOD("get_points"), &Voronoi::get_points);
	ClassDB::bind_method(D_METHOD("set_points", "points"), &Voronoi::set_points);

	ClassDB::bind_method(D_METHOD("compute", "displacement"), &Voronoi::compute);
	ClassDB::bind_method(D_METHOD("get_face", "fragment", "face"), &Voronoi::get_face);
	ClassDB::bind_method(D_METHOD("get_num_frags"), &Voronoi::get_num_frags);
	ClassDB::bind_method(D_METHOD("get_num_faces", "frag"), &Voronoi::get_num_faces);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR3_ARRAY, "points"), "set_points", "get_points");
}


#include "voronoi.h"
#define EPSILON 1.19e-07

void Voronoi::setup(Vector3 min, Vector3 max)
{
	x_min = min.x;
	x_max = max.x;
	y_min = min.y;
	y_max = max.y;
	z_min = min.z;
	z_max = max.z;
}

void Voronoi::add_point(Vector3 point)
{
	points.push_back(point);
	dirty = true;
}

void Voronoi::set_points(PackedVector3Array array)
{
	points = array;
	dirty = true;
}

PackedVector3Array Voronoi::get_points()
{
	return points;
}

/** Computes the Voronoi tessellation and stores the results. */
void Voronoi::compute()
{
	if (!dirty && !frags.is_empty())
	{
		WARN_PRINT("returned cached frags");
		return;
	}

	Vector<PackedVector3Array> frag;
	PackedVector3Array face;

	int nx = 0, ny = 0, nz = 0;
	double dx = x_max - x_min, dy = y_max - y_min, dz = z_max - z_min;
	double ilscale = pow(points.size() / (5.6 * dx * dy * dz), 1 / 3.0); // "borrowed" from voro++ because pre_container is broken
	nx = int(dx * ilscale + 1);
	ny = int(dy * ilscale + 1);
	nz = int(dz * ilscale + 1);

	voro::container con(x_min, x_max, y_min, y_max, z_min, z_max, nx, ny, nz, false, false, false, 8);

	for (int i = 0; i < points.size(); i++)
	{ // Work around voro++ not allowing assignments to containers
		con.put(i, points[i].x, points[i].y, points[i].z);
	}

	voro::c_loop_all loop(con);
	voro::voronoicell cell;

	if (loop.start())
		do
			if (con.compute_cell(cell, loop))
			{

				std::vector<int> fverts;
				std::vector<double> verts;

				cell.face_vertices(fverts);
				cell.vertices(verts);

				real_t x;
				real_t y;
				real_t z;

				int n = fverts[0]; // number of points in face

				for (int j = 1; j < fverts.size(); j++)
				{

					if (n == 0)
					{
						frag.push_back(face); // new face
						n = fverts[j];
					}
					else
					{
						x = verts[fverts[j] * 3];
						y = verts[fverts[j] * 3 + 1];
						z = verts[fverts[j] * 3 + 2];

						Vector3 vec(x, y, z);

						// Store vertex positions into the face
						face.push_back(vec);
					}
					n--;
				}

				frags.push_back(frag); // new frag
				frag.clear();
			}
		while (loop.inc());

	dirty = false;
}

PackedVector3Array Voronoi::get_face(int frag_idx, int face_idx) {
	return frags[frag_idx][face_idx];
}

int Voronoi::get_num_frags() {
	return frags.size();
}

int Voronoi::get_num_faces(int frag_idx) {
	return frags[frag_idx].size();
}

void Voronoi::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("setup", "min", "max"), &Voronoi::setup);
	ClassDB::bind_method(D_METHOD("add_point", "point"), &Voronoi::add_point);

	ClassDB::bind_method(D_METHOD("get_face"), &Voronoi::get_face);
	ClassDB::bind_method(D_METHOD("get_num_frags"), &Voronoi::get_num_frags);
	ClassDB::bind_method(D_METHOD("get_num_faces", "frag_idx"), &Voronoi::get_num_faces);

	ClassDB::bind_method(D_METHOD("get_points"), &Voronoi::get_points);
	ClassDB::bind_method(D_METHOD("set_points", "points"), &Voronoi::set_points);

	ClassDB::bind_method(D_METHOD("compute"), &Voronoi::compute);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR3_ARRAY, "points"), "set_points", "get_points");
}

#ifndef VORONOI_CLASS_H
#define VORONOI_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include "voro/src/voro++.hh"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/templates/vector.hpp>

using namespace godot;

class Voronoi : public RefCounted {
    GDCLASS(Voronoi, RefCounted);

protected:

    static void _bind_methods();

private:
    
    PackedVector3Array points;
    int particle_id;
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double z_min;
    double z_max;

public:

    std::vector<std::vector<PackedVector3Array>> frags;

    void setup(Vector3 min, Vector3 max);
    void add_point(Vector3 point);
    void set_points(PackedVector3Array array);
    PackedVector3Array get_points();

    void compute();
    PackedVector3Array get_face(int frag, int face);
    int get_num_frags();
    int get_num_faces(int frag_idx);

    Voronoi(){};
    ~Voronoi() = default;

};
#endif // ! VORONOI_CLASS_H

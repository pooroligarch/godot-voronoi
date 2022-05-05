# godot-voronoi
This extension adds voronoi diagram support (using [voro++](https://github.com/chr1shr/voro)) to Godot. Useful for procedural destruction, texturing (?) and probably some other things.

## Using
To get started, create a new `Voronoi` object and `setup()` it with the bounding box. `get_aabb().position` and `end` are great for that.

`Voronoi::compute()` returns a `PackedVector3Array` with a special format - one empty (`Vector3()`) vector starts a new face, two empty vectors in a row start a new fragment. See [Breakable.gd](https://github.com/pooroligarch/godot-voronoi-project/blob/master/Breakable.gd) for an example.
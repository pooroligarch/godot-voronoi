
#include "voro++.hh"
#include <iostream>
#include <vector>
using namespace voro;

// Set up constants for the container geometry
const double x_min=-1,x_max=1;
const double y_min=-1,y_max=1;
const double z_min=-1,z_max=1;
const double cvol=(x_max-x_min)*(y_max-y_min)*(z_max-z_min);

// Set up the number of blocks that the container is divided into
const int n_x=6,n_y=6,n_z=6;

// Set the number of particles that are going to be randomly introduced
const int particles=20;

// This function returns a random double between 0 and 1
double rnd() {return double(rand())/RAND_MAX;}

int main() {
	int i;
	double x,y,z;

	// Create a container with the geometry given above, and make it
	// non-periodic in each of the three coordinates. Allocate space for
	// eight particles within each computational block
	container con(x_min,x_max,y_min,y_max,z_min,z_max,n_x,n_y,n_z,
			false,false,false,8);

	// Randomly add particles into the container
	for(i=0;i<particles;i++) {
		x=x_min+rnd()*(x_max-x_min);
		y=y_min+rnd()*(y_max-y_min);
		z=z_min+rnd()*(z_max-z_min);
		con.put(i,x,y,z);
	}

	c_loop_all loop(con);
	voronoicell cell;

	if(loop.start()) do if(con.compute_cell(cell, loop)) {
            // Get the position of the current particle under consideration
			int i = 0;

            std::vector<int> fverts;
			std::vector<double> verts;

			cell.face_vertices(fverts);
			cell.vertices(verts);

			for (int j = 1; j < sizeof(fverts); j++) {
				std::cout << "[" << fverts[j] << "] ";
				std::cout << verts[fverts[j]*3] << " " << verts[fverts[j]*3 + 1] << " " << verts[fverts[j]*3 + 2] << "; ";
			}
			std::cout << std::endl;

        } while (loop.inc());
}

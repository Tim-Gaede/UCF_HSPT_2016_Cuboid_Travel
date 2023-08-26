/*
This problem is finding the shortest distance between two points on a rectangular prism.
Since the surface of rectangular prisms are flat, we can project the surface of our prism onto a 2D coordinate system.
From there, all we have to do is calculate the distance between two points on a plane, and we have our answer.

There are three cases: the two points are on the same surface of the prism, adjacent surfaces of the prism, or opposite surfaces of the prism.

The first case allows us to travel directly to the end point by finding the Euclidean distance between the two points.

For the second case, we have a choice between one of three paths to get to the end location. For the first path we can project the ending
surface directly next to the starting surface and take the Euclidean distance. For the other two paths, we can realize that there are two
surfaces adjacent to the starting surface and the ending surface. We can also use these surfaces as intermediate nodes, though this involves
rotating the ending surface with respect to the third surface. Once we calculate the distances of all three paths, we just choose the one
that gives us the smallest distance.

For the third case, we have more choices (detailed below), as up to 3 intermediate walls could be necessary.
*/

#include <math.h>
#include <stdio.h>

#define INF 1000000000000000000.0

double dist(double xa, double ya, double xb, double yb); //Explained below
double min(double a, double b); //Explained below
void update(); // Explained below

double ans;

int main(int argc, char **argv)
{
	//First we read in the number of test cases we have
	int runs, iter;
	scanf("%d", &runs);

	//We loop until we run out of test cases and process the test cases as we get them
	for(iter = 1; iter <= runs; iter++)
	{
		//Read in the size of the prism and the start and end locations
		int x, y, z, x1, y1, z1, x2, y2, z2;
		scanf("%d %d %d %d %d %d %d %d %d", &x, &y, &z, &x1, &y1, &z1, &x2, &y2, &z2);
		// First we want to adjust the frame of reference so that the starting point is on the top or bottom (z1 = 0 or z1 = z).
		// Swap all x coordinates with all z coordinates if the starting point is on the left or right (x1 = 0 or x1 = x).
		// Swap all y coordinates with all z coordinates if the starting point is on the front or back (y1 = 0 or y1 = y).
		if(z1 != 0 && z1 != z)
		{
		  if(y1 == 0 || y1 == y)
		  {
		    int temp = z1; z1 = y1; y1 = temp;
		    temp = z; z = y; y = temp;
		    temp = z2; z2 = y2; y2 = temp;
		  }
		  else if(x1 == 0 || x1  == x)
		  {
		    int temp = x1; x1 = z1; z1 = temp;
		    temp = x; x = z; z = temp;
		    temp = x2; x2 = z2; z2 = temp;
		  }
		}
		// If the starting point is on the top, flip everything upside down spot it is on the bottom.
		if(z1 == z)
		{
		  z1 = z - z1;
		  z2 = z - z2;
		}
		//Our answer starts at infinity, but will change once we start trying possible paths
		ans = INF;
		//Case 1: the starting and ending points are on the same surface
		if(z1 == z2)
		{
			ans = min(ans, dist(x1, y1, x2, y2));
		}
		//Case 2: the starting and ending points are on opposite surfaces
		else if(abs(z1 - z2) == z)
		{
			// Try all possible paths visiting the back wall first.
			update(x, y, z, x1, y1, z1, x2, y2, z2);
			// Flip the y coordinates so that the back and front walls are switched.
			y1 = y - y1;
			y2 = y - y2;
			// Try all paths visiting the current back wall (previously the front wall).
			update(x, y, z, x1, y1, z1, x2, y2, z2);
			// Flip back the y coordinates.
			y1 = y - y1;
			y2 = y - y2;
			// Switch the x and y coordinates so that the front wall becomes the left wall and vice versa.
			// Also the back wall becomes the right wall and vice versa.
			int temp = x1; x1 = y1; y1 = temp;
	    temp = x; x = y; y = temp;
	    temp = x2; x2 = y2; y2 = temp;
	    // Try all paths visiting the current back wall (previously the right wall).
	    update(x, y, z, x1, y1, z1, x2, y2, z2);
	    // Flip the y coordinates.
			y1 = y - y1;
			y2 = y - y2;
			// Try all paths visiting the current back wall (previously the left wall).
			update(x, y, z, x1, y1, z1, x2, y2, z2);
			// Flip back the y coordinates.
			y1 = y - y1;
			y2 = y - y2;
		}
		//Case 3: The starting and ending surfaces are on adjacent surfaces
		else
		{
		  // Flip the x and y coordinates so that the end point is on the left wall (x = 0).
		  if(x2 != 0 && x2 != x)
		  {
		    int temp = x2; x2 = y2; y2 = temp;
		    temp = x1; x1 = y1; y1 = temp;
		    temp = x; x = y; y = temp;
		  }
		  if(x2 == x)
		  {
		    x2 = x - x2; x1 = x - x1;
		  }
		  ans = min(ans, dist(x1, y1, -z2, y2)); // directly to left wall
		  ans = min(ans, dist(x1, y1, -(y - y2), y+z2)); // back wall first
		  ans = min(ans, dist(x1, y1, -y2, -z2)); // front wall first
		}
  	//Print our answer
		printf("Universe #%d: %.2lf\n", iter, ans);
	}
	return 0;
}

// Tries all possible paths visiting no more than 5 sides going from the point (x1, y1, z1) on the floor
// to the point (x2, y2, z2) on the ceiling assuming the first side we go to is the back wall.
void update(int x, int y, int z, int x1, int y1, int z1, int x2, int y2, int z2)
{
  ans = min(ans, dist(x1, y1, x2, y+z+(y-y2))); // over the back of the box
	ans = min(ans, dist(x1, y1, -(y-y2), y+z+x2)); // over the back then left of the box
	ans = min(ans, dist(x1, y1, x+(y-y2), y+z+(x-x2))); // over the back then right of the box
	ans = min(ans, dist(x1, y1, -y-x2, y+z+y2)); // back, left, front
	ans = min(ans, dist(x1, y1, x+y+x-x2, y+z+y2)); // back, right, front
}

//This function finds the Euclidean distance between two points using the standard distance formula
double dist(double xa, double ya, double xb, double yb)
{
	double dx = xb - xa, dy = yb - ya;
	return sqrt(dx * dx + dy * dy);
}

//This function returns the smaller of the two values it's given
double min(double a, double b)
{
	if(a < b) return a;
	else return b;
}

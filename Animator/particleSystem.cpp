#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "modelerui.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
//#include <limits>
#include <FL/gl.h>
#include <GL/glu.h>
#include "FL/glut.h"

using namespace std;

//static float prevT;
const int NUM = 80;
const float LAST_TIME = 100;
/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() : restitution("Restitution", 0.0f, 2.0f, 1.0f, 0.1f)
{
	// YOUR CODE HERE
	fs.push_back(Gravity(9.8));
	fs.push_back(Viscous(0.6));
	collision_eps = 0.0001;

	// Leave these here; the UI needs them to work correctly.
	dirty = false;
	simulate = false;
}




/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	fs.clear();
	ps.clear();
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
	// YOUR CODE HERE
	resetSimulation(t);
	// These values are used by the UI ...
	// negative bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
	// YOUR CODE HERE
	resetSimulation(t);

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
	// YOUR CODE HERE
	ps.clear();

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	// YOUR CODE HERE
	float deltaT = t - prevT;
	float k_r = restitution.getValue();
	for (std::vector<Particle>::iterator itP = ps.begin(); itP != ps.end(); ++itP) {

		Vec3f netForce(0, 0, 0);
		Vec3f preX = itP->getX();
		Vec3f preV = itP->getV();
		Vec3f curX = preX + preV * deltaT;
		Vec3f curV;

		for (std::vector<Force>::iterator itF = fs.begin(); itF != fs.end(); ++itF) {
			// loop through two forces
			Vec3f fVec(0, 0, 0);
			if (itF->getName() == 'g') {
				fVec = itF->getForce(itP->getM());
			}
			else {
				fVec = itF->getForce(itP->getV());
			}
			netForce = netForce + fVec;
		}
		// netF force = total force at this point
		Vec3f accelVec = netForce / itP->getM();
		curV = accelVec * deltaT + preV;

		// collision detection
		Vec3f x;
		x[0] = curX[0];
		x[1] = curX[1];
		x[2] = curX[2];
		if (x[1] < collision_eps || (x - sphereCenter).length() < 1.0 + collision_eps) {
			// distanse < epsilon
			Vec3f n;
			if (curX[1] < -2.0 + collision_eps) {
				// collision with the groud
				n = Vec3f(0, 1, 0);
			}
			else {
				// collision with the sphere
				n = (x - sphereCenter);
				n.normalize();
			}
			Vec3f v;
			v[0] = curV[0];
			v[1] = curV[1];
			v[2] = curV[2];
			if (v*n < 0) {
				// actually collision
				Vec3f vn;
				Vec3f vt;

				vn = (n*v)*n;
				vt = v - vn;
				v = vt - k_r * vn;

				curV[0] = v[0];
				curV[1] = v[1];
				curV[2] = v[2];

				curX = preX;
			}
		}

		// set the new v and x for the particle
		itP->setV(curV);
		itP->setX(curX);
	}
	// Debugging info
	/*if( t - prevT > .08 )
		printf("(!!) Dropped Frame %lf (!!)\n", t-prevT);*/
	prevT = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	// YOUR CODE HERE
	int count = 0;
	if (!simulate) { return; }
	for (std::vector<Particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
		glPushMatrix();
		if (it->getD() <= LAST_TIME) {
			it->setD(it->getD() + 1.0);
			Vec3f p = it->getX();
			setDiffuseColor(1.0, 0.0, 0.0);
			setAmbientColor(1.0, 0.0, 0.0);
			glTranslatef(GLfloat(p[0]), GLfloat(p[1]), GLfloat(p[2]));
			// dust color
			glColor3f(1, 0.0, 0.0);
			glutSolidSphere(0.1, 16, 16);
		}
		else {
			count++;
		}
		glPopMatrix();
	}
	int subcount = 0;
	while (count > 0 && ps.size() > 0) {
		if ((ps.begin() + subcount)->getD() > LAST_TIME) {
			ps.erase(ps.begin() + subcount);
			count--;
		}
		else {
			subcount++;
		}
	}
}




/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	// TODO (baking is extra credit)
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	// TODO (baking is extra credit)
}

void ParticleSystem::addParticle(Vec3f WorldPointPos, float pa) {
	while (ps.size() < NUM)
	{
		double tmp = rand() * 3.14 / RAND_MAX;
		Vec3f initV = Vec3f(cos(tmp), 5.0 * pa, 1.0 + abs(sin(tmp) * abs(100 * zdistance)));
		ps.push_back(Particle(0.5, WorldPointPos, initV, rand() / 5.0f));
	}
}


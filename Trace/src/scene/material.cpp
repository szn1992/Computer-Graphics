#include "ray.h"
#include "material.h"
#include "light.h"

#include "../fileio/imageio.h"

using namespace std;
extern bool debugMode;


// Apply the Blinn-Phong model to this point on the surface of the object, 
//  returning the color of that point.
Vec3d Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

		// For now, this method just returns the diffuse color of the object.
		// This gives a single matte color for every distinct surface in the
		// scene, and that's it.  Simple, but enough to get you started.
		// (It's also inconsistent with the Phong model...)

		// Your mission is to fill in this method with the rest of the phong
		// shading model, including the contributions of all the light sources.
		// You will need to call both distanceAttenuation() and shadowAttenuation()
		// somewhere in your code in order to compute shadows and light falloff.
		if (debugMode)
			std::cout << "Debugging the Phong code (or lack thereof...)" << std::endl;

		// When you're iterating through the lights,
		// you'll want to use code that looks something
		// like this:
		Vec3d light = ke(i);
		Vec3d normal = i.N;
		Vec3d iDot = r.at(i.t);
		if (r.getDirection() * normal > 0) {
			normal = -normal;
			light += prod(prod(scene->ambient(), ka(i)), kt(i));
		}
		else {
			light += prod(scene->ambient(), ka(i));
		}

		for (vector<Light*>::const_iterator litr = scene->beginLights();
			litr != scene->endLights();
			++litr)
		{
			Light* pLight = *litr;

			double distAttenuation = pLight->distanceAttenuation(iDot);
			Vec3d shadowAttenuation = pLight->shadowAttenuation(iDot);
			Vec3d atten = distAttenuation * shadowAttenuation;
			Vec3d L = pLight->getDirection(iDot);


			if (L * normal > 0) {
				Vec3d H = (L + -1 * r.getDirection());
				if (H.length() != 0)
					H.normalize();

				double sDot = max(0.0, normal * H);
				Vec3d dTerm = kd(i) * (normal * L);
				Vec3d sTerm = ks(i) * (pow(sDot, shininess(i)));
				Vec3d newLight = dTerm + sTerm;
				newLight = prod(newLight, pLight->getColor());

				light += prod(atten, newLight);
			}
		}

	return light;
}


TextureMap::TextureMap( string filename )
{
    data = load( filename.c_str(), width, height );
    if( 0 == data )
    {
        width = 0;
        height = 0;
        string error( "Unable to load texture map '" );
        error.append( filename );
        error.append( "'." );
        throw TextureMapException( error );
    }
}

Vec3d TextureMap::getMappedValue( const Vec2d& coord ) const
{
	// YOUR CODE HERE

    // In order to add texture mapping support to the 
    // raytracer, you need to implement this function.
    // What this function should do is convert from
    // parametric space which is the unit square
    // [0, 1] x [0, 1] in 2-space to Image coordinates,
    // and use these to perform bilinear interpolation
    // of the values.


	double xReal = coord[0] * width; // 0 <= xReal <= N-1
	double yReal = coord[1] * height; // 0 <= yReal <= M-1
	int x0 = int(xReal), y0 = int(yReal);
	double dx = xReal - x0, dy = yReal - y0, omdx = 1 - dx, omdy = 1 - dy;
	Vec3d bilinear = omdx*omdy*getPixelAt(x0, y0) + omdx*dy*getPixelAt(x0, y0 + 1) +
		dx*omdy*getPixelAt(x0 + 1, y0) + dx*dy*getPixelAt(x0 + 1, y0 + 1);
	return bilinear;
}


Vec3d TextureMap::getPixelAt( int x, int y ) const
{
    // This keeps it from crashing if it can't load
    // the texture, but the person tries to render anyway.
    if (0 == data)
      return Vec3d(1.0, 1.0, 1.0);

    if( x >= width )
       x = width - 1;
    if( y >= height )
       y = height - 1;

    // Find the position in the big data array...
    int pos = (y * width + x) * 3;
    return Vec3d( double(data[pos]) / 255.0, 
       double(data[pos+1]) / 255.0,
       double(data[pos+2]) / 255.0 );
}

Vec3d MaterialParameter::value( const isect& is ) const
{
    if( 0 != _textureMap )
        return _textureMap->getMappedValue( is.uvCoordinates );
    else
        return _value;
}

double MaterialParameter::intensityValue( const isect& is ) const
{
    if( 0 != _textureMap )
    {
        Vec3d value( _textureMap->getMappedValue( is.uvCoordinates ) );
        return (0.299 * value[0]) + (0.587 * value[1]) + (0.114 * value[2]);
    }
    else
        return (0.299 * _value[0]) + (0.587 * _value[1]) + (0.114 * _value[2]);
}


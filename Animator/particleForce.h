#ifndef __PARTICLE_FORCE_H__
#define __PARTICLE_FORCE_H__

class Particle {
public:
	Particle();
	Particle(float m, Vec3f x, Vec3f v, float d) { x_ = x; v_ = v; m_ = m; d_ = d; };
	void setX(Vec3f x) {x_ = x;};
	void setV(Vec3f v) {v_ = v;};
	void setD(float d) { d_ = d; };

	Vec3f getX() {return x_;};
	Vec3f getV() {return v_;};
	float getM() {return m_;};
	float getD() { return d_; };

protected:
	Vec3f x_;
	Vec3f v_;
	float m_;
	float d_;
};

class Force {
public:
	void setCoeff(float coeff) {k = coeff;};
	float getName() {return name;};
	virtual Vec3f getForce(float m) {return Vec3f(0.0f, -m*k, 0.0f);};
	virtual Vec3f getForce(Vec3f v) {return ((double)-k) * v;};
protected:
	// g for gravity, k_d for viscous drag
	float k;
	char name;
};

class Gravity: public Force {
public:
	Gravity(float g) {k = g; name = 'g';};
};

class Viscous: public Force {
public:
	Viscous(float k_v) {k = k_v; name = 'v';};
};

#endif
// The sample box model.  You should build a file
// very similar to this for when you make your model in
// order to plug in to the animator project.

#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning (disable : 4786)
#pragma warning (disable : 4312)


#include "modelerview.h"
#include "modelerdraw.h"
#include "modelerui.h"
#include "vault.h"
#include "FL/glut.h"
#include "model.h"
#include <cmath>

const double PI = 3.1415926;

void ground(float h);

void body();
void eyes(float e);
void mouth(int divisions, float width, float height);
void leg(float h);
void arm(float h);
void foot();

class DumbMan : public Model
{
	Texture2D b_checkers;
	Texture2D e_checkers;
	Texture2D l_checkers;
	RangeProperty bendRotation, bodyRotation, eyeRotation, mouthY, mouthX,
		leftlowerarmX, leftupperarmX, leftupperarmY, leftarmWave,
		rightlowerarmX, rightupperarmX, rightupperarmY, rightarmWave,
		leftlowerlegX, leftupperlegX, leftupperlegY, leftlegWave,
		rightlowerlegX, rightupperlegX, rightupperlegY, rightlegWave,
		leftfootX, leftfootY, rightfootX, rightfootY;

public:
	DumbMan() :
		Model("Dumb Man"),
		b_checkers("body_texture.png"),
		e_checkers("eye.png"),
		l_checkers("lip.png"),
		bendRotation("bend rotation", -90, 90, 0, 0.1),
		bodyRotation("body rotation", -180, 180, 0, 0.1),
		eyeRotation("eye rotation", -90, 90, 0, 0.1),
		mouthY("mouthY", -0.5, 1, 0, 0.01),
		mouthX("mouthX", -1, 1, 0, 0.01),
		leftlowerarmX("left lower arm X", -180, 10, 0, 0.1),
		leftupperarmX("left upper arm X", -180, 180, 0, 0.1),
		leftupperarmY("left upper arm Y", -180, 180, 0, 0.1),
		rightlowerarmX("right lower arm X", -180, 10, 0, 0.1),
		rightupperarmX("right upper arm X", -180, 180, 0, 0.1),
		rightupperarmY("right upper arm Y", -180, 180, 0, 0.1),
		leftlowerlegX("left lower leg X", -10, 180, 0, 0.1),
		leftupperlegX("left upper leg X", -180, 180, 0, 0.1),
		leftupperlegY("left upper leg Y", -180, 180, 0, 0.1),
		rightlowerlegX("right lower leg X", -10, 180, 0, 0.1),
		rightupperlegX("right upper leg X", -180, 180, 0, 0.1),
		rightupperlegY("right upper leg Y", -180, 180, 0, 0.1),
		leftfootX("left foot X", -30, 90, 0, 0.1),
		leftfootY("left foot Y", -90, 90, 0, 0.1),
		rightfootX("right foot X", -30, 90, 0, 0.1),
		rightfootY("right foot Y", -90, 90, 0, 0.1),
		leftarmWave("left arm wave", -180, 180, 0, 0.1),
		rightarmWave("right arm wave", -180, 180, 0, 0.1),
		leftlegWave("left leg wave", -180, 180, 0, 0.1),
		rightlegWave("right leg wave", -180, 180, 0, 0.1)

	{
		properties.add(&bendRotation)
		.add(&bodyRotation)
		.add(&eyeRotation)
		.add(&mouthY)
		.add(&mouthX)
		.add(&leftlowerarmX)
		.add(&leftupperarmX)
		.add(&leftupperarmY)
		.add(&rightlowerarmX)
		.add(&rightupperarmX)
		.add(&rightupperarmY)
		.add(&leftlowerlegX)
		.add(&leftupperlegX)
		.add(&leftupperlegY)
		.add(&rightlowerlegX)
		.add(&rightupperlegX)
		.add(&rightupperlegY)
		.add(&leftfootX)
		.add(&leftfootY)
		.add(&rightfootX)
		.add(&rightfootY)
		.add(&leftarmWave)
		.add(&rightarmWave)
		.add(&leftlegWave)
		.add(&rightlegWave);

	}
	virtual void load();
	virtual void draw(int);
};

void DumbMan::load() {
	b_checkers.load();
	e_checkers.load();
	l_checkers.load();
}

void DumbMan::draw(int divisions)
{
	/* pick up the slider values */
	float bend = bendRotation.getValue();
	float alpha = bodyRotation.getValue();
	float e = eyeRotation.getValue();
	float m_x = mouthX.getValue();
	float m_y = mouthY.getValue();
	float lla_x = leftlowerarmX.getValue();
	float lua_x = leftupperarmX.getValue();
	float lua_y = leftupperarmY.getValue();
	float rla_x = rightlowerarmX.getValue();
	float rua_x = rightupperarmX.getValue();
	float rua_y = rightupperarmY.getValue();
	float lll_x = leftlowerlegX.getValue();
	float lul_x = leftupperlegX.getValue();
	float lul_y = leftupperlegY.getValue();
	float rll_x = rightlowerlegX.getValue();
	float rul_x = rightupperlegX.getValue();
	float rul_y = rightupperlegY.getValue();
	float lf_x = leftfootX.getValue();
	float lf_y = leftfootY.getValue();
	float rf_x = rightfootX.getValue();
	float rf_y = rightfootY.getValue();
	float la = leftarmWave.getValue();
	float ra = rightarmWave.getValue();
	float ll = leftlegWave.getValue();
	float rl = rightlegWave.getValue();

	glPushMatrix();
		b_checkers.use();
		glTranslatef(0.0, 2.5, 0.0);
		glRotatef(bend, 1.0, 0.0, 0.0);
		glRotatef(alpha, 0.0, 1.0, 0.0);
		body();
		e_checkers.use();
		eyes(e);
		l_checkers.use();
		mouth(divisions, m_x, m_y);

		b_checkers.use();
		float upper_length = 1.3f;
		float lower_length = 1.0f;
		glPushMatrix();
			glTranslatef(1.4f, 0.55f, 0.0f);
			glRotatef(90.0 + lua_x + la, 1.0, 0.0, 0.0);
			glRotatef(20.0 + lua_y, 0.0, 1.0, 0.0);
			arm(upper_length);

			glPushMatrix();
				glTranslatef(0.0, 0.0, upper_length);
				if (la > 10)
					la = 10;
				glRotatef(lla_x + la, 1.0, 0.0, 0.0);
				arm(lower_length);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-1.4f, 0.55f, 0.0f);
			glRotatef(90.0 + rua_x + ra, 1.0, 0.0, 0.0);
			glRotatef(-20.0 + rua_y, 0.0, 1.0, 0.0);
			arm(upper_length);

			glPushMatrix();
				glTranslatef(0.0, 0.0, upper_length);
				if (ra > 10)
					ra = 10;
				glRotatef(rla_x + ra, 1.0, 0.0, 0.0);
				arm(lower_length);
			glPopMatrix();
		glPopMatrix();


		upper_length = 1.0f;
		lower_length = 1.3f;
		glPushMatrix();
			glTranslatef(0.4f, -1.8f, 0.0f);
			glRotatef(90.0 + lul_x + ll, 1.0, 0.0, 0.0);
			glRotatef(5.0 + lul_y, 0.0, 1.0, 0.0);
			leg(upper_length);

			glPushMatrix();
				glTranslatef(0.0, 0.0, upper_length);
				if (ll < -10)
					ll = -10;
				glRotatef(lll_x + ll, 1.0, 0.0, 0.0);
				leg(lower_length);

				glPushMatrix();
					glTranslatef(0.0, -0.1, lower_length);
					glRotatef(-90.0 + lf_x, 1.0, 0.0, 0.0);
					glRotatef(lf_y, 0.0, 1.0, 0.0);
					foot();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.4f, -1.8f, 0.0f);
			glRotatef(90.0 + rul_x + rl, 1.0, 0.0, 0.0);
			glRotatef(-5.0 + rul_y, 0.0, 1.0, 0.0);
			leg(upper_length);

			glPushMatrix();
				glTranslatef(0.0, 0.0, upper_length);
				if (rl < -10)
					rl = -10;
				glRotatef(rll_x + rl, 1.0, 0.0, 0.0);
				leg(lower_length);

				glPushMatrix();
					glTranslatef(0.0, -0.1, lower_length);
					glRotatef(-90.0 + rf_x, 1.0, 0.0, 0.0);
					glRotatef(rf_y, 0.0, 1.0, 0.0);
					foot();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void body() {
	glPushMatrix();
		glScalef(1.5f, 2.0f, 1.5f);
		drawSphere(1.0);
	glPopMatrix();
}

void eyes(float e) {
	glPushMatrix();
		glTranslatef(0.4f, 0.9f, 1.16f);
		glRotatef(-100, 1.0, 0.0, 0.0);
		glRotatef(e, 0.0, 0.0, 1.0);
		drawSphere(0.15);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.4f, 0.9f, 1.16f);
		glRotatef(-100, 1.0, 0.0, 0.0);
		glRotatef(e, 0.0, 0.0, 1.0);
		drawSphere(0.15);
	glPopMatrix();
}

void mouth(int divisions, float width, float height) {
	glPushMatrix();
		glTranslatef(0.0f, 0.4f, 1.44f);
		glRotatef(-9, 1.0, 0.0, 0.0);
		glScalef(1.0f + width, 0.5f + height, 1.0f);
		glutSolidTorus(0.05, 0.3, divisions, divisions);
	glPopMatrix();
}

void arm(float h) {					
	drawCylinder(h, 0.1, 0.1);
}


void leg(float h) {					
	drawCylinder(h, 0.1, 0.1);
}


void foot() {
	drawCylinder(0.7, 0.1, 0.1);
}

/** The scene, which includes the lights and models. */


class Scene : public Model {
protected:
///////////////////////////////// TEXTURES ////////////////////////////////////
	Texture2D texture;

///////////////////////////////// SHADERS /////////////////////////////////////
	ShaderProgram shader;
	ShaderProgram spotshader;
	ShaderProgram cartoonshader;
	ShaderProgram bumpshader;
//////////////////////////////// PROPERTIES ///////////////////////////////////
	// Switches for spheres
	BooleanProperty useTexture;
#if !defined(ANIMATOR) || defined(ANIMATOR557)
	// Switch for showing/hiding reference unit sphere
	BooleanProperty showReferenceUnitSphere;

	// Lets you pick what shapes to use for the default model!
	ChoiceProperty shapeChoice;
#endif // #ifndef ANIMATOR

	ChoiceProperty useShader;

	// Some slider properties
	RangeProperty rotateX, rotateY;
	RangeProperty brightness;

	// Diffuse color picker
	RGBProperty diffuse;


	// Scene lights
	PointLight pointLight;
	SpotLight spotLight;
	DirectionalLight directionalLight;

	DumbMan dumb;
public:
	
	

	/** Construct the scene */
	Scene() :
	    // You have to call the parent class's constructor, to provide a
	    // name for the model.
	    Model("Scene"),

		// Construct textures and shaders. 
		// They won't be loaded until the model is drawn for the first time.
		texture("checkers.png"),
		shader("shader.vert", "shader.frag", NULL), 
		spotshader("shader.vert", "spotlight.frag", NULL),
		cartoonshader("shader.vert", "cartoon.frag", NULL),
		bumpshader("bump.vert", "bump.frag", NULL),
		// Call the constructors for the lights
		pointLight("Point Light", GL_LIGHT1, /**direction part**/ -5, 5, 5, /**diffuse part**/ 1.0, 0.5, 0.5, 
		/**specular part**/ 1.0, 0.5, 0.5, /**ambient part**/ .2f, 0.1, 0.1 /**attenuation part**/, 0.4, 0.7, 0),
		spotLight("Spot Light", GL_LIGHT2, /**direction part**/ 0, 5, 5, /**diffuse part**/ 1.0, 0.5, 0.5, 
		/**specular part**/ 1.0, 0.5, 0.5, /**ambient part**/ .2f, 0.1, 0.1 /**attenuation part**/, 0.4, 0.7, 0,
		/**spot direction**/0.0, 1.0, 0.5, /**cutoff**/30.0, /**exponent**/1.0),
		directionalLight("Directional Light", GL_LIGHT0, /**direction part**/ 5, 5, 5, /**diffuse part**/ 0.0f, 0.5, 0.5f, 
		/**specular part**/ 0.0f, 0.5f, 0.5f )

		// Now, call the constructors for each Property:
		, useTexture("Use Checkered Texture", true),
#if !defined(ANIMATOR) || defined(ANIMATOR557)
		showReferenceUnitSphere("Show Reference Unit Sphere", false),
		shapeChoice("Model Shape:", "Sphere|Cube|Cylinder|Torus|Icosahedron|Teapot|Revolution|Dumb Man", 0), //457 animator UI only allows shapes
#endif // #ifndef ANIMATOR
		useShader("Shaders:", "Pointlight|SpotLight|Cartoon|Bump|None", 0),
		rotateX("Rotate Basic Shape X", -180, 180, 0, 1),
		rotateY("Rotate Basic Shape Y", -180, 180, 0, 1),
		brightness("Brightness", 0.0f, 1.0f, 1.0f, 0.1f),
		diffuse("Diffuse Color", 1.0, 0.7, .4)
	{
		// If you have child Models, like the MobileLight model from model.h,
		// you can add their property groups, and they will appear in the list
		// in the top left corner of Modeler, under this model's entry:
		properties.add(pointLight.getProperties())
				  .add(spotLight.getProperties())
				  .add(directionalLight.getProperties())
				  ;
		properties.add(dumb.getProperties());
		// Finally, add all the properties to this model's PropertyGroup.
		properties.add(&useTexture);
#if !defined(ANIMATOR) || defined(ANIMATOR557)
		properties.add(&showReferenceUnitSphere)
				  .add(&shapeChoice);
#endif // #ifndef ANIMATOR
		properties.add(&useShader)
				.add(&rotateX)
				.add(&rotateY);
		properties.add(&brightness)
				  .add(&diffuse)
				  ;
	}



	/**
	 * Modeler calls this method once an OpenGL context becomes available,
	 * (which happens before the first time draw() is called), and whenever
	 * you click the "Reload Textures and Shaders" menu option.
	 *
	 * Make sure you call the load() methods of each of your textures and
	 * shaders here.
	 */
	void load() {
		texture.load();
		dumb.load();
		shader.load();
		spotshader.load();
		cartoonshader.load();
		bumpshader.load();
	}

	/**
	 * Modeler calls this method many times a second when the Animate
	 * checkbox is checked.
	 */
	void tick() {
		// You can put code here to increment animation counters for
		// extra credit.
	}
	
#if !defined(ANIMATOR) || defined(ANIMATOR557)
	/** Draw the model. */
	void drawModel() {

		int divisions=16; 		
        switch(ModelerDrawState::Instance()->m_quality)
        {
        case HIGH: 
			divisions = 64; break;
		case MEDIUM: 
			divisions = 20; break;
		case LOW:
			divisions = 12; break;
		case POOR:
			divisions = 8; break;
        }

		// Remember our current modelview matrix, which is in world space.
		glPushMatrix();

		    // Rotate around the X-axis
			glRotatef(rotateX.getValue(), 1, 0, 0);

			// Rotate around the Y-axis
			glRotatef(rotateY.getValue(), 0, 1, 0);


			// Draw some object at the origin.
			switch (shapeChoice.getValue()) {
			case 0: // sphere
				glRotatef(-90.f,1.f,0.f,0.f);
				glutSolidSphere(1.0, divisions, divisions);
				glRotatef(90.f,1.f,0.f,0.f);
				break;
			case 1: // box
				glRotatef(-45.f,0.f,1.f,0.f);
				glutSolidCube(2);
				glRotatef(45.f,0.f,1.f,0.f);
				break;
			case 2: // cylinder
				glRotatef(90.f,1.f,0.f,0.f);
				drawCylinder(1.8,1.0,1.8);
				glRotatef(-90.f,1.f,0.f,0.f);
				break;
			case 3: // torus
				glutSolidTorus(.5,1,divisions,divisions);
				break;
			case 4: // icosahedron
				glutSolidIcosahedron();
				break;
			case 5: // teapot
				glutSolidTeapot(1);
				break;
			case 6:
				drawRevolution(1);
				break;
			case 7:
				dumb.draw(divisions);
				break;
			}

#if !defined(ANIMATOR) || defined(ANIMATOR557)
			if (showReferenceUnitSphere.getValue() ) {
			    // Now go 1 units in the +y direction,
				// and 1 units in the NEGATIVE x direction.
				glTranslated(-2, -1, 0);
			    // Now go 1 units in the +y direction,
				// and 1 units in the NEGATIVE x direction.
					glRotatef(-90.f,1.f,0.f,0.f);
					drawSphere(.5);
					glRotatef(90.f,1.f,0.f,0.f);
				glPopMatrix();
			}
			
#endif
			// Go back to "world space"
			glPopMatrix();

		{

			// Draw the floor.  Here's a basic example of texture mapping.
			glBegin(GL_QUADS);
			glTexCoord2f(0, 1); // specify the texture coordinate
			glNormal3f(0, 1, 0); // specify the surface's normal at this vertex
			glVertex3f(-10, -2, -10); // both before its corresponding vertex

			glTexCoord2f(1, 1);
			glNormal3f(0, 1, 0);
			glVertex3f(10, -2, -10);

			glTexCoord2f(1, 0);
			glNormal3f(0, 1, 0);
			glVertex3f(10, -2, 10);

			glTexCoord2f(0, 0);
			glNormal3f(0, 1, 0);
			glVertex3f(-10, -2, 10);
			glEnd();
		}
	}

#endif // #ifndef ANIMATOR

	/** Draw the scene. */
	void draw() {
#if !defined(ANIMATOR) || defined(ANIMATOR557)
#endif // #ifndef ANIMATOR
		// The lights must be drawn FIRST, so the other scene elements
		// can get lit!
		pointLight.draw();
		spotLight.draw();
		directionalLight.draw();
		//reset colors
		setDiffuseColor(
			diffuse.getRed(),
			diffuse.getGreen(),
			diffuse.getBlue());

		setAmbientColor(0.2, 0.2, 0.2);

		setSpecularColor(1,1,1);
		setShininess(pow(2.71828183f, 3));

		// Use the texture if desired.
		if (useTexture.getValue() && useShader.getValue() != 3) {
			if (shapeChoice.getValue() != 7)
				texture.use();
		} else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Use the shader if desired.
		/*if (useShader.getValue()) {
			shader.use();
			// glGetUniformLocation gets the memory location of a variable with the given char* name, in this case "brightness"
			// for the given shader program, identified by its ID.
			GLint brightnessVariableLocation = glGetUniformLocation( shader.getID(), "brightness" );
			// glUniform1f sets the value of a particular uniform variable location with a single float value (hence the suffix "1f")
			glUniform1f(brightnessVariableLocation, brightness.getValue());

		}
		else
		{
			if (glUseProgram) { glUseProgram(0); } //don't use shader
		}*/
		GLint brightnessVariableLocation;
		switch (useShader.getValue()) {
			case 0: 
				shader.use();
				brightnessVariableLocation = glGetUniformLocation(shader.getID(), "brightness");
				glUniform1f(brightnessVariableLocation, brightness.getValue());
				break;
			case 1: 
				spotshader.use();
				brightnessVariableLocation = glGetUniformLocation(spotshader.getID(), "brightness");
				glUniform1f(brightnessVariableLocation, brightness.getValue());
				break;
			case 2: 
				cartoonshader.use();
				brightnessVariableLocation = glGetUniformLocation(cartoonshader.getID(), "brightness");
				glUniform1f(brightnessVariableLocation, brightness.getValue());
				break;
			case 3:
				bumpshader.use();
				brightnessVariableLocation = glGetUniformLocation(bumpshader.getID(), "brightness");
				glUniform1f(brightnessVariableLocation, brightness.getValue());
				break;
			case 4:
				glUseProgram(0);
				break;
		}

#if defined(ANIMATOR) && !defined(ANIMATOR557)
//Animator 457 only!
		// Draw the default sphere for collision
		glPushMatrix();
		glTranslatef(sphereCenterX.getValue(), sphereCenterY.getValue(), sphereCenterZ.getValue());
		glutSolidSphere(1.0, 20, 20); // the 20's are arbitary
		glPopMatrix();

		// Draw robot arm
		robotArm.draw();

#else
//modeler 457 & Animator 557
		// Call a class method that draws our model.
		drawModel();
		// Stop applying shaders to objects.
		// This if-statement makes sure that glUseProgram is not a null
		// function pointer (which it will be if GLEW couldn't initialize).
		if (glUseProgram) {
			glUseProgram(0);
		}

		// Stop applying textures to objects
		glBindTexture(GL_TEXTURE_2D, 0);
#endif
	}
};

/**
 * The program starts here.
 */

int main()
{
	// Tell the FLTK library to let us use OpenGL
	Fl::gl_visual(FL_RGB);

	init_load_curve_file();

	// Instantiate Modeler
	ModelerUserInterface ui;

	// Give Modeler your scene.
	// Modeler will free this pointer when it exits.
	ui.setModel(new Scene());


	// Run the application
    return ui.run();
}
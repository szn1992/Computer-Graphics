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
#include "cameras.h"
#include <cmath>


const double PI = 3.1415926;
static float curZ = 0.0f;

void body();
void eyes(float e, float ey, float ez);
void mouth(int divisions, float width, float height, float r, float mpy, float mpz, Mat4f CameraMatrix, float pa);
void leg(float h);
void arm(float h);
void foot();

Mat4f getModelViewMatrix()
{
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	return matMV.transpose(); // convert to row major
}


void SpawnParticles(Mat4f CameraTransforms, float angle)
{
	Mat4f CurrentModelViewMatrix = getModelViewMatrix();
	Mat4f WorldMatrix = CameraTransforms.inverse() * CurrentModelViewMatrix;
	Vec3f WorldPointPos = WorldMatrix * Vec3f(0, 0, 0);
	ModelerUserInterface* ui = ModelerUserInterface::getInstance();
	ParticleSystem* ps = ui->getParticleSystem();
	ps->addParticle(WorldPointPos, angle);
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
//////////////////////////////// PROPERTIES ///////////////////////////////////
	// Switches for spheres
	BooleanProperty useTexture;
	// Switch for showing/hiding reference unit sphere
	BooleanProperty showReferenceUnitSphere;

	// Lets you pick what shapes to use for the default model!
	ChoiceProperty shapeChoice;

	ChoiceProperty useShader;

	// Some slider properties
	RangeProperty rotateX, rotateY;
	RangeProperty brightness;

	// Diffuse color picker
	RGBProperty diffuse;


	// Scene lights
	PointLight pointLight;
	DirectionalLight directionalLight;

	// A robot arm model
	ParticleSystem ps;

	Texture2D by_checkers;
	Texture2D bb_checkers;
	Texture2D bg_checkers;
	Texture2D e_checkers;
	Texture2D l_checkers;
	Texture2D background_checkers;
	RangeProperty bodyX, bodyZ, bodyY, bendRotation, bodyRotation, eyeRotation, mouthY, mouthX,
		leftlowerarmX, leftupperarmX, leftupperarmY, leftarmWave,
		rightlowerarmX, rightupperarmX, rightupperarmY, rightarmWave,
		leftlowerlegX, leftupperlegX, leftupperlegY, leftlegWave,
		rightlowerlegX, rightupperlegX, rightupperlegY, rightlegWave,
		leftfootX, leftfootY, rightfootX, rightfootY,
		eyesadjustZ, eyesadjustY, mouthadjustZ, mouthadjustY, mouthRotation, lengthadjust, particleangle;
	ChoiceProperty dumbChoice, colorChoice;
public:

	/** Modeler calls this to get our particle system. */
	ParticleSystem* getParticleSystem() { 
		return &ps; 
	}
	
	RangeProperty sphereCenterX, sphereCenterY, sphereCenterZ;
	

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
		// Call the constructors for the lights
		pointLight("Point Light", GL_LIGHT1, /**direction part**/ -5, 5, 5, /**diffuse part**/ 1.0, 0.5, 0.5, 
		/**specular part**/ 1.0, 0.5, 0.5, /**ambient part**/ .2f, 0.1, 0.1 /**attenuation part**/, 0.4, 0.7, 0),
		directionalLight("Directional Light", GL_LIGHT0, /**direction part**/ 5, 5, 5, /**diffuse part**/ 0.0f, 0.5, 0.5f, 
		/**specular part**/ 0.0f, 0.5f, 0.5f )

		// Now, call the constructors for each Property:
		, useTexture("Use Checkered Texture", true),
		showReferenceUnitSphere("Show Reference Unit Sphere", false),
		shapeChoice("Model Shape:", "Sphere|Cube|Cylinder|Torus|Icosahedron|Teapot|Revolution|Dumb Man", 7), //457 animator UI only allows shapes
		useShader("Shaders:", "Pointlight|SpotLight|Cartoon|None", 1),
		rotateX("Rotate Basic Shape X", -180, 180, 0, 1),
		rotateY("Rotate Basic Shape Y", -180, 180, 0, 1),
		brightness("Brightness", 0.0f, 1.0f, 1.0f, 0.1f),
		diffuse("Diffuse Color", 1.0, 0.7, .4)
		,
		sphereCenterX("Collision Sphere Center (X)", -6.0, 6.0, -5.0, 0.1),
		sphereCenterY("Collision Sphere Center (Y)", -6.0, 6.0, 0, 0.1),
		sphereCenterZ("Collision Sphere Center (Z)", -6.0, 6.0, 0, 0.1),

		by_checkers("yellow.png"),
		bb_checkers("blue.png"),
		bg_checkers("green.png"),
		e_checkers("eye.png"),
		l_checkers("lip.png"),
		background_checkers("background.png"),
		bodyX("body translate X", -2, 2, 0, 0.1),
		bodyY("body translate Y", -2, 2, 0, 0.1),
		bodyZ("body translate Z", -2, 2, 0, 0.1),
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
		rightlegWave("right leg wave", -180, 180, 0, 0.1),
		eyesadjustY("eye position adjust Y", -1, 1, 0, 0.01),
		eyesadjustZ("eye position adjust Z", -1, 1, 0, 0.01),
		mouthadjustY("mouth position adjust Y", -1, 1, 0, 0.01),
		mouthadjustZ("mouth position adjust Z", -1, 1, 0, 0.01),
		mouthRotation("mouth rotation adjust", -20, 20, 0, 0.1),
		lengthadjust("length adjust", -1, 1, 0, 0.01),
		particleangle("Particle spread angle", -1, 1, 0, 0.01),
		dumbChoice("Choose your dumb man:", "dumb1|dumb2|dumb3", 0),
		colorChoice("Choose color:", "yellow|blue|green", 0)


	{
		// If you have child Models, like the MobileLight model from model.h,
		// you can add their property groups, and they will appear in the list
		// in the top left corner of Modeler, under this model's entry:
		properties.add(pointLight.getProperties())
				  .add(directionalLight.getProperties())
				  ;

		properties.add(&dumbChoice)
			.add(&colorChoice)
			.add(&eyesadjustY)
			.add(&eyesadjustZ)
			.add(&mouthadjustY)
			.add(&mouthadjustZ)
			.add(&mouthRotation)
			.add(&lengthadjust)
			.add(&bodyX)
			.add(&bodyY)
			.add(&bodyZ)
			.add(&bendRotation)
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
			.add(&rightlegWave)
			.add(&(ps.restitution))
			.add(&particleangle);

		// Finally, add all the properties to this model's PropertyGroup.
		properties.add(&useTexture);
		properties.add(&showReferenceUnitSphere)
				  .add(&shapeChoice);
		properties.add(&useShader)
				.add(&rotateX)
				.add(&rotateY);
		properties.add(&sphereCenterX)
				.add(&sphereCenterY)
				.add(&sphereCenterZ);
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
		shader.load();
		spotshader.load();
		cartoonshader.load();
		by_checkers.load();
		bb_checkers.load();
		bg_checkers.load();
		e_checkers.load();
		l_checkers.load();
		background_checkers.load();
	}

	/**
	 * Modeler calls this method many times a second when the Animate
	 * checkbox is checked.
	 */
	void tick() {
		// You can put code here to increment animation counters for
		// extra credit.
	}
	
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
				

				switch (dumbChoice.getValue()) {
				case 0:
					reload_curve_file("./curves/dumb_man.apts");
					break;
				case 1:
					reload_curve_file("./curves/dumb_man1.apts");
					break;
				case 2:
					reload_curve_file("./curves/dumb_man2.apts");
					break;
				}
				/* pick up the slider values */
				float bodyx = bodyX.getValue();
				float bodyy = bodyY.getValue();
				float bodyz = bodyZ.getValue();
				ps.setZDistance(bodyz - curZ);
				curZ = bodyz;
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
				float ey = eyesadjustY.getValue();
				float ez = eyesadjustZ.getValue();
				float mpy = mouthadjustY.getValue();
				float mpz = mouthadjustZ.getValue();
				float mr = mouthRotation.getValue();
				float h = lengthadjust.getValue();
				float pa = particleangle.getValue();

				switch (colorChoice.getValue()) {
				case 0:
					by_checkers.use();
					break;
				case 1:
					bb_checkers.use();
					break;
				case 2:
					bg_checkers.use();
					break;
				}

				//ModelerView::draw();

				Mat4f CameraMatrix = getModelViewMatrix();

				glPushMatrix();
				glTranslatef(0.0 + bodyx, 2.5 + bodyy, 0.0 + bodyz);
				glRotatef(bend, 1.0, 0.0, 0.0);
				glRotatef(alpha, 0.0, 1.0, 0.0);
				setDiffuseColor(0.25, 0.25, 0.85);
				setAmbientColor(0.25, 0.25, 0.85);
				body();
				e_checkers.use();
				setDiffuseColor(0.0, 0.0, 0.0);
				setAmbientColor(0.0, 0.0, 0.0);
				eyes(e, ey, ez);
				l_checkers.use();
				setDiffuseColor(1.0, 0.25, 0.25);
				setAmbientColor(1.0, 0.25, 0.25);
				mouth(divisions, m_x, m_y, mr, mpy, mpz, CameraMatrix, pa);

				switch (colorChoice.getValue()) {
				case 0:
					by_checkers.use();
					break;
				case 1:
					bb_checkers.use();
					break;
				case 2:
					bg_checkers.use();
					break;
				}
				float upper_length = 1.3f;
				float lower_length = 1.0f;
				setDiffuseColor(0.25, 0.25, 0.85);
				setAmbientColor(0.25, 0.25, 0.85);
				glPushMatrix();
				glTranslatef(1.4f, 0.55f, 0.0f);
				glRotatef(90.0 + lua_x + la, 1.0, 0.0, 0.0);
				glRotatef(20.0 + lua_y, 0.0, 1.0, 0.0);
				arm(upper_length + h);

				glPushMatrix();
				glTranslatef(0.0, 0.0, upper_length + h);
				if (la > 10)
					la = 10;
				glRotatef(lla_x + la, 1.0, 0.0, 0.0);
				arm(lower_length + h);
				glPopMatrix();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-1.4f, 0.55f, 0.0f);
				glRotatef(90.0 + rua_x + ra, 1.0, 0.0, 0.0);
				glRotatef(-20.0 + rua_y, 0.0, 1.0, 0.0);
				arm(upper_length + h);

				glPushMatrix();
				glTranslatef(0.0, 0.0, upper_length + h);
				if (ra > 10)
					ra = 10;
				glRotatef(rla_x + ra, 1.0, 0.0, 0.0);
				arm(lower_length + h);
				glPopMatrix();
				glPopMatrix();


				upper_length = 1.0f;
				lower_length = 1.3f;
				glPushMatrix();
				glTranslatef(0.4f, -1.8f, 0.0f);
				glRotatef(90.0 + lul_x + ll, 1.0, 0.0, 0.0);
				glRotatef(5.0 + lul_y, 0.0, 1.0, 0.0);
				leg(upper_length + h);

				glPushMatrix();
				glTranslatef(0.0, 0.0, upper_length + h);
				if (ll < -10)
					ll = -10;
				glRotatef(lll_x + ll, 1.0, 0.0, 0.0);
				leg(lower_length + h);

				glPushMatrix();
				glTranslatef(0.0, -0.1, lower_length + h);
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
				leg(upper_length + h);

				glPushMatrix();
				glTranslatef(0.0, 0.0, upper_length + h);
				if (rl < -10)
					rl = -10;
				glRotatef(rll_x + rl, 1.0, 0.0, 0.0);
				leg(lower_length + h);

				glPushMatrix();
				glTranslatef(0.0, -0.1, lower_length + h);
				glRotatef(-90.0 + rf_x, 1.0, 0.0, 0.0);
				glRotatef(rf_y, 0.0, 1.0, 0.0);
				foot();
				glPopMatrix();
				glPopMatrix();
				glPopMatrix();
				glPopMatrix();
				setDiffuseColor(0.8, 0.8, 0.8);
				setAmbientColor(0.8, 0.8, 0.8);
				background_checkers.use();
				init_load_curve_file();
				break;
				
			}

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

	void body() {
		glPushMatrix();
		//glScalef(1.5f, 2.0f, 1.5f);
		//drawSphere(1.0);
		drawRevolution(1.0);
		glPopMatrix();
	}

	void eyes(float e, float ey, float ez) {
		glPushMatrix();
		glTranslatef(0.4f, 0.9f + ey, 1.16f + ez);
		glRotatef(-100, 1.0, 0.0, 0.0);
		glRotatef(e, 0.0, 0.0, 1.0);
		drawSphere(0.15);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.4f, 0.9f + ey, 1.16f + ez);
		glRotatef(-100, 1.0, 0.0, 0.0);
		glRotatef(e, 0.0, 0.0, 1.0);
		drawSphere(0.15);
		glPopMatrix();
	}

	void mouth(int divisions, float width, float height, float r, float mpy, float mpz, Mat4f CameraMatrix, float pa) {
		glPushMatrix();
		glTranslatef(0.0f, 0.4f + mpy, 1.44f + mpz);
		glRotatef(-9 + r, 1.0, 0.0, 0.0);
		glScalef(1.0f + width, 0.5f + height, 1.0f);
		SpawnParticles(CameraMatrix, pa);
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

	/** Draw the scene. */
	void draw() {
		// The lights must be drawn FIRST, so the other scene elements
		// can get lit!
		pointLight.draw();
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
		if (useTexture.getValue()) {
			texture.use();
		}
		else {
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
			//glUseProgram(0);
			break;
		}
		
		glPushMatrix();
		glTranslatef(sphereCenterX.getValue(), sphereCenterY.getValue(), sphereCenterZ.getValue());
		ps.setSphereCenter(sphereCenterX.getValue(), sphereCenterY.getValue(), sphereCenterZ.getValue());
		glutSolidSphere(1.0, 20, 20); // the 20's are arbitary
		glPopMatrix();

		drawModel();
		// Stop applying shaders to objects.
		// This if-statement makes sure that glUseProgram is not a null
		// function pointer (which it will be if GLEW couldn't initialize).
		if (glUseProgram) {
			glUseProgram(0);
		}

		// Stop applying textures to objects
		glBindTexture(GL_TEXTURE_2D, 0);

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
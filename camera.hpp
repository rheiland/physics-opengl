#ifndef H_CAMERA_HPP
#define H_CAMERA_HPP

#include <cmath>

#ifdef __APPLE__
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

namespace mn {


struct Vector {
	float x, y, z;

	Vector(float theX = 0, float theY = 0, float theZ = 0)
		: x(theX), y(theY), z(theZ) { }

	float length() const {
		return std::sqrt(x*x + y*y + z*z);
	}

	void normalize() { *this *= 1 / length(); }

	Vector &operator+=(const Vector &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector &operator-=(const Vector &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector &operator*=(float r) {
		x *= r;
		y *= r;
		z *= r;
		return *this;
	}

	Vector &operator/=(float r) {
		return *this *= 1 / r;
	}

	Vector  operator- () const {
		return Vector(-x, -y, -z);
	}

	Vector  operator+ () const {
		return *this;
	}
};


inline Vector operator+(Vector a, const Vector &b) { return a += b; }
inline Vector operator-(Vector a, const Vector &b) { return a -= b; }
inline Vector operator*(Vector a, float r) { return a *= r; }
inline Vector operator*(float r, Vector a) { return a *= r; }
inline Vector operator/(Vector a, float r) { return a /= r; }


struct Camera {
	Camera() : eye(0, 0, 5), rotX(M_PI/2), rotY(-M_PI/2), valid(false) { }

	const Vector &getEye() const { return eye; }
	void setEye(const Vector &v) { eye = v; }
	float getEyeX() const { return eye.x; }
	float getEyeY() const { return eye.y; }
	float getEyeZ() const { return eye.z; }

	void moveLeft(float x) { eye += x * left; }
	void moveTop(float y) { eye += y * top; }
	void moveForward(float z) { eye += z * forward; }
	void move(const Vector &v) {
		((eye += v.x * left) += v.y * top) += v.z * forward;
	}

	float getRotX() const { return rotX; }
	float getRotY() const { return rotY; }
	void setRotX(float v) {
		rotX = v; valid = false;
		if (rotX > M_PI - 0.1) rotX = M_PI - 0.1;
		else if (rotX < 0.1) rotX = 0.1;
	}
	void setRotY(float v) { rotY = v; valid = false; }

	void rotateTop(float x) { setRotX(rotX - x); }
	void rotateLeft(float y) { rotY -= y; valid = false; }
	void rotate(const Vector &v) { setRotX(rotX - v.x); rotY -= v.y; }

	const Vector &getForward() const {
		if (!valid) update();
		return forward;
	}
	Vector getCenter() const { return 3.0 * getForward() + eye; }
	float getCenterX() const { return 3.0 * getForward().x + eye.x; }
	float getCenterY() const { return 3.0 * getForward().y + eye.y; }
	float getCenterZ() const { return 3.0 * getForward().z + eye.z; }

	const Vector &getTop() { if (!valid) update(); return top; }
	float getTopX() { return getTop().x; }
	float getTopY() { return getTop().y; }
	float getTopZ() { return getTop().z; }

	void lookAt() {
		if (!valid) update();
		gluLookAt(eye.x, eye.y, eye.z,
		          3.0 * forward.x + eye.x,
		          3.0 * forward.y + eye.y,
		          3.0 * forward.z + eye.z,
		          0, 1, 0);
	}


	/* Camera control part */
	static void setSize(int w, int h) {
		wndWidth = w, wndHeight = h;
		glutWarpPointer(wndWidth >> 1, wndHeight >> 1);
	}
	static void setDefaultCamera(Camera *theCamera) {
		camera = theCamera;
	}
	static Camera *getDefaultCamera() { return camera; }
	static void defaultLookAt() {
		if (camera) camera->lookAt();
	}
	static unsigned long getTicks() { return ticks; }
	static void setTickRedisplays(bool val) { tickRedisplays = val; }
	static void registerHandlers();

	typedef void(*KeyboardFunc)(unsigned key, bool down, int x, int y);
	static void setKeyboardFunc(KeyboardFunc theKeyboardFunc) {
		keyboardFunc = theKeyboardFunc;
	}
	typedef void(*ResizeFunc)(int w, int h);
	static void setResizeFunc(ResizeFunc theResizeFunc) {
		resizeFunc = theResizeFunc;
	}

private:
	void update() const;

	Vector eye;
	float rotX, rotY;
	mutable Vector forward, top, left;
	mutable bool valid;


	/* Camera control part */
	static void handleKeyboardDown(unsigned char key, int x, int y);
	static void handleKeyboardUp(unsigned char key, int x, int y);
	static void handleSpecialKeyboardDown(int key, int x, int y);
	static void handleSpecialKeyboardUp(int key, int x, int y);
	static void handleMouse(int button, int state, int x, int y);
	static void handleTick(int to);
	static void handleMotion(int x, int y);
	static void handleResize(int w, int h);

	static Camera *camera;
	static int wndWidth, wndHeight;
	static KeyboardFunc keyboardFunc;
	static ResizeFunc resizeFunc;
	static unsigned long mouseButtons;
	static bool tickRedisplays;
	static unsigned long ticks;
};



struct CameraControl {

private:
	static unsigned long ticks;
	static Camera *camera;
};



}

#endif
/*
 * src/gl/quadric.hpp
 * Copyright 2010 by Michal Nazarewicz    <mina86@mina86.com>
 *               and Maciej Swietochowski <m@swietochowski.eu>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef H_QUADRIC_HPP
#define H_QUADRIC_HPP

#include <new>

#ifdef __APPLE__
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

namespace gl {

struct Quadric {
	Quadric() throw(std::bad_alloc) : quad(gluNewQuadric()) {
		if (!quad) {
			throw std::bad_alloc();
		}
	}

	~Quadric() {
		gluDeleteQuadric(quad);
	}

	void setDrawStyle(GLenum drawStyle) {
		gluQuadricDrawStyle(quad, drawStyle);
	}

	operator GLUquadric *() const { return quad; }

private:
	Quadric(Quadric &);
	void operator=(Quadric &);

	GLUquadric *quad;
};

}

#endif
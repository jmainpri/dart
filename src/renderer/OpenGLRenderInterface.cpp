/*
 * Copyright (c) 2011, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Jie (Jay) Tan <jtan34@cc.gatech.edu>
 * Date: 07/18/2011
 *
 * Geoorgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include "OpenGLRenderInterface.h"
#include "utils/LoadOpengl.h"
#include <iostream>
#include <assimp/cimport.h>

#include <kinematics/Skeleton.h>
#include <kinematics/BodyNode.h>
#include <kinematics/Shape.h>
#include <kinematics/ShapeMesh.h>

using namespace std;
using namespace Eigen;

namespace renderer {

    void OpenGLRenderInterface::initialize() {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glCullFace(GL_FRONT);
        glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glShadeModel(GL_SMOOTH);
        clear(Vector3d(1.0, 1.0, 1.0));
    }

    void OpenGLRenderInterface::destroy() {

    }

    void OpenGLRenderInterface::setViewport(int _x,int _y,int _width,int _height) {
        glViewport(_x, _y, _width, _height);
        mViewportX = _x;
        mViewportY = _y;
        mViewportWidth = _width;
        mViewportHeight = _height;
    }

    void OpenGLRenderInterface::getViewport(int& _x, int& _y, int& _width, int& _height) const {
        _x = mViewportX;
        _y = mViewportY;
        _width = mViewportWidth;
        _height =mViewportHeight;
    }

    void OpenGLRenderInterface::clear(const Vector3d& _color) {
        glClearColor((GLfloat)_color[0], (GLfloat)_color[1], (GLfloat)_color[2], 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRenderInterface::setDefaultLight() {

    }

    void OpenGLRenderInterface::turnLightsOff() {
        glDisable(GL_LIGHTING);
    }

    void OpenGLRenderInterface::turnLightsOn() {
        //not finished yet
        glEnable(GL_LIGHTING);	
    }

    void OpenGLRenderInterface::setMaterial(const Vector3d& _diffuse, const Vector3d& _specular, double _cosinePow) {
        
    }

    void OpenGLRenderInterface::getMaterial(Vector3d& _diffuse, Vector3d& _specular, double& _cosinePow) const {
        
    }

    void OpenGLRenderInterface::setDefaultMaterial() {
        
    }

    void OpenGLRenderInterface::pushMatrix() {
        glPushMatrix();
    }

    void OpenGLRenderInterface::popMatrix() {
        glPopMatrix();
    }

    void OpenGLRenderInterface::pushName(int _id) {
        glPushName(_id);
    }

    void OpenGLRenderInterface::popName() {
        glPopName();
    }

    void OpenGLRenderInterface::translate(const Vector3d& _offset) {
        glTranslated(_offset[0], _offset[1], _offset[2]);
    }

    void OpenGLRenderInterface::rotate(const Vector3d& _axis, double _rad) {
        glRotated(_rad, _axis[0], _axis[1], _axis[2]);
    }

    void OpenGLRenderInterface::scale(const Vector3d& _scale) {
        glScaled(_scale[0], _scale[1], _scale[2]);
    }

    void OpenGLRenderInterface::drawEllipsoid(const Vector3d& _size) {
        glScaled(_size(0), _size(1), _size(2));
        glutSolidSphere(0.5, 16, 16);
    }

    void OpenGLRenderInterface::drawCube(const Vector3d& _size) {
        glScaled(_size(0), _size(1), _size(2));
        glutSolidCube(1.0);
    }

    void color4_to_float4(const aiColor4D *c, float f[4])
    {
        f[0] = c->r;
        f[1] = c->g;
        f[2] = c->b;
        f[3] = c->a;
    }

    void set_float4(float f[4], float a, float b, float c, float d)
    {
        f[0] = a;
        f[1] = b;
        f[2] = c;
        f[3] = d;
    }

    // This function is taken from the examples coming with assimp
    void applyMaterial(const struct aiMaterial *mtl)
    {
        float c[4];

        GLenum fill_mode;
        int ret1, ret2;
        aiColor4D diffuse;
        aiColor4D specular;
        aiColor4D ambient;
        aiColor4D emission;
        float shininess, strength;
        int two_sided;
        int wireframe;
        unsigned int max;

        set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
            color4_to_float4(&diffuse, c);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

        set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
            color4_to_float4(&specular, c);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

        set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
            color4_to_float4(&ambient, c);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

        set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
            color4_to_float4(&emission, c);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

        max = 1;
        ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
        if(ret1 == AI_SUCCESS) {
            max = 1;
            ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
            if(ret2 == AI_SUCCESS)
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
            else
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        }
        else {
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
            set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
        }

        max = 1;
        if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
            fill_mode = wireframe ? GL_LINE : GL_FILL;
        else
            fill_mode = GL_FILL;
        glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

        max = 1;
        if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
            glDisable(GL_CULL_FACE);
        else 
            glEnable(GL_CULL_FACE);
    }


    // This function is taken from the examples coming with assimp
    void recursiveRender (const struct aiScene *sc, const struct aiNode* nd) {
        unsigned int i;
        unsigned int n = 0, t;
        aiMatrix4x4 m = nd->mTransformation;

        // update transform
        aiTransposeMatrix4(&m);
        glPushMatrix();
        glMultMatrixf((float*)&m);

        // draw all meshes assigned to this node
        for (; n < nd->mNumMeshes; ++n) {
            const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

            applyMaterial(sc->mMaterials[mesh->mMaterialIndex]);

            if(mesh->mNormals == NULL) {
                glDisable(GL_LIGHTING);
            } else {
                glEnable(GL_LIGHTING);
            }

            for (t = 0; t < mesh->mNumFaces; ++t) {
                const struct aiFace* face = &mesh->mFaces[t];
                GLenum face_mode;

                switch(face->mNumIndices) {
                    case 1: face_mode = GL_POINTS; break;
                    case 2: face_mode = GL_LINES; break;
                    case 3: face_mode = GL_TRIANGLES; break;
                    default: face_mode = GL_POLYGON; break;
                }

                glBegin(face_mode);

                for(i = 0; i < face->mNumIndices; i++) {
                    int index = face->mIndices[i];
                    if(mesh->mColors[0] != NULL)
                        glColor4fv((GLfloat*)&mesh->mColors[0][index]);
                    if(mesh->mNormals != NULL) 
                        glNormal3fv(&mesh->mNormals[index].x);
                    glVertex3fv(&mesh->mVertices[index].x);
                }

                glEnd();
            }

        }

        // draw all children
        for (n = 0; n < nd->mNumChildren; ++n) {
            recursiveRender(sc, nd->mChildren[n]);
        }

        glPopMatrix();
    }

    void OpenGLRenderInterface::drawMesh(const Vector3d& _size, const aiScene *_mesh) {
    	if(_mesh)
    		recursiveRender(_mesh, _mesh->mRootNode);
    }

    void OpenGLRenderInterface::drawList(GLuint index) {
    	glCallList(index);
    }

    void OpenGLRenderInterface::compileList(kinematics::Skeleton *_skel) {
    	if(_skel == 0)
    		return;

    	for(int i=0; i < _skel->getNumNodes(); i++) {
    		compileList(_skel->getNode(i));
    	}
    }

    void OpenGLRenderInterface::compileList(kinematics::BodyNode *_node) {
    	if(_node == 0)
    		return;

    	compileList(_node->getShape());
    }

    //FIXME: Use polymorphism instead of switch statements
    void OpenGLRenderInterface::compileList(kinematics::Shape *_shape) {
    	if(_shape == 0)
    		return;
		GLuint index;
    	switch(_shape->getShapeType()) {
    	case kinematics::Shape::P_UNDEFINED:
    		break;
    	case kinematics::Shape::P_BOX:
    		
    		index = glGenLists(1);
    		glNewList(index, GL_COMPILE);
    		OpenGLRenderInterface::drawCube(_shape->getDim());
    		glEndList();
			_shape->setColList(index);
			break;
    	case kinematics::Shape::P_CYLINDER:
    		break;
    	case kinematics::Shape::P_ELLIPSOID:
    		break;
    	case kinematics::Shape::P_SPHERE:
    		break;
    	case kinematics::Shape::P_MESH:
    		//FIXME: Separate these calls once BodyNode is refactored to contain
    		// both a col Shape and vis Shape.
    		kinematics::ShapeMesh* shapeMesh = dynamic_cast<kinematics::ShapeMesh*>(_shape);

			if(shapeMesh == 0)
				return;

			shapeMesh->setDisplayList(compileList(shapeMesh->getMesh()));

    		_shape->setColList(compileList(_shape->getCollisionMesh()));
    		break;
    	}
    }

    GLuint OpenGLRenderInterface::compileList(const aiScene *_mesh) {
    	if(!_mesh)
    		return 0;

    	// Generate one list
    	GLuint index = glGenLists(1);
    	// Compile list
    	glNewList(index, GL_COMPILE);
    	drawMesh(Vector3d::Ones(), _mesh);
    	glEndList();

    	return index;
    }

    void OpenGLRenderInterface::draw(kinematics::Skeleton *_skel, bool _vizCol, bool _colMesh) {
    	if(_skel == 0)
    		return;

    	for(int i=0; i < _skel->getNumNodes(); i++) {
    		draw(_skel->getNode(i), _vizCol, _colMesh);
    	}
    }

    void OpenGLRenderInterface::draw(kinematics::BodyNode *_node, bool _vizCol, bool _colMesh) {
    	if(_node == 0)
    		return;

    	// Get world transform
    	Affine3d pose;
    	pose.matrix() = _node->getWorldTransform();

    	// GL calls
    	if(_vizCol && _node->getColliding()) {
    		glDisable(GL_TEXTURE_2D);
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(1.0f, .1f, .1f);
    	}

    	glPushMatrix();
    	glMultMatrixd(pose.data());

    	draw(_node->getShape(), _colMesh);

    	glColor3f(1.0f,1.0f,1.0f);
		glEnable( GL_TEXTURE_2D );
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
    }

    //FIXME: Refactor this to use polymorphism.
    void OpenGLRenderInterface::draw(kinematics::Shape *_shape, bool _colMesh) {
    	if(_shape == 0)
    		return;

		Affine3d pose = _shape->getTransform();

		glPushMatrix();
		glMultMatrixd(pose.data());

    	switch(_shape->getShapeType()) {
    	case kinematics::Shape::P_UNDEFINED:
    		break;
    	case kinematics::Shape::P_BOX:
    		//FIXME: We are not in a glut instance
    		//drawCube(_shape->getDim());
    		break;
    	case kinematics::Shape::P_CYLINDER:
    		//FIXME: Implement this
    		break;
    	case kinematics::Shape::P_ELLIPSOID:
    		//FIXME: We are not in a glut instance
    		//drawEllipsoid(_shape->getDim());
    		break;
    	case kinematics::Shape::P_SPHERE:
    		//FIXME: We are not in a glut instance
    		//drawEllipsoid(_shape->getDim());
    		break;
    	case kinematics::Shape::P_MESH:
    		kinematics::ShapeMesh* shapeMesh = dynamic_cast<kinematics::ShapeMesh*>(_shape);

    		if(shapeMesh == 0)
    			return;

    		const aiScene* model = _colMesh ? shapeMesh->getCollisionMesh() : shapeMesh->getMesh();
    		const GLuint index = _colMesh ? shapeMesh->getColList() : shapeMesh->getDisplayList();

    		if(index) {
    			drawList(index);
    		} else {
    			drawMesh(Vector3d::Ones(), model);
    		}
    		break;
    	}

    	glPopMatrix();
    }

    void OpenGLRenderInterface::setPenColor(const Vector4d& _col) {
        glColor4d(_col[0], _col[1], _col[2], _col[3]);
    }

    void OpenGLRenderInterface::setPenColor(const Vector3d& _col) {
        glColor4d(_col[0], _col[1], _col[2], 1.0);
    }

    void OpenGLRenderInterface::readFrameBuffer(DecoBufferType _buffType, DecoColorChannel _ch, void *_pixels) {

    }
    
    void OpenGLRenderInterface::saveToImage(const char *_filename, DecoBufferType _buffType) {

    }

} // namespace renderer

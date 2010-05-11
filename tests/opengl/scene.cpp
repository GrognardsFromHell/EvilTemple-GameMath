
#include <windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <string>
#include <sstream>

#include "tga.h"
#include "model.h"
#include "material.h"
#include "materialstate.h"

#include "../../include/gamemath.h"
using namespace GameMath;

#include <QtCore/QFile>

void Draw(Model *model);
void Draw(Model *model, MaterialState *material);

Model model;

#define HANDLE_GL_ERROR handleGlError(__FILE__, __LINE__);
inline static void handleGlError(const char *file, int line) {
	std::string error;

	GLenum glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		std::stringstream errorString;

		errorString << "OpenGL error in " << file << " @ line " << line << ": " << gluErrorString(glErr) << std::endl;

		error.append(errorString.str());

		glErr = glGetError();
	}

	if (error.size() > 0) {
		MessageBoxA(NULL, error.c_str(), "OpenGL Error", MB_OK|MB_ICONERROR);
	}
}

RenderStates renderStates;

class FileTextureSource : public TextureSource
{
public:
	SharedTexture loadTexture(const QString &name)
	{
		// Try loading the TGA image
		TGAImg img;
		img.Load(qPrintable(name)); // TODO: Error handling, Best bet: load some "error" texture instead

		SharedTexture texture(new Texture);
		texture->load(img);

		return texture;
	}
};

FileTextureSource textureSource;

bool CreateScene(int w, int h, const char *modelFile)
{
	glEnable(GL_MULTISAMPLE);

	if (!model.open(modelFile, renderStates)) {
		MessageBoxA(NULL, qPrintable(model.error()), "Error", MB_OK|MB_ICONERROR);
		return false;
	}
		
	glEnable(GL_DEPTH_TEST); // Use Z-Buffer & depth testing
	HANDLE_GL_ERROR

	glEnable(GL_CULL_FACE);
	HANDLE_GL_ERROR
		
	// TODO: Convert this to Matrix4-only code (don't use the GL matrix stack)
	Matrix4 projectionMatrix = Matrix4::ortho(-w/2, w/2, -h/2, h/2, 1, 3628);
	renderStates.setProjectionMatrix(projectionMatrix);

	Vector4 eyeVector(250.0, 500, 500, 0);
	Vector4 centerVector(0, 10, 0, 0);
	Vector4 upVector(0, 1, 0, 0);
	Matrix4 viewMatrix = Matrix4::lookAt(eyeVector, centerVector, upVector);

	renderStates.setViewMatrix(viewMatrix);
		
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_BLEND); HANDLE_GL_ERROR
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); HANDLE_GL_ERROR
	
	return true;
}

void DestroyScene()
{
	model.close();
}

void DrawScene(double secondsElapsed)
{
	const double anglePerSecond = 90;
	float angle = (float)(secondsElapsed * anglePerSecond);

	// We only draw once all messages have been processed
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f ); HANDLE_GL_ERROR
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); HANDLE_GL_ERROR

	Quaternion q = Quaternion::fromAxisAndAngle(0, 1, 0, angle * 0.01745329251994329576923690768489);
	Matrix4 m = Matrix4::transformation(Vector4(1, 1, 1, 0), q, Vector4(0, 0, 0, 0));
	renderStates.setWorldMatrix(m);

	Draw(&model);
}

void Draw(Model *model) {
	for (int faceGroupId = 0; faceGroupId < model->faces; ++faceGroupId) {
		const FaceGroup &faceGroup = model->faceGroups[faceGroupId];
		MaterialState *material = faceGroup.material;

		if (!material)
			continue;

		for (int i = 0; i < material->passes.size(); ++i) {
			MaterialPassState &pass = material->passes[i];

			pass.program.bind(); HANDLE_GL_ERROR

			// Bind texture samplers
			for (int j = 0; j < pass.textureSamplers.size(); ++j) {
				pass.textureSamplers[j].bind(); HANDLE_GL_ERROR
			}

			// Bind uniforms
			for (int j = 0; j < pass.uniforms.size(); ++j) {
				pass.uniforms[j].bind(); HANDLE_GL_ERROR
			}

			// Bind attributes
			for (int j = 0; j < pass.attributes.size(); ++j) {
				MaterialPassAttributeState &attribute = pass.attributes[j];

				// Bind the correct buffer
				switch (attribute.bufferType) {
				case 0:
					glBindBuffer(GL_ARRAY_BUFFER, model->positionBuffer);
					break;
				case 1:
					glBindBuffer(GL_ARRAY_BUFFER, model->normalBuffer);
					break;
				case 2:
					glBindBuffer(GL_ARRAY_BUFFER, model->texcoordBuffer);
					break;
				}

				// Assign the attribute
				glEnableVertexAttribArray(attribute.location);
				glVertexAttribPointer(attribute.location, attribute.binding.components(), attribute.binding.type(), 
					attribute.binding.normalized(), attribute.binding.stride(), (GLvoid*)attribute.binding.offset());
				HANDLE_GL_ERROR
			}

			// Set render states
			foreach (const SharedMaterialRenderState &state, pass.renderStates) {
				state->enable();
			}
		
			// Draw the actual model
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceGroup.buffer); HANDLE_GL_ERROR
			glDrawElements(GL_TRIANGLES, faceGroup.elementCount, GL_UNSIGNED_SHORT, 0); HANDLE_GL_ERROR

			// Reset render states to default
			foreach (const SharedMaterialRenderState &state, pass.renderStates) {
				state->disable();
			}

			// Unbind textures
			for (int j = 0; j < pass.textureSamplers.size(); ++j) {
				pass.textureSamplers[j].unbind();
			}

			// Unbind attributes
			for (int j = 0; j < pass.attributes.size(); ++j) {
				MaterialPassAttributeState &attribute = pass.attributes[j];
				glDisableVertexAttribArray(attribute.location); HANDLE_GL_ERROR
			}

			pass.program.unbind();
		}
	}
}

void Draw(Model *model, MaterialState *material) {
	for (int faceGroupId = 0; faceGroupId < model->faces; ++faceGroupId) {
		const FaceGroup &faceGroup = model->faceGroups[faceGroupId];

		for (int i = 0; i < material->passes.size(); ++i) {
			MaterialPassState &pass = material->passes[i];

			pass.program.bind(); HANDLE_GL_ERROR

				// Bind texture samplers
				for (int j = 0; j < pass.textureSamplers.size(); ++j) {
					pass.textureSamplers[j].bind(); HANDLE_GL_ERROR
				}

				// Bind uniforms
				for (int j = 0; j < pass.uniforms.size(); ++j) {
					pass.uniforms[j].bind(); HANDLE_GL_ERROR
				}

				// Bind attributes
				for (int j = 0; j < pass.attributes.size(); ++j) {
					MaterialPassAttributeState &attribute = pass.attributes[j];

					// Bind the correct buffer
					switch (attribute.bufferType) {
					case 0:
						glBindBuffer(GL_ARRAY_BUFFER, model->positionBuffer);
						break;
					case 1:
						glBindBuffer(GL_ARRAY_BUFFER, model->normalBuffer);
						break;
					case 2:
						glBindBuffer(GL_ARRAY_BUFFER, model->texcoordBuffer);
						break;
					}

					// Assign the attribute
					glEnableVertexAttribArray(attribute.location);
					glVertexAttribPointer(attribute.location, attribute.binding.components(), attribute.binding.type(), 
						attribute.binding.normalized(), attribute.binding.stride(), (GLvoid*)attribute.binding.offset());
					HANDLE_GL_ERROR
				}

				// Set render states
				foreach (const SharedMaterialRenderState &state, pass.renderStates) {
					state->enable();
				}

				// Draw the actual model
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceGroup.buffer); HANDLE_GL_ERROR
					glDrawElements(GL_TRIANGLES, faceGroup.elementCount, GL_UNSIGNED_SHORT, 0); HANDLE_GL_ERROR

					// Reset render states to default
					foreach (const SharedMaterialRenderState &state, pass.renderStates) {
						state->disable();
				}

				// Unbind textures
				for (int j = 0; j < pass.textureSamplers.size(); ++j) {
					pass.textureSamplers[j].unbind();
				}

				// Unbind attributes
				for (int j = 0; j < pass.attributes.size(); ++j) {
					MaterialPassAttributeState &attribute = pass.attributes[j];
					glDisableVertexAttribArray(attribute.location); HANDLE_GL_ERROR
				}

				pass.program.unbind();
		}
	}
}

void KeyPressed(int key)
{
	static bool viewOne = true;

	if (key == VK_F1) {

		Matrix4 viewMatrix;

		if (!viewOne) {
			Vector4 eyeVector(250.0, 500, 500, 0);
			Vector4 centerVector(0, 10, 0, 0);
			Vector4 upVector(0, 1, 0, 0);
			viewMatrix = Matrix4::lookAt(eyeVector, centerVector, upVector);
		} else {

			// 135,0000005619373 Rotation around Y axis
			// -44,42700648682643 Rotation around X axis

			Quaternion rot1 = Quaternion::fromAxisAndAngle(0, 1, 0, -135.0000005619373);
			Quaternion rot2 = Quaternion::fromAxisAndAngle(1, 0, 0, -44.42700648682643);

			viewMatrix = Matrix4::transformation(Vector4(1,1,1,0),rot1,Vector4(0,0,-2000,0)) * Matrix4::transformation(Vector4(1,1,1,0),rot2,Vector4(0,0,0,0));
		}

		renderStates.setViewMatrix(viewMatrix);

		viewOne = !viewOne;

	}
}

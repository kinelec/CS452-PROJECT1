#ifndef INITSHADERS_H_
#define INITSHADERS_H_
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdio>
#include <iostream>
using namespace std;

//functions
GLuint createShader(GLenum type, const GLchar* source);
const GLchar* loadShader(const char* fname);
GLuint createProgram(const vector<GLuint> list);
void transform(GLuint prog);

typedef struct{
	GLenum type;
  	const char* fname;
} Shader;


//initialize the shaders
GLuint initShaders(Shader* s){
  
  	Shader* shade = s;
  	vector<GLuint> list;
  
  	while(shade->type != GL_NONE){
		list.push_back(createShader(shade->type,loadShader(shade->fname)));
    	++shade;
  	}
  
  	GLuint prog = createProgram(list);
 	glUseProgram(prog);
  
	//camera movement
  	glm::mat4 view;
  	view = glm::lookAt(
 	  	glm::vec3(0.0f, 0.0f, 50.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
  	GLint tmpLocation = glGetUniformLocation(prog, "viewMatrix");
  	glUniformMatrix4fv(tmpLocation, 1, GL_FALSE, &view[0][0]);
  
	//perspective viewing
  	glm::mat4 mainMatrix;
  	mainMatrix = glm::perspective(57.0,1.0,.1,500.0);
  	tmpLocation = glGetUniformLocation(prog, "Matrix");
  	glUniformMatrix4fv(tmpLocation, 1, GL_FALSE, &mainMatrix[0][0]);
  
  	return prog;
   
}

//load vertex & fragment shaders
const GLchar* loadShader(const char* fname){

  	FILE* file = fopen(fname, "rb");

  	if(!file){
    	fprintf(stderr,"unable to open file '%s'\n",fname);
    	return NULL;
  	}
  
  	//get file length
  	fseek(file, 0, SEEK_END);
  	long fsize=ftell(file);
  	fseek(file, 0, SEEK_SET);
  	 
  	GLchar* source= new GLchar[fsize+1];
  	fread(source, 1, fsize, file);
    
  	if(ftell(file) == 0){
    	fprintf(stderr, "File '%s' is empty.\n",fname);
    	return NULL;
  	}
  	fclose(file);
  	source[fsize] = 0;
  	return const_cast<const GLchar*>(source); 
}

//this function create your shader
GLuint createShader(GLenum type, const GLchar* source){
  
  	GLuint shade = glCreateShader(type);
  	glShaderSource(shade, 1, &source, NULL);
  	glCompileShader(shade);
  	
	//debugging compile
  	GLint compstat;
  	glGetShaderiv(shade, GL_COMPILE_STATUS, &compstat);
  	if(!compstat){
    	GLint logsize;
    	glGetShaderiv(shade, GL_INFO_LOG_LENGTH, &logsize);
    	GLchar* log = new GLchar[logsize+1];
    	glGetShaderInfoLog(shade,logsize,&logsize,log);
    	const char *info = NULL;
    	switch(type){
      		case GL_VERTEX_SHADER: info = "vertex"; break;
      		case GL_GEOMETRY_SHADER_EXT: info = "geometric"; break;
      		case GL_FRAGMENT_SHADER: info = "fragment"; break;
    	}
    	fprintf(stderr,"\nCompile failure in %u shader: %s\n Err msg:\n%s\n",type,info,log);
    	delete[] log;
  	}
  	return shade;
}

//create shading program to link the shader to
GLuint createProgram(const vector<GLuint> list){

  	GLuint prog = glCreateProgram();
  
  	for(GLuint x = 0; x < list.size(); x++){
		glAttachShader(prog,list[x]);
	}

 	glBindAttribLocation(prog, 0, "in_position");
  	glBindAttribLocation(prog, 1, "in_color");
  	glLinkProgram(prog);
  
	//debugging linking
  	GLint linkstat;
  	glGetProgramiv(prog, GL_LINK_STATUS, &linkstat);
  	if(!linkstat){
    	GLint linksize;
    	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &linksize);
    	GLchar *log = new GLchar[linksize+1];
    	glGetProgramInfoLog(prog,linksize,&linksize,log);
    	fprintf(stderr,"\nShader linking failed. Err Msg: %s\n",log);
    	delete[] log;
    	for(GLuint x = 0; x < list.size(); x++){
			glDeleteShader(list[x]);
		}
  	}
  	return prog;
}

#endif

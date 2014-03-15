#include "initShaders.h"
#include <cstdlib>
using namespace std;

void rotate(GLuint locate);

GLuint vaoID,vboID[2],eboID;
GLuint prog;

glm::vec3 triTran;

GLfloat vertices[]={
	4.0f,0.0f,0.0f,
    0.0f,7.0f,0.0f,
    -4.0f,0.0f,0.0f
					};

GLfloat colors[]={
	0.0f,0.0f,1.0f,1.0f,
	1.0f,0.0f,1.0f,1.0f,
	0.0f,0.5f,1.0f,1.0f,					
	              		};

void init(){
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 150, 500, 500);	
	glGenVertexArrays(1,&vaoID);
	glBindVertexArray(vaoID);
	
	glGenBuffers(2, vboID);
	glBindBuffer(GL_ARRAY_BUFFER,vboID[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  

	Shader s[]={
  	{ GL_VERTEX_SHADER , "vertexshader.glsl"},
  	{ GL_FRAGMENT_SHADER , "fragmentshader.glsl"}, 
  	{ GL_NONE , NULL} 
  	};
		
  	prog=initShaders(s);
  
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);  
}


void display(SDL_Window* screen){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 trans;
	
	trans = glm::translate(trans,triTran);
    
	//transformation matrix
  	GLint tmpLocation = glGetUniformLocation(prog,"modelMatrix");
	glUniformMatrix4fv(tmpLocation,1,GL_FALSE,&trans[0][0]);
	
	glDrawArrays(GL_TRIANGLES,0,3);
	glFlush();

	glMatrixMode(GL_MODELVIEW);
	//gluLookAt();
	glLoadIdentity();
	SDL_GL_SwapWindow(screen);
}

void input(SDL_Window* screen){
	
	//keyboard events
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_QUIT:exit(0);break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:exit(0);
					case SDLK_a:triTran.x-=2;break;
					case SDLK_d:triTran.x+=2;break;
				}
		}
	}
}


int main(int argc, char **argv){
	SDL_Window *window;
	
	//initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
    	SDL_Quit();
    	exit(1);
	}

	window = SDL_CreateWindow("Sidescroller",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,SDL_WINDOW_OPENGL);
	
	if(window==NULL){
		fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
	}

	SDL_GLContext glcontext=SDL_GL_CreateContext(window);
	
	//initialize glew
  	glewExperimental=GL_TRUE;
  	if(glewInit()){
    	fprintf(stderr, "Unable to initalize GLEW");
    	exit(EXIT_FAILURE);
  	}
  
	init();
	
	while(true){
		input(window);
		display(window);
	}

	SDL_GL_DeleteContext(glcontext);
  	SDL_DestroyWindow(window);
  	SDL_Quit();
 
  	return 0;
}

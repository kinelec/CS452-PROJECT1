#ifndef PAINT_H_
#define PAINT_H_
#include "initShaders.h"

GLuint prog;

//generates random float from min and max
GLfloat randomgen(float min, float max){
	return min + static_cast <GLfloat> (rand())/(static_cast <GLfloat> (RAND_MAX/(max-min)));
}

class paint{
public:
	//buffers
	GLuint vaoID,vboID[2],eboID;
	//vertex arrays & size
	GLfloat* vertarray;
	int vertsize;
	//color arrays & size
	GLfloat* colorarray;
	int colorsize;
	//elems arrays & size
  	GLubyte* elemsarray;
  	int elemssize;
	//transformation
	glm::vec3 objTran;
	//if moves move is set to 1; on collision move is set to 2
	int move;
	//speed
	float speed;

	void draw(){
		//buffers
		glGenVertexArrays(1,&vaoID);
    	glBindVertexArray(vaoID);
    	glBindBuffer(GL_ARRAY_BUFFER,vboID[0]);
    	glBufferData(GL_ARRAY_BUFFER,vertsize,vertarray,GL_STATIC_DRAW);
    	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)0);
    	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    	glBufferData(GL_ARRAY_BUFFER,colorsize,colorarray,GL_STATIC_DRAW);
    	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboID);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER,elemssize,elemsarray,GL_STATIC_DRAW);
		//transformation
		glm::mat4 trans;
		trans=glm::translate(trans,objTran);
    	GLint tempLoc = glGetUniformLocation(prog,"modelMatrix");
    	glUniformMatrix4fv(tempLoc,1,GL_FALSE,&trans[0][0]);

		glEnableVertexAttribArray(0);
    	glEnableVertexAttribArray(1);		

		//movement; generates random x value
		if(move == 1){
			if(objTran.y <= -31.5){
				objTran.y = 27.0;
				objTran.x = randomgen(-27.0, 2.5);
			}
			else objTran.y -= speed;
		}
		//when collision occurs
		else if(move == 2){
			objTran.y = -80.0;
		}

		glDrawElements(GL_POLYGON,elemssize,GL_UNSIGNED_BYTE,NULL);
  	}
			
};

#endif

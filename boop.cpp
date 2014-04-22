#include "initShaders.h"
#include "paint.h"
using namespace std;

GLfloat pit,yaw,scalar=1;
//initial block speed
GLfloat blockspeed = 0.3;

//timer
time_t timer;

//initial lives & score
int lives = 3;
int score = 0;

//shaders
Shader s[]={
	{ GL_VERTEX_SHADER , "vertexshader.glsl"},
	{ GL_FRAGMENT_SHADER , "fragmentshader.glsl"}, 
	{ GL_NONE , NULL} 
    };

//for key events
bool pressed = false;
SDL_Event event;
SDL_Event lastkey;

//player
paint player;

//blocks to avoid
class avoidarray{
public:
	int size;
	paint blocks[300];
	void init(){
		size=0;
	}
} avoidObjects;

//diamonds to collect
class collectarray{
public:
	int size;
	paint diamonds[300];
	void init(){
		size=0;
	}	
} collectObjects;


//creates the objects using their vertexarray, colorarray, elemsarray
void createObject(GLfloat vert[], int vsize, GLfloat color[], int csize, GLubyte elems[], int esize, paint &obj, int move){
	obj.move = move;
	obj.vertarray = (GLfloat*)malloc(sizeof(GLfloat)*vsize);
	obj.vertsize = vsize;
	memcpy(obj.vertarray, vert, 4*vsize);

	obj.colorarray = (GLfloat*)malloc(sizeof(GLfloat)*csize);
	obj.colorsize = csize;
	memcpy(obj.colorarray, color, 4*csize);
	
	obj.elemsarray = (GLubyte*)malloc(sizeof(GLfloat)*esize);
	obj.elemssize = esize;
	memcpy(obj.elemsarray, elems, esize);
	
	obj.objTran.y = 29.0;
	obj.objTran.x = randomgen(-27.0, 2.5);
	
	//buffers
	glGenBuffers(2, obj.vboID);
	glGenBuffers(1,&obj.eboID);
}


//creates player triangle
void playerD(){
	GLfloat vert[] = {0.0f,0.0f,
						 2.0f,2.0f,
						 4.0f,0.0f};
						 
	GLfloat color[]={1.0f,1.0f,1.0f,1.0f,
					   1.0f,1.0f,1.0f,1.0f,
					   1.0f,1.0f,1.0f,1.0f};
					   
	GLubyte elems[]={0,1,2};

	createObject(vert, sizeof(vert), color, sizeof(color), elems, sizeof(elems), player, 0);
	//sets initial position of player
	player.objTran.y = -25.0;
	player.objTran.x = -12.0;
}



void init(){
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 800);	
  	prog=initShaders(s);
 
	playerD(); 
}

//collision detection
void collision(){
	int i;
	
	for(i = 0;i < collectObjects.size; i++){
		if(((-0.5)*(collectObjects.diamonds[i].objTran.y)+((player.objTran.x+3.5)-12.0) >= collectObjects.diamonds[i].objTran.x) && ((0.5)*(collectObjects.diamonds[i].objTran.y)+((player.objTran.x)+12.0) <= (collectObjects.diamonds[i].objTran.x))){
			if((collectObjects.diamonds[i].objTran.y-3.0<=player.objTran.y) && ((collectObjects.diamonds[i].objTran.y>=player.objTran.y))){		
					score++;
					printf("score: %d\n", score);
					collectObjects.diamonds[i].move = 2;
			}
		}
	}

	for(i = 0; i < avoidObjects.size; i++){	
		if(((-0.5)*(avoidObjects.blocks[i].objTran.y)+((player.objTran.x+2.0)-11.0) >= avoidObjects.blocks[i].objTran.x) && ((0.5)*(avoidObjects.blocks[i].objTran.y)+((player.objTran.x)+11.0) <= (avoidObjects.blocks[i].objTran.x))){
			if((avoidObjects.blocks[i].objTran.y-2.0<=player.objTran.y)&&((avoidObjects.blocks[i].objTran.y>=player.objTran.y))){
				
				if(lives == 3){
						printf("Oh no! Only 2 lives remaining.\n");
						sleep(1);
						lives--;
						//remove object from view
						avoidObjects.blocks[i].move = 2;
						//speed some blocks up
						blockspeed = 0.5;
					}
				else if(lives == 2){
						printf("Only one life left!\n");
						sleep(1);
						lives--;
						avoidObjects.blocks[i].move = 2;
						blockspeed = 0.7;
					}
				else if(lives == 1){				
						sleep(2);
						printf("Game Over\n");
						printf("Final score: %d\n", score);
						exit(0);
					}
			}
		}
	}
}

//red blocks to avoid
void avoidObj(){
	GLfloat vert[] = {0.0f,0.0f,
				    0.0f,2.0f,
				    2.0f,2.0f,
				    2.0f,0.0f};
					
	int i;
	GLfloat color[] = {1.0f,0.0f,0.0f,1.0f,
					   1.0f,0.0f,0.0f,1.0f,
					   1.0f,0.0f,0.0f,1.0f,
					   1.0f,0.0f,0.0f,1.0f};
	
	avoidObjects.blocks[avoidObjects.size].speed = blockspeed;
	   
	GLubyte elems[]={0,1,2,3};
	
	createObject(vert, sizeof(vert), color, sizeof(color), elems, sizeof(elems), avoidObjects.blocks[avoidObjects.size], 1);
	avoidObjects.size++;
}

//blue diamonds to collect
void collectObj(){
	GLfloat vert[]={
	0.0f,3.0f,
	1.0f,0.0f,
	0.0f,-3.0f,	
	-1.0f,0.0f
					};

GLfloat color[]={
	0.0f,0.5f,1.0f,1.0f,
	0.0f,0.5f,1.0f,1.0f,
	0.0f,0.5f,1.0f,1.0f,
	0.0f,0.5f,1.0f,1.0f,						
	              		};
											
 	GLubyte elems[]={0,1,2,3,0};

	GLfloat speedvarient = randomgen(1.0, 3.0);
					   
	
	createObject(vert, sizeof(vert), color, sizeof(color), elems, sizeof(elems), collectObjects.diamonds[collectObjects.size], 1);
	collectObjects.diamonds[collectObjects.size].speed = 1.0/speedvarient;
	collectObjects.size++;
}

void display(SDL_Window* screen){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	collision();
	player.draw();

	int i;
	for(i=0; i<avoidObjects.size; i++){
		avoidObjects.blocks[i].draw();
	}

	for(i=0; i<collectObjects.size; i++){
		collectObjects.diamonds[i].draw();
	}

	glFlush();
	SDL_GL_SwapWindow(screen);
}

void input(SDL_Window* screen){
	if(pressed){
		if(player.objTran.x <= -27){
			player.objTran.x = -27;
		}
		else if(lastkey.key.keysym.sym == SDLK_LEFT){
 			player.objTran.x -= 0.5;
		}
		if(player.objTran.x >= 2.5){ 
			player.objTran.x = 2.5;
		}
		else if(lastkey.key.keysym.sym == SDLK_RIGHT){
			player.objTran.x += 0.5;
		}
	}
    while ( SDL_PollEvent(&event) )
    {
        switch (event.type)
        {
            case SDL_QUIT:
				break;
            case SDL_KEYDOWN:
                pressed = true;
                break;
            case SDL_KEYUP:
                pressed = false;
                break;
            default:
                break;
        }
    }
	lastkey = event;
}

int main(int argc, char **argv){
	printf("Avoid the red boxes. Collect the blue diamonds.\nYou have 3 lives.\n");
	avoidObjects.init();
	collectObjects.init();
	SDL_Window *window;
	
	//initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
    	SDL_Quit();
    	exit(1);
	}

	window = SDL_CreateWindow("Christiansen & Kinel Project1",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,SDL_WINDOW_OPENGL);
	
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
	int prevsecond = 0;
	while(true){
    	if(avoidObjects.size < 300){
			if(collectObjects.size < 300){
    			if((int)difftime(time(NULL),timer)!= prevsecond){
    				if(avoidObjects.size < 3){
    					if((int)difftime(time(NULL), timer)%1 == 0)
    						avoidObj();
    				}
    				else{
    					if((int)difftime(time(NULL), timer)%10 == 0){
    						avoidObj();
							collectObj();
						}
					}	
    			}
    		}
    	}
    	prevsecond = (int)difftime(time(NULL), timer); //set the second to compare next iteration
    
		input(window);
		display(window);
	}

	SDL_GL_DeleteContext(glcontext);
  	SDL_DestroyWindow(window);
  	SDL_Quit();
 
  	return 0;
}

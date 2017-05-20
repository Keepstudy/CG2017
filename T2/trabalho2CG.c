/**
 * ---------------------------------------------------------------------
 * TRABALHO 2 - COMPUTAÇÃO GRÁFICA
 * @author João Victor Lopes da Silva Guimarães	8936843 
 * @author Laércio de Oliveira Júnior           8922198
 * @author Luiz Ricardo G. da Silva             8937402
 * @author Otávio Augusto F. Sousa              8936801
 * 
 * para compilar
 * $ gcc -o trabalho2CG trabalho2CG.c -lglut -lGLU -lGL -lm
 *
 * para executar
 * $ ./trabalho2CG
 * 
 * COMANDOS
 * w, a, s, d - Translada a camera nos eixos x e y relativos à camera
 * 
 * setas direcionais, q, e- Altera a rotação da camera em torno do ponto
 * focal
 * 
 * pgup,pgdn - Transladam a camera no eixo do vetor view-up
 * 
 * home,end - Zoom in, Zoom out
 * 
 * R - reseta a posição da câmera
 * 
 * 1,2 - Translada cubo
 * 3,4 - Rotaciona tetraedro
 * 5,6 - Escala Esfera
 * 
 * ---------------------------------------------------------------------
 */

#include <stdio.h>
#include <GL/glut.h>


/*--------------------------------------------------------------------*/
/*STRUCTS E DECLARAÇÕES DOS OBJETOS*/
/*--------------------------------------------------------------------*/
GLfloat fov, fAspect, zNear, zFar, R, G, B;

//Para transladar a Câmera
GLfloat obsX, obsY, obsZ;
//Posição da Camera
GLfloat eyeX, eyeY, eyeZ;
//Rotação da Camera
GLfloat rotX, rotY, rotZ;
//Posição do ponto de foco
GLfloat centerX, centerY, centerZ;
//Vetor view-up
GLfloat upx, upy, upz;



//Coordenadas do centro do cubo
GLfloat cubeX, cubeY, cubeZ;

//Angulo de rotação do Tetraedro
GLfloat tetRot;

//Fator de escala da esfera
GLfloat esfEscala;


void Initialize();
void Draw();
void Grid();
void Cube(float x, float y, float z, GLfloat R, GLfloat G, GLfloat B);
void Window(GLsizei w, GLsizei h);
void Visualization();
void gerenciaTeclado(unsigned char tecla, int x, int y);
void teclasExtras(int teclas, int x, int y);
void inicializaCubo();


void Initialize(){
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST); //depth buffering

	zNear = 0.5;
	zFar = 500;
    obsX = obsY = obsZ = 0;
    rotX = rotY = rotZ = 0;
    centerX = 1;
    centerY = 0;
    centerZ = 0;
    
    upx=0;
    upy=0;
    upz=1;
    
    eyeX = eyeY = eyeZ = 0;
    fov = 90;	
    
    cubeX=50.0f;
    cubeY=cubeZ=0.0f;
    
    tetRot = 0;
    
    esfEscala = 1;
    
    
}


/*--------------------------------------------------------------------*/
/*FUNÇÕES DE CÂMERA*/
/*--------------------------------------------------------------------*/

/***
 * Função callback para redimensionar a tela
 * @param w = screen width, largura da tela
 * @param h = screen height, altura da tela
 */

void Window(GLsizei w, GLsizei h){
	if (h == 0) //evitar div 0
		h = 1;

	glViewport(0, 0, w, h); //dimensoes viewport
	fAspect = (GLfloat)w/(GLfloat)h; //correcao do aspecto

	Visualization();
}

/***
 * Função para transformações na câmera
 */
void Visualization(){
	//GL_PROJECTION usado para manipulação da câmera
	glMatrixMode(GL_PROJECTION);
	//Carrega identidade
	glLoadIdentity();

	//Especifica o frustum da câmera
	gluPerspective(fov, fAspect, zNear, zFar);

	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upx, upy, upz);

	glTranslatef(-obsX, -obsY, -obsZ);
	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);
	glRotatef(rotZ, 0, 0, 1);
}




/*--------------------------------------------------------------------*/
/*FUNÇÕES DOS OBJETOS*/
/*--------------------------------------------------------------------*/

void transladaCubo(int sinal){
	if(sinal == 1 && cubeX < 200){
		cubeX+=10.0f;
		cubeY+=5.0f;
		cubeZ+=2.0f;
	}
	else if(sinal==0 && cubeX > -200){
		cubeX-=10.0f;
		cubeY-=5.0f;
		cubeZ-=2.0f;
	}
}

void rotacionaTetraedro(int sinal){
	if(sinal==1)
		tetRot = ((int)tetRot + 10)%360;
	else if(sinal==0)
		tetRot = ((int)tetRot - 10 + 360)%360;
}

void escalaEsfera(int sinal){
	if(sinal==1 && esfEscala < 10)
		esfEscala+=0.5;
		
	else if(sinal==0 && esfEscala > 0.5)
		esfEscala-=0.5;
}

/*--------------------------------------------------------------------*/
/*FUNÇÕES DE DESENHO*/
/*--------------------------------------------------------------------*/

/**
 * Função que desenha linhas encima dos eixos coordenados
 */
void desenhaEixos(){
	glLineWidth(5.0f);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);//Eixo X
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(50.0f, 0.0f, 0.0f);
	glEnd();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);//Eixo Y
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 50.0f, 0.0f);
	glEnd();
	
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);//Eixo Z
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 50.0f);
	glEnd();	
	
}


/**
 * Definir o Tetraedro, 3 vertices na base, 1 na ponta
 * Base1-0,0,0
 * Base2-10,0,0
 * Base2-0,10,0
 * Topo-0,0,10
 */

void desenhaTetraedro(){
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//Sem o push e o pop, tudo é transladado para a posição
	glLoadIdentity();
	glRotatef(tetRot, 1, 1, 1);	
	
	
	glLineWidth(5.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 25.0f, 0.0f);
		glVertex3f(-25.0f, -25.0f, 0.0f);
		glVertex3f(25.0f, -25.0f, 0.0f);
	glEnd();
	
	
	glBegin(GL_TRIANGLES);
		glVertex3f(25.0f, -25.0f, 0.0f);
		glVertex3f(0.0f, 25.0f, 0.0f);
		glVertex3f(0.0f, -25.0f, -25.0f);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		glVertex3f(-25.0f, -25.0f, 0.0f);
		glVertex3f(0.0f, 25.0f, 0.0f);
		glVertex3f(0.0f, -25.0f, -25.0f);
	glEnd();	
	
	glBegin(GL_TRIANGLES);
		glVertex3f(-25.0f, -25.0f, 0.0f);
		glVertex3f(25.0f, -25.0f, 0.0f);
		glVertex3f(0.0f, -25.0f, -25.0f);
	glEnd();		
	
	glPopMatrix();
}

void desenhaEsfera(){
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//Sem o push e o pop, tudo é transladado para a posição
	glLoadIdentity();
	glScalef(esfEscala, esfEscala, esfEscala);		
	
	glColor3f(0.0f, 0.0f,0.0f);
	glLineWidth(1.0f);
	glutWireSphere(50, 50, 50);
	
	glPopMatrix();
}

/**
 * 
 */
void desenhaCubo(){
		
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//Sem o push e o pop, tudo é transladado para a posição
	glLoadIdentity();
	glTranslatef(cubeX,cubeY,cubeZ);
	
	glColor3f(0.0f, 1.0f,1.0f);
	glLineWidth(5.0f);
	glutWireCube(20);
	
	glPopMatrix();
}




void Draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*Grid();
	Cube(5.0, 5.0, 5.0, 1, 0, 0);
	Cube(-5.0, 5.0, 5.0, 1, 1, 1);
	Cube(5.0, -5.0, 5.0, 0, 0, 1);
	Cube(5.0, 5.0, -5.0, 1, 1, 0);
	Cube(-5.0, -5.0, 5.0, 1, 0, 1);
	Cube(-5.0, 5.0, -5.0, 0, 1, 1);
	Cube(5.0, -5.0, -5.0, 0, 1, 0);
	Cube(-5.0, -5.0, -5.0, 0, 0, 0);*/
	
	//desenha um cubo azul em wireframe
	
	desenhaEsfera();
	desenhaCubo();
	desenhaEixos();	
	desenhaTetraedro();
	
	glutSwapBuffers();
}


/*--------------------------------------------------------------------*/
/*FUNÇÕES DE INPUT*/
/*--------------------------------------------------------------------*/
/**
 * Função para gerenciar input do teclado
 * @param tecla = caracter pressionado
 * @param x = posição x do mouse no momento de teclagem
 * @param y = posição y do mouse no momento de teclagem
 */
void gerenciaTeclado(unsigned char tecla, int x, int y){
	if(tecla == 27) //Esc
		exit(0);

	//move a camera para frente
    if (tecla == 'w')
        obsX +=0.5;
    
    //move a camera para a esquerda
    if (tecla == 'a')
        obsY +=0.5;     
    
    //move a camera para trás
    if (tecla == 's')
        obsX -=0.5;
        
	//move a camera para a direita
    if (tecla == 'd')
        obsY -=0.5;

    if (tecla == 'q')
        rotX +=5;

    if (tecla == 'e')
        rotX -=5;

    if (tecla == 'R')
        Initialize();

    if (tecla == 'z')
        glEnable(GL_DEPTH_TEST);

    if (tecla == 'Z')
        glDisable(GL_DEPTH_TEST);
        
	if (tecla == '1'){
		transladaCubo(1);
	}
	
	if (tecla == '2'){
		transladaCubo(0);
	}
	
	if (tecla == '3'){
		rotacionaTetraedro(1);
	}
		
	if (tecla == '4'){
		rotacionaTetraedro(0);
	}		
		
		
    if (tecla == '5'){
		escalaEsfera(1);	
	}   
    if (tecla == '6'){
		escalaEsfera(0);	
	}   
       
    Visualization();
	glutPostRedisplay();
}

/**
 * Função para gerenciar o input de teclas extras/especiais
 * @param tecla = caracter pressionado
 * @param x = posição x do mouse no momento de teclagem
 * @param y = posição y do mouse no momento de teclagem
 */
void teclasExtras(int tecla, int x, int y){
	switch (tecla){
		
		//Zoom in por decremento do fov.
		//fov menor reduz 
		case GLUT_KEY_HOME:
            if(fov >= 10)
                fov -= 5;
            break;
		
		//Zoom Out por incremento do fov
		case GLUT_KEY_END:
            if(fov <= 150)
                fov += (GLfloat)5;
			break;

        case GLUT_KEY_UP:
            rotY += (GLfloat)5;
            break;

        case GLUT_KEY_LEFT:
            rotZ -= (GLfloat)5;
            break;

        case GLUT_KEY_DOWN:
            rotY -= (GLfloat)5;
        	break;

        case GLUT_KEY_RIGHT:
            rotZ += (GLfloat)5;
            break;

        case GLUT_KEY_PAGE_UP:
            obsZ += (GLfloat) 0.1;
            break;

        case GLUT_KEY_PAGE_DOWN:
            obsZ -= (GLfloat) 0.1;
            break;
	}
	Visualization();
	glutPostRedisplay();
}


/*--------------------------------------------------------------------*/
/*MAIN*/
/*--------------------------------------------------------------------*/

int main(int argc, char* argv[]){
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(350, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Visualização 3D");
	glutDisplayFunc(Draw);
	glutReshapeFunc(Window);

	glutKeyboardFunc(gerenciaTeclado);
	
	glutSpecialFunc(teclasExtras);
	Initialize();
	glutMainLoop();

	return 0;
}


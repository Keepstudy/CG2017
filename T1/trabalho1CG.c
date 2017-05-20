/**
 * ---------------------------------------------------------------------
 * TRABALHO 1 - COMPUTAÇÃO GRÁFICA
 * @author João Victor Lopes da Silva Guimarães	8936843 
 * @author Laércio de Oliveira Júnior           8922198
 * @author Luiz Ricardo G. da Silva             8937402
 * @author Otávio Augusto F. Sousa              8936801
 * 
 * para compilar
 * $ gcc -o trabalho1CG trabalho1CG.c -lglut -lGLU -lGL -lm
 *
 * para executar
 * $ ./trabalho1CG
 * ---------------------------------------------------------------------
 */


#include<GL/glut.h>
#include<stdio.h>
#include<math.h>

#define TIME 33

/*--------------------------------------------------------------------*/
/*STRUCTS DOS OBJETOS*/
/*--------------------------------------------------------------------*/


//QUADRADO
typedef struct {
	//posição do ponto central do quadrado
	GLfloat center_x;
	GLfloat center_y;

	//tamanho do lado do quadrado
	GLsizei rsize;

	//Componentes do vetor direção do quadrado.
	//vetor deve ter norma = 1
	GLfloat direction_x;
	GLfloat direction_y;

	//Velocidade escalar, usada em X e Y
	GLfloat speed;

} square;



/*--------------------------------------------------------------------*/
/*FUNÇÕES DOS OBJETOS*/
/*--------------------------------------------------------------------*/

/**
 * Função que inicializa os valores de um quadrado.
 * @param sqr = ponteiro para square
 * @param x = posição inicial do centro do quadrado no eixo X 
 * @param y = posição inicial do centro do quadrado no eixo Y
 * @param size = tamanho do quadrado;
 * @param speed = velocidade do quadrado
 */

void initSquare (square* sqr, GLfloat x, GLfloat y, GLsizei size, GLfloat speed){
	sqr->center_x = x;
	sqr->center_y = y;

	sqr->direction_x = 0.0f;
	sqr->direction_y = 0.0f;

	sqr->rsize = size;
	sqr->speed = speed;	
}

/**
 * Verifica se o Quadrado permanece dentro dos limites da tela
 * @param sqr = quadrado a verificar a posição.
 * @return 1 = Esta nos limites, 0 = Saiu dos limites
 */ 
int checkSquare(square sqr){

	//se atravessou o limite do valor máximo da janela
	if(sqr.center_x - glutGet(GLUT_WINDOW_WIDTH) > sqr.rsize/ 2 ||
			sqr.center_y - glutGet(GLUT_WINDOW_HEIGHT) > sqr.rsize/ 2 )
	{
		return 0;	
	}

	//se atravessou o limite do zero
	else if(sqr.center_x < -sqr.rsize/2 ||
			sqr.center_y < -sqr.rsize/2){
		return 0;
	}

	else
	{
		return 1;
	}

}

/**
 * Função de mover o quadrado. Recalcula a posição e realiza 
 * deslocamento caso ele saia dos limites da tela.
 * 
 * @param sqr = Ponteiro (ou endereço) do quadrado.
 * @return void
 */

void moveSquare(square* sqr){

	int isWithinBounds = checkSquare(*sqr);

	/* se o quadrado saiu dos limites da tela */
	if (!isWithinBounds)
	{
		/* 
		   busca binaria para encontrar a quantidade de passos que o 
		   centro do quadrado deve andar na direção contrária para que
		   apareça no outro extremo da tela
		 */
		int lo = -2e9, hi = -1, mid;
		square sqaux;
		sqaux.rsize = sqr->rsize;
		while(lo < hi)
		{
			mid = (lo + hi) / 2;

			/* tenta voltar 'mid' passos */
			sqaux.center_x = sqr->center_x + mid * sqr->direction_x * sqr->speed;
			sqaux.center_y = sqr->center_y + mid * sqr->direction_y * sqr->speed;

			if (checkSquare(sqaux))
				hi = mid - 1;
			else
				lo = mid;
		}
		lo++;
		/* em 'lo', está a resposta do algoritmo */
		sqr->center_x += lo * sqr->direction_x * sqr->speed;
		sqr->center_y += lo * sqr->direction_y * sqr->speed;
	}
	else
		sqr->center_x += sqr->direction_x*sqr->speed, 
			sqr->center_y += sqr->direction_y*sqr->speed;
}

/***
 * Função que modifica o vetor direção do quadrado.
 * @param sqr = ponteiro para o quadrado, x = posição X do click, y = 
 * posição Y do click
 */

void directSquare(square* sqr, int x, int y){

	float norm;

	/*Calcula a diferença entre o vetor posição do click e o vetor
	 * posição do quadrado, obtendo o vetor direção em relação ao
	 * centro do quadrado
	 */

	x -= sqr->center_x;
	y -= sqr->center_y;

	norm = sqrt(x*x+y*y);

	if(norm > 0){
		//Transforma em um vetor de norma 1
		sqr->direction_x = x/norm;
		sqr->direction_y = y/norm;
	}

}

/*--------------------------------------------------------------------*/
/*DECLARAÇÃO DOS OBJETOS*/
/*--------------------------------------------------------------------*/


square square1;
square square2;
square square3;
square square4;


/*--------------------------------------------------------------------*/
/*FUNÇÕES DO OPENGL*/
/*--------------------------------------------------------------------*/

/**
 * Função de callback para desenhar
 */
void Draw(void) {

	// Define a cor de fundo da cena
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Aplica a cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);	

	glBegin(GL_QUADS);

	/*QUADRADO 1*/
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(square1.center_x - square1.rsize / 2 , square1.center_y - square1.rsize / 2);
	glVertex2f(square1.center_x + square1.rsize / 2 , square1.center_y - square1.rsize / 2);
	glVertex2f(square1.center_x + square1.rsize / 2 , square1.center_y + square1.rsize / 2);
	glVertex2f(square1.center_x - square1.rsize / 2 , square1.center_y + square1.rsize / 2);

	/*QUADRADO 2*/
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(square2.center_x - square2.rsize / 2 , square2.center_y - square2.rsize / 2);
	glVertex2f(square2.center_x + square2.rsize / 2 , square2.center_y - square2.rsize / 2);
	glVertex2f(square2.center_x + square2.rsize / 2 , square2.center_y + square2.rsize / 2);
	glVertex2f(square2.center_x - square2.rsize / 2 , square2.center_y + square2.rsize / 2);

	/*QUADRADO 3*/
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(square3.center_x - square3.rsize / 2 , square3.center_y - square3.rsize / 2);
	glVertex2f(square3.center_x + square3.rsize / 2 , square3.center_y - square3.rsize / 2);
	glVertex2f(square3.center_x + square3.rsize / 2 , square3.center_y + square3.rsize / 2);
	glVertex2f(square3.center_x - square3.rsize / 2 , square3.center_y + square3.rsize / 2);

	/*QUADRADO 4*/
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(square4.center_x - square4.rsize / 2 , square4.center_y - square4.rsize / 2);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(square4.center_x + square4.rsize / 2 , square4.center_y - square4.rsize / 2);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(square4.center_x + square4.rsize / 2 , square4.center_y + square4.rsize / 2);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(square4.center_x - square4.rsize / 2 , square4.center_y + square4.rsize / 2);

	glEnd();


	glutSwapBuffers();
}

/**
 * Função de callback para animação
 */
void Timer(int value)
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	// Move os quadrados    
	moveSquare(&square1);
	moveSquare(&square2);
	moveSquare(&square3);
	moveSquare(&square4);


	// Redesenha os quadrados com as novas coordenadas
	glutPostRedisplay();

	glutTimerFunc(TIME, Timer, 1);
}

/**
 * Função de callback para gerenciar o mouse
 */

void Mouse(int button, int state, int x, int y)
{

	if (button == GLUT_RIGHT_BUTTON || button == GLUT_LEFT_BUTTON)
	{		
		if (checkSquare(square1) + checkSquare(square2) + checkSquare(square3) + checkSquare(square4) == 4) 
		{
			directSquare(&square1, x, y);
			directSquare(&square2, x, y);
			directSquare(&square3, x, y);
			directSquare(&square4, x, y);
		}
	}
}


int main(int argc, char* argv[])
{
	int window_width = 900;
	int window_height = 600;

	initSquare(&square1, 240.0f, 240.0f, 20, 3.0f);
	initSquare(&square2, 270.0f, 240.0f, 30, 1.5f);
	initSquare(&square3, 240.0f, 210.0f, 12, 7.5f);
	initSquare(&square4, 270.0f, 200.0f, 40, 6.0f);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Trabalho 1 CG");

	glutDisplayFunc(Draw);

	glutMouseFunc(Mouse);

	gluOrtho2D(0, window_width, window_height, 0);

	glutTimerFunc(TIME, Timer, 1);

	// Loop da glut
	glutMainLoop();
}

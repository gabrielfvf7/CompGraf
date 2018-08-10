#include <windows.h>  
#include <GL/glut.h>  
#include <math.h>
#include <iostream>	//include das bibliotecas usadas no programa

//Nome: Gabriel Felipe Vargas Ferreira
//DRE: 114184435
using std::cout; //Isso me permitia usar o cout, para printar valores na tela e me ajudar a fazer o programa

float x = 0.0; //definição de x como float para uso posterior
float y = 0.0; //definição de y como float para uso posterior
float verticeX[1000]; //array usado para desenhar, usando o mouse, as coordenadas X 
float verticeY[1000]; //array usado para desenhar, usando o mouse, as coordenadas Y 
float verticePx[1000] = {}; //array para desenhar o polígono após cortar
float verticePy[1000] = {}; //array para desenhar o polígono após cortar
int n = 0; //quantidade de pontos criados com o mouse
int m = 0; //quantidade de pontos criados após cortar as arestas
int a = 4; //variavel auxiliar que usei para definir quando eram as linhas das extremidades

void cortar(int v1, int v2) { //funcao usada para cortar as arestas
	while (v2 < n) { //enquanto o segundo vertice da linha for menor que o numero de pontos, ira realizar as operacoes
		if (v2>1 && v2 < n-1 ) { //forma que encontrei para definir se a linha atual era uma das extremidades
			a = 3; //se nao for uma das extremidades, serão feitos apenas 3 pontos nela
		} else { a = 4; } //caso contrario, sera uma das extremidades, e 4 pontos serao criados
		float moduloX = verticeX[v2] - verticeX[v1]; //calculo o modulo da linha em X
		moduloX = moduloX / 4; //e a divido em 4 partes iguais
		float moduloY = verticeY[v2] - verticeY[v1]; //calculo o modulo da linha em Y
		moduloY = moduloY / 4; //e a divido em 4 partes iguais

		for (int aux = 0; aux < a; aux++) { //for usado para criar os pontos
			if (m == 0) { //se for o primeiro ponto, ele será igual ao criado com o mouse
				verticePx[m] = verticeX[v1];
				verticePy[m] = verticeY[v1];
			}
			else if (aux == 0) { //caso contrario ele seja o primeiro novo ponto da linha atual, será igual a soma do primeiro ponto da linha original com o modulo dela
				verticePx[m] = verticeX[v1] + moduloX;
				verticePy[m] = verticeY[v1] + moduloY;
			}
			else { //e por ultimo, se não for igual a nenhum dos casos, sera igual a soma do ponto anterior com o modulo da linha
				verticePx[m] = verticePx[m - 1] + moduloX;
				verticePy[m] = verticePy[m - 1] + moduloY;
			}
			m++; //incremento a quantidade de pontos criados cortando o poligono
		}
		v1++; v2++; //incremento os vertices originais
	}
	glutPostRedisplay(); //reseto o display
}

void mouse(int button, int state, int X, int Y) { //funcao para pegar os cliques do mouse
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x = (2.*X) / 400 - 1.; //ajusto a coordenada de X
		y = -(2.*Y) / 400 + 1.; //ajusto a coordenada de Y
		verticeX[n] = x; //vertice X será a posição em X do clique do mouse
		verticeY[n] = y; //vertice Y será a posição em Y do clique do mouse
		n++; //incremento a quantidade de pontos criados com clique do mouse
		glutPostRedisplay(); //reseto o display
	}
}

void teclado(unsigned char key, int X, int Y) { //funcao para pegar as teclas do teclado
	switch (key) {
		case 'r': //caso a tecla batida seja r, ira chamar a funcao para cortar as arestas do poligono
			cortar(0, 1); //chamada da funcao
			break;
	}

}

void reset() { //funcao que criei para resetar os estados dos pontos e vertices apos cada corte do poligono
	for (int i = 0; i < m; i++) { //ira ter m iteracoes, onde m é o numero de vertices criados apos cortar o poligono
		verticeX[i] = verticePx[i]; verticePx[i] = 0; //passo os valores dos novos vertices X para o array dos vertices X originais
		verticeY[i] = verticePy[i]; verticePy[i] = 0; //passo os valores dos novos vertices Y para o array dos vertices Y originais
	}
	n = m; //entao, o numero de vertices originais será igual ao de novos
	m = 0; //reseto o numero de vertices novos
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(5);
	glColor3f(0.0, 1.0, 0.0);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP); //inicio do desenho das linhas
	if (m == 0) { //se nao tiverem sido criados novos pontos, ao cortar o poligono, será desenhado os vertices dos cliques do mouse
		for (int i = 0; i < n; i++) { //serao desenhados n pontos, equivalentes aos cliques do mouse
			glVertex2f(verticeX[i], verticeY[i]);
		} //OBS: n só será igual ao numero de cliques do mouse na primeira vez que passar por aqui, nas seguintes
		  //será igual ao numero de pontos criados apos o corte anterior
	}
	else { //se tiverem sido criados novos pontos, mas nao foram ainda desenhados, os desenhara aqui
		for (int i = 0; i < m; i++) { //rodara m vezes, onde m é o numero de novos pontos criados
			glVertex2f(verticePx[i], verticePy[i]);
		}
		reset(); //apos desenhar tudo, chama a funcao para resetar os estados dos vertices e pontos
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Tarefa_1");
	glutMouseFunc(mouse);
	glutKeyboardFunc(teclado);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
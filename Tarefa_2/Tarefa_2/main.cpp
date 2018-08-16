﻿#include <windows.h>  
#include <GL/glut.h>  
#include <math.h>
#include <cmath>
#include <iostream>	//include das bibliotecas usadas no programa

//Nome: Gabriel Felipe Vargas Ferreira
//DRE: 114184435
using std::cout; //Isso me permitia usar o cout, para printar valores na tela e me ajudar a fazer o programa

float x = 0.0; //definicao de x como float para uso posterior
float y = 0.0; //definicao de y como float para uso posterior
float verticeX[1000]; //array usado para desenhar, usando o mouse, as coordenadas X 
float verticeY[1000]; //array usado para desenhar, usando o mouse, as coordenadas Y 
float posicaoPx = 0;  //posicao em X do ponto clicado
float posicaoPy = 0;  //posicao em Y do ponto clicado
int n = 0; //quantidade de pontos criados com o mouse
int i = 0; //variavel usada nos for do programa
int v2 = 0; //ultimo vertice do poligono
bool fechou = false; //variavel booleana usada para saber se o poligono foi fechado
bool clicouPonto = false; //variavel booleana usada pra saber se foi clicado um ponto
int intercepta = 0; //variavel usada para saber quantas vezes a resta ligando o ponto clicado a um fora da regiao intercepta o poligono

float prodVet(float Px, float Py, float Vx, float Vy) { //funcao usada para calcular o produto vetorial
	float prodVet = Px * Vy - Py * Vx; //local onde calculo o produto vetorial
	return prodVet;
}

void fechar() { //funcao usada para fechar o poligno, usei a letra 'r' do teclado para fechar
	float moduloX = verticeX[v2 - 1] - verticeX[0]; //aqui calculo a distancia do ultimo vertice em X do poligono com o primeiro
	float moduloY = verticeY[v2 - 1] - verticeY[0]; //aqui calculo a distancia do ultimo vertice em Y do poligono com o primeiro

	if (abs(moduloX) <= 0.03 && abs(moduloY) <= 0.03) { //se a distancia desse ultimo vertice for suficientemente proxima ao primeiro, eu fecho o poligono, eu defini essa distancia como 0.03, podendo ser mudada
		verticeX[v2 - 1] = verticeX[0]; //aqui coloco a posicao do ultimo vertice em X exatamente igual ao do primeiro
		verticeY[v2 - 1] = verticeY[0]; //aqui coloco a posicao do ultimo vertice em X exatamente igual ao do primeiro
		fechou = true; //defino a variavel fechou como true para indicar que o poligono ja foi fechado
		glutPostRedisplay(); //reseto o display
	}
}

void dentro() { //funcao para descobrir se o ponto clicado pelo mouse esta dentro do poligono, ela eh chamada ao apertar a tecla 'p' do teclado
	intercepta = 0; //como a funcao acabou de ser chamada, a quantidade de vezes que a aresta ligando o ponto a um ponto fora da regiao, intercepta o poligono, eh zero
	float PQx = 1 - posicaoPx; //defino coordenada X da aresta que liga o ponto P e o ponto Q (fora da regiao do poligono)
	float PQy = 1 - posicaoPy; //defino coordenada Y da aresta que liga o ponto P e o ponto Q (fora da regiao do poligono)
	float arestaX; //coordenada X da aresta do poligono a qual estarei verificando se foi interceptada por PQ
	float arestaY; //coordenada Y da aresta do poligono a qual estarei verificando se foi interceptada por PQ
	for (i = 0; i < n-1; i++) { //rodo um for para cada aresta do poligono
		arestaX = verticeX[i + 1] - verticeX[i]; //defino a posicao X da aresta atual
		arestaY = verticeY[i + 1] - verticeY[i]; //defino a posicao Y da aresta atual
		
		//considerando como AB a aresta do ponto clicado com o ponto fora da regiao
		//e CD como a aresta atual do poligono
		//os produtos vetoriais a seguir serao:
		float prodVet1 = prodVet(PQx, PQy, (verticeX[i] - posicaoPx), (verticeY[i] - posicaoPy)); //(AB x AD)
		float prodVet2 = prodVet(PQx, PQy, (verticeX[i + 1] - posicaoPx), (verticeY[i + 1] - posicaoPy));//(AB x AC)
		float prodVet3 = prodVet(arestaX, arestaY, (posicaoPx - verticeX[i]), (posicaoPy - verticeY[i]));//(CD x CA)
		float prodVet4 = prodVet(arestaX, arestaY, (1 - verticeX[i]), (1 - verticeY[i]));//(CD x CB)

		if ((prodVet1 * prodVet2) < 0 && (prodVet3 * prodVet4) < 0) { //aqui verifico se as duas arestas estao se interceptando
			intercepta++; //caso estejam, incremento em um a variavel intercepta
		}
	}

	if (intercepta % 2 == 0) { //se o numero de vezes que intercepta for par, o ponto esta fora do poligono
		cout << "PONTO ESTA FORA DO POLIGONO! \n";
	}
	else { //se o numero de vezes que intercepta for impar, o ponto esta dentro do poligono
		cout << "PONTO ESTA DENTRO DO POLIGONO! \n";
	}
}

void mouse(int button, int state, int X, int Y) { //funcao para pegar os cliques do mouse
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (fechou == false) {
			x = (2.*X) / 400 - 1.; //ajusto a coordenada de X
			y = -(2.*Y) / 400 + 1.; //ajusto a coordenada de Y
			verticeX[n] = x; //vertice X sera a posicao em X do clique do mouse
			verticeY[n] = y; //vertice Y sera a posicao em Y do clique do mouse
			n++; //incremento a quantidade de pontos criados com clique do mouse
			glutPostRedisplay(); //reseto o display
		}
		else if (fechou == true) {
			x = (2.*X) / 400 - 1.; //ajusto a coordenada de X
			y = -(2.*Y) / 400 + 1.; //ajusto a coordenada de Y
			posicaoPx = x; //coordenada X do ponto clicado apos o poligono fechar
			posicaoPy = y; //coordenada Y do ponto clicado apos o poligono fechar
			clicouPonto = true; //atribuo o valor de true a essa variavel apos eu ter clicado um ponto dentro/fora do poligono
			glutPostRedisplay(); //reseto o display
		}
	}
}

void teclado(unsigned char key, int X, int Y) { //funcao para pegar as teclas do teclado
	switch (key) {
	case 'r': //caso seja teclado r, o programa chama a funcao para fechar o poligono
		fechar(); //essa funcao so funciona se o ultimo vertice for suficientemente proximo do primeiro
		break;
	case 'p': //caso seja teclado p, chama a funcao para verificar se o ponto clicado esta dentro do poligono
		dentro();
		break;
	}

}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(5);
	glPointSize(5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP); 
		for (i = 0; i < n; i++) { 
			glVertex2f(verticeX[i], verticeY[i]); //aqui desenho as arestas do poligono
		}
		v2 = i;
	glEnd();
	if (fechou == true && clicouPonto == true) { //se o poligono estiver fechado e ter sido clicado um ponto com o mouse, desenhara esse ponto
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		glVertex2f(posicaoPx, posicaoPy);
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Tarefa_2");
	glutMouseFunc(mouse);
	glutKeyboardFunc(teclado);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
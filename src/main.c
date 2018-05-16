#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pdi.h"

/** Declaração das novas funções **/
void verdeParaCinza(Imagem *in, Imagem *cinza);
void atribuiFundoCinza(Imagem *cinza, Imagem *buffer, Imagem *fundoAmpliado, Imagem *saida);
void atribuiFundoCinzaBordaBorrada (Imagem *cinza, Imagem *buffer, Imagem *fundoAmpliado, Imagem *saida);

int main() {
	//Todas as imagens que serão utilizadas.
	char * imagens[30] = {
		"../imagens/0.bmp",
		"../imagens/1.bmp",
		"../imagens/2.bmp",
		"../imagens/3.bmp",
		"../imagens/4.bmp",
		"../imagens/5.bmp",
		"../imagens/6.bmp",
		"../imagens/7.bmp",
		"../imagens/8.bmp"
	};

	char nome[30];

	//Declaração das imagens auxiliares
	Imagem *in, *buffer, *fundoAmpliado;
	Imagem *cinza, *saida;
	Imagem *fundo = abreImagem("fundo.bmp", 3);

	//Laço para todas as imagens.
	for(int cont = 1; cont <= 9; cont += 1) {
		//Abre a imagem da vez em 3 canais (RGB).
		in 		= abreImagem(imagens[cont - 1], 3);

		sprintf(nome, "../resultados/%d1original.bmp", cont);
		salvaImagem(in, nome);

		//Inicializa imagens auxiliares.
		buffer 	= criaImagem(in->largura, in->altura, in->n_canais);
		cinza 	= criaImagem(in->largura, in->altura, 1);
		fundoAmpliado = criaImagem(in->largura, in->altura, in->n_canais);
		saida 	= criaImagem(in->largura, in->altura, in->n_canais);
		
		//buffer recebe imagem RGB
		copiaConteudo(in, buffer);

		//Transforma in em imagem HSL
		RGBParaHSL(in, in);

		//Redimensiona o fundo com o tamanho da imagem original
		redimensionaNN(fundo, fundoAmpliado);

		//Transforma cada pixel em tonalidade de cinza de acordo com o
		//seu distanciamento da matiz verde: 105
		verdeParaCinza(in, cinza);

		//Imprime a imagem "mapa"
		sprintf(nome, "../resultados/%d2cinza.bmp", cont);
		salvaImagem(cinza, nome);

		//Atribui a imagem final em saida, de acordo com o mapa
		//Se a tonalidade cinza for claro é verde, então recebe fundo
		atribuiFundoCinza(cinza, buffer, fundoAmpliado, saida);
		
		//Imprime a imagem de saida
		sprintf(nome, "../resultados/%d3chroma.bmp", cont);
		salvaImagem(saida, nome);

		//Atribui a imagem final em saida, de acordo com o mapa
		//Se a tonalidade cinza for claro é verde, então recebe fundo
		atribuiFundoCinzaBordaBorrada(cinza, buffer, fundoAmpliado, saida);
		
		//Imprime a imagem de saida
		sprintf(nome, "../resultados/%d4borrado.bmp", cont);
		salvaImagem(saida, nome);
	}

	// Destroi todas as imagens alocadas
	destroiImagem(in);
	destroiImagem(buffer);
	destroiImagem(fundo);
	destroiImagem(fundoAmpliado);
	destroiImagem(cinza);
	destroiImagem(saida);

	return 0;
}

// Para cada pixel, atribui tonalidade escura de acordo com o afastamento da matiz com verde
void verdeParaCinza(Imagem *in, Imagem *cinza){
	for(int y = 0; y < in->altura; y += 1) {
		for(int x = 0; x < in->largura; x += 1) {
			if((in->dados[0][y][x] > 65 && in->dados[0][y][x] < 145) 
					&& (in->dados[1][y][x] > 0.25f) 
					&& (in->dados[2][y][x] > 0.15f && in->dados[2][y][x] < 0.715f))
				cinza->dados[0][y][x] = fabs((in->dados[0][y][x] - abs(105 - in->dados[0][y][x]))
						/(105 + abs(105 - in->dados[0][y][x])));
			else if((in->dados[0][y][x] > 90 && in->dados[0][y][x] < 120) 
					&& (in->dados[1][y][x] > 0.5f)
					&& (in->dados[2][y][x] > 0.715f && in->dados[2][y][x] < 0.85f))
				cinza->dados[0][y][x] = fabs((in->dados[0][y][x] - abs(105 - in->dados[0][y][x]))
						/(105 + abs(105 - in->dados[0][y][x])));
			else
				cinza->dados[0][y][x] = fabs((in->dados[0][y][x] - abs(105 - in->dados[0][y][x]))
						/(255 + abs(105 - in->dados[0][y][x])));
		}
	}	
}

// Se o pixel for escuro atribui a imagem. Se não, atribui fundo
void atribuiFundoCinza(Imagem *cinza, Imagem *buffer, Imagem *fundoAmpliado, Imagem *saida){
	for(int y = 0; y < cinza->altura; y += 1) {
		for(int x = 0; x < cinza->largura; x += 1) {
			if(cinza->dados[0][y][x] > 0.5f) {
				saida->dados[0][y][x] = fundoAmpliado->dados[0][y][x];
				saida->dados[1][y][x] = fundoAmpliado->dados[1][y][x];
				saida->dados[2][y][x] = fundoAmpliado->dados[2][y][x];
			}		
			else { 
				saida->dados[0][y][x] = buffer->dados[0][y][x];
				saida->dados[1][y][x] = buffer->dados[1][y][x];
				saida->dados[2][y][x] = buffer->dados[2][y][x];
			}		
		}
	}		
}

void atribuiFundoCinzaBordaBorrada (Imagem *cinza, Imagem *buffer, Imagem *fundoAmpliado, Imagem *saida) {
	int cor = 0;
	int verde = 0;
	float somatorioR = 0;
	float somatorioG = 0;
	float somatorioB = 0;

	for(int y = 1; y < cinza->altura - 1; y += 1) {
		for(int x = 1; x < cinza->largura - 1; x += 1) {
			cor = 0;
			verde = 0;
			somatorioR = 0;
			somatorioG = 0;
			somatorioB = 0;
			for(int j = y - 1; j <= y + 1; j += 1) {
				for(int i = x - 1; i <= x + 1; i += 1) {
					if(cinza->dados[0][j][i] > 0.5f) {
						verde += 1;
						somatorioR += fundoAmpliado->dados[0][j][i];
						somatorioG += fundoAmpliado->dados[1][j][i];
						somatorioB += fundoAmpliado->dados[2][j][i];
					}
					else {
						cor += 1;
						somatorioR += buffer->dados[0][j][i];
						somatorioG += buffer->dados[1][j][i];
						somatorioB += buffer->dados[2][j][i];						
					}
				}
			}
			if (verde == 9) {
				saida->dados[0][y][x] = fundoAmpliado->dados[0][y][x];
				saida->dados[1][y][x] = fundoAmpliado->dados[1][y][x];
				saida->dados[2][y][x] = fundoAmpliado->dados[2][y][x];
			}
			else if(cor == 9){
				saida->dados[0][y][x] = buffer->dados[0][y][x];
				saida->dados[1][y][x] = buffer->dados[1][y][x];
				saida->dados[2][y][x] = buffer->dados[2][y][x];
			}
			else {				
				saida->dados[0][y][x] = somatorioR/9;
				saida->dados[1][y][x] = somatorioG/9;
				saida->dados[2][y][x] = somatorioB/9;
			}
		}
	}
}

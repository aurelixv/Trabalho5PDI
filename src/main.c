#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pdi.h"

void verdeParaCinza(Imagem *in, Imagem *cinza);
void atribuiFundoCinza(Imagem *cinza, Imagem *buffer, Imagem *fundoAmpliado, Imagem *saida);

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

	Imagem *in, *buffer, *fundoAmpliado;
	Imagem *cinza, *saida;
	Imagem *fundo = abreImagem("fundo.bmp", 3);

	//Laço para todas as imagens.
	for(int cont = 0; cont < 9; cont += 1) {
		//Abre a imagem da vez em 3 canais (RGB).
		in 		= abreImagem(imagens[cont], 3);
		//Inicializa imagens auxiliares.
		buffer 	= criaImagem(in->largura, in->altura, in->n_canais);
		cinza 	= criaImagem(in->largura, in->altura, 1);
		fundoAmpliado = criaImagem(in->largura, in->altura, in->n_canais);
		saida 	= criaImagem(in->largura, in->altura, in->n_canais);
				
		copiaConteudo(in, buffer);
		RGBParaHSL(in, in);

		redimensionaNN(fundo, fundoAmpliado);

		verdeParaCinza(in, cinza);

		sprintf(nome, "../resultados/%dcinza.bmp", cont);
		salvaImagem(cinza, nome);

		atribuiFundoCinza(cinza, buffer, fundoAmpliado, saida);
		
		sprintf(nome, "../resultados/%dcinzaTeste.bmp", cont);
		salvaImagem(saida, nome);
	}

	destroiImagem(in);
	destroiImagem(buffer);
	destroiImagem(fundo);
	destroiImagem(fundoAmpliado);
	destroiImagem(cinza);
	destroiImagem(saida);

	return 0;
}

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
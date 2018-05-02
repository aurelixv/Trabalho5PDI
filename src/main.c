#include <stdio.h>
#include <stdlib.h>
#include "pdi.h"

#define threshold 0.6f

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

	Imagem *in, *out, *buffer, *hsl;

	//Laço para todas as imagens.
	for(int cont = 0; cont < 9; cont += 1) {
		//Abre a imagem da vez em 3 canais (RGB).
		in 		= abreImagem(imagens[cont], 3);
		//Inicializa imagens auxiliares.
		out 	= criaImagem(in->largura, in->altura, in->n_canais);
		buffer 	= criaImagem(in->largura, in->altura, in->n_canais);
		hsl 	= criaImagem(in->largura, in->altura, in->n_canais);
		
		RGBParaHSL(in, buffer);
		ajustaHSL(buffer, hsl, 0, 0, 0);

		//Percorre cada pixel da imagem.
		for(int y = 1; y < in->altura-1; y += 1) {
			for(int x = 1; x < in->largura-1; x += 1) {
				if(cont == 1 || cont == 3){
					if(hsl->dados[0][y][x] > 100 && hsl->dados[0][y][x] < 130 && (
							(hsl->dados[0][y-1][x] > 100 && hsl->dados[0][y-1][x] < 130) ||
							(hsl->dados[0][y+1][x] > 100 && hsl->dados[0][y+1][x] < 130) ||
							(hsl->dados[0][y+1][x-1] > 100 && hsl->dados[0][y+1][x-1] < 130) ||
							(hsl->dados[0][y+1][x+1] > 100 && hsl->dados[0][y+1][x+1] < 130) ||
							(hsl->dados[0][y-1][x-1] > 100 && hsl->dados[0][y-1][x-1] < 130) ||
							(hsl->dados[0][y-1][x+1] > 100 && hsl->dados[0][y-1][x+1] < 130) ||
							(hsl->dados[0][y][x-1] > 100 && hsl->dados[0][y][x-1] < 130) ||
							(hsl->dados[0][y][x+1] > 100 && hsl->dados[0][y][x+1] < 130))) {
						out->dados[0][y][x] = 0.5f;
						out->dados[1][y][x] = 0.5f;
						out->dados[2][y][x] = 0.5f;
					}
					else {
						out->dados[0][y][x] = in->dados[0][y][x];
						out->dados[1][y][x] = in->dados[1][y][x];
						out->dados[2][y][x] = in->dados[2][y][x];
					}
				}
				else{
					if(hsl->dados[0][y][x] > 95 && hsl->dados[0][y][x] < 138 && (
							(hsl->dados[0][y-1][x] > 95 && hsl->dados[0][y-1][x] < 138) ||
							(hsl->dados[0][y+1][x] > 95 && hsl->dados[0][y+1][x] < 138) ||
							(hsl->dados[0][y+1][x-1] > 95 && hsl->dados[0][y+1][x-1] < 138) ||
							(hsl->dados[0][y+1][x+1] > 95 && hsl->dados[0][y+1][x+1] < 138) ||
							(hsl->dados[0][y-1][x-1] > 95 && hsl->dados[0][y-1][x-1] < 138) ||
							(hsl->dados[0][y-1][x+1] > 95 && hsl->dados[0][y-1][x+1] < 138) ||
							(hsl->dados[0][y][x-1] > 95 && hsl->dados[0][y][x-1] < 138) ||
							(hsl->dados[0][y][x+1] > 95 && hsl->dados[0][y][x+1] < 138))) {
						out->dados[0][y][x] = 0.5f;
						out->dados[1][y][x] = 0.5f;
						out->dados[2][y][x] = 0.5f;
					}
					else {
						out->dados[0][y][x] = in->dados[0][y][x];
						out->dados[1][y][x] = in->dados[1][y][x];
						out->dados[2][y][x] = in->dados[2][y][x];
					}
				}
			}
		}
		sprintf(nome, "../resultados/teste%d.bmp", cont);
		salvaImagem(out, nome);
	}

	destroiImagem(in);
	destroiImagem(out);
	destroiImagem(buffer);
	destroiImagem(hsl);

	return 0;
}

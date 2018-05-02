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

	Imagem *in, *out, *buffer, *fundoAmpliado;
	Imagem *fundo = abreImagem("fundo.bmp", 3);

	//Laço para todas as imagens.
	for(int cont = 0; cont < 9; cont += 1) {
		//Abre a imagem da vez em 3 canais (RGB).
		in 		= abreImagem(imagens[cont], 3);
		//Inicializa imagens auxiliares.
		out 	= criaImagem(in->largura, in->altura, in->n_canais);
		buffer 	= criaImagem(in->largura, in->altura, in->n_canais);
		fundoAmpliado = criaImagem(in->largura, in->altura, in->n_canais);
		copiaConteudo(in, buffer);
		redimensionaNN(fundo, fundoAmpliado);
		RGBParaHSL(in, in);	

		int flag = 1;	
		
		//Percorre cada pixel da imagem.
		for(int y = 1; y < in->altura - 1; y += 1) {
			for(int x = 1; x < in->largura - 1; x += 1) {
				/* printf("Pixel %d\n", x * y);
				printf("R: %.2f", in->dados[0][y][x]);
				printf("G: %.2f", in->dados[1][y][x]);
				printf("B: %.2f\n", in->dados[2][y][x]); */

				/* if(in->dados[0][y][x] < 0.3f
					&& in->dados[1][y][x] > 0.4f
					&& in->dados[2][y][x] < 0.3f) {
						out->dados[0][y][x] = 0.0f;
						out->dados[1][y][x] = 0.0f;
						out->dados[2][y][x] = 0.0f;
				}
				else {
					out->dados[0][y][x] = in->dados[0][y][x];
					out->dados[1][y][x] = in->dados[1][y][x];
					out->dados[2][y][x] = in->dados[2][y][x];
				} */

				/* if(in->dados[1][y][x] / (in->dados[0][y][x] + in->dados[2][y][x]) > 1) {
					out->dados[0][y][x] = 0.0f;
					out->dados[1][y][x] = 0.0f;
					out->dados[2][y][x] = 0.0f;
				}
				else {
					out->dados[0][y][x] = in->dados[0][y][x];
					out->dados[1][y][x] = in->dados[1][y][x];
					out->dados[2][y][x] = in->dados[2][y][x];
				} */								

				int verde = 0;
				int cor	  = 0;

				float somatorioR = 0;
				float somatorioG = 0;
				float somatorioB = 0;


				for(int j = y - 1; j <= y + 1; j += 1) {
					for(int i = x - 1; i <= x + 1; i += 1) {						
						if(!((in->dados[0][j][i] > 65 && in->dados[0][j][i] < 145) &&
							(in->dados[1][j][i] > 0.25f) &&
							(in->dados[2][j][i] > 0.15f && in->dados[2][j][i] < 0.85f))) {
							cor += 1;
							somatorioR += buffer->dados[0][j][i];
							somatorioG += buffer->dados[1][j][i];
							somatorioB += buffer->dados[2][j][i];
						}
						else {							
							verde += 1;
							somatorioR += fundoAmpliado->dados[0][j][i];
							somatorioG += fundoAmpliado->dados[1][j][i];
							somatorioB += fundoAmpliado->dados[2][j][i];
						}
					}		
				}
				
				
				if (verde == 9) {
					out->dados[0][y][x] = fundoAmpliado->dados[0][y][x];
					out->dados[1][y][x] = fundoAmpliado->dados[1][y][x];
					out->dados[2][y][x] = fundoAmpliado->dados[2][y][x];					
				}
				else if(cor == 9){
					out->dados[0][y][x] = buffer->dados[0][y][x];
					out->dados[1][y][x] = buffer->dados[1][y][x];
					out->dados[2][y][x] = buffer->dados[2][y][x];					
				}
				else {
					out->dados[0][y][x] = somatorioR/9;
					out->dados[1][y][x] = somatorioG/9;
					out->dados[2][y][x] = somatorioB/9;
				}

				/* printf("Pixel %d\n", x * y);
				printf("H: %.2f", in->dados[0][y][x]);
				printf("S: %.2f", in->dados[1][y][x]);
				printf("L: %.2f\n", in->dados[2][y][x]); */
				
			}
		}		

		sprintf(nome, "../resultados/teste%d.bmp", cont);
		salvaImagem(out, nome);
	}

	destroiImagem(in);
	destroiImagem(out);
	destroiImagem(buffer);

	return 0;
}

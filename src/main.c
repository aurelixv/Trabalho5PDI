#include <stdio.h>
#include <stdlib.h>
#include "pdi.h"

void binarizaVerde(Imagem *in, Imagem *bin);
void atribuiFundo(Imagem *in, Imagem *buffer, Imagem *fundoAmpliado, Imagem *out);
void atribuiFundoBin(Imagem *bin, Imagem *buffer, Imagem *fundoAmpliado);

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

	Imagem *in, *out, *buffer, *fundoAmpliado, *bin;
	Imagem *fundo = abreImagem("fundo.bmp", 3);

	//Laço para todas as imagens.
	for(int cont = 0; cont < 9; cont += 1) {
		//Abre a imagem da vez em 3 canais (RGB).
		in 		= abreImagem(imagens[cont], 3);
		//Inicializa imagens auxiliares.
		out 	= criaImagem(in->largura, in->altura, in->n_canais);
		buffer 	= criaImagem(in->largura, in->altura, in->n_canais);
		bin 	= criaImagem(in->largura, in->altura, in->n_canais);
		fundoAmpliado = criaImagem(in->largura, in->altura, in->n_canais);
		copiaConteudo(in, buffer);
		
		copiaConteudo(in, bin);
		
		redimensionaNN(fundo, fundoAmpliado);
		RGBParaHSL(in, in);

		binarizaVerde(in, bin);

		atribuiFundoBin(bin, buffer, fundoAmpliado);
		
		sprintf(nome, "../resultados/binTeste%d.bmp", cont);
		salvaImagem(bin, nome);

		//Percorre cada pixel da imagem.
		atribuiFundo(in, buffer, fundoAmpliado, out);

		sprintf(nome, "../resultados/teste%d.bmp", cont);
		salvaImagem(out, nome);
	}

	destroiImagem(in);
	destroiImagem(out);
	destroiImagem(buffer);
	destroiImagem(bin);
	destroiImagem(fundoAmpliado);

	return 0;
}

void atribuiFundo(Imagem *in, Imagem *buffer, Imagem *fundoAmpliado, Imagem *out){
	int verde, cor;
	float somatorioR, somatorioG, somatorioB;

	for(int y = 1; y < in->altura - 1; y += 1) {
		for(int x = 1; x < in->largura - 1; x += 1) {								
			verde = 0;
			cor	  = 0;	
			somatorioR = 0;
			somatorioG = 0;
			somatorioB = 0;
			for(int j = y - 1; j <= y + 1; j += 1) {
				for(int i = x - 1; i <= x + 1; i += 1) {						
					if(!((in->dados[0][j][i] > 65 && in->dados[0][j][i] < 145) &&
						(in->dados[1][j][i] > 0.25f) &&
						(in->dados[2][j][i] > 0.15f && in->dados[2][j][i] < 0.715f))) {
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
		}
	}
}

void binarizaVerde(Imagem *in, Imagem *bin){
	for(int y = 1; y < in->altura - 1; y += 1) {
		for(int x = 1; x < in->largura - 1; x += 1) {

			for(int j = y - 1; j <= y + 1; j += 1) {
				for(int i = x - 1; i <= x + 1; i += 1) {

					if(!((in->dados[0][j][i] > 65 && in->dados[0][j][i] < 145) &&
						(in->dados[1][j][i] > 0.25f) &&
						(in->dados[2][j][i] > 0.15f && in->dados[2][j][i] < 0.715f))) {
						bin->dados[0][j][i] = 0.0f;
						bin->dados[1][j][i] = 0.0f;
						bin->dados[2][j][i] = 0.0f;
					}
					else {
						bin->dados[0][j][i] = 1.0f;
						bin->dados[1][j][i] = 1.0f;
						bin->dados[2][j][i] = 1.0f;
					}
				}		
			}
		}
	}
}

void atribuiFundoBin(Imagem *bin, Imagem *buffer, Imagem *fundoAmpliado){

	for(int y = 0; y < bin->altura; y += 1) {
		for(int x = 0; x < bin->largura; x += 1) {
			if(bin->dados[0][y][x] == 1.0f && bin->dados[1][y][x] == 1.0f &&
					bin->dados[2][y][x] == 1.0f) {
				bin->dados[0][y][x] = fundoAmpliado->dados[0][y][x];
				bin->dados[1][y][x] = fundoAmpliado->dados[1][y][x];
				bin->dados[2][y][x] = fundoAmpliado->dados[2][y][x];
			}
			else {							
				bin->dados[0][y][x] = buffer->dados[0][y][x];
				bin->dados[1][y][x] = buffer->dados[1][y][x];
				bin->dados[2][y][x] = buffer->dados[2][y][x];
			}				
		}
	}		
}
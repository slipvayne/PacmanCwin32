#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4995)
#pragma warning(disable : 4133)
#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#include <stdlib.h>
#include <aclapi.h>
#include <time.h>
#include <ctype.h>
#include "Servidor.h"

#define PIPE_NAME "\\\\.\\pipe\\xpacman"
#define PIPE_NAME_GHOSTS "\\\\.\\pipe\\xghosts"
#define PIPE_NAME_PACS "\\\\.\\pipe\\xpacs"
#define PIPE_NAME_BONUS "\\\\.\\pipe\\xbonus"
#define PIPE_NAME_PAC "\\\\.\\pipe\\xpac"




DWORD WINAPI ActualizarFantasmas(LPVOID param)
{
	
	int i,z,random,contador=0,posOcupada=0;
	int Xpossivel,Ypossivel,POSpossivel;
	int jogo=(int)(param);
	DWORD n;

	srand ((unsigned int) time(NULL) );

	for(i=0; i < fantasmasTotal[jogo]; i++)
	{
	if(fantasmasTotal[jogo]==3){ //se forem 3 (default)
	fantasmas[jogo][0].bitmap=IDB_gVermelho; fantasmas[jogo][0].x = 320; fantasmas[jogo][0].y = 240; //VERMELHO
	fantasmas[jogo][1].bitmap=IDB_gRosa; fantasmas[jogo][1].x = 360; fantasmas[jogo][1].y = 240; //ROSA
	fantasmas[jogo][2].bitmap=IDB_gVerde; fantasmas[jogo][2].x = 400; fantasmas[jogo][2].y = 240; //VERDE
	break;}

	random = rand() % 3;
	if(random==0)
	{
		fantasmas[jogo][i].bitmap=IDB_gVermelho; fantasmas[jogo][i].x = 320; fantasmas[jogo][i].y = 240; //VERMELHO
	}
	if(random==1){
		fantasmas[jogo][i].bitmap=IDB_gRosa; fantasmas[jogo][i].x = 360; fantasmas[jogo][i].y = 240; //ROSA
	}
	if(random==2){
		fantasmas[jogo][i].bitmap=IDB_gVerde; fantasmas[jogo][i].x = 400; fantasmas[jogo][i].y = 240; //VERDE
	}
	}
	
	while(1)
	{
		if(jogoON[jogo]==0) { ExitThread(1); }	

		Sleep(300);


		WaitForSingleObject(hMutex1,INFINITE); 

				
		for(i=1 ; i <= totalJogadores[jogo] ; i++)
		{
	
			if (!WriteFile(hPipeFantasmas[jogo][i], &fantasmasTotal[jogo], sizeof(int), &n, NULL)) // envia numero de fantasmas
			{  
				//printf("[GHOSTS-erro] Erro a escrever no pipe (%d)...o numero de fantasmas![1]\n",i);
			}
			
		}

	//	ResetEvent(hVerificaPosicao); //bloqueia tudo ao verificar posicao

			for(i=0 ; i < fantasmasTotal[jogo] ;)
			{

				for(z=0 ; z < fantasmasTotal[jogo] ; z++) //verificar sempre as posicoes, podem-se utilizar as mm variaveis...
				{
					Xpossivel = fantasmas[jogo][z].x / 40;
					Ypossivel = fantasmas[jogo][z].y / 40;
					POSpossivel = Xpossivel + (Ypossivel*20); 

					if(mapa[jogo][POSpossivel].jogador>=1 && mapa[jogo][POSpossivel].jogador<4) { printf("\n---------[GHOST-KILL] Jogador %d DEAD!---------\n",mapa[jogo][POSpossivel].jogador); pacupdates[jogo][mapa[jogo][POSpossivel].jogador].emJogo=0; pacupdates[jogo][mapa[jogo][POSpossivel].jogador].bitmap=IDB_pacDead; mapa[jogo][POSpossivel].bitmap=IDB_pacDead; }
				}


				random = rand() % 4; // gerar um num aleatorio entre 0 e 3

				posOcupada=0;

				switch(random)
				{
					case 0:  // Ghost up

						Xpossivel = fantasmas[jogo][i].x / 40;
						Ypossivel = (fantasmas[jogo][i].y - 40) / 40;
						POSpossivel = Xpossivel + (Ypossivel*20); 

						for(z=0 ; z < fantasmasTotal[jogo] ; z++)
						{
						if(Xpossivel==fantasmas[jogo][z].x && Ypossivel==fantasmas[jogo][z].y) { 
							printf("[GHOSTS-POSICAO] Ja existe um fantasma nessa posicao! \n"); 
							posOcupada=1; 
							break;
						}
						}

						if((mapa[jogo][POSpossivel].bitmap==IDB_pCaminho || mapa[jogo][POSpossivel].bitmap==IDB_Bola || mapa[jogo][POSpossivel].bitmap==IDB_pacBaixo
							|| mapa[jogo][POSpossivel].bitmap==IDB_pacCima || mapa[jogo][POSpossivel].bitmap==IDB_pacDir || mapa[jogo][POSpossivel].bitmap==IDB_pacEsq || mapa[jogo][POSpossivel].bitmap==IDB_pSaida || mapa[jogo][POSpossivel].bitmap==IDB_pacDead) && posOcupada==0){

							if(mapa[jogo][POSpossivel].bitmap==IDB_Bola) { fantasmas[jogo][i].tipo = 1; }else
								if(mapa[jogo][POSpossivel].bitmap==IDB_pacDead) { fantasmas[jogo][i].tipo= 2;} 
								else { fantasmas[jogo][i].tipo= 0;}  //se 1 tinha bola antes, senao caminho

							fantasmas[jogo][i].y = fantasmas[jogo][i].y - 40;
					
							i++;
								if(mapa[jogo][POSpossivel].jogador>=1 && mapa[jogo][POSpossivel].jogador<4) { printf("\n---------[GHOST-KILL] Jogador %d DEAD!---------\n",mapa[jogo][POSpossivel].jogador); pacupdates[jogo][mapa[jogo][POSpossivel].jogador].emJogo=0; pacupdates[jogo][mapa[jogo][POSpossivel].jogador].bitmap=IDB_pacDead; mapa[jogo][POSpossivel].bitmap=IDB_pacDead; }
								// && .emjogo!!!
						}
							
					break;

					case 1: // Ghost Down

						Xpossivel = fantasmas[jogo][i].x / 40;
						Ypossivel = (fantasmas[jogo][i].y + 40) / 40;
						POSpossivel = Xpossivel + (Ypossivel*20); 

						for(z=0 ; z < fantasmasTotal[jogo] ; z++)
						{
							if(Xpossivel==fantasmas[jogo][z].x && Ypossivel==fantasmas[jogo][z].y) { 
								printf("[GHOSTS-POSICAO] Ja existe um fantasma nessa posicao! \n"); 
								posOcupada=1; 
								break;
							}
						}

						if((mapa[jogo][POSpossivel].bitmap==IDB_pCaminho || mapa[jogo][POSpossivel].bitmap==IDB_Bola || mapa[jogo][POSpossivel].bitmap==IDB_pacBaixo || 
							 mapa[jogo][POSpossivel].bitmap==IDB_pacCima || mapa[jogo][POSpossivel].bitmap==IDB_pacDir || mapa[jogo][POSpossivel].bitmap==IDB_pacEsq || mapa[jogo][POSpossivel].bitmap==IDB_pSaida || mapa[jogo][POSpossivel].bitmap==IDB_pacDead)  && posOcupada==0){

							if(mapa[jogo][POSpossivel].bitmap==IDB_Bola) { fantasmas[jogo][i].tipo = 1; }else
								if(mapa[jogo][POSpossivel].bitmap==IDB_pacDead) { fantasmas[jogo][i].tipo= 2;}
								else { fantasmas[jogo][i].tipo= 0;} 

							fantasmas[jogo][i].y = fantasmas[jogo][i].y + 40;	
				
							i++;
								if(mapa[jogo][POSpossivel].jogador>=1 && mapa[jogo][POSpossivel].jogador<4) { printf("\n---------[GHOST-KILL] Jogador %d DEAD!---------\n",mapa[jogo][POSpossivel].jogador); pacupdates[jogo][mapa[jogo][POSpossivel].jogador].emJogo=0; pacupdates[jogo][mapa[jogo][POSpossivel].jogador].bitmap=IDB_pacDead; mapa[jogo][POSpossivel].bitmap=IDB_pacDead; }

						}
												
					break;

					case 2: // Ghost right

				
						Xpossivel = (fantasmas[jogo][i].x + 40) / 40;
						Ypossivel = fantasmas[jogo][i].y / 40;
						POSpossivel = Xpossivel + (Ypossivel*20); 

						for(z=0 ; z < fantasmasTotal[jogo] ; z++)
						{
							if(Xpossivel==fantasmas[jogo][z].x && Ypossivel==fantasmas[jogo][z].y) { 
								printf("[GHOSTS-POSICAO] Ja existe um fantasma nessa posicao! \n"); 
								posOcupada=1; 
								break;
							}
						}

						if((mapa[jogo][POSpossivel].bitmap==IDB_pCaminho || mapa[jogo][POSpossivel].bitmap==IDB_Bola || mapa[jogo][POSpossivel].bitmap==IDB_pacBaixo || 
							mapa[jogo][POSpossivel].bitmap==IDB_pacCima || mapa[jogo][POSpossivel].bitmap==IDB_pacDir || mapa[jogo][POSpossivel].bitmap==IDB_pacEsq || mapa[jogo][POSpossivel].bitmap==IDB_pSaida || mapa[jogo][POSpossivel].bitmap==IDB_pacDead)  && posOcupada==0){

							if(mapa[jogo][POSpossivel].bitmap==IDB_Bola) { fantasmas[jogo][i].tipo = 1; }
							else if(mapa[jogo][POSpossivel].bitmap==IDB_pacDead) { fantasmas[jogo][i].tipo= 2;} 
							else { fantasmas[jogo][i].tipo= 0;} 

							fantasmas[jogo][i].x = fantasmas[jogo][i].x + 40;	
					
							i++;
								if(mapa[jogo][POSpossivel].jogador>=1 && mapa[jogo][POSpossivel].jogador<4) { printf("\n---------[GHOST-KILL] Jogador %d DEAD!---------\n",mapa[jogo][POSpossivel].jogador); pacupdates[jogo][mapa[jogo][POSpossivel].jogador].emJogo=0; pacupdates[jogo][mapa[jogo][POSpossivel].jogador].bitmap=IDB_pacDead; mapa[jogo][POSpossivel].bitmap=IDB_pacDead; }

						}

											
					break;

					case 3: // Ghost Left

				
						Xpossivel = (fantasmas[jogo][i].x - 40) / 40;
						Ypossivel = fantasmas[jogo][i].y / 40;
						POSpossivel = Xpossivel + (Ypossivel*20); 

						for(z=0 ; z < fantasmasTotal[jogo] ; z++)
						{
							if(Xpossivel==fantasmas[jogo][z].x && Ypossivel==fantasmas[jogo][z].y) { 
								printf("[GHOSTS-POSICAO] Ja existe um fantasma nessa posicao! \n"); 
								posOcupada=1; 
								break;
							}
						}

						if((mapa[jogo][POSpossivel].bitmap==IDB_pCaminho || mapa[jogo][POSpossivel].bitmap==IDB_Bola || mapa[jogo][POSpossivel].bitmap==IDB_pacBaixo || 
							mapa[jogo][POSpossivel].bitmap==IDB_pacCima || mapa[jogo][POSpossivel].bitmap==IDB_pacDir || mapa[jogo][POSpossivel].bitmap==IDB_pacEsq || mapa[jogo][POSpossivel].bitmap==IDB_pSaida || mapa[jogo][POSpossivel].bitmap==IDB_pacDead)  && posOcupada==0){

							if(mapa[jogo][POSpossivel].bitmap==IDB_Bola) { fantasmas[jogo][i].tipo = 1; }else
								if(mapa[jogo][POSpossivel].bitmap==IDB_pacDead) { fantasmas[jogo][i].tipo= 2;}
								else { fantasmas[jogo][i].tipo= 0;} 

							fantasmas[jogo][i].x = fantasmas[jogo][i].x - 40;	
					
							i++;
							if(mapa[jogo][POSpossivel].jogador>=1 && mapa[jogo][POSpossivel].jogador<4) { printf("\n---------[GHOST-KILL] Jogador %d DEAD!---------\n",mapa[jogo][POSpossivel].jogador); pacupdates[jogo][mapa[jogo][POSpossivel].jogador].emJogo=0; pacupdates[jogo][mapa[jogo][POSpossivel].jogador].bitmap=IDB_pacDead; mapa[jogo][POSpossivel].bitmap=IDB_pacDead; }

					}

													
					break;
				}
			}

		

		for(i=0; i < 280 ; i++)
		{
			mapa[jogo][i].jogador=0; //reset posicao dos jogadores apos verificar se nao foi morto pelo fantasma
		}

	//	SetEvent(hVerificaPosicao);

		
			for(i=1; i <= totalJogadores[jogo] ; i++)
			{

					if (!WriteFile(hPipeFantasmas[jogo][i], &fantasmas[jogo], sizeof(MAP)*20, &n, NULL)) 
					{  
						//printf("[GHOSTS-erro] Erro a escrever no pipe (%d)...os fantasmas todos![2]\n",i);				
	
					}

	    	}		


			ReleaseMutex(hMutex1); 
					
		
	}
	return 0;
}
/////////////////////////////////////////////////////////

DWORD WINAPI ActualizarPacs(LPVOID param)
{
	int i,conta=0;
	DWORD n;
	int jogo=(int)(param);


	while(1)
	{
		

		conta=0;

		Sleep(75);

		for(i=1 ; i <= totalJogadores[jogo] ; i++)
		{

			if(pacupdates[jogo][i].emJogo==1){
				conta++;
			}					
				
		}


		if(conta==0){ 
			printf("\n------\n O JOGO (%d) ACABOU! TODOS OS PACS FORAM MORTOS!\n---------\n",jogo);
			jogoON[jogo]=0;			
		}
	

		for(i=1 ; i <= totalJogadores[jogo] ; i++)
		{
		
			if (!WriteFile(hPipePacs[jogo][i], &totalJogadores[jogo], sizeof(int), &n, NULL)) // envia numero de fantasmas
			{  
				// printf("[PACS-erro] Erro a escrever no pipe (%d)...o numero dos pacs![1]\n",i);
			}

		}


		for(i=1 ; i <= totalJogadores[jogo] ; i++)
		{
			if(jogoON[jogo]==0){ pacupdates[jogo][i].clientJogoON=-1; }

			if (!WriteFile(hPipePacs[jogo][i], &pacupdates[jogo], sizeof(PACMAN)*4, &n, NULL)) // envia numero de fantasmas
			{  
				// printf("[PACS-erro] Erro a escrever no pipe (%d)...a info dos pacs![2]\n",i);
			}
		}

		if(jogoON[jogo]==0) { ExitThread(1); }	

	}
	return 0;
}

/////////////////////////////////////////////////////////


DWORD WINAPI VerificaBonus(LPVOID param)
{
	int i,conta=0;
	int jogo=(int)(param);


	while(1)
	{
		if(jogoON[jogo]==0) { ExitThread(1); return 0;}	

		conta=0;

		Sleep(2500);

		WaitForSingleObject(hMutex1,INFINITE); 

	
		for(i=0 ; i <= 280 ; i++)
		{
			if(mapa[jogo][i].bitmap==IDB_Bola)
				conta++;
		}
		//printf("\nFaltam apanhar %d bolas!",conta);

		if(conta==0){ 
			printf("\n--------\n Todos os bonus foram consumidos!..O jogo (%d) acabou!\n------\n",jogo);
			jogoON[jogo]=0;
			ExitThread(1); return 0;
		}


		ReleaseMutex(hMutex1); 
	}
	return 0;
	
}

//Ler txt com a info de utilizadores
JOGADOR LerJogadores()
{

	FILE *f;
	int i;	

	strcpy(MSGSCORES,"");

	fopen_s(&f,"jogadores.txt","rt");

	if(f == NULL)
	{
		printf( "Erro ao ler ficheiro de jogadores\n");
		return *jogadores;
	}

			
	fscanf_s(f, "%d",&numJogadoresRegistados); 

	printf("Jogadores Registados:\n");

	for(i=0; i < numJogadoresRegistados ; i++)
	{

		fscanf(f, "%s %s %d\n %d\n",jogadores[i].utilizador,jogadores[i].password,&jogadores[i].pontuacaoTotal,&jogadores[i].jogosTotal); 
		jogadores[i].logado=0;

		printf("%s - %s\n",jogadores[i].utilizador,jogadores[i].password);

		sprintf(MSGSCORES,"%s\r\n\tJogador: %s\r\nPontuacao Total: %d \r\nJogos Total: %d\r\n\r\n",MSGSCORES,jogadores[i].utilizador,jogadores[i].pontuacaoTotal,jogadores[i].jogosTotal); 
	
	}	


	fclose(f);

	return *jogadores;
}

void ESCREVEJogadores()
{

	FILE *f;
	int i;

	fopen_s(&f,"jogadores.txt","wt");

	if(f == NULL)
	{
		printf( "Erro ao abrir ficheiro para escrever jogadores!\n");
		return;
	}

			fprintf(f,"%d\n",numJogadoresRegistados); 
			for(i=0; i < numJogadoresRegistados ; i++)
			{
				fprintf(f,"%s\n",jogadores[i].utilizador); 
				fprintf(f,"%s\n",jogadores[i].password);
				fprintf(f,"%d\n",jogadores[i].pontuacaoTotal);
				fprintf(f,"%d\n",jogadores[i].jogosTotal);
			}

			fclose(f);
}


/////////////////////////////////////////////////////////

DWORD WINAPI AtendeCliente1(LPVOID param)
{
		
		char MSG[256];
		DWORD n;
		HANDLE hPipe = (HANDLE)param;
		int i,ret,id;
		int cliente = totalJogadoresTOTAL;
		DADOS login;

		id=GetCurrentThreadId();

		LerJogadores();

	
		do{	
			
			ret = ReadFile(hPipe, &login, sizeof(login), &n, NULL);   
			if (!ret || !n)
			{
				printf("[%d LOGIN] Erro ao ler dados [FIRST 1]\n",id);
				
				return -1;
			}
			
			while(login.COD==1) //SE O CLIENT QUISER CRIAR NOVA CONTA...
			{
				strcpy(MSG,"");

				printf("[%d NOVA CONTA] Username: %s e Pass: %s \n",id,login.utilizador,login.password);

				for(i=0; i < numJogadoresRegistados ; i++)
				{
					if( strcmp(jogadores[i].utilizador,login.utilizador)==0)
					{
						strcpy(MSG,"LOGINFAIL"); printf("[%d CRIAR-fail] Ja existe uma conta com os mesmos dados!\n",id);
						login.COD=2;
						if (!WriteFile(hPipe, &login, (DWORD) sizeof(login), &n, NULL)) 
						{
							printf("[%d LOGIN] Erro ao escrever dados [2]\n",id);
							
							return -1;
						}
						break;
					}
				}
				
				if(strcmp(MSG,"LOGINFAIL")!=0)
				{ //CRIA CONTA!!
					
					strcpy(jogadores[numJogadoresRegistados].utilizador,login.utilizador);
					strcpy(jogadores[numJogadoresRegistados].password,login.password);
					numJogadoresRegistados++;

					ESCREVEJogadores();//gravar para TXT

					login.COD=3;
					if (!WriteFile(hPipe, &login, (DWORD) sizeof(login), &n, NULL)) 
					{
						printf("[%d LOGIN] Erro ao escrever dados [3]\n",id);
						
						return -1;
					}

					printf("[%d CRIAR-OK] Uma nova conta foi criada e guardada para o ficheiro!\n",id);

					

				}
																
				ret = ReadFile(hPipe, &login, sizeof(login), &n, NULL);   
				if (!ret || !n)
				{
					printf("[%d LOGIN] Erro ao ler dados [4]\n",id);
					
					return -1;
				}

				if(login.COD==0) //se o codigo ja for de login.. para
				{
					break;
				}
			}
						
			printf("[%d LOGIN-dados] Username: %s e Pass: %s \n",id,login.utilizador,login.password);

			for(i=0; i < numJogadoresRegistados ; i++)
			{	
			
				if(strcmp(login.utilizador,jogadores[i].utilizador)==0 && strcmp(login.password,jogadores[i].password)==0)
				{
					if (jogadores[i].logado==1)
					{
						strcpy(MSG,"LOGINFAIL");
						printf("[%d LOGIN-fail] A conta ja esta a ser utilizada!\n",id);
						break;
					}else{

				strcpy(MSG,"LOGINOK");				
				printf("[%d LOGIN-ok] Dados da conta correctos!\n",id);			

					}
				}
			}
			if(strcmp(MSG,"LOGINOK")!=0)
			{ 
				strcpy(MSG,"LOGINFAIL"); printf("[%d LOGIN-fail] Dados da conta incorrectos!\n",id);
													
			}
				
			if (!WriteFile(hPipe, MSG, (DWORD) sizeof(MSG), &n, NULL)) 
			{
					printf("[%d LOGIN] Erro ao escrever dados [2]\n",id);
							
					return -1;
			}
											
		}while(strcmp(MSG,"LOGINFAIL")==0);

		jogadores[i].logado=1;

	
		if (!WriteFile(hPipe, &MSGSCORES, (DWORD) sizeof(MSGSCORES), &n, NULL)) {
			printf("[%d LOGIN] Erro a enviar os HighScores\n",id);

			return -1; }

		if (!WriteFile(hPipe, &totalJogos, (DWORD) sizeof(int), &n, NULL)) {
			printf("[%d LOGIN] Erro a enviar o numero de jogos para client\n",id);

			return -1; }

		

		if(totalJogos!=0)
		{
			
			if (!WriteFile(hPipe, &jogosDetalhes, (DWORD) sizeof(jogosDetalhes), &n, NULL)) {
				printf("[%d LOGIN] Erro ao enviar os detalhes do jogo\n",id);
				return -1; }
		}

	
	
		ReadFile(hPipe, &MSG, sizeof(MSG), &n, NULL);   
		if (!ret || !n)
		{
			printf("[%d LOGIN] Erro ao ler dados [5]\n",id);

			return -1;
		}



		if(strcmp(MSG,"CRIARNOVOJOGO")==0)
		{
			totalJogos++;
			if(totalJogos>5) { printf("\n Lista de jogos cheia!! \n"); ExitThread(1); return 0; }

		
			sprintf(jogosDetalhes[totalJogos],"Jogo %d",totalJogos);

			//SE mais ke 5..negar

			totalJogadores[totalJogos]++;

			strcpy(pacupdates[totalJogos][1].utilizador,login.utilizador);

			jogoEscolhido=totalJogos;
	
			Threads2[totalJogos][1] = CreateThread(NULL,0,AtendeCliente2,(LPVOID)hPipe,0,NULL);
	
			
		}

		if(strcmp(MSG,"Jogo 1")==0){

			totalJogadores[1]++;
			jogoEscolhido=1;

			strcpy(pacupdates[1][totalJogadores[1]].utilizador,login.utilizador);

			if(totalJogadores[1]>=3){

				sprintf(jogosDetalhes[jogoEscolhido],"Jogo %d (Cheio)",totalJogos);
			}

			
			Threads2[1][totalJogadores[1]] = CreateThread(NULL,0,AtendeCliente2,(LPVOID)hPipe,0,NULL);	

		}

		if(strcmp(MSG,"Jogo 2")==0){

			totalJogadores[2]++;
			jogoEscolhido=2;

			strcpy(pacupdates[2][totalJogadores[2]].utilizador,login.utilizador);

			if(totalJogadores[2]>=3){

				sprintf(jogosDetalhes[jogoEscolhido],"Jogo %d (Cheio)",totalJogos);
			}


			Threads2[2][totalJogadores[2]] = CreateThread(NULL,0,AtendeCliente2,(LPVOID)hPipe,0,NULL);
	

		}

		if(strcmp(MSG,"Jogo 3")==0){

			totalJogadores[3]++;

			jogoEscolhido=3;

			strcpy(pacupdates[3][totalJogadores[3]].utilizador,login.utilizador);

			if(totalJogadores[3]>=3){

				sprintf(jogosDetalhes[jogoEscolhido],"Jogo %d (Cheio)",totalJogos);
			}
	
			Threads2[3][totalJogadores[3]] = CreateThread(NULL,0,AtendeCliente2,(LPVOID)hPipe,0,NULL);
	

		}

		if(strcmp(MSG,"Jogo 4")==0){

			totalJogadores[4]++;

			jogoEscolhido=4;

			strcpy(pacupdates[4][totalJogadores[4]].utilizador,login.utilizador);

			if(totalJogadores[4]>=3){

				sprintf(jogosDetalhes[jogoEscolhido],"Jogo %d (Cheio)",totalJogos);
			}
		
			Threads2[4][totalJogadores[4]] = CreateThread(NULL,0,AtendeCliente2,(LPVOID)hPipe,0,NULL);
		

		}

		if(strcmp(MSG,"Jogo 5")==0){

			totalJogadores[5]++;

			jogoEscolhido=5;

			strcpy(pacupdates[5][totalJogadores[5]].utilizador,login.utilizador);

			if(totalJogadores[5]>=3){

				sprintf(jogosDetalhes[jogoEscolhido],"Jogo %d (Cheio)",totalJogos);
			}

			Threads2[5][totalJogadores[5]] = CreateThread(NULL,0,AtendeCliente2,(LPVOID)hPipe,0,NULL);		

		}

			
				
		ExitThread(1);
		return 0;
					
}


DWORD WINAPI AtendeCliente2(LPVOID param)
{
		
		DWORD n;
		HANDLE hPipe = (HANDLE)param;
		int ret,id,i,j;
		int jogo=jogoEscolhido;
		int cliente=totalJogadores[jogoEscolhido];
		PACMAN pac;

	
		id=GetCurrentThreadId();
		

	
		if (!WriteFile(hPipe, &cliente, (DWORD) sizeof(int), &n, NULL)) {
			printf("[%d LOGIN] Erro ao enviar o numero de cliente para o client\n",id);

			return -1; }	
				
		if(totalJogadores[jogo]==1) {

					
			
			}
			
		
		
			Sleep(150);
	//////////////////FANTASMAS////////////////////////////

		hPipeFantasmas[jogo][totalJogadores[jogo]] = CreateNamedPipe((LPCWSTR)PIPE_NAME_GHOSTS, PIPE_ACCESS_OUTBOUND, PIPE_WAIT  | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE , PIPE_UNLIMITED_INSTANCES, 256, 256, 1000, &sa);  		
				
	if (hPipeFantasmas[jogo][totalJogadores[jogo]]==NULL) 
		{
		printf("[ERRO] Criar o pipe para actualizar os fantasmas\n");
		
		return 0;
		}
	

	ConnectNamedPipe(hPipeFantasmas[jogo][totalJogadores[jogo]], NULL); 		
					

	//////////////////////PACS BROADCAST////////////////////////
	

	hPipePacs[jogo][totalJogadores[jogo]] = CreateNamedPipe((LPCWSTR)PIPE_NAME_PACS, PIPE_ACCESS_OUTBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, PIPE_UNLIMITED_INSTANCES, 256, 256, 1000, &sa);  		

	if (hPipePacs[jogo][totalJogadores[jogo]]==NULL) 
	{
		printf("[ERRO] Criar o pipe para actualizar os PACS\n");

		return 0;
	}


	ConnectNamedPipe(hPipePacs[jogo][totalJogadores[jogo]], NULL); 	


	///////////////////////1 PAC ONLY//////////////////////////////

	hPipeRecebePac[jogo][totalJogadores[jogo]] = CreateNamedPipe((LPCWSTR)PIPE_NAME_PAC, PIPE_ACCESS_DUPLEX , PIPE_WAIT | PIPE_TYPE_MESSAGE 
		| PIPE_READMODE_MESSAGE, PIPE_UNLIMITED_INSTANCES, 256, 256, 1000, NULL); 

	if (hPipeRecebePac[jogo][totalJogadores[jogo]]==NULL) 
	{
		printf("[ERRO] Criar o pipe para actualizar o PAC\n");

		return 0;
	}



	ConnectNamedPipe(hPipeRecebePac[jogo][totalJogadores[jogo]], NULL); 	


	/////////////////////////////////////////////////////
	
	pacupdates[jogo][cliente].emJogo=1;
	pacupdates[jogo][cliente].clientJogoON=1; 
	jogoON[jogo]=1;

				if(totalJogadores[jogo]==1) { //se for o primeiro jogador.. recebe os dados do mapa/bonus

				printf("[%d THREAD] A ler numero de ghosts....!\n",id);


				ret = ReadFile(hPipe, &fantasmasTotal[jogo], sizeof(int), &n, NULL);   
				if (!ret || !n)
					{
						printf("[%d SERVIDOR-FAIL] Erro ao ler os dados dos ghosts\n",id);

						return -1;
					}else{ printf("[%d SERVIDOR-OK] Foi lido com sucesso o numero de ghosts! (%d) \n",id,fantasmasTotal[jogo]); 
				}
									
					
				printf("[%d THREAD] A ler dados do mapa.....!\n",id);


				ret = ReadFile(hPipe, &mapa[jogo], sizeof(MAP)*280, &n, NULL);   
				if (!ret || !n)
				{
					printf("[%d SERVIDOR-FAIL] Erro ao ler os dados do mapa [1]\n",id);
					
					return -1;
				}else{ printf("[%d SERVIDOR-OK] Foi lido com sucesso os dados do mapa!\n"); }

				//////////////////////////////// THREAD fantasmas!!!
				hThreadFantasmas[jogo] = CreateThread(NULL,0,ActualizarFantasmas,(LPVOID)jogo,0,NULL); 
				//////////////////////////////
				//////////////////////////////// THREAD pacs!!!
				hThreadPacs[jogo] = CreateThread(NULL,0,ActualizarPacs,(LPVOID)jogo,0,NULL); 
				//////////////////////////////
				//////////////////////////////// THREAD bonus!!!
				hThreadBonus[jogo] = CreateThread(NULL,0,VerificaBonus,(LPVOID)jogo,0,NULL); 
				//////////////////////////////

				

				}else{ 
					
					if (!WriteFile(hPipe, &mapa[jogo], (DWORD) sizeof(MAP)*280, &n, NULL)) {
					printf("[%d LOGIN] Erro ao escrever dados do mapa para o cliente 2 ou 3 \n",id);
					
					return -1; }	
				}


			

				printf("[%d THREAD] Inicio do jogo!\n",id);	

						

		while(1)
		{			
			if(jogoON[jogo]==0){ printf("A terminar este jogo... \n"); 
									
			break; }
			//	}
			//if( pacupdates[jogo][cliente].emJogo!=1) { return; } //DEAD!
			
			// mapa[jogo][pac.pos].jogador=1; //caso ter ficado na mm pos (visto ke so actualiza quando se mexe)

			WaitForSingleObject(hMutex1,INFINITE); 

			if(pacupdates[jogo][cliente].emJogo==1){

			if(!ReadFile(hPipeRecebePac[jogo][cliente], &pac,(DWORD) sizeof(PACMAN),&n,NULL))
			{ 
				printf("[%d THREAD] Nao foi possivel receber nova POSICAO....saindo!\n",id);  break;
			}

	
			//printf("O jogador tem a pontuacao %d | %d | %d!\n",pac.pos,jogo,cliente);		
			
			
			pacupdates[jogo][cliente].x=pac.x;
			pacupdates[jogo][cliente].y=pac.y;
			pacupdates[jogo][cliente].pos=pac.pos;

			

			pacupdates[jogo][cliente].bitmap=pac.bitmap;
			mapa[jogo][pac.pos].bitmap=IDB_pCaminho;//se um pac passou nessa posicao..ja nao ha bonus!
			
			
			pacupdates[jogo][cliente].pontuacao=pac.pontuacao;				
	
			mapa[jogo][pac.pos].jogador=cliente;
			}
			else{
			mapa[jogo][pac.pos].bitmap=IDB_pacDead;
			pacupdates[jogo][cliente].bitmap=IDB_pacDead;
			ReadFile(hPipeRecebePac[jogo][cliente], &pac,(DWORD) sizeof(PACMAN),&n,NULL);
			}

			ReleaseMutex(hMutex1); 

			

			
		}

		Sleep(200);

		printf("[%d THREAD] Vou desligar o pipe e thread... (DisconnectNamedPipe/CloseHandle)\n",id);
		if(! DisconnectNamedPipe(hPipe)){ perror("[THREAD] Erro ao desligar o pipe!"); return -1; }
		Sleep(50);

		fantasmasTotal[jogo]=0;
		jogoON[jogo]=-1;

		WaitForSingleObject(hMutex1,INFINITE); 

		if(cliente==1){ //so a thread do 1º jogador atualiza (alterar para melhor dps)

		for(i=0; i<100; i++)
		{
			for(j=1; j<=3; j++)
			{
			if(strcmp(jogadores[i].utilizador,pacupdates[jogo][j].utilizador)==0)
			{
				jogadores[i].jogosTotal++;
				jogadores[i].pontuacaoTotal+=pacupdates[jogo][j].pontuacao;
				jogadores[i].logado=0;

			}
			}
		}

		sprintf(jogosDetalhes[jogo],"Jogo %d (Terminado)",jogo); //se ja terminou apresentar 

		}
		 

		ESCREVEJogadores(); //escreve info para o txt

		ReleaseMutex(hMutex1);

		
		/*
		ZeroMemory(pacupdates,sizeof(pacupdates)*4);
		ZeroMemory(fantasmas,sizeof(fantasmas));
		ZeroMemory(hMutex1,sizeof(hMutex1));
		ZeroMemory(hPipeFantasmas,sizeof(hPipeFantasmas)*4);
		ZeroMemory(hPipePacs,sizeof(hPipePacs)*4);
		ZeroMemory(hPipeBonus,sizeof(hPipeBonus)*4);
		ZeroMemory(hPipeRecebePac,sizeof(hPipeRecebePac)*4);
		ZeroMemory(hThreadFantasmas,sizeof(hThreadFantasmas));
		ZeroMemory(hThreadPacs,sizeof(hThreadPacs));
		ZeroMemory(hThreadBonus,sizeof(hThreadBonus));*/
	
	
						//hVerificaPosicao;
		ExitThread(1);
		return 0;
		
					
}



void main(void) {

	HANDLE hPipe,Threads[100];
	int i=0;

	
	hMutex1=CreateMutex(NULL,FALSE,NULL);
	//hVerificaPosicao=CreateEvent(NULL,TRUE,TRUE,NULL);

	LerJogadores();

			
	printf("\n[SERVIDOR] A iniciar servidor... \n"); 
	servidorON=TRUE;
	
	while(servidorON)
	{
		
		printf("[SERVIDOR] A tentar criar o pipe '%s'\n", PIPE_NAME);

		hPipe = CreateNamedPipe((LPCWSTR)PIPE_NAME, PIPE_ACCESS_DUPLEX , 
			PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 
			PIPE_UNLIMITED_INSTANCES, 256, 256, 1000, 
			(LPSECURITY_ATTRIBUTES) &sa);


		if(hPipe==INVALID_HANDLE_VALUE){  
			status = GetLastError(); 
			printf("[SERVIDOR] Erro a criar o pipe! (ERRO NUM: %ld)\n",status);
			break;  
		}


		printf("[SERVIDOR] A espera de uma ligacao de um cliente... \n");
		ConnectNamedPipe(hPipe, NULL); 
		
			
	
		totalJogadoresTOTAL++;
		Threads[totalJogadoresTOTAL] = CreateThread(NULL,0,AtendeCliente1,(LPVOID)hPipe,0,NULL);
				

		printf("ThreadTOTAL: %d \n",totalJogadoresTOTAL);
						

	}
	

	for(i =0;i < totalJogadoresTOTAL; i++)
	{
		DisconnectNamedPipe(hPipeFantasmas[i]);
		CloseHandle(hPipeFantasmas[i]);
	}
		 
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	getchar();
	exit(1);
			
}




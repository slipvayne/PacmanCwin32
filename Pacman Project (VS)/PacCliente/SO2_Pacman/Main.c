/* ===================================================== */
/*                      PACMAN                           */
/*                   PROJECTO SO2                        */
/*            Ricardo Germano - 21190726                 */
/* ===================================================== */

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4995)
#pragma warning(disable : 4133)
#pragma warning(disable : 4129)
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"
#include "Main.h"

DWORD WINAPI EnviaPacUpdate(LPVOID param) //Envia a posicao do pac para o servidor!
{
	DWORD n;


	while(1)
	{
		Sleep(25);
		
		if (hPipeEnviaPac==NULL) { MessageBox(hDlg, "Error a abrir pipe do servidor para enviar pac!", "Erro", MB_OK); }

		if (!WriteFile(hPipeEnviaPac, &pac, (DWORD) sizeof(PACMAN), &n, NULL)) 
			MessageBox(hDlg, "Nao foi possivel actualizar a pos do pac", "Error", MB_OK);
		
	}



}


DWORD WINAPI LerGhostsUpdates(LPVOID param) //le os updates dos fantamas
{
	HANDLE hFont,hOldFont;
	BOOL ret = FALSE;
	DWORD n;
	int i,n_ghosts;
	HDC hdc,auxmemdc;
	

	
	while(1)
	{
	
	//Sleep(25);

		hdc=GetDC((HWND)param);
		auxmemdc=CreateCompatibleDC(hdc);

		
		ret = ReadFile(hPipeFantasmas, &n_ghosts, sizeof(int), &n, NULL);   
		if (!ret)
		{
		
			hFont=CreateFont(12,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
			hOldFont = (HFONT)SelectObject(hdc, hFont);	
			TextOut(hdc, 240, 260 , "Erro a ler ghosts! [1]", (int)strlen("Erro a ler ghosts! [1]"));
			SelectObject(hdc, hOldFont);	
			
		}
		
		WaitForSingleObject(hMutex1,500); 

			for(i  =0 ; i < n_ghosts ; i++)
			{

			if (ghosts[i].tipo==1) { hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_Bola)); }
			else if(ghosts[i].tipo==2){ hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_pacDead)); }
			else { hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_pCaminho));	}			
			
			SelectObject(auxmemdc, hbit);		
			BitBlt(hdc, ghosts[i].x,ghosts[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);		
			BitBlt(memdc, ghosts[i].x,ghosts[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);
				

			}

		ReleaseMutex(hMutex1); 

	

				ret = ReadFile(hPipeFantasmas, &ghosts, sizeof(MAP)*20, &n, NULL);   
				if (!ret)
				{
					hFont=CreateFont(12,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
					hOldFont = (HFONT)SelectObject(hdc, hFont);	
					TextOut(hdc, 240, 260 , "Erro a ler ghosts! [2]", (int)strlen("Erro a ler ghosts! [2]"));
					SelectObject(hdc, hOldFont);	
				
				}					
	

	WaitForSingleObject(hMutex1,500); 



	for(i = 0;i < n_ghosts; i++)
	{
							
		hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(ghosts[i].bitmap));	

		SelectObject(auxmemdc, hbit);		
		BitBlt(hdc, ghosts[i].x,ghosts[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);		
		BitBlt(memdc, ghosts[i].x,ghosts[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);				
	}

	ReleaseMutex(hMutex1); 
				
				
		ReleaseDC((HWND)param, hdc);
		DeleteDC(auxmemdc);
	}
	
		
}

DWORD WINAPI LerPacsUpdates(LPVOID param) //update de todos os pacs
{
	HANDLE hFont,hOldFont;
	BOOL ret = FALSE;
	DWORD n;
	int i=0;
	HDC hdc,auxmemdc;
	

	while(1)
	{
		

		hdc=GetDC((HWND)param);
		auxmemdc=CreateCompatibleDC(hdc);
		
		ret = ReadFile(hPipePacs, &tjogadores, sizeof(int), &n, NULL);   
		if (!ret)
		{
			hFont=CreateFont(12,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
			hOldFont = (HFONT)SelectObject(hdc, hFont);	
			TextOut(hdc, 300, 220 , "Erro a ler numero pacs! [1]", (int)strlen("Erro a ler numero pacs! [1]"));
			SelectObject(hdc, hOldFont);	
		}

		
		WaitForSingleObject(hMutex1,500);

	
		for(i=1;i<=tjogadores;i++){

			
			if(i!=souOclienteN){ //nao actualiza do proprio client

			hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_pCaminho));	//volta ao bitmap caminho
				
			SelectObject(auxmemdc, hbit);		
			BitBlt(hdc, pacupdates[i].x,pacupdates[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);		
			BitBlt(memdc, pacupdates[i].x,pacupdates[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);		
			}else if(pacupdates[i].bitmap==IDB_pacDead)
			{
				hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_pacDead));	//volta ao bitmap caminho

				SelectObject(auxmemdc, hbit);		
				BitBlt(hdc, pacupdates[i].x,pacupdates[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);		
				BitBlt(memdc, pacupdates[i].x,pacupdates[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);	

			}

				
		}

		ReleaseMutex(hMutex1); 



		ret = ReadFile(hPipePacs, &pacupdates, sizeof(PACMAN)*4, &n, NULL);   
		if (!ret)
		{
			hFont=CreateFont(12,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
			hOldFont = (HFONT)SelectObject(hdc, hFont);	
			TextOut(hdc, 300, 220 , "Erro a ler pacs! [1]", (int)strlen("Erro a ler pacs! [1]"));
			SelectObject(hdc, hOldFont);	
		}

		
		WaitForSingleObject(hMutex1,500);

	
		for(i=1;i<=tjogadores;i++){

			if(pacupdates[i].clientJogoON==-1){
				MessageBox(hDlg, "\nFINISH!\nEste Jogo Acabou!\n", "GAME OVER", MB_OK);			
				exit(1);
				return 1;
			}

			if(i==souOclienteN) { if (pacupdates[souOclienteN].emJogo==0) {
//MessageBox(hDlg, "Ups! Foi comido por um Bad Guy\nFique a espera que o jogo termine!", "Game Over", MB_OK);
//break;
			}}

			if(i!=souOclienteN){ //nao actualiza do proprio client

		hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(pacupdates[i].bitmap));	

		SelectObject(auxmemdc, hbit);		
		BitBlt(hdc, pacupdates[i].x,pacupdates[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);		
		BitBlt(memdc, pacupdates[i].x,pacupdates[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);
			}else{
				if(pacupdates[i].bitmap==IDB_pacDead){
					hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_pacDead));	
					SelectObject(auxmemdc, hbit);		
					BitBlt(hdc, pacupdates[i].x,pacupdates[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);		
					BitBlt(memdc, pacupdates[i].x,pacupdates[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);
				}

			}
		}

	
		ReleaseMutex(hMutex1); 

		////////////////////////////////////

		ReleaseDC((HWND)param, hdc);
		DeleteDC(auxmemdc);	
	}
	
}



// ============================================================================
// WinMain()
// ============================================================================

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, 
				   LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;			// hWnd é o handler da janela, gerado mais abaixo 
						// por CreateWindow()
	MSG lpMsg;			// MSG é uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX é uma estrutura cujos membros servem para 
						// definir as características da classe da janela
	hInstance = hInst;
// ============================================================================
// 1. Definição das características da janela "wcApp" 
//    (Valores dos elementos da estrutura "wcApp" do tipo WNDCLASSEX)
// ============================================================================

	wcApp.cbSize = sizeof(WNDCLASSEX);	// Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;			// Instância da janela actualmente exibida 
										// ("hInst" é parâmetro de WinMain e vem 
										// inicializada daí)
	wcApp.lpszClassName = szProgName;	// Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = WndProc;		// Endereço da função de processamento da janela 
										// ("WndProc" foi declarada no início e encontra-se
										// mais abaixo)
	wcApp.style = CS_HREDRAW | CS_VREDRAW;			// Estilo da janela: Fazer o redraw
													// se for modificada horizontal ou
													// verticalmente
	/*OUTROS ESTILOS POSSíVEIS: CS_DBLCLKS			permite a captura de duplo cliques do rato
								CS_NOCLOSE			retira o 'X' na barra para fechar a aplicação*/

	wcApp.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PACICON));	// "hIcon" = handler do ícon normal
													// "NULL" = Icon definido no Windows
													// "IDI_AP..." Ícone "aplicação"
	wcApp.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PACICON));	// "hIcon" = handler do ícon pequeno
													// "NULL" = Icon definido no Windows
													// "IDI_WIN..." Ícon "Wind.NT logo"
	/*OUTROS TIPOS DE ICONES:	IDI_ASTERISK		Same as IDI_INFORMATION.
								IDI_ERROR			Hand-shaped icon.
								IDI_EXCLAMATION		Same as IDI_WARNING.
								IDI_HAND			Same as IDI_ERROR. 
								IDI_INFORMATION		Asterisk icon.
								IDI_QUESTION		Question mark icon.
								IDI_WARNING			Exclamation point icon*/

	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);	// "hCursor" = handler do cursor (rato)
													// "NULL" = Forma definida no Windows
													// "IDC_ARROW" Aspecto "seta" 
	/*OUTROS TIPOS DE CURSORES:	IDC_CROSS IDC_HAND IDC_HELP IDC_UPARROW IDC_WAIT */



	wcApp.lpszMenuName = (LPCTSTR) IDR_MENU1; //mostrar menu na janela // Classe do menu que a janela pode ter
													// (NULL = não tem menu)
	wcApp.cbClsExtra = 0;							// Livre, para uso particular
	wcApp.cbWndExtra = 0;							// Livre, para uso particular

	wcApp.hbrBackground = GetStockObject(WHITE_BRUSH); // "hbrBackground" = handler para 
														// "brush" de pintura do fundo da
														// janela. Devolvido por
														// "GetStockObject". Neste caso o
														// fundo vai ser branco
	/*OUTRAS CORES DE BRUSH:	BLACK_BRUSH  DKGRAY_BRUSH GRAY_BRUSH LTGRAY_BRUSH  */


// ============================================================================
// 2. Registar a classe "wcApp" no Windows
// ============================================================================
	if (!RegisterClassEx(&wcApp))
		return(0);

// ============================================================================
// 3. Criar a janela
// ============================================================================
	hWnd = CreateWindow(
		szProgName,				// Nome da janela (programa) definido acima
		"XPacMan",	// Texto que figura na barra da janela
		WS_OVERLAPPED | WS_CAPTION 
		| WS_SYSMENU | WS_MINIMIZEBOX,	// Estilo da janela (WS_OVERLAPPED= normal)
								// Outros valores: WS_HSCROLL, WS_VSCROLL
								// (Fazer o OR "|" do que se pretender)
		CW_USEDEFAULT,			// Posição x pixels (default=à direita da última)
		CW_USEDEFAULT,			// Posição y pixels (default=abaixo da última)
		800,			// Largura da janela (em pixels)
		600,			// Altura da janela (em pixels)
		(HWND) HWND_DESKTOP,	// handle da janela pai (se se criar uma a partir 
								// de outra) ou HWND_DESKTOP se a janela for
								// a primeira, criada a partir do "desktop"
		(HMENU) NULL,			// handle do menu da janela (se tiver menu)
		(HANDLE) hInst,			// handle da instância do programa actual
								// ("hInst" é declarado num dos parâmetros
								// de WinMain(), valor atribuído pelo Windows)
		0);			// Não há parâmetros adicionais para a janela

// ============================================================================
// 4. Mostrar a janela
// ============================================================================
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela, devolvido 
								// por "CreateWindow"; "nCmdShow"= modo de
								// exibição (p.e. normal, modal); é passado
								// como parâmetro de WinMain()

	UpdateWindow(hWnd);			// Refrescar a janela (Windows envia à janela
								// uma mensagem para pintar, mostrar dados,
								// (refrescar), etc)

// ============================================================================
// 5. Loop de Mensagens
// ============================================================================

	atalhos=LoadAccelerators(hInst,MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&lpMsg,NULL,0,0)) {	
		if (!IsDialogMessage(hDlg, &lpMsg)) {
			if(!TranslateAccelerator(hWnd, atalhos, &lpMsg)){
				TranslateMessage(&lpMsg);	// Pré-processamento da mensagem
				DispatchMessage(&lpMsg);	// Enviar a mensagem traduzida de volta
			}								// ao Windows
		}
	}
// ============================================================================
// 6. Fim do programa
// ============================================================================
	return((int)lpMsg.wParam);		// Retorna-se sempre o parâmetro "wParam" da
								// estrutura "lpMsg"
}


// ============================================================================
// FUNÇÂO DE PROCESSAMENTO DA JANELA
// ============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) 
{
	HDC hdc, auxmemdc;	
	DWORD n;
	PAINTSTRUCT PtStc;	
	HFONT hFont, hOldFont;
	HMENU menuh;
	int z;


	switch (messg) {

		case WM_CREATE:		

			//REGISTO!!!
			if(RegCreateKeyEx(HKEY_CURRENT_USER,"\Software\\XPacMan",0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &chave, &registoI) != ERROR_SUCCESS)
			{
				MessageBox(hWnd, "Erro ao criar ou abrir a chave!", "Registo", MB_OK);
			}

			 if (registoI == REG_OPENED_EXISTING_KEY) // se ja houver
			{  		
				tamanhoK=sizeof(login.utilizador);
				RegQueryValueEx(chave, "utilizador", NULL, NULL, (LPBYTE)login.utilizador, &tamanhoK);
				tamanhoK=sizeof(login.ip);	
				RegQueryValueEx(chave, "ip", NULL, NULL, (LPBYTE)login.ip, &tamanhoK); 
			
			}
			

			hdc=GetDC(hWnd);
			memdc=CreateCompatibleDC(hdc);					
			hbit=CreateCompatibleBitmap(hdc, 800, 600);	
			SelectObject(memdc, hbit);
			hbrush=GetStockObject(WHITE_BRUSH);				
			SelectObject(memdc, hbrush);
			PatBlt(memdc, 0, 0, 800, 600, PATCOPY);	

			hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_INIC));	
			auxmemdc=CreateCompatibleDC(hdc);	
			SelectObject(auxmemdc, hbit);		
			BitBlt(hdc, 0 ,0 , 800, 600, auxmemdc, 0,0, SRCCOPY);
			ReleaseDC(hWnd, hdc);
			BitBlt(memdc, 0 ,0 , 800, 600, auxmemdc, 0,0, SRCCOPY);
			DeleteDC(auxmemdc);
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE){
				if(MessageBox(hWnd,"Tem a certeza que deseja sair ?","Sair",MB_YESNO | MB_ICONQUESTION) == IDYES)
				PostQuitMessage(0);	}
			
			if((wParam == VK_RIGHT || wParam == tDIREITA) && estadoJogo==1){		
				if(pacDir){
				pacDir = 1;
				moverPacman(hWnd);	
				mostraPontuacao(hWnd);}
			}					
			if((wParam == VK_LEFT || wParam == tESQUERDA) && estadoJogo==1){		
				if(pacDir){
				pacDir = 2;
				moverPacman(hWnd);	
				mostraPontuacao(hWnd);}
			}	
			if((wParam == VK_UP || wParam == tCIMA) && estadoJogo==1){		
				if(pacDir){
				pacDir = 3;
				moverPacman(hWnd);	
				mostraPontuacao(hWnd);}
			}
			if((wParam == VK_DOWN || wParam == tBAIXO) && estadoJogo==1 ){
				if(pacDir){
				pacDir = 4;
				moverPacman(hWnd);
				mostraPontuacao(hWnd);}
			}				
			break;

			case WM_KEYUP:
				
				if(verPont == 1 && estadoJogo == 1 && wParam != VK_TAB)			
				{				
					verPont = 0;
					DestroyWindow(hDlg);
				}		
				break;				


		case WM_LBUTTONDOWN: // Botão esquerdo		

	if(estadoJogo==0){

		for(z=279; z>=0; z--)
		{
			if(mapa[z].x < (LOWORD(lParam))){
				pac.x=mapa[z].x;
			break;}
		}
		for(z=279; z>=0; z--)
		{
			if(mapa[z].y < (HIWORD(lParam))){
				pac.y=mapa[z].y;
				break;}
		}

		for(z=0; z<280;z++)
		{ 
			if(mapa[z].x==pac.x && mapa[z].y==pac.y && (mapa[z].bitmap==IDB_Bola || mapa[z].bitmap==IDB_pCaminho) && z!=0 && z!=19 && z!=279 && z!=260 && z!=9 && z!=270 && z!=269) 
			{		
					if(mapa[z].bitmap==IDB_Bola) pontuacao=1;
					else pontuacao=0;

					pac.pos=z;
					pacDir = 0; 
					moverPacman(hWnd);
					imprime(hWnd);
					mostraPontuacao(hWnd);					
					estadoJogo=1;		

			}
		}	
	}
	
		case WM_PAINT:

		hdc=BeginPaint(hWnd, &PtStc);
		
		BitBlt(hdc, 0,0, 800, 600, memdc, 0, 0, SRCCOPY);
		

		if(estadoJogo==0) {					
				hFont=CreateFont(22,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
				hOldFont = (HFONT)SelectObject(hdc, hFont);	
				TextOut(hdc, 210, 525 , "Define a posição inicial do pacman com o rato!", (int)strlen("Define a posição inicial do pacman com o rato!"));
				SelectObject(hdc, hOldFont);				
		}
		
		if(pacDir){ mostraPontuacao(hWnd); }

		EndPaint(hWnd, &PtStc);

		break;

		case WM_CLOSE:	
			if(MessageBox(hWnd,"Tem a certeza que deseja sair ?","Sair",MB_YESNO | MB_ICONQUESTION) == IDYES)
			{PostQuitMessage(0);}
		break;		
		
		case WM_COMMAND:
			switch(LOWORD(wParam)){	

				case ID_JOGO_SAIR:		
					if(MessageBox(hWnd,"Tem a certeza que deseja sair ?","Sair",MB_YESNO | MB_ICONQUESTION) == IDYES)
					PostQuitMessage(0);										
					break;

				case ID_JOGO_INICIO:
					
					if(estadoJogo==1 && MessageBox(hWnd,"Tem a certeza que deseja começar um novo jogo ?","Novo Jogo",MB_YESNO | MB_ICONQUESTION) == IDNO)
						{ break; }
					if(estadoJogo==1 && logado!=0) { logado=0; }

				    if(logado==0) {
					DialogBox(hInstance, (LPCSTR) IDD_Login, hWnd, (DLGPROC) DialogLogin);
					}

					if(logado==1) {									
					menuh=GetMenu(hWnd);
					EnableMenuItem(menuh,ID_Pontuacao,MF_ENABLED);
					pac.pos=0; pac.x=0; pac.y=0; pacDir=1;										
					estadoJogo=0;

					if(primeiroOuEmAndamento==1){ //se ja houver.. recebe mapa

						if (!ReadFile(hPipe,&mapa,(DWORD) sizeof(MAP)*280,&n,NULL))	{
							MessageBox(hWnd, "Não foi possível ler o mapa existente!", "Erro", MB_OK);
							exit(1);
						}
					}
					
					if(primeiroOuEmAndamento==0){ 
						criaMapa(); //cria mapa e envia para servidor						
					}
					pontuacao = 0;
					moverPacman(hWnd);
					imprime(hWnd);
					mostraPontuacao(hWnd);		
					hMutex1=CreateMutex(NULL,FALSE,NULL);
					ThreadFantasmas = CreateThread(NULL,0,LerGhostsUpdates,(LPVOID)hWnd,0,NULL);
					ThreadPacs = CreateThread(NULL,0,LerPacsUpdates,(LPVOID)hWnd,0,NULL);
					ThreadEnviaPac = CreateThread(NULL,0,EnviaPacUpdate,(LPVOID)hWnd,0,NULL);	
					EnableMenuItem(menuh,ID_HIGHSCORES,MF_ENABLED);
					InvalidateRect(hWnd, NULL, 1);		
					}
					break;

				case ID_Controlos:

				DialogBox(hInstance, (LPCSTR) IDD_dControlos, hWnd, (DLGPROC) DialogProc); 
				break;

				case ID_Objectos:

				DialogBox(hInstance, (LPCSTR) IDD_dObjectos, hWnd, (DLGPROC) DialogProc2); 
				break;

				case ID_HIGHSCORES:

				DialogBox(hInstance, (LPCSTR) IDD_HSCORES, hWnd, (DLGPROC) DialogHSCORES); 
				break;

				case ID_Pontuacao:
				
				if(verPont == 0 && estadoJogo == 1){			
				verPont = 1;
				hDlg = CreateDialog(hInstance, (LPCTSTR) IDD_dPontuacao, hWnd, (DLGPROC) Pontuacao);
				}
				else
				{ 
					verPont = 0;
					DestroyWindow(hDlg);
				}
				break;

		}
			

		default:
			return(DefWindowProc(hWnd,messg,wParam,lParam));
			break;
	}
	return(0);
}


//==========================FUNCOES=========================

void criaMapa(){

	int i,linha=0,conta=0,pos=0,atribBolas=0;
	DWORD n;
	MAP aux;

//***************************linha 0*****************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;
		aux.bitmap = IDB_pInferior;

	if(i==19 || i==9 || i==0) aux.bitmap = IDB_pCaminho;
	
		mapa[conta] = aux;
		conta++;	
	}
	linha++;

//***************************linha 1****************************************************

	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;
		
		if(i==0)aux.bitmap = IDB_pDireita;
		else
		if(i==19)aux.bitmap = IDB_pEsquerda;
		else
		if(i==9)aux.bitmap = IDB_pVertical;
		else aux.bitmap = IDB_pCaminho;

		mapa[conta] = aux;
		conta++;
	}
	linha++;

	//***************************linha 2****************************************************

	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;
		
		if(i==2 || i==6 || i==11 || i==15) 	aux.bitmap = IDB_pHorizEsq;
		else
		if(i==4 || i==7 || i==13 || i==17) 	aux.bitmap = IDB_pHorizDir;
		else
		if(i==3 || i==4 || i==12 || i==16) 	aux.bitmap = IDB_pHoriz;
		else
		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else
		if(i==9) aux.bitmap = IDB_pVertBaixo;
		else aux.bitmap = IDB_pCaminho;

		mapa[conta] = aux;
		conta++;
	}
	linha++;

	//****************linha 3*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) 	aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;
	
		mapa[conta] = aux;
		conta++;
	}
	linha++;


	//****************linha 4*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;
		if(i==2 || i==17) aux.bitmap = IDB_pVertCima;
		else 
		if(i==7) aux.bitmap = IDB_pHorizEsq;
		else
		if(i>=8 && i<=11) aux.bitmap = IDB_pHoriz;
		else
		if(i==12) aux.bitmap = IDB_pHorizDir;		
	
		mapa[conta] = aux;
		conta++;
	}
	linha++;


	//****************linha 5*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;

		if(i==2) aux.bitmap = IDB_pEsquerda;
		else
		if(i==17) aux.bitmap = IDB_pDireita;
		else
		if((i>=3 && i<=4) || (i>=15 && i<=16) ) aux.bitmap = IDB_pHoriz;
		else
		if(i==14) aux.bitmap = IDB_pHorizEsq;
		else
		if(i==5) aux.bitmap = IDB_pHorizDir;	

		if(i>=6 && i<=13) aux.bitmap = IDB_pCaminho;
	
		mapa[conta] = aux;
		conta++;
	}
	linha++;

		//****************linha 6*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;

		if(i==2 || i==17) aux.bitmap = IDB_pVertical;
		else
		if(i==8 || i==11) aux.bitmap = IDB_pSuperior;
		else
		if(i==9 || i==10) aux.bitmap = IDB_pSaida;
		else
		if(i==7) aux.bitmap = IDB_pSupEsq;
		else
		if(i==12) aux.bitmap = IDB_pSupDir;

		if(i==6 || i==13) aux.bitmap = IDB_pCaminho;
			
		mapa[conta] = aux;
		conta++;
	}
	linha++;

		//****************linha 7*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;

		if(i>=8 && i<=11) aux.bitmap = IDB_pInferior;	
		if(i==7) aux.bitmap = IDB_pInfEsq;
		if(i==12) aux.bitmap = IDB_pInfDir;
		if(i==5 || i==14) aux.bitmap = IDB_pVertCima;
		if(i==2 || i==17) aux.bitmap = IDB_pVertBaixo;
		if(i==6 || i==13) aux.bitmap = IDB_pCaminho;

	
		mapa[conta] = aux;
		conta++;
	}
	linha++;

	//****************linha 8*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else
		aux.bitmap = IDB_pCaminho;
	
		if(i==5 || i==14) aux.bitmap = IDB_pVertical;	
		if(i>=6 && i<=13) aux.bitmap = IDB_pCaminho;

		mapa[conta] = aux;
		conta++;
	}
	linha++;


	//****************linha 9*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;

		if(i==7) aux.bitmap = IDB_pHorizEsq;
		if(i>=8 && i<=11) aux.bitmap = IDB_pHoriz;
		if(i==12) aux.bitmap = IDB_pHorizDir;
		if(i==5 || i==14) aux.bitmap = IDB_pVertical;
		if(i==2 || i==16) aux.bitmap = IDB_pHorizEsq;
		if(i==3 || i==17) aux.bitmap = IDB_pHorizDir;
		if(i==9 || i==10) aux.bitmap = IDB_pSuperior;

		mapa[conta] = aux;
		conta++;
	}
	linha++;

	//****************linha 10*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;

		if(i==5 || i==14) aux.bitmap = IDB_pVertical;
		if(i==9) aux.bitmap = IDB_pInfEsq;
		if(i==10) aux.bitmap = IDB_pInfDir;

		mapa[conta] = aux;
		conta++;
	}
	linha++;

	//****************linha 11*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;
			
		if(i==2 || i==12) aux.bitmap = IDB_pHorizEsq;
		if(i==7 || i==17) aux.bitmap = IDB_pHorizDir;
		if(i>=3 && i<=6 || i>=13 && i<=16)  aux.bitmap = IDB_pHoriz;
		if(i==5 || i==14) aux.bitmap = IDB_pInferior;

		

		mapa[conta] = aux;
		conta++;
	}
	linha++;

	
	//****************linha 12*****************************************************************
	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;

		if(i==0) aux.bitmap = IDB_pDireita;
		else
		if(i==19) aux.bitmap = IDB_pEsquerda;
		else aux.bitmap = IDB_pCaminho;

		if(i==9) aux.bitmap = IDB_pSupEsq;
		if(i==10) aux.bitmap = IDB_pSupDir;

		if(i==1 || i==18) aux.bitmap = IDB_pCaminho;

		mapa[conta] = aux;
		conta++;
	}
	linha++;

	//***************************linha 13****************************************************

	for(i=0;i<20;i++)
	{
		aux.x = i*40;
		aux.y = linha*40;
		
		if(i>=1 && i<=18) aux.bitmap = IDB_pSuperior;
		else aux.bitmap = IDB_pCaminho;

		if(i==9 || i==10) aux.bitmap = IDB_pCaminho;

		mapa[conta] = aux;
		conta++;
	}
	linha++;

	//////////////////////////////////////RandomBolas
	srand ( (unsigned int)time(NULL) );
	atribBolas=BolasJogo;

	while(atribBolas>0)
	{
		
		if(( pos=rand()%238+21) && mapa[pos].bitmap==IDB_pCaminho)
		{
			mapa[pos].bitmap=IDB_Bola;
			atribBolas--;
		}
	}


	if (!WriteFile(hPipe, &mapa, (DWORD) sizeof(MAP)*280, &n, NULL)) {
		MessageBox(hDlg, "Não foi possível enviar os dados do mapa para o servidor!", "Erro", MB_OK);
		//exit(1);
	}


}

void imprime(HWND hWnd)
{
	int i;
	HDC hdc, auxmemdc;
	hdc=GetDC(hWnd);
	auxmemdc=CreateCompatibleDC(hdc);

	WaitForSingleObject(hMutex1,500); 

	for(i=0;i<280;i++)
	{
		
		hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(mapa[i].bitmap));			
		SelectObject(auxmemdc, hbit);		
		BitBlt(hdc, mapa[i].x,mapa[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);		
		BitBlt(memdc, mapa[i].x,mapa[i].y, 40, 40, auxmemdc, 0,0, SRCCOPY);
		
	}

	ReleaseMutex(hMutex1);

	ReleaseDC(hWnd, hdc);
	DeleteDC(auxmemdc);

}





void moverPacman(HWND hWnd)
{
	if (pacupdates[souOclienteN].emJogo==1) {
	int xInic,xFinal,yInic,yFinal,posInic,posFinal;

	Sleep(75);
	
	xInic=pac.x;
	yInic=pac.y;
	posInic = pac.pos;
	mapa[pac.pos].jogador = 0;

	if(pacDir == 0) //inicio
	{
		pacDir = 1; //direita
		pac.bitmap = IDB_pacDir; 		
		mapa[pac.pos].bitmap = pac.bitmap;
		mapa[pac.pos].jogador = 1;
		
	}
	else
	if(pacDir == 1) //direita
	{		pac.bitmap = IDB_pacDir;
			mapa[pac.pos].bitmap = pac.bitmap;
		if(mapa[pac.pos+1].bitmap == IDB_pCaminho || mapa[pac.pos+1].bitmap == IDB_Bola)
		{
			if(mapa[pac.pos+1].bitmap == IDB_Bola) pontuacao++;
		//	if(mapa[pac.pos+1].bitmap == IDB_BolaBonus) pontuacao+=10;
			mapa[pac.pos+1].bitmap = pac.bitmap;
			mapa[pac.pos].bitmap = IDB_pCaminho;
			pac.x += 40;
			pac.pos++;
			mapa[pac.pos].jogador = 1;
		}
	}
	else
	if(pacDir == 2) //esquerda
	{		pac.bitmap = IDB_pacEsq;
			mapa[pac.pos].bitmap = pac.bitmap;
		if(mapa[pac.pos-1].bitmap == IDB_pCaminho || mapa[pac.pos-1].bitmap == IDB_Bola)
		{
			if(mapa[pac.pos-1].bitmap == IDB_Bola) pontuacao++;
		//	if(mapa[pac.pos-1].bitmap == IDB_BolaBonus) pontuacao+=10;
			mapa[pac.pos-1].bitmap = pac.bitmap;
			mapa[pac.pos].bitmap = IDB_pCaminho;
			pac.x -= 40;
			pac.pos--;
			mapa[pac.pos].jogador = 1;
		}
	}
	else
	if(pacDir == 3)
	{		pac.bitmap = IDB_pacCima;
			mapa[pac.pos].bitmap = pac.bitmap;
		if(mapa[pac.pos-20].bitmap == IDB_pCaminho || mapa[pac.pos-20].bitmap == IDB_Bola)
		{
			if(mapa[pac.pos-20].bitmap == IDB_Bola) pontuacao++;
			//if(mapa[pac.pos-20].bitmap == IDB_BolaBonus) pontuacao+=10;
			mapa[pac.pos-20].bitmap = pac.bitmap;
			mapa[pac.pos].bitmap = IDB_pCaminho;
			pac.y -= 40;
			pac.pos = pac.pos-20;
			mapa[pac.pos].jogador = 1;
		}
	}
	else
	if(pacDir == 4)
	{		pac.bitmap = IDB_pacBaixo;
			mapa[pac.pos].bitmap = pac.bitmap;
		if(mapa[pac.pos+20].bitmap == IDB_pCaminho || mapa[pac.pos+20].bitmap == IDB_Bola)
		{
			if(mapa[pac.pos+20].bitmap == IDB_Bola) pontuacao++;
		  //if(mapa[pac.pos+20].bitmap == IDB_BolaBonus) pontuacao+=10;
			mapa[pac.pos+20].bitmap = pac.bitmap;
			mapa[pac.pos].bitmap = IDB_pCaminho;
			pac.y += 40;
			pac.pos = pac.pos+20;
			mapa[pac.pos].jogador = 1;
		}
	}

	xFinal=pac.x; yFinal=pac.y;
	posFinal = pac.pos;
	pac.pontuacao = pontuacao;

	if (xFinal==xInic && yInic==xFinal) { mapa[pac.pos].jogador = 1;}

		
	alteraPac(hWnd,xInic,yInic,xFinal,yFinal,posInic,posFinal);
	}
}

void alteraPac(HWND hWnd, int xInic,int yInic, int xFinal,int yFinal,int posInic, int posFinal)
{
	HDC hdc,auxmemdc;

	WaitForSingleObject(hMutex1,500); 

	hdc=GetDC(hWnd);
	auxmemdc=CreateCompatibleDC(hdc);	

	hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(mapa[posInic].bitmap));		
	
	SelectObject(auxmemdc, hbit);		
	BitBlt(hdc, xInic,yInic, 40, 40, auxmemdc, 0,0, SRCCOPY);
	BitBlt(memdc, xInic,yInic, 40, 40, auxmemdc, 0,0, SRCCOPY);

	hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(mapa[posFinal].bitmap));	

	SelectObject(auxmemdc, hbit);	
	BitBlt(hdc, xFinal,yFinal, 40, 40, auxmemdc, 0,0, SRCCOPY);
	BitBlt(memdc, xFinal,yFinal, 40, 40, auxmemdc, 0,0, SRCCOPY);

	ReleaseDC(hWnd, hdc);	
	DeleteDC(auxmemdc);	

	ReleaseMutex(hMutex1); 
		
}

void mostraPontuacao(HWND hWnd)
{
	int i=0, Objectos=0;
	HDC hdc;
	HFONT hFont, hOldFont;
	char frase[200]="\0";

	WaitForSingleObject(hMutex1,500); 

	hdc=GetDC(hWnd);	
	SetBkColor(hdc,RGB(0,0,0));


	if(estadoJogo==1){
	hFont=CreateFont(26,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");	
	sprintf_s(buffer, 300, "%d",pontuacao);

	SetTextColor(hdc,RGB(98,179,100));
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	TextOut(hdc,300,5,"Pontuação:", (int) strlen("Pontuação:")); 
	SetTextColor(hdc,RGB(255,255,255));
	hFont=CreateFont(26,0,0,0,FW_NORMAL,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	TextOut(hdc,420,5,buffer, (int) strlen(buffer));
	
		
	hFont=CreateFont(16,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");	
	sprintf_s(buffer, 300, "%d",tjogadores);

	SetTextColor(hdc,RGB(98,179,100));
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	TextOut(hdc,600,5,"Online:", (int) strlen("Online:")); 
	SetTextColor(hdc,RGB(255,255,255));
	hFont=CreateFont(16,0,0,0,FW_NORMAL,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	TextOut(hdc,650,5,buffer, (int) strlen(buffer));

	if(tjogadores==2)
	{
	
		sprintf_s(buffer, 300, "(1) [%s] Pontos: %d",pacupdates[1].utilizador,pacupdates[1].pontuacao);

		hFont=CreateFont(16,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc,40,530,buffer, (int) strlen(buffer));

		sprintf_s(buffer, 300, "(2) [%s] Pontos: %d",pacupdates[2].utilizador,pacupdates[2].pontuacao);

		hFont=CreateFont(16,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc,340,530,buffer, (int) strlen(buffer));

	

	}

	if(tjogadores==3)
	{
	
		sprintf_s(buffer, 300, "(1) [%s] Pontos: %d",pacupdates[1].utilizador,pacupdates[1].pontuacao);

		hFont=CreateFont(16,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc,40,530,buffer, (int) strlen(buffer));

		sprintf_s(buffer, 300, "(2) [%s] Pontos: %d",pacupdates[2].utilizador,pacupdates[2].pontuacao);

		hFont=CreateFont(16,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc,340,530,buffer, (int) strlen(buffer));

		sprintf_s(buffer, 300, "(3) [%s] Pontos: %d",pacupdates[3].utilizador,pacupdates[3].pontuacao);

		hFont=CreateFont(16,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc,600,530,buffer, (int) strlen(buffer));
		
	
	}
	}

		
	SetTextColor(hdc,RGB(255,242,0));
	hFont=CreateFont(26,0,0,0,FW_HEAVY,0,0,0,0,0,0,CLEARTYPE_QUALITY,FF_DECORATIVE,"SYSTEM_FIXED_FONT");
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	TextOut(hdc,45,5,"iPac-Man", (int) strlen("iPac-Man"));

    SelectObject(hdc, hOldFont); 

	SetBkMode(memdc,TRANSPARENT);
	SetTextColor(memdc,RGB(255, 255, 255));

	

	//Verifica FIM	(É O SERVER KE VERIFICA AGORA!);

	
	for(i=0;i<280;i++)
	{
		if(mapa[i].bitmap == IDB_Bola)
			Objectos = 1;
	}
	

	if(Objectos == 0)
	{
		sprintf_s(frase,200,"%s %d %s ","Pontuação Final:",pontuacao,"\n\nO jogo acabou, quer jogar novamente?");
		if(MessageBox(hWnd,frase,"iPac-Man",MB_YESNO) == IDYES)
		{
			
				estadoJogo = 0;
				pacDir = 0; 
				pac.pos = 0;
				pac.x =0; pac.y=0;
				pontuacao = 0;
				criaMapa();
				moverPacman(hWnd);
				imprime(hWnd);
				InvalidateRect(hWnd, NULL, 1);
				
		}
		else
		{
			MessageBox(hWnd,"O programa vai agora ser encerrado!","iPac-Man",MB_OK);
			DestroyWindow(hWnd);
			PostQuitMessage(0);
		}
	}
	

	ReleaseMutex(hMutex1); 

}


// ============================================================================
// FUNÇÂO DE PROCESSAMENTO DA DIALOG BOX
// ============================================================================

BOOL CALLBACK DialogLogin(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) 
{
	
	DWORD n;	

	switch(messg) 
	{
	
	case WM_INITDIALOG:	
		

	SetDlgItemText(hWnd, IDC_SRVIP, login.ip);
	//SetDlgItemText(hWnd, IDC_Username, login.utilizador);
	//SetDlgItemText(hWnd, IDC_Password, login.password);

	SetDlgItemText(hWnd, IDC_Username, login.utilizador);
	//SetDlgItemText(hWnd, IDC_Password, "ger");

		case WM_COMMAND:
			switch(LOWORD(wParam)) 
			{


			case IDC_NCONTA:

				GetDlgItemText(hWnd, IDC_SRVIP, login.ip, 16);
				GetDlgItemText(hWnd, IDC_Username, login.utilizador, 25);
				GetDlgItemText(hWnd, IDC_Password, login.password, 25);

				//REGISTRO
			
				RegDeleteKey(chave,NULL);

				RegSetValueEx(chave, "utilizador", 0, REG_SZ, (LPBYTE)login.utilizador, strlen(login.utilizador)); 
				RegSetValueEx(chave, "ip", 0, REG_SZ, (LPBYTE)login.ip, strlen(login.ip));  

				RegCloseKey(chave);



				login.COD=1; // 1 COMO NOVA CONTA

				sprintf(PIPE_NAME,"\\\\%s\\pipe\\xpacman",login.ip);

				if(hPipe==NULL){	
					hPipe = CreateFile(PIPE_NAME, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hPipe==NULL) { 
						MessageBox(hWnd, "Error a abrir pipe do servidor! Servidor ON?", "Erro", MB_OK);
						exit(1);
					}
				}


				if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
					MessageBox(hWnd, "Não foi possível ligar ao servidor! Tente mais tarde!", "Erro", MB_OK);
					exit(1);
				}

								
	
			if (!WriteFile(hPipe, &login, (DWORD) sizeof(login), &n, NULL)) {
				MessageBox(hWnd, "Não foi possível escrever no pipe!", "Erro", MB_OK);
				exit(1);
			}

			
			if (!ReadFile(hPipe,&login,(DWORD) sizeof(login),&n,NULL))	{
				MessageBox(hWnd, "Não foi possível ler do pipe!", "Erro", MB_OK);
				exit(1);
			}
			
			if(login.COD==2){
				MessageBox(hWnd, "Já existe uma conta com esse nome, tente outra!", "Erro", MB_OK);
			}

			if(login.COD==3){
			MessageBox(hWnd, "Nova conta criada com sucesso!\nJá pode fazer login!", "XPacMan", MB_OK);
			}

			login.COD=0; // COMO LOGIN NORMAL

			EndDialog(hWnd, 0);
			return 1;
			
			case IDOK:				
			 
		GetDlgItemText(hWnd, IDC_SRVIP, login.ip, 16);
  		GetDlgItemText(hWnd, IDC_Username, login.utilizador, 25);
		GetDlgItemText(hWnd, IDC_Password, login.password, 25);

		RegDeleteKey(chave,NULL);

			RegSetValueEx(chave, "utilizador", 0, REG_SZ, (LPBYTE)login.utilizador, strlen(login.utilizador)); 
			RegSetValueEx(chave, "ip", 0, REG_SZ, (LPBYTE)login.ip, strlen(login.ip));  
			
		RegCloseKey(chave);

	
		if(esteveEmJogo==0){

		sprintf(PIPE_NAME,"\\\\%s\\pipe\\xpacman",login.ip);
	
	//	strcat(PIPE_NAME,login.ip);
	//	strcat(PIPE_NAME,PNAME);


		if(hPipe==NULL){	
		hPipe = CreateFile(PIPE_NAME, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hPipe==NULL) { 
			MessageBox(hWnd, "Error a abrir pipe do servidor! Servidor ON?", "Erro", MB_OK);
			exit(1);
		}
		}
		

		if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
			MessageBox(hWnd, "Não foi possível ligar ao servidor! Tente mais tarde!", "Erro", MB_OK);
			exit(1);
			}

		}
		
			if (!WriteFile(hPipe, &login, (DWORD) sizeof(login), &n, NULL)) {
				MessageBox(hWnd, "Não foi possível escrever no pipe [1]!", "Erro", MB_OK);
			exit(1);
			}

			if (!ReadFile(hPipe, MSGLogin,(DWORD) sizeof(MSGLogin),&n,NULL))	{
				MessageBox(hWnd, "Não foi possível ler do pipe [2]!", "Erro", MB_OK);
			exit(1);
			}
			

			if(strcmp(MSGLogin,"LOGINFAIL")==0)
			{
			MessageBox(hWnd,"Dados de login incorrectos ou conta ja logada!\nTente novamente, ou crie uma nova conta!","Erro",MB_OK);
			logado=0;
			esteveEmJogo=0;
			strcpy(PIPE_NAME,"\\\\");
			EndDialog(hWnd, 0);	
			exit(1);
			return 1;
			}else{
				MessageBox(hWnd,"Login efectuado com sucesso!\nCarregue em OK para iniciar um novo jogo!","XPacMan",MB_OK);
				logado=1;
				esteveEmJogo=1;
				

				DialogBox(hInstance, (LPCSTR) IDD_JogosCriar, hWnd, (DLGPROC) DialogJogosCriar);
					

			    EndDialog(hWnd, 0);				  
				  return 1;
			}			
					return 1;

				case IDCANCEL:
					EndDialog(hWnd, 0);
					return 1;
			}
		}	

	return 0;
}



BOOL CALLBACK DialogProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) 
{
	char BAIXO[2] = "\0", CIMA[2]  = "\0", ESQ[2]  = "\0", DIR[2]  = "\0";
	char aux[200] = "\0";

	CIMA[0] = tCIMA; BAIXO[0] = tBAIXO; ESQ[0] = tESQUERDA; DIR[0] = tDIREITA;


	switch(messg) 
	{
	
	case WM_INITDIALOG:	

	SetDlgItemText(hWnd, IDC_CIMA, CIMA);
	SetDlgItemText(hWnd, IDC_BAIXO, BAIXO);
	SetDlgItemText(hWnd, IDC_ESQUERDA, ESQ);
	SetDlgItemText(hWnd, IDC_DIREITA, DIR);



		case WM_COMMAND:
			switch(LOWORD(wParam)) 
			{
				case IDOK:
					GetDlgItemText(hWnd, IDC_CIMA, aux, 2);
					tCIMA=aux[0];
					GetDlgItemText(hWnd, IDC_BAIXO, aux, 2);
					tBAIXO=aux[0];
					GetDlgItemText(hWnd, IDC_ESQUERDA, aux, 2);
					tESQUERDA=aux[0];
					GetDlgItemText(hWnd, IDC_DIREITA, aux, 2);
					tDIREITA=aux[0];

					EndDialog(hWnd, 0);
					return 1;

				case IDCANCEL:
					EndDialog(hWnd, 0);
					return 1;
			}
		}	

	return 0;
}

BOOL CALLBACK DialogProc2(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) 
{
	int aux;
	
	switch(messg) 
	{	
		case WM_INITDIALOG:	
	SetDlgItemInt(hWnd, IDC_Objectos, BolasJogo, TRUE);
	SetDlgItemInt(hWnd, IDC_GHOSTS, FantasmasJogo, TRUE);

		case WM_COMMAND:
			switch(LOWORD(wParam)) 
			{
				case IDOK:					
					aux = GetDlgItemInt(hWnd, IDC_Objectos,FALSE,FALSE);	
					if(aux<=0 || aux>=138)
					{
						MessageBox(hWnd,"O número máximo de objectos é 137!","XPacMan",MB_OK);
						SetDlgItemInt(hWnd, IDC_Objectos, BolasJogo, TRUE);
						return 0;
					}else{ BolasJogo=aux; }

					aux = GetDlgItemInt(hWnd, IDC_GHOSTS,FALSE,FALSE);	
					if(aux<=0 || aux>20)
					{
						MessageBox(hWnd,"O número máximo de fantasmas é 20!","XPacMan",MB_OK);
						SetDlgItemInt(hWnd, IDC_GHOSTS, FantasmasJogo, TRUE);
						return 0;
					}else{ FantasmasJogo=aux; }

					
					EndDialog(hWnd, 0);
					return 1;

				case IDCANCEL:
					EndDialog(hWnd, 0);
					return 1;
			}
		}	

	return 0;
}

BOOL CALLBACK Pontuacao(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) 
{
	char buffer[256] = "\0";

	WaitForSingleObject(hMutex1,500); 


	if(tjogadores==1){
	sprintf_s(buffer, 256, "Pontuações:\nJogador [%s] .................... %d",pacupdates[1].utilizador,pacupdates[1].pontuacao);
	}
	if(tjogadores==2){
	sprintf_s(buffer, 256, "Pontuações:\nJogador [%s] .................... %d\nJogador [%s] .................... %d",
		pacupdates[1].utilizador,pacupdates[1].pontuacao,pacupdates[2].utilizador,pacupdates[2].pontuacao);
	}
	if(tjogadores==3){
		sprintf_s(buffer, 256, "Pontuações:\nJogador [%s] .................... %d\nJogador [%s] .................... %d\nJogador [%s] .................... %d",
			pacupdates[1].utilizador,pacupdates[1].pontuacao,pacupdates[2].utilizador,pacupdates[2].pontuacao,pacupdates[3].utilizador,pacupdates[3].pontuacao);
	}

	ReleaseMutex(hMutex1); 



	switch(messg) 
	{
	case WM_INITDIALOG:


		SetDlgItemText(hWnd, IDC_mostraP,(LPCSTR) buffer);
		break;	
	}	
return 0;
}

BOOL CALLBACK DialogHSCORES(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) 
{
	//int i,indice;

	switch(messg) 
	{

		case WM_INITDIALOG:

			SetDlgItemText(hWnd, IDC_EDIT1, MSGSCORES);

		case WM_COMMAND:
			switch(LOWORD(wParam)) 
			{
			case IDOK:
				EndDialog(hWnd, 0);				
				return 1;
			}
	}

	return 0;
}


BOOL CALLBACK DialogJogosCriar(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) 
{
	int i,totalJogos=0,vemdecriar=0,indice;
	DWORD n;




	switch(messg) 
	{	
	case WM_INITDIALOG:	


		if (!ReadFile(hPipe, &MSGSCORES,(DWORD) sizeof(MSGSCORES),&n,NULL))	{
			MessageBox(hWnd, "Não foi possível ler os High Scores!", "Erro", MB_OK);
			exit(1);
		}

		
		if (!ReadFile(hPipe, &totalJogos,(DWORD) sizeof(int),&n,NULL))	{
			MessageBox(hWnd, "Não foi possível ler do pipe! [4]", "Erro", MB_OK);
			exit(1);
		}
		

		if(totalJogos!=0){

			if (!ReadFile(hPipe, &jogosDetalhes,(DWORD) sizeof(jogosDetalhes),&n,NULL))	{
				MessageBox(hWnd, "Não foi possível ler do pipe! [5]", "Erro", MB_OK);
				exit(1);
			}
	  }


		for(i=1; i<=totalJogos; i++){
			SendDlgItemMessage(hWnd,IDC_LIST1,LB_ADDSTRING,0,(LPARAM)jogosDetalhes[i]);
				}




	case WM_COMMAND:
		switch(LOWORD(wParam)) 
		{
			
		case IDC_Criarnovo:

			DialogBox(hInstance, (LPCSTR) IDD_dObjectos, hWnd, (DLGPROC) DialogProc2);

			strcpy(MSGLogin,"CRIARNOVOJOGO");

			if (!WriteFile(hPipe, &MSGLogin, (DWORD) sizeof(MSGLogin), &n, NULL)) {
				MessageBox(hWnd, "Não foi possível escrever no pipe [CRIAR]!", "Erro", MB_OK);
				exit(1);
			} 


			if(strcmp(MSGLogin,"CRIARNOVOJOGO")==0)
			{
						
				
				MessageBox(hWnd,"É o primeiro a jogar neste jogo!","Informação",MB_OK);
				primeiroOuEmAndamento=0;	
				

				if (!WriteFile(hPipe, &FantasmasJogo, (DWORD) sizeof(int), &n, NULL)) {
					MessageBox(hWnd, "Não foi possível escrever no pipe!", "Erro", MB_OK);
					exit(1);
				} // ENVIA FANTASMAS

			}
			vemdecriar=1;
		
				
			//SEM BREAK PARA NAO REPETIR A CRIACAO DOS PIPES

		case IDC_ENTRARJOGO:

			if(vemdecriar==0){

			strcpy(MSGLogin,"");

			indice=SendDlgItemMessage(hWnd,IDC_LIST1,LB_GETCURSEL,0,0);
			SendDlgItemMessage(hWnd,IDC_LIST1,LB_GETTEXT,indice,(LPARAM)MSGLogin);

			if(strcmp(MSGLogin,"Jogo 1 (Cheio)")==0 || strcmp(MSGLogin,"Jogo 2 (Cheio)")==0 
				|| strcmp(MSGLogin,"Jogo 3 (Cheio)")==0 || strcmp(MSGLogin,"Jogo 4 (Cheio)")==0
				 || strcmp(MSGLogin,"Jogo 4 (Cheio)")==0 || strcmp(MSGLogin,"Jogo 5 (Cheio)")==0) {
				MessageBox(hWnd, "Esse jogo ja esta cheio, escolha outro ou crie um novo!", "Erro", MB_OK);
					break;
			}

					if(strcmp(MSGLogin,"Jogo 1 (Terminado)")==0 || strcmp(MSGLogin,"Jogo 2 (Terminado)")==0 
						|| strcmp(MSGLogin,"Jogo 3 (Terminado)")==0 || strcmp(MSGLogin,"Jogo 4 (Terminado)")==0
						|| strcmp(MSGLogin,"Jogo 4 (Terminado)")==0 || strcmp(MSGLogin,"Jogo 5 (Terminado)")==0) {
							MessageBox(hWnd, "Esse jogo ja terminou, escolha outro ou crie um novo!", "Erro", MB_OK);
							break;

			}
			if(strcmp(MSGLogin,"")==0){
				MessageBox(hWnd, "Tem de seleccionar um jogo para entrar!", "Erro", MB_OK);
				break;
			}

		
		
			if (!WriteFile(hPipe, &MSGLogin, (DWORD) sizeof(MSGLogin), &n, NULL)) {
				MessageBox(hWnd, "Não foi possível escrever no pipe [RETOMAR]!", "Erro", MB_OK);
				exit(1);
			}

			
			if(strcmp(MSGLogin,"CRIARNOVOJOGO")!=0)
			{

			MessageBox(hWnd,"Vai entrar num jogo ja em curso!","Informação",MB_OK);
			primeiroOuEmAndamento=1;	

			}

			}

			if (!ReadFile(hPipe, &souOclienteN,(DWORD) sizeof(int),&n,NULL))	{
				MessageBox(hWnd, "Erro a receber num de cliente", "Erro", MB_OK);
				exit(1);
			}

		


			sprintf(PIPE_NAME_GHOTS,"\\\\%s\\pipe\\xghosts",login.ip);
			sprintf(PIPE_NAME_PACS,"\\\\%s\\pipe\\xpacs",login.ip);
			sprintf(PIPE_NAME_ENVIAPAC,"\\\\%s\\pipe\\xpac",login.ip);

			Sleep(50);
			if(hPipeFantasmas==NULL){	
				hPipeFantasmas = CreateFile(PIPE_NAME_GHOTS, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hPipeFantasmas==NULL) { MessageBox(hWnd, "Error a abrir pipe do servidor para actualizar ghosts!", "Erro", MB_OK); exit(1); }
			}
			Sleep(50);
			if(hPipePacs==NULL){	
				hPipePacs = CreateFile(PIPE_NAME_PACS, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hPipePacs==NULL) { MessageBox(hWnd, "Error a abrir pipe do servidor para actualizar pacs!", "Erro", MB_OK); exit(1); }
			}

			Sleep(50);
			if(hPipeEnviaPac==NULL){	
				hPipeEnviaPac = CreateFile(PIPE_NAME_ENVIAPAC, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hPipeEnviaPac==NULL) { MessageBox(hWnd, "Error a abrir pipe do servidor para enviar pac!", "Erro", MB_OK); exit(1); }
			}


			EndDialog(hWnd, 0);
			return 1;
			break;



		case IDCANCEL:
			EndDialog(hWnd, 0);
			exit(1);
			return 1;
		}
	}	

	return 0;
}


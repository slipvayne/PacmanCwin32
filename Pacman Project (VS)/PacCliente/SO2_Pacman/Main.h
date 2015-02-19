char PIPE_NAME[50] , PIPE_NAME_GHOTS[50], PIPE_NAME_PACS[50], PIPE_NAME_ENVIAPAC[50]; 

void criaMapa();
void imprime(HWND hWnd);
void moverPacman(HWND hWnd);
void mostraGhosts(HWND hWnd);
void alteraPac(HWND hWnd, int xInic,int yInic, int xFinal, int yFinal,int posInic, int posFinal);
void mostraPontuacao(HWND hWnd);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc2(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Pontuacao(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogLogin(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogJogosCriar(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogHSCORES(HWND, UINT, WPARAM, LPARAM);

static char jogosDetalhes[6][100];

static DWORD registoI;
static HKEY chave;

static HDC memdc;
static HBRUSH hbrush;
static HINSTANCE hInstance;
static HBITMAP hbit;
static HACCEL atalhos;
static HWND hDlg;
static HANDLE hPipe, 
	hPipeFantasmas, hPipePacs, hPipeEnviaPac, 
	ThreadFantasmas, ThreadPacs, ThreadEnviaPac, hMutex1;

static int pacDir, // 1-direita, 2-esquerda, 3-cima, 4-baixo
	pontuacao=0,
	estadoJogo=-1,
	verPont=0,
	BolasJogo=100,
	FantasmasJogo=3,
	logado=0,
	esteveEmJogo=0,
	primeiroOuEmAndamento=-1,
	tjogadores=1, // 0 se primeiro, 1 se em andamento
	souOclienteN,
	tamanhoK;

static char tCIMA='W',
	tBAIXO='S',
	tESQUERDA='A',
	tDIREITA='D',
	buffer[200]="\0", MSGLogin[256]="\0", MSGSCORES[512]="\0";;

char szProgName[8] = "XPacman";


typedef struct{
	int x;
	int y;
	int bitmap;
	int tipo;
	int jogador;
}MAP;
static MAP mapa[280];
static MAP ghosts[20];

typedef struct{
	int x;
	int y;
	int pos;
	int bitmap;
	int pontuacao;
	int emJogo;
	char utilizador[25];
	int clientJogoON;
}PACMAN;
static PACMAN pac;
static PACMAN pacupdates[4];



typedef struct{
	char ip[16];
	char utilizador[25];
	char password[25];
	int COD; //COD extra caso necessario
}DADOS;
static DADOS login;







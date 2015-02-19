typedef struct{
	char ip[16]; //so necessario para o client 
	char utilizador[25];
	char password[25];
	int COD; //COD extra caso necessario (para n haver mais read/write escusadamente)
}DADOS;

typedef struct{
	char utilizador[25];
	char password[25];
	int logado;
	int pontuacaoTotal;
	int jogosTotal;	
}JOGADOR;
static JOGADOR jogadores[100]; 

typedef struct{
	int x;
	int y;
	int bitmap;
	int tipo;
	int jogador;
}MAP;
static MAP mapa[6][280];
static MAP fantasmas[6][20]; 

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
static PACMAN pacupdates[6][4];

static HANDLE hMutex1, 
	hPipeFantasmas[6][4],hPipePacs[6][4],hPipeBonus[6][4],hPipeRecebePac[6][4],
	hThreadFantasmas[6],hThreadPacs[6],hThreadBonus[6],Threads2[6][4],
	hVerificaPosicao;

static int jogadoresMAX=4,
	numJogadoresRegistados=0,
	fantasmasTotal[6]={0}, 
	totalJogadores[6] = {0}, 
	totalJogadoresTOTAL=0,
	status,
	jogoON[6]={-1},
	totalJogos=0,
	jogoEscolhido=0;

static char jogosDetalhes[6][100], MSGSCORES[512]="\0";

static BOOL servidorON;

static SECURITY_ATTRIBUTES sa;


DWORD WINAPI AtendeCliente2(LPVOID);
DWORD WINAPI ActualizarFantasmas(LPVOID);
DWORD WINAPI ActualizarPacs(LPVOID);
DWORD WINAPI VerificaBonus(LPVOID);
DWORD WINAPI AtendeCliente1(LPVOID);

JOGADOR LerJogadores();
void ESCREVEJogadores();

#define IDB_pacBaixo                    110
#define IDB_pacCima                     111
#define IDB_pacDir                      112
#define IDB_pacEsq                      113
#define IDB_INIC                        116
#define IDB_Bola                        117
#define IDB_pCaminho                    118
#define IDB_pParede                     119
#define IDB_pDireita                    120
#define IDB_pEsquerda                   121
#define IDB_pHoriz                      122
#define IDB_pHorizEsq                   123
#define IDB_pInferior                   124
#define IDB_pInfDir                     125
#define IDB_pInfEsq                     126
#define IDB_pSuperior                   127
#define IDB_pSupDir                     128
#define IDB_pVertical                   129
#define IDB_pVertBaixo                  130
#define IDB_pVertCima                   131
#define IDB_pHorizDir                   132
#define IDB_pSupEsq                     133
#define IDB_pSaida                      136
#define IDB_gAzul                       144
#define IDB_gBranco                     145
#define IDB_gRosa                       146
#define IDB_gVerde                      147
#define IDB_gVermelho                   148
#define IDB_pacDead                     149



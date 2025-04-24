/*
Título do Projeto: Quiz de Conhecimentos Gerais
Autor: Diogo Bilreiro & Ricardo Evans
Data e hora: 09-04-2025 10:00
Descrição: Um quiz interativo com dois níveis de dificuldade.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <locale.h>
#include <windows.h> // Para system("cls") no Windows

#define MAX_NOME 50
#define MAX_PERGUNTA 256
#define MAX_RESPOSTAS 4
#define MAX_JOGADORES 100
#define MAX_QUIZ 20

// ANSI cores
#define ANSI_RESET "\x1b[0m"
#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_PURPLE "\e[0;35m"

typedef enum
{
    FACIL = 1,
    MEDIO,
    DIFICIL
} Dificuldade;
typedef enum
{
    MULTIPLA,
    NUMERICA,
    VF
} TipoPergunta;
typedef struct
{
    char nome[MAX_NOME];
    int pontuacao;
} Jogador;

typedef struct
{
    char texto[MAX_PERGUNTA];
    TipoPergunta tipo;
    Dificuldade dificuldade;
    char opcoes[MAX_RESPOSTAS][MAX_PERGUNTA]; // apenas se for MULTIPLA
    int correta;                              // índice da resposta correta ou valor correto
} Pergunta;

void limparTela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void identificarJogador(char *nomeJogador)
{
    printf("Digite o seu nome ou nickname: ");
    fgets(nomeJogador, MAX_NOME, stdin);
    nomeJogador[strcspn(nomeJogador, "\n")] = 0; // remover \n
}

int carregarPerguntas(Pergunta perguntas[], const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir o ficheiro");
        return 0;
    }

    int total = 0;
    while (fscanf(file, "%d %d\n", (int *)&perguntas[total].tipo, (int *)&perguntas[total].dificuldade) == 2)
    {
        fgets(perguntas[total].texto, MAX_PERGUNTA, file);
        perguntas[total].texto[strcspn(perguntas[total].texto, "\n")] = 0;

        if (perguntas[total].tipo == MULTIPLA)
        {
            for (int i = 0; i < MAX_RESPOSTAS; i++)
            {
                fgets(perguntas[total].opcoes[i], MAX_PERGUNTA, file);
                perguntas[total].opcoes[i][strcspn(perguntas[total].opcoes[i], "\n")] = 0;
            }
        }

        fscanf(file, "%d\n", &perguntas[total].correta);
        total++;
    }

    fclose(file);
    return total;
}

int carregarPerguntasBinario(Pergunta perguntas[]) {
    FILE *file = fopen("perguntas.bin", "rb");
    if (!file) return 0;
    int total = fread(perguntas, sizeof(Pergunta), 100, file);
    fclose(file);
    return total;
}

void salvarPerguntasBinario(Pergunta perguntas[], int total) {
    FILE *file = fopen("perguntas.bin", "wb");
    if (!file) {
        perror("Erro ao salvar no ficheiro binário");
        return;
    }
    fwrite(perguntas, sizeof(Pergunta), total, file);
    fclose(file);
}

void salvarHistorico(const char *nomeJogador, int pontuacao)
{
    FILE *file = fopen("historico.txt", "a");
    if (!file)
        return;

    time_t agora = time(NULL);
    struct tm *info = localtime(&agora);
    char dataHora[100];
    strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", info);

    fprintf(file, "%s | %s | Pontuacao: %d\n", nomeJogador, dataHora, pontuacao);
    fclose(file);
}

void mostrarHistorico(const char *nomeJogador)
{
    FILE *file = fopen("historico.txt", "r");
    if (!file)
    {
        printf(ANSI_RED "Nenhum historico encontrado.\n" ANSI_RESET);
        return;
    }

    char linha[256];
    printf(ANSI_YELLOW ANSI_BOLD"=== SEU HISTORICO DE QUIZ ===\n\n"ANSI_RESET);
    int encontrouHistorico = 0; // Variável para verificar se há histórico para o jogador

    while (fgets(linha, sizeof(linha), file))
    {
        char nomeNoHistorico[MAX_NOME];
        // Tenta ler o nome do jogador no início da linha
        if (sscanf(linha, "%[^|]", nomeNoHistorico) == 1)
        {
            // Remove espaços em branco no final do nome lido do histórico
            nomeNoHistorico[strcspn(nomeNoHistorico, " \r\n")] = 0;

            // Compara o nome lido com o nome do jogador atual
            if (strcmp(nomeNoHistorico, nomeJogador) == 0)
            {
                printf(ANSI_BOLD "%s" ANSI_RESET, linha);
                encontrouHistorico = 1;
            }
        }
    }

    fclose(file);

    if (!encontrouHistorico)
    {
        printf(ANSI_RED "Nenhum historico encontrado para %s.\n" ANSI_RESET, nomeJogador);
    }

    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
}

int carregarRanking(Jogador ranking[])
{
    FILE *file = fopen("ranking.txt", "r");
    int total = 0;
    if (!file)
        return 0;

    while (fscanf(file, "%[^|]|%d\n", ranking[total].nome, &ranking[total].pontuacao) == 2)
    {
        total++;
    }

    fclose(file);
    return total;
}

void salvarRanking(Jogador ranking[], int total)
{
    FILE *file = fopen("ranking.txt", "w");
    if (!file)
        return;

    for (int i = 0; i < total && i < 10; i++)
    {
        fprintf(file, "%s|%d\n", ranking[i].nome, ranking[i].pontuacao);
    }

    fclose(file);
}

void atualizarRanking(const char *nome, int pontuacao)
{
    Jogador ranking[11]; // até 11 para incluir o novo
    int total = carregarRanking(ranking);

    strncpy(ranking[total].nome, nome, MAX_NOME);
    ranking[total].pontuacao = pontuacao;
    total++;

    // Ordenar decrescente
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = i + 1; j < total; j++)
        {
            if (ranking[j].pontuacao > ranking[i].pontuacao)
            {
                Jogador tmp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = tmp;
            }
        }
    }

    if (total > 10)
        total = 10;
    salvarRanking(ranking, total);
}

void mostrarRanking()
{
    Jogador ranking[10];
    int total = carregarRanking(ranking);

    printf(ANSI_PURPLE "=== RANKING DOS MELHORES JOGADORES ===\n\n" ANSI_RESET);
    for (int i = 0; i < total; i++)
    {
        printf(ANSI_BOLD "%dº - %s: %d pontos\n" ANSI_RESET, i + 1, ranking[i].nome, ranking[i].pontuacao);
    }
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
}

int obterRespostaMultipla(int tempoLimiteSegundos)
{
    time_t inicio = time(NULL);
    char buffer[10] = {0};
    int pos = 0;

    printf("Você tem %d segundos para responder (1 a 4): ", tempoLimiteSegundos);
    fflush(stdout);

    while (1)
    {
        if (_kbhit())
        {
            char c = _getch();
            if (c == '\r')
                break; // ENTER
            if (c >= '1' && c <= '4' && pos == 0)
            {
                buffer[pos++] = c;
                printf("%c", c);
            }
        }
        if (difftime(time(NULL), inicio) >= tempoLimiteSegundos)
        {
            printf("\nTempo esgotado!\n");
            return -1;
        }
    }

    buffer[pos] = '\0';
    printf("\n");
    return atoi(buffer);
}

int obterRespostaNumerica(int tempoLimiteSegundos)
{
    time_t inicio = time(NULL);
    char buffer[20] = {0};
    int pos = 0;

    printf("Você tem %d segundos para responder (número): ", tempoLimiteSegundos);
    fflush(stdout);

    while (1)
    {
        if (_kbhit())
        {
            char c = _getch();
            if (c == '\r')
                break;
            if ((c >= '0' && c <= '9') || (c == '-' && pos == 0))
            {
                if (pos < sizeof(buffer) - 1)
                {
                    buffer[pos++] = c;
                    printf("%c", c);
                }
            }
        }
        if (difftime(time(NULL), inicio) >= tempoLimiteSegundos)
        {
            printf("\nTempo esgotado!\n");
            return -999999; // código de erro
        }
    }

    buffer[pos] = '\0';
    printf("\n");
    return atoi(buffer);
}

char obterRespostaVF(int tempoLimiteSegundos)
{
    time_t inicio = time(NULL);

    printf("Você tem %d segundos para responder (V/F): ", tempoLimiteSegundos);
    fflush(stdout);

    while (1)
    {
        if (_kbhit())
        {
            char c = _getch();
            if (c == 'V' || c == 'v' || c == 'F' || c == 'f')
            {
                printf("%c\n", c);
                return c;
            }
        }
        if (difftime(time(NULL), inicio) >= tempoLimiteSegundos)
        {
            printf("\nTempo esgotado!\n");
            return 'X'; // código de erro
        }
    }
}

void jogarQuiz(Pergunta perguntas[], int total, const char *nomeJogador)
{
    limparTela();

    int dificuldade, qtd;
    char op;
    do
    {
        limparTela();
        printf(ANSI_CYAN "=============================\n" ANSI_RESET);
        printf(ANSI_CYAN "=== ESCOLHA A DIFICULDADE ===\n" ANSI_RESET);
        printf(ANSI_CYAN "=============================\n" ANSI_RESET);
        printf(ANSI_BLUE ANSI_BOLD "  [1] " ANSI_RESET "- Facil\n");
        printf(ANSI_BLUE ANSI_BOLD "  [2] " ANSI_RESET "- Medio\n");
        printf(ANSI_BLUE ANSI_BOLD "  [3] " ANSI_RESET "- Dificil\n");
        printf(ANSI_RED ANSI_BOLD "  [4] " ANSI_RESET "- Voltar ao Nenu\n");
        printf("\nDigite sua opcao (1-4): ");


        char tecla = getch(); // Captura sem ENTER
        printf("%c\n", tecla); // Mostra a tecla pressionada

        if (tecla == '4') {
            return; // Volta ao menu
        }

        dificuldade = tecla - '0';

        if (dificuldade < 1 || dificuldade > 3)
        {
            Beep(750, 300); // som de erro
            system("COLOR C"); // mudança de cor ao erro
            printf("\nOpcao invalida! Por favor, escolha entre (1-4).\n");
            Sleep(1000);
        }
        else
        {
            break;
        }
    } while (1);

    limparTela();
    printf("Quantas perguntas quer responder? ");
    scanf("%d", &qtd);
    getchar();

    int pontos = 0, respondidas = 0;
    srand(time(NULL));

    int usadas[total]; // Array para controlar as perguntas usadas
    for (int i = 0; i < total; i++) {
        usadas[i] = 0; // Inicializa todas como não usadas
    }

    while (respondidas < qtd)
    {
        int indice_pergunta;
        int tentativas = 0;
        do {
            indice_pergunta = rand() % total;
            tentativas++;
            if (tentativas > total * 2) { // Evita loop infinito se não houver perguntas da dificuldade desejada
                printf(ANSI_YELLOW "\nNão há mais perguntas da dificuldade selecionada.\n" ANSI_RESET);
                goto fim_quiz; 
            }
        } while (perguntas[indice_pergunta].dificuldade != dificuldade || usadas[indice_pergunta]);

        usadas[indice_pergunta] = 1; // Marca a pergunta como usada

        Pergunta p = perguntas[indice_pergunta];
        printf(ANSI_BOLD "\nPergunta: %s\n", p.texto, ANSI_RESET);

        if (p.tipo == MULTIPLA)
        {
            int ordem[] = {0, 1, 2, 3};
            for (int j = 0; j < 4; j++)
            {
                int k = rand() % 4;
                int tmp = ordem[j];
                ordem[j] = ordem[k];
                ordem[k] = tmp;
            }

            for (int j = 0; j < 4; j++)
            {
                printf("%d) %s\n", j + 1, p.opcoes[ordem[j]]);
            }

            int r = obterRespostaMultipla(15);

            if (r == -1)
            {
                printf("Você não respondeu a tempo. A resposta correta era: %s\n", p.opcoes[p.correta]);
            }
            else if (ordem[r - 1] == p.correta)
            {
                printf("Correto!\n");
                pontos += 2 * dificuldade;
            }
            else
            {
                system("COLOR C"); // muda cor para vermelho ao errar
                printf("Errado | Resposta Correta: %s\n", p.opcoes[p.correta]);
                Sleep(1000);         // pausa para o jogador ver a cor
                system("COLOR 0F"); // volta à cor padrão
            }

        }
        else if (p.tipo == NUMERICA)
        {
            int r = obterRespostaNumerica(20);

            if (r == -999999)
            {
                printf("Você não respondeu a tempo. A resposta correta era: %d\n", p.correta);
            }
            else if (r == p.correta)
            {
                printf("Correto!\n");
                pontos += 4 * dificuldade;
            }
            else
            {
                system("COLOR C");
                printf("Errado | Resposta correta: %d\n", p.correta);
                Sleep(1000);
                system("COLOR 0F");
            }

        }
        else if (p.tipo == VF)
        {
            char r = obterRespostaVF(10);

            if (r == 'X')
            {
                printf("Você não respondeu a tempo.\n");
            }
            else if ((r == 'V' || r == 'v') && p.correta == 1 ||
                     (r == 'F' || r == 'f') && p.correta == 0)
            {
                printf("Correto!\n");
                pontos += 6 * dificuldade;
            }
            else
            {
                system("COLOR C");
                printf("Errado!\n");
                Sleep(1000);
                system("COLOR 0F");
            }

        }
        respondidas++;
    }

fim_quiz:
    printf("\n%s, A sua pontuação final é de: %d\n", nomeJogador, pontos);
    salvarHistorico(nomeJogador, pontos);
    atualizarRanking(nomeJogador, pontos);
}

void adicionarPergunta(Pergunta perguntas[], int *total)
{
    limparTela();
    Pergunta nova;
    int tipo, dificuldade;

    // Tipo de pergunta
    do
    {
        printf(ANSI_CYAN ANSI_BOLD"===============================\n" ANSI_RESET);
        printf(ANSI_CYAN ANSI_BOLD"=== ADICIONAR NOVA PERGUNTA ===\n" ANSI_RESET);
        printf(ANSI_CYAN ANSI_BOLD"===============================\n\n" ANSI_RESET);
        printf(ANSI_BLUE ANSI_BOLD "  [0] " ANSI_RESET "- Escolha Multipla\n");
        printf(ANSI_BLUE ANSI_BOLD "  [1] " ANSI_RESET "- Numerica\n");
        printf(ANSI_BLUE ANSI_BOLD "  [2] " ANSI_RESET "- Verdadeiro ou Falso\n");
        printf(ANSI_RED ANSI_BOLD "  [3] " ANSI_RESET "- Voltar ao Menu\n");
        printf("Escolha: ");


        char tecla = _getch();  // Captura uma tecla
        printf("%c\n", tecla);  // Mostra a tecla pressionada

        if (tecla == '3')
        {
            return;
        }

        if (tecla >= '0' && tecla <= '2')
        {
            tipo = tecla - '0';
            nova.tipo = tipo;
            break;
        }
        else
        {
            Beep(750, 300);        // beep - som de erro
            system("COLOR 0C");    // muda cor para vermelho (erro)
            printf("Opcao invalida! Pressione 0, 1 ou 2.\n");
            Sleep(1000);           // espera 1 segundo
            limparTela();         // limpa a tela
            system("COLOR 0F");    // volta à cor padrão
        }
    } while (1);

    int Dificuldade;
    do
    {
        limparTela();
        printf(ANSI_CYAN "=============================\n" ANSI_RESET);
        printf(ANSI_CYAN "=== ESCOLHA A DIFICULDADE ===\n" ANSI_RESET);
        printf(ANSI_CYAN "=============================\n" ANSI_RESET);
        printf(ANSI_BLUE ANSI_BOLD "  [1] " ANSI_RESET "- Facil\n");
        printf(ANSI_BLUE ANSI_BOLD "  [2] " ANSI_RESET "- Medio\n");
        printf(ANSI_BLUE ANSI_BOLD "  [3] " ANSI_RESET "- Dificil\n");
        printf("\nEscolha: ");

        char tecla = getch(); // Captura sem ENTER
        printf("%c\n", tecla); // Mostra a tecla pressionada

        if (tecla >= '1' && tecla <= '3')
        {
            dificuldade = tecla - '0'; // Converte o caractere '1', '2' ou '3' para o inteiro 1, 2 ou 3
            nova.dificuldade = dificuldade;
            break;
        }
        else
        {
            Beep(750, 300); // Som de erro
            system("COLOR C"); // Muda a cor para indicar erro
            printf("Dificuldade invalida! Por favor, escolha entre 1 e 3.\n");
            Sleep(1000); // Espera um pouco para o utilizador ver a mensagem
            system("COLOR 0F"); // Volta à cor padrão
        }
    } while (1);

    // Texto da pergunta
    limparTela();
    printf("Digite o texto da pergunta:\n> ");
    fgets(nova.texto, MAX_PERGUNTA, stdin);
    nova.texto[strcspn(nova.texto, "\n")] = 0;

    if (nova.tipo == MULTIPLA)
    {
        printf("Digite as 4 opcoes:\n");
        for (int i = 0; i < 4; i++)
        {
            printf("Opcao %d: ", i + 1);
            fgets(nova.opcoes[i], MAX_PERGUNTA, stdin);
            nova.opcoes[i][strcspn(nova.opcoes[i], "\n")] = 0;
        }

        do
        {
            printf("Qual o numero da resposta correta (1-4)? ");
            if (scanf("%d", &nova.correta) != 1 || nova.correta < 1 || nova.correta > 4)
            {
                printf("Opcao invalida!\n");
                while (getchar() != '\n');
                continue;
            }
            nova.correta -= 1; // ajustar para índice 0
            break;
        } while (1);
        getchar(); // limpa buffer
    }
    else if (nova.tipo == NUMERICA)
    {
        printf("Digite a resposta correta (valor numérico): ");
        scanf("%d", &nova.correta);
        getchar();
    }
    else if (nova.tipo == VF)
    {
        do
        {
            char r;
            printf("Resposta correta é Verdadeiro (V) ou Falso (F)? ");
            scanf(" %c", &r);
            getchar();
            if (r == 'V' || r == 'v')
            {
                nova.correta = 1;
                break;
            }
            else if (r == 'F' || r == 'f')
            {
                nova.correta = 0;
                break;
            }
            else
            {
                printf("Resposta inválida!\n");
            }
        } while (1);
    }

    // Adiciona à lista e grava no ficheiro
    perguntas[*total] = nova;
    (*total)++;

    FILE *file = fopen("perguntas.txt", "a");
    if (file)
    {
        fprintf(file, "%d %d\n", nova.tipo, nova.dificuldade);
        fprintf(file, "%s\n", nova.texto);
        if (nova.tipo == MULTIPLA)
        {
            for (int i = 0; i < 4; i++)
            {
                fprintf(file, "%s\n", nova.opcoes[i]);
            }
        }
        fprintf(file, "%d\n", nova.correta);
        fclose(file);
    }
    else
    {
        printf("Erro ao gravar no ficheiro.\n");
    }

    printf("\nPergunta adicionada com sucesso!\n");
    printf("Pressione ENTER para continuar...");
    getchar();
}

int main()
{
    Pergunta perguntas[100];
    char nomeJogador[MAX_NOME];
    int totalPerguntas = carregarPerguntasBinario(perguntas);

    if (totalPerguntas == 0) {
        totalPerguntas = carregarPerguntas(perguntas, "perguntas.txt");
        if (totalPerguntas > 0) {
            salvarPerguntasBinario(perguntas, totalPerguntas); // salva txt -> binário
        }
    }

    if (totalPerguntas == 0) {
        printf("Nenhuma pergunta carregada.\n");
        return 1;
    }

    identificarJogador(nomeJogador);

    int opcao;
    do
    {
        setlocale(LC_ALL, "Portuguese");
        limparTela();
        printf(ANSI_CYAN "=================================================\n" ANSI_RESET);
        printf(ANSI_BOLD ANSI_GREEN "         BEM-VINDO(A) AO QUIZ INTERATIVO        \n" ANSI_RESET);
        //printf("        Realizado por Diogo.B e Ricardo.E\n" ANSI_RESET);
        printf(ANSI_CYAN "=================================================\n" ANSI_RESET);

        printf(ANSI_YELLOW "Olá, %s! Bem-vindo(a) ao jogo.\n\n" ANSI_RESET, nomeJogador);

        printf(ANSI_BOLD "Escolha uma opcao:\n\n" ANSI_RESET);

        printf(ANSI_BLUE ANSI_BOLD "  [1] " ANSI_RESET "Jogar Quiz\n");
        printf(ANSI_BLUE ANSI_BOLD "  [2] " ANSI_RESET "Instrucoes de utilizacao do quiz\n");
        printf(ANSI_BLUE ANSI_BOLD "  [3] " ANSI_RESET "Adicionar pergunta\n");
        printf(ANSI_BLUE ANSI_BOLD "  [4] " ANSI_RESET "Ver historico\n");
        printf(ANSI_BLUE ANSI_BOLD "  [5] " ANSI_RESET "Ver ranking\n");
        printf(ANSI_RED  ANSI_BOLD "  [6] " ANSI_RESET "Sair\n");

        // Usando getch() para capturar a opção
        printf("\n" ANSI_BOLD "Digite a opcao desejada: " ANSI_RESET);
        char tecla = _getch();  // Espera a tecla ser pressionada
        opcao = tecla - '0';    // Converte o caractere para número (exemplo: '1' -> 1)

        switch (opcao)
        {
        case 1:
            jogarQuiz(perguntas, totalPerguntas, nomeJogador);
            printf("\nPressione qualquer tecla para voltar ao menu...");
            opcao = getch();  // Aguarda o usuário pressionar qualquer tecla para voltar
            break;
        case 2:
            limparTela();
            printf("Este quiz e maioritariamente sobre perguntas de cultura geral, \ngrande parte das perguntas foram geradas por inteligencia artificial. \n\n- Para responder as respostas coloque Verdadeiro(V) ou Falso(F).\n");
            printf("\nPressione qualquer tecla para voltar ao menu.");
            _getch();  // Aguarda o usuário pressionar qualquer tecla para voltar
            break;
        case 3:
            adicionarPergunta(perguntas, &totalPerguntas);
            break;
        case 4:
            limparTela();
            mostrarHistorico(nomeJogador);
            break;
        case 5:
            limparTela();
            mostrarRanking();
            break;
        case 6:
            limparTela();
            printf("\nA sair, obrigado por jogar.\n\n");
            break;
        default:
            Beep(750, 300); // beep - som de erro
            system("COLOR C"); // mudança de cor ao erro
            printf(ANSI_RED "\nOpcao invalida! Tente novamente.\n" ANSI_RESET);
            Sleep(1250);
            break;
        }

    } while (opcao != 6);

    return 0;
}

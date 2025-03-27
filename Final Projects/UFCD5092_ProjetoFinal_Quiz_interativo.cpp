/*
Título do Projeto: Quiz de Conhecimentos Gerais
Autor: Diogo Bilreiro
Data e hora: 21-03-2025 10:00
Descrição: Um quiz interativo com dois níveis de dificuldade.
*/
#include <stdio.h>
#include <stdlib.h> // gerar números aleatórios
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h> // Para usar o tipo bool

int main()
{

    char opcao, escolha;
    bool sair = true;
    char opcao_quiz;
    bool opcao_valida = false;
    int respostaCerta = 0;
    int counter_quiz = 0;

    char respostaErrada[25][200] = {};

    char perguntaFacil[25][2][200] = {
        {"A Grande Muralha da China e visivel do espaco.", "F"},
        {"Os morcegos sao cegos.", "F"},
        {"A agua ferve a 100 graus Celsius ao nivel do mar", "V"},
        {"O Monte Evereste e a montanha mais alta do sistema solar.", "F"},
        {"Os diamantes sao feitos de carbono puro.", "V"},
        {"Os gatos tem visao noturna perfeita.", "F"},
        {"O ouro e um bom condutor de eletricidade.", "V"},
        {"Os camaleoes mudam de cor para se camuflarem.", "V"},
        {"A luz viaja mais rapido que o som.", "V"},
        {"Os tubaroes podem sentir uma gota de sangue a quilometros de distancia.", "V"},
        {"A penicilina foi descoberta por Alexander Fleming.", "V"},
        {"Os ursos polares sao canhotos.", "F"},
        {"O planeta Marte e conhecido como o Planeta vermelho.", "V"},
        {"As impressoes digitais de cada pessoa sao unicas.", "V"},
        {"Os peixes dourados tem uma memoria de tres segundos.", "F"},
        {"A Terra e o unico planeta do sistema solar com agua liquida na superficie.", "V"},
        {"Os humanos usam apenas 10% do seu cerebro.", "F"},
        {"As cobras podem ouvir musica.", "F"},
        {"O acido citrico e encontrado em frutas citricas como limoes e laranjas.", "V"},
        {"As estrelas cadentes sao meteoros que entram na atmosfera da Terra.", "V"},
        {"Os elefantes tem medo de ratos.", "F"},
        {"A gravidade e mais forte na Lua do que na Terra.", "F"},
        {"As borboletas provam com os pes.", "V"},
        {"O mel nunca estraga.", "V"},
        {"As plantas respiram oxigenio e libertam dioxido de carbono.", "V"},
    };

    char perguntaDificil[25][2][200] = {

        {"A teoria da relatividade geral de Einstein preve a existencia de buracos brancos.", "V"},
        {"O paradoxo de Fermi sugere que a vida extraterrestre e comum no universo.", "F"},
        {"A entropia do universo esta a diminuir.", "F"},
        {"A mecanica quantica e a relatividade geral sao teorias compativeis.", "F"},
        {"A constante cosmologica representa a energia escura do universo.", "V"},
        {"O efeito Casimir demonstra a existencia de energia do ponto zero no vacuo.", "V"},
        {"A teoria das cordas postula que todas as particulas fundamentais sao vibracoes de cordas unidimensionais.", "V"},
        {"O teorema de Godel prova que todos os sistemas axiomaticos consistentes sao completos.", "F"},
        {"A hipotese de Riemann tem implicacoes na distribuicao dos numeros primos.", "V"},
        {"A criptografia quantica e incondicionalmente segura.", "V"},
        {"A inteligencia artificial geral (AGI) ja foi alcançada.", "F"},
        {"A teoria da evolucao por selecao natural foi proposta por Charles Darwin e Alfred Russel Wallace simultaneamente.", "V"},
        {"O genoma humano e composto por mais de 100.000 genes.", "F"},
        {"A epigenetica estuda as mudanças na expressao genetica que nao envolvem alteracoes na sequencia do ADN.", "V"},
        {"A fotossintese e um processo endotermico.", "V"},
        {"A supercondutividade ocorre apenas a temperaturas proximas do zero absoluto.", "F"},
        {"A fusao nuclear e o processo que alimenta as estrelas.", "V"},
        {"A teoria do caos demonstra que sistemas deterministicos podem ser imprevisiveis.", "V"},
        {"A nanotecnologia permite a manipulacao da materia em escala atomica e molecular.", "V"},
        {"A tecnologia CRISPR-Cas9 permite a edicao precisa do genoma.", "V"},
        {"A teoria da panspermia postula que a vida na Terra teve origem em outros planetas.", "V"},
        {"A teoria da abiogenese explica a origem da vida a partir de materia nao viva.", "V"},
        {"A simulacao de Boltzmann sugere que o universo pode ser uma simulacao computacional.", "V"},
        {"A teoria M unifica todas as cinco teorias das cordas.", "V"},
        {"A computacao quantica pode resolver problemas que sao intrataveis para computadores classicos.", "V"},
    };

    srand(time(NULL));
    int numeroAleatorio[10];
    int counter = 0;

    do
    {
        system("cls");
        system("COLOR 0F");

        for (int i = 1; i <= 48; i++)
        {
            printf("*");
        }
        printf("\n*  Quiz v1.0 - Diogo Bilreiro  |  Menu Inicial *\n");
        for (int i = 1; i <= 44; i++)
        {
            printf("*");
        }
        printf("2025");
        printf("\n\n");

        printf("Escolha uma opcao (1, 2 ou 3): \n");
        printf("1.  Qual o nivel de dificuldade que pretende jogar?\n");
        printf("2.  Instrucoes de utilizacao do quiz.\n");
        printf("3.  Sair.\n\n");
        printf("Digite a sua escolha: ");
        opcao = getche();

        switch (opcao)
        {

        case '1':
            respostaCerta = 0;            // limpar variavel
            numeroAleatorio[10] = {};     // limpar variavel
            respostaErrada[20][100] = {}; // limpar variavel
            counter = 0;                  // resetar counter para entrar no while e ter novas perguntas aleatorias

            // obter 10 numeros aleatórios diferentes
            while (counter < 10)
            {
                int numero = rand() % 25;
                bool rep = false; // caso seja repetido não guarda

                for (int i = 0; i < counter; i++)
                {
                    if (numeroAleatorio[i] == numero)
                    {
                        rep = true;
                        break;
                    }
                }

                if (!rep)
                {
                    numeroAleatorio[counter] = numero;
                    counter++;
                }
            }

            do
            {
                system("cls");
                printf("Escolha o nivel: (10 perguntas)\n1. Nivel Facil\n2. Nivel Dificil\n");
                printf("3. Voltar ao Menu.\n\n");
                printf("Digite a sua escolha: ");
                system("COLOR 0F");
                escolha = getche();

                if (escolha == '3')
                {
                    break; // sair do case 3 e voltar ao menu
                }
                if (escolha != '1' && escolha != '2')
                {
                    Beep(750, 300);     // beep - som de erro
                    system("COLOR 7C"); // mudança de cor ao erro
                    printf("\nOpcao invalida! Tente novamente.\n");
                    Sleep(1250);
                }

            } while (escolha != '1' && escolha != '2');
            if (escolha == '3')
            {
                continue;
            }
            counter_quiz++; // contador de quizzes jogados. Coloquei aqui para apenas contar a escolha 1 e 2.
            escolha -= '0'; // converter para inteiro

            if (escolha == 1)
            {
                // Nivel Fácil
                system("cls");
                for (int i = 1; i <= 11; i++)
                {
                    printf("*"); // asteriscos cima.
                }
                printf("\n");
                printf("Nivel Facil\n");
                for (int i = 1; i <= 11; i++)
                { // asteriscos baixo.
                    printf("*");
                }
                printf("\n");

                // Apresentação de perguntas - Nivel fácil
                int counter_respostaErrada = 0; // Variavel para contar as respostas que estão erradas
                for (int i = 0; i <= 9; i++)// Fazer as perguntas por ordem
                {                         
                    opcao_valida = false; // reseta a variavel para cada pergunta

                    do
                    { // do dentro do for para fazer as perguntas e poder repetir quando se coloca um caracter inválido
                        system("COLOR 0F");
                        printf("%d. %s \n\n", i + 1, perguntaFacil[numeroAleatorio[i]][0]);
                        printf("  Resposta: ");
                        opcao_quiz = getche();
                        printf("\n\n");

                        if (opcao_quiz == 'V' || opcao_quiz == 'v' || opcao_quiz == 'F' || opcao_quiz == 'f')
                        {
                            if (toupper(opcao_quiz) == perguntaFacil[numeroAleatorio[i]][1][0])
                            {
                                respostaCerta++;
                                opcao_valida = true; // aceita a opçao quando ela está certa
                            }
                            else
                            {
                                strcpy(respostaErrada[counter_respostaErrada], perguntaFacil[numeroAleatorio[i]][0]);
                                counter_respostaErrada = counter_respostaErrada + 1;
                                opcao_valida = true; // aceita a opçao quando ela está errada, mas válida
                            }
                        }
                        else
                        {                       // caso seja colocada uma resposta errada, ou seja, um caracter inválido
                            Beep(750, 300);     // beep - som de erro
                            system("COLOR 7C"); // mudança de cor ao existir um erro
                            printf("Opcao invalida, por favor coloque 'v' para Verdadeiro e 'f' para Falso!\n\n");
                            Sleep(1250); // pausa para ler o erro
                        }

                    } while (!opcao_valida); // Se a opção for inválida, repete a pergunta
                }
            }
            else if (escolha == 2)
            {
                // Nivel Dificil
                system("cls");
                for (int i = 1; i <= 14; i++)
                {
                    printf("*");
                }
                printf("\n");
                printf("Nivel Dificil\n");

                for (int i = 1; i <= 14; i++)
                {
                    printf("*");
                }
                printf("\n");

                // Apresentação de perguntas - Nivel difícil
                int counter_respostaErrada = 0; // Variavel para contar as erradas
                for (int i = 0; i <= 9; i++)
                {                         // Fazer as perguntas ordenadas
                    opcao_valida = false; // reseta a variavel para cada pergunta
                    do
                    { // do dentro do for para fazer as perguntas e poder repetir quando se coloca um caracter inválido
                        system("COLOR 0F");
                        printf("%d. %s \n\n", i + 1, perguntaDificil[numeroAleatorio[i]][0]);
                        printf("  Resposta: ");
                        opcao_quiz = getche();
                        printf("\n\n");

                        if (opcao_quiz == 'V' || opcao_quiz == 'v' || opcao_quiz == 'F' || opcao_quiz == 'f')
                        {
                            if (toupper(opcao_quiz) == perguntaDificil[numeroAleatorio[i]][1][0])
                            {
                                respostaCerta++;
                                opcao_valida = true; // aceita a opçao quando ela é certa
                            }
                            else
                            {
                                strcpy(respostaErrada[counter_respostaErrada], perguntaDificil[numeroAleatorio[i]][0]);
                                counter_respostaErrada = counter_respostaErrada + 1;
                                opcao_valida = true; // aceita a opçao quando ela é errada, mas válida
                            }
                        }
                        else
                        {                                                                                                // caso se coloque uma resposta inválida
                            Beep(750, 300);                                                                              // beep - som de erro
                            system("COLOR 7C");                                                                          // mudança de cor ao erro
                            printf("-- Opcao invalida, por favor coloque 'v' para Verdadeiro e 'f' para Falso! --\n\n"); // Com beep1
                            Sleep(1250);                                                                                 // pausa para ler o erro
                        }

                    } while (!opcao_valida); // opção valida faz seguir para outra pergunta
                }
            }
            else
            {
                Beep(750, 300);                                                           // beep - som de erro
                system("COLOR 7C");                                                       // mudança de cor ao erro
                printf("\nNao existe essa 'opcao', tente novamente uma opcao valida!\a"); // beep
                Sleep(1250);
                break;
            }

            printf("Deseja ver quantas respostas estao certas?(S para Sim) ou (Qualquer tecla para Nao): ");
            opcao_quiz = getche();
            if (opcao_quiz == 's' || opcao_quiz == 'S')
            {
                system("cls");
                system("COLOR 0F");

                for (int i = 1; i <= 52; i++)
                {
                    printf("*");
                }
                printf("\nParabens!! Acertou %d perguntas e errou %d perguntas.\n", respostaCerta, 10 - respostaCerta);
                for (int i = 1; i <= 52; i++)
                {
                    printf("*");
                }
                printf("\n-- As Perguntas que errou foram: --\n\n");
                for (int j = 0; j < (10 - respostaCerta); j++)
                {
                    printf("%s \n", respostaErrada[j]);
                }
                printf("\nPressione alguma tecla para poder continuar:\n");
                opcao = getche();
            }

            system("cls");
            printf("Deseja repetir o quiz?(S para Sim) ou (Qualquer tecla para Nao):\n");
            opcao_quiz = getche(); // apenas para ter que clicar em alguma coisa e poder ler o texto

            if (opcao_quiz == 's' || opcao_quiz == 'S')
            {
                break; // sair do case 1 e voltar ao menu
            }
            else
            {
                sair = false; // sair do loop do quiz
                break;        // sair do case 1 e como sair = false sair do quiz
            }

        case '2':
            system("cls");
            printf("Este quiz e maioritariamente sobre perguntas de cultura geral, \ngrande parte das perguntas foram geradas por inteligencia artificial. \n\n- Para responder as respostas coloque Verdadeiro(V) ou Falso(F).\n");
            printf("\nPressione alguma tecla para poder voltar ao menu.");
            opcao = getche(); // apenas para ter que clicar em alguma coisa e poder ler o texto
            break;            // sair do case 2 e voltar ao menu

        case '3':
            system("cls");
            printf("\nA sair, obrigado por jogar.\n\n");
            sair = false; // sair do loop do quiz
            break;        // sair do case 3 e voltar ao menu

        default:
            Beep(750, 300);// beep - som de erro
            system("COLOR 7C"); // mudança de cor ao erro
            printf("\nNao existe essa 'opcao', tente novamente uma opcao valida!");
            Sleep(1250);
            break;
        }

    } while (sair);
    system("cls");
    for (int i = 1; i <= 48; i++)
    {
    printf("-");
    }
    printf("\n Obrigado por ter jogado %d vez(es) ao meu quiz!\n", counter_quiz);
        for (int i = 1; i <= 48; i++)
    {
        printf("-");
    }
}

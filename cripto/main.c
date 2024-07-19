#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Criptografar
void criptografar(char *mensagem) {
    int i, j, inicio, fim;
    int len = strlen(mensagem);

    // inverter palavras
    for(i = 0; i <= len; i++) {
        if(mensagem[i] == ' ' || mensagem[i] == '\0') {
            inicio = (i == 0) ? 0 : fim + 1;
            fim = i - 1;
            while(inicio < fim) {
                char temp = mensagem[inicio];
                mensagem[inicio] = mensagem[fim];
                mensagem[fim] = temp;
                inicio++;
                fim--;
            }
        }
    }

    for(i = 0; i < len; i++) {
        mensagem[i] = ((mensagem[i] + 5 - 32) % 95) + 32;
    }
}

void descriptografar(char *mensagem) {
    int i, j, inicio, fim;
    int len = strlen(mensagem);

    for(i = 0; i < len; i++) {
        mensagem[i] = ((mensagem[i] - 5 - 32 + 95) % 95) + 32;
    }

    // parte que desinverte a palavra
    for(i = 0; i <= len; i++) {
        if(mensagem[i] == ' ' || mensagem[i] == '\0') {
            inicio = (i == 0) ? 0 : fim + 1;
            fim = i - 1;
            while(inicio < fim) {
                char temp = mensagem[inicio];
                mensagem[inicio] = mensagem[fim];
                mensagem[fim] = temp;
                inicio++;
                fim--;
            }
        }
    }
}

// ler conteudo
char* lerArquivo(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);

    char *conteudo = (char*)malloc(tamanho + 1);
    if (conteudo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }

    fread(conteudo, 1, tamanho, arquivo);
    conteudo[tamanho] = '\0';

    fclose(arquivo);
    return conteudo;
}
void salvarArquivo(const char *nomeArquivo, const char *conteudo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "%s", conteudo);
    fclose(arquivo);
}

int main() {
    char *nomeArquivo = "teste.txt";
    char *nomeArquivoCriptografado = "teste_criptografado.txt";
    char *nomeArquivoDescriptografado = "teste_descriptografado.txt";
    char mensagem[1000];
    int opcao;
    const char senha[] = "dupla10";
    char entradaSenha[100];

    do {
        printf("\nMenu:\n");
        printf("1. Criar arquivo\n");
        printf("2. Ler arquivo\n");
        printf("3. Criptografar arquivo\n");
        printf("4. Descriptografar arquivo\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();
        switch(opcao) {
            case 1:
                printf("Digite o conteudo para salvar no arquivo: ");
                fgets(mensagem, sizeof(mensagem), stdin);
                mensagem[strcspn(mensagem, "\n")] = 0;
                salvarArquivo(nomeArquivo, mensagem);
                printf("Arquivo criado com sucesso!\n");
                break;
            case 2: {
                char *conteudo = lerArquivo(nomeArquivo);
                printf("Conteudo do arquivo:\n%s\n", conteudo);
                free(conteudo);
                break;
            }
            case 3: {
                char *conteudo = lerArquivo(nomeArquivo);
                strcpy(mensagem, conteudo);
                free(conteudo);
                criptografar(mensagem);
                salvarArquivo(nomeArquivoCriptografado, mensagem);
                printf("Arquivo criptografado com sucesso!\n");
                break;
            }
            case 4: {
                printf("Digite a senha para descriptografar o arquivo: ");
                fgets(entradaSenha, sizeof(entradaSenha), stdin);
                entradaSenha[strcspn(entradaSenha, "\n")] = 0;

                if(strcmp(entradaSenha, senha) == 0) {
                    char *conteudo = lerArquivo(nomeArquivoCriptografado);
                    strcpy(mensagem, conteudo);
                    free(conteudo);
                    descriptografar(mensagem);
                    salvarArquivo(nomeArquivoDescriptografado, mensagem);
                    printf("Arquivo descriptografado com sucesso!\n");
                } else {
                    printf("Senha incorreta!\n");
                }
                break;
            }
            case 5:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 5);

    return 0;
}

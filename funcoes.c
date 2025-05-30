#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "funcoes.h"

static int ultimo_id; // Variável estática para armazenar o último ID utilizado

// Função para carregar o último ID usado do arquivo
void carregar_ultimo_id() {  
    FILE *arq = fopen("gerenciamento.csv", "r");  
    if (arq == NULL) {
        ultimo_id = 0;  // Se arquivo não existe, começa do zero
        return;
    }  

    char linha[256];  
    int maior_id = 0;  

    // Pula o cabeçalho  
    fgets(linha, sizeof(linha), arq);  

    while (fgets(linha, sizeof(linha), arq)) {  
        int id;  
        if (sscanf(linha, "%d;", &id) == 1 && id > maior_id) {  
            maior_id = id;  
        }  
    }  

    ultimo_id = maior_id;  // Atualiza o último ID conhecido  
    fclose(arq);  
}

// Função para gerar um novo ID único
int gerar_novo_id() {
    return ++ultimo_id;
}

// Função que será utilizada na função de coletar dados para que sejam adicionados itens.
// Segue-se, linha por linha:
// O parâmetro a ser recebido é a string/frase/descrição do item a ser adicionado
// A variável 'quantidade_palavras' será retornada posteriormente, e tem a função de ser incrementada toda vez que uma palavra for encontrada
// A variável 'verificador' funciona apenas como um auxiliar para que o loop funcione -> Verifica espaços ou letras
// A variável 'i' tem a função apenas de iniciar o loop
int contar_palavras_descricao(const char *str) {
    int quantidade_palavras = 0;
    int verificador = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace(str[i])) {
            verificador = 0;
        } else if (verificador == 0) {
            verificador = 1;
            quantidade_palavras++;
        }
    }
    return quantidade_palavras;
}

// Função para contar linhas de um arquivo CSV
int contar_linhas_csv(const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "r");
    if (arq == NULL) {
        puts("Erro ao abrir o arquivo!");
        return 0;
    }

    char linha[256];
    int linhas = 0;

    while (fgets(linha, sizeof(linha), arq)) {
        linhas++;
    }

    fclose(arq);
    return linhas;
}

// Função para limpar o buffer do teclado
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função principal de coleta
produto_colecao coletor_add() {
    produto_colecao item_variador;
    item_variador.identificador = gerar_novo_id();

    int palavras = 0;

    do {
        printf("Informe a descricao do item: ");
        if (fgets(item_variador.descricao, 256, stdin) == NULL) {
            printf("Erro ao ler a descricao!\n");
            continue;
        }

        item_variador.descricao[strcspn(item_variador.descricao, "\n")] = 0;
        palavras = contar_palavras_descricao(item_variador.descricao);

        if (palavras < 2) {
            printf("DIGITE AO MENOS 2 PALAVRAS PARA DESCRICAO!\n");
        }

    } while (palavras < 2);

    do {
        printf("Informe a quantidade deste item: ");
        if (scanf("%d", &item_variador.quantidade) != 1) {
            printf("INFORME UM VALOR VALIDO!\n");
            limpar_buffer();  // ADICIONADO
            item_variador.quantidade = -1;
        } else if (item_variador.quantidade < 1) {
            printf("INFORME UM VALOR POSITIVO MAIOR OU IGUAL A 1!\n");
        }
        limpar_buffer();  // ADICIONADO
    } while (item_variador.quantidade < 1);

    return item_variador;
}

// Função para escrever item no arquivo
void cadastrar_arquivo(produto_colecao produto){
    FILE *arq = fopen("gerenciamento.csv", "r");
    if(arq == NULL){
        puts("NENHUM ARQUIVO FOI ENCONTRADO!");
        puts("CRIANDO NOVO ARQUIVO...");

        arq = fopen("gerenciamento.csv", "w");
        fprintf(arq, "IDENTIFICADOR;DESCRIÇÃO;QUANTIDADE\n");
        fclose(arq);
    } else {
        fclose(arq);
    }

    arq = fopen("gerenciamento.csv", "a");

    if(arq == NULL){
        puts("ERRO AO CRIAR/ABRIR ARQUIVO!");
        return;
    }

    fprintf(arq, "%d;%s;%d\n", produto.identificador, produto.descricao, produto.quantidade);
    fclose(arq);
    printf("\nO ITEM FOI CADASTRADO COM SUCESSO!\n");  
}

// Função para consultar item pelo identificador
void consultar_item_arquivo(int identificador) {
    FILE *arq = fopen("gerenciamento.csv", "r");

    if (arq == NULL) {
        printf("VERIFIQUE SE HA ITENS A SEREM CONSULTADOS!\n");
        return;
    }

    char cada_linha[256];
    int leitura_id;

    fgets(cada_linha, sizeof(cada_linha), arq);

    while (fgets(cada_linha, sizeof(cada_linha), arq)) {
        char linha[256];
        strcpy(linha, cada_linha);

        char *id_str = strtok(linha, ";");
        char *descricao = strtok(NULL, ";");
        char *quantidade_str = strtok(NULL, ";");

        if (id_str != NULL && descricao != NULL && quantidade_str != NULL) {
            leitura_id = atoi(id_str);

            if (leitura_id == identificador) {
                printf("Item encontrado:\n");
                printf("ID: %d | Descrição: %s | Quantidade: %s\n", leitura_id, descricao, quantidade_str);
                fclose(arq);
                return;
            }
        }
    }

    printf("Item com ID %d nao encontrado.\n", identificador);
    fclose(arq);
}

// Função usada para remover item do arquivo utilizando um arquivo temporário
void remover_item_arquivo(int identificador){
    FILE *arq = fopen("gerenciamento.csv", "r");
    FILE *temporario = fopen("temporario.csv", "w");

    if (arq == NULL){
        printf("VERIFIQUE SE HA ITENS A SEREM REMOVIDOS!\n");
        return;
    } else if (temporario == NULL){
        printf("ERRO DE EXECUCAO!");
        return;
    }

    char cada_linha[256];
    int leitura_id;
    char copia_linha[256];
    int quant_remover = 0;

    fgets(cada_linha, sizeof(cada_linha), arq); // Para Lê o cabeçalho
    fputs(cada_linha, temporario);

    while(fgets(cada_linha, sizeof(cada_linha), arq)){
        strcpy(copia_linha, cada_linha); // Cópia necessesária pois o strtok altera a string original

        char *id_str = strtok(copia_linha, ";");

        leitura_id = atoi(id_str); // Transformado o id para inteiro

        if(leitura_id == identificador){
            quant_remover++;
        } else {
            fputs(cada_linha, temporario); // Escreve a linha no arquivo temporário, caso não seja encontrado o identificador
        }
    }

    fclose(arq);
    fclose(temporario);

    if(quant_remover != 0){
        remove("gerenciamento.csv");
        rename("temporario.csv", "gerenciamento.csv");
        printf("O ITEM COM ID %d FOI REMOVIDO COM SUCESSO!\n", identificador);
    } else {
        remove("temporario.csv");
        printf("O ITEM COM ID %d NAO FOI ENCONTRADO!\n", identificador);
    }
}

// Função para alterar os dados dos itens também utilizando um arquivo temporário
produto_colecao alterar_descricao_quantidade(int identificador){
    produto_colecao item_variador;

    do{
        limpar_buffer();  // ADICIONADO
        printf("Informe a NOVA descricao do item: ");
        fgets(item_variador.descricao, 256, stdin);

        item_variador.descricao[strcspn(item_variador.descricao, "\n")] = 0;

        if(contar_palavras_descricao(item_variador.descricao) < 2){
            printf("DIGITE AO MENOS 2 PALAVRAS PARA DESCRICAO!\n");
        }

    }while(contar_palavras_descricao(item_variador.descricao) < 2);

    do {
        printf("Informe a NOVA quantidade deste item: ");
        if (scanf("%d", &item_variador.quantidade) != 1) {
            printf("INFORME UM VALOR VALIDO!\n");
            limpar_buffer();  // ADICIONADO
            item_variador.quantidade = -1;
        } else if (item_variador.quantidade < 1) {
            printf("INFORME UM VALOR POSITIVO MAIOR OU IGUAL A 1!\n");
        }
        limpar_buffer();  // ADICIONADO
    } while (item_variador.quantidade < 1);

    item_variador.identificador = identificador;
    return item_variador;
}

// Função para alterar os dados dos itens também utilizando um arquivo temporário
void alterar_descricao_arquivo(int identificador){
    produto_colecao novo_item;

    FILE *arq = fopen("gerenciamento.csv", "r");
    FILE *temporario = fopen("temporario.csv", "w");

    if(arq == NULL){
        printf("VERIFIQUE SE HA ITENS A SEREM MODIFICADOS");
        return;
    } else if(temporario == NULL){
        printf("ERRO DE EXECUCAO!");
        return;
    }

    char cada_linha[256];
    int quant_modificados = 0;
    char copia_linha[256];
    int leitura_id;

    fgets(cada_linha, sizeof(cada_linha), arq); // Lê o cabeçalho 
    fputs(cada_linha, temporario); // Escrevendo o cabeçalho no arquivo temporário


    while(fgets(cada_linha, sizeof(cada_linha), arq)){
        strcpy(copia_linha, cada_linha); // Necessário pois o strtok modifica a string original
        
        char *id_str = strtok(copia_linha, ";");
        leitura_id = atoi(id_str); // Transformando o id para inteiro

        if(leitura_id == identificador){
            quant_modificados++;
            novo_item = alterar_descricao_quantidade(identificador);
            fprintf(temporario, "%d;%s;%d\n", novo_item.identificador, novo_item.descricao, novo_item.quantidade);
        } else {
            fputs(cada_linha, temporario);
        }
    }

    fclose(arq);
    fclose(temporario);

    if (quant_modificados > 0) {
        if (remove("gerenciamento.csv") != 0) {
            perror("Falha ao remover o arquivo original");
            return;
        }
        if (rename("temporario.csv", "gerenciamento.csv") != 0) {
            perror("Falha ao renomear o arquivo temporario");
            return;
        }
        printf("\nITEM MODIFICADO COM SUCESSO!\n");
    } else {
        remove("temporario.csv"); // Remove o temporário se nenhum item foi deletado
        printf("Item nao encontrado.\n");
    }
}

// Função utilizada para conseguir listar todo o acervo criado
void listar_arquivo(){
    FILE *arq = fopen("gerenciamento.csv", "r"); // Abre o arquivo em modo leitura
    if (arq == NULL) {
        printf("VERIFIQUE SE HA ITENS A SEREM LISTADOS!");
        return;
    }

    char cada_linha[256]; // Buffer para armazenar cada linha do arquivo

    fgets(cada_linha, sizeof(cada_linha), arq);
    
    printf("\n=== LISTA DE ITENS ===\n");
    printf("ID  | DESCRICAO          | QUANTIDADE\n");
    printf("----|--------------------|-----------\n");

    while (fgets(cada_linha, sizeof(cada_linha), arq) != NULL) {
        cada_linha[strcspn(cada_linha, "\n")] = '\0';

        char *id_str = strtok(cada_linha, ";");
        char *descricao = strtok(NULL, ";");
        char *quantidade_str = strtok(NULL, ";");

        if (id_str != NULL && descricao != NULL && quantidade_str != NULL) {
            printf("%-3s | %-18s | %s\n", id_str, descricao, quantidade_str);
        } else {
            printf("Formato invalido na linha: %s\n", cada_linha);
        }
    }
    fclose(arq); 
    printf("\n\t\tTem %d itens\n", contar_linhas_csv("gerenciamento.csv") - 1); // -1 para ignorar cabeçalho
}

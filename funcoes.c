#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "funcoes.h"

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

// Função que será utilizada na função de coletar dados para que sejam adicionados itens.
// Segue-se, linha por linha:
// O parâmetro a ser recebido é a string/frase/descrição do item a ser adicionado
// A variável 'quantidade_palavras' será retornada posteriormente, e tem a função de ser incrementada toda vez que uma palavra for encontrada
// A variável 'verificador' funciona apenas como um auxiliar para que o loop funcione -> Verifica espaços ou letras
// A variável 'i' tem a função apenas de iniciar o loop
int contar_palavras_descricao(const char *str){
    int quantidade_palavras;
    int verificador;
    int i;
    
    for(i = 0; str[i] != '\0'; i++){
        if(isspace(str[i])){ // Caso seja espaço, será retornado 'true', e a condição do if é atendida (verificador recebe 0).
            verificador = 0;
        } else if(verificador == 0){ // Quando a condição acima não for atendida, significa que é uma letra. Será verificado se o caractere anterior é um espaço
            verificador = 1; // confirmado que é um espaço, será incrementado na variável 'quantidade_palavras'
            quantidade_palavras = quantidade_palavras + 1;
        }
        }

    return quantidade_palavras;
    }

produto_colecao coletor_add(){ 
    produto_colecao item_variador;


    do{
    printf("Informe a descrição do item: ");
    while (getchar() != '\n'); // limpa o buffer
    if (fgets(item_variador.descricao, 256, stdin) == NULL) {
        printf("Erro ao ler a descrição!\n");
        continue;
    }

    item_variador.descricao[strcspn(item_variador.descricao, "\n")] = 0; // Removendo o '\n'

    if(contar_palavras_descricao(item_variador.descricao) < 2){
        printf("DIGITE AO MENOS 2 PALAVRAS PARA DESCRIÇÃO!\n");
    }

    }while(contar_palavras_descricao(item_variador.descricao) < 2); // O loop continua caso sejam digitados menos de duas palavras


    do {
        printf("Informe a quantidade deste item: ");
        
        if (scanf("%d", &item_variador.quantidade) != 1) { // Verifica se foi digitado realmente um número
            printf("INFORME UM VALOR VÁLIDO!\n");
            while (getchar() != '\n'); // limpa lixo do buffer
            item_variador.quantidade = -1; // força repetir
        } else if (item_variador.quantidade < 1) {
            printf("INFORME UM VALOR POSITIVO MAIOR OU IGUAL A 1!\n");
        }
    
    }while (item_variador.quantidade < 1);
    

    item_variador.identificador = contar_linhas_csv("gerenciamento.csv");


    return item_variador;
}

void cadastrar_arquivo(produto_colecao produto){

    FILE *arq = fopen("gerenciamento.csv", "r");
    if(arq == NULL){
        puts("NENHUM ARQUIVO FOI ENCONTRADO!");
        puts("CRIANDO NOVO ARQUIVO...");

        arq = fopen("gerenciamento.csv", "w");
        fprintf(arq, "IDENTIFICADOR;DESCRIÇÃO;QUANTIDADE\n");
        fclose(arq);

    }
    else{
        fclose(arq);
    }

    arq = fopen("gerenciamento.csv", "a");

    if(arq == NULL){
        puts("ERRO AO CRIAR/ABRIR ARQUIVO!");
        return;
    }

    fprintf(arq, "%d;%s;%d\n", produto.identificador, produto.descricao, produto.quantidade);
       
    }


void consultar_item_arquivo(int identificador) {
    FILE *arq = fopen("gerenciamento.csv", "r");

    if (arq == NULL) {
        printf("VERIFIQUE SE HÁ ITENS A SEREM CONSULTADOS!\n");
        return;
    }

    char cada_linha[256];
    int leitura_id;

    // Lê o cabeçalho e ignora
    fgets(cada_linha, sizeof(cada_linha), arq);

    while (fgets(cada_linha, sizeof(cada_linha), arq)) {
        // Faz uma cópia da linha para manipular com strtok
        char linha[256];
        strcpy(linha, cada_linha);

        char *id_str = strtok(linha, ";"); // Vai delimitar de acordo com o ';'
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

    printf("Item com ID %d não encontrado.\n", identificador);
    fclose(arq);
}

void remover_item_arquivo(int identificador){
    FILE *arq = fopen("gerenciamento.csv", "r");
    FILE *temporario = fopen("temporario.csv", "w");

    if (arq == NULL){
        printf("VERIFIQUE SE HÁ ITENS A SEREM REMOVIDOS!\n");
        return;
    } else if (temporario == NULL){
        printf("ERRO DE EXECUÇÃO!");
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
            quant_remover = quant_remover + 1;
        } else {
            fputs(cada_linha, temporario); // Escreve a linha no arquivo temporário, caso não seja encontrado o identificador
        }
    }

    fclose(arq);
    fclose(temporario);

    if(quant_remover != 0){
        remove("gerenciamento.csv");
        rename("temporario.csv", "gerenciamento.csv"); // Renomenado para o se tornar o arquivo original
        printf("O ITEM COM ID %d FOI REMOVIDO COM SUCESSO!\n", identificador);
    } else {
        remove("temporario.csv");
        printf("O ITEM COM ID %d NÃO FOI ENCONTRADO!\n", identificador);
    }
}

produto_colecao alterar_descricao_quantidade(int identificador){
    produto_colecao item_variador;


    do{
    while (getchar() != '\n'); // Limpeza do buffer
    printf("Informe a NOVA descrição do item: ");
    fgets(item_variador.descricao, 256, stdin);

    item_variador.descricao[strcspn(item_variador.descricao, "\n")] = 0; // Removendo o '\n'

    if(contar_palavras_descricao(item_variador.descricao) < 2){
        printf("DIGITE AO MENOS 2 PALAVRAS PARA DESCRIÇÃO!\n");
    }

    }while(contar_palavras_descricao(item_variador.descricao) < 2); // O loop continua caso sejam digitados menos de duas palavras


    do {
        printf("Informe a NOVA quantidade deste item: ");
        
        if (scanf("%d", &item_variador.quantidade) != 1) { // Verifica se foi digitado realmente um número
            printf("INFORME UM VALOR VÁLIDO!\n");
            while (getchar() != '\n'); // limpa lixo do buffer
            item_variador.quantidade = -1; // força repetir
        } else if (item_variador.quantidade < 1) {
            printf("INFORME UM VALOR POSITIVO MAIOR OU IGUAL A 1!\n");
        }
    
    } while (item_variador.quantidade < 1);

    item_variador.identificador = identificador;

    return item_variador;

}


void alterar_descricao_arquivo(int identificador){

    produto_colecao novo_item;

    FILE *arq = fopen("gerenciamento.csv", "r");
    FILE *temporario = fopen("temporario.csv", "w");

    if(arq == NULL){
        printf("VERIFIQUE SE HÁ ITENS A SEREM MODIFICADOS");
        return;
    } else if(temporario == NULL){
        printf("ERRO DE EXECUÇÃO!");
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
            quant_modificados += 1;
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
                perror("Falha ao renomear o arquivo temporário");
                return;
            }
            printf("Item removido com sucesso!\n");
        } else {
            remove("temporario.csv"); // Remove o temporário se nenhum item foi deletado
            printf("Item não encontrado.\n");
        }
    }

void listar_arquivo(){
    FILE *arq = fopen("gerenciamento.csv", "r"); // Abre o arquivo em modo leitura
    if (arq == NULL) {
        printf("VERIFIQUE SE HÁ ITENS A SEREM LISTADOS!");
        return;
    }

    char cada_linha[256]; // Buffer para armazenar cada linha do arquivo

    fgets(cada_linha, sizeof(cada_linha), arq);
    

    printf("\n=== LISTA DE ITENS ===\n");
    printf("ID  | DESCRIÇÃO          | QUANTIDADE\n");
    printf("----|--------------------|-----------\n");

    // Lê e exibe cada linha restante
    while (fgets(cada_linha, sizeof(cada_linha), arq) != NULL) {
        // Remove o '\n' do final da linha (se existir)
        cada_linha[strcspn(cada_linha, "\n")] = '\0';

        // Extrai os campos usando strtok()
        char *id_str = strtok(cada_linha, ";");
        char *descricao = strtok(NULL, ";");
        char *quantidade_str = strtok(NULL, ";");

        if (id_str != NULL && descricao != NULL && quantidade_str != NULL) {
            printf("%-3s | %-18s | %s\n", id_str, descricao, quantidade_str);
        } else {
            printf("Formato inválido na linha: %s\n", cada_linha);
        }
    }

    fclose(arq); 
}
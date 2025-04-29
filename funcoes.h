#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct {
    int identificador;
    char descricao[256];
    int quantidade;
} produto_colecao;

int contar_linhas_csv(const char *nome_arquivo);
int contar_palavras_descricao(const char *str);
produto_colecao coletor_add();
void carregar_ultimo_id();
void limpar_buffer();
int gerar_novo_id();
void cadastrar_arquivo(produto_colecao produto);
void consultar_item_arquivo(int identificador);
void remover_item_arquivo(int identificador);
produto_colecao alterar_descricao_quantidade(int identificador);
void alterar_descricao_arquivo(int identificador);
void listar_arquivo();

#endif

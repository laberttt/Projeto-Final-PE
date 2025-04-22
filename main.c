#include "funcoes.h"

int main() {
    produto_colecao teste;
    teste = coletor_add();
    cadastrar_arquivo(teste);
    consultar_item_arquivo(0);
    listar_arquivo();

    return 0;
}

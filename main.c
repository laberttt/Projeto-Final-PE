#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int id, opc;
    produto_colecao item;

    do
    {
       printf("======== Menu ========\n");
       printf("1. Cadastrar um item\n");
       printf("2. Consultar um item\n");
       printf("3. Remover um item\n");
       printf("4. Alterar a descricao ou quantidade de um item\n");
       printf("5. Listar todo o seu acervo\n");
       printf("0. Sair\n");
       printf("Digite uma opcao: ");
       scanf("%d", &opc);

       switch (opc)
       {
        case 0:
            printf("Fechando Programa...");
            break;
        case 1:
            item = coletor_add();
            cadastrar_arquivo(item);
            break;
        case 2:
            printf("Digite o id do item que queres consultar: ");
            scanf("%d", &id);
            consultar_item_arquivo(id);
            break;
        case 3:
            printf("Digite o id do item que queres remover: ");
            scanf("%d", &id);
            remover_item_arquivo(id);
            break;
        case 4:
            printf("Digite o id do item que queres alterar: ");
            scanf("%d", &id);
            alterar_descricao_arquivo(id);
            break;
        case 5:
            listar_arquivo();
            break;
        default:
            printf(">>>>>Opcao Invalida digite novamente.<<<<<");
            break;
       }
    } while (opc != 0);
    

    return 0;
}

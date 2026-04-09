// SISTEMA DE GERENFIAMENTO DE TAREFAS --
//
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// CONSTANTES PARA CONTROLE -
#define MAX_TAREFAS 100
#define TAM_TITULO 100

typedef struct {
    int id;
    char titulo[TAM_TITULO];
    int concluida; // 0 = pendente, 1 = concluida
} Tarefa;


Tarefa tarefas[MAX_TAREFAS];
int totalTarefas = 0;
int proximoId = 1;

// PROTOTIPOS DA FUNC`S
void adicionarTarefa();
void listarTarefas();
void marcarConcluida();
void removerTarefa();
void editarTarefa();
void estatisticas();
void salvarEmArquivo();
void carregarDoArquivo();
void exibirMenu();
void limparBuffer();
void inicializarSistema();

// FUNC PRINCIPAL -----------------------------------
int main (){

    int opcao;
    int resultado;

    inicializarSistema(); // Limpa toda a memoria antes de comecar
    carregarDoArquivo();

    do{
        exibirMenu();
        printf("Escolha uma opcao: ");

        resultado = scanf("%d", &opcao);

        if(resultado != 1){
            printf("Erro: Digite apenas um numero!\n");
            limparBuffer();
            continue;
        }

        limparBuffer();

        switch(opcao) {

            case 1: adicionarTarefa(); break;
            case 2: listarTarefas(); break;
            case 3: marcarConcluida(); break;
            case 4: removerTarefa(); break;
            case 5: editarTarefa(); break;
            case 6: estatisticas(); break;
            case 7: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");

        }

    } while (opcao != 7);

    salvarEmArquivo();

    return 0;

}

//------------------------------------------------------------------------------------------------------
void inicializarSistema(){
    // INICIARLIZAR TODAS AS TAREFAS COM VALORES PADRAO
    for(int i = 0; i < MAX_TAREFAS; i++){
        tarefas[i].id = 0;
        tarefas[i].titulo[0] = '\0'; //string vazia
        tarefas[i].concluida = 0;
    }
}
//------------------------------------------------------------------------------------------------------
void exibirMenu(){

    printf("\n=== SISTEMA DE TAREFAS ===\n");
    printf("1. Adicionar tarefa\n");
    printf("2. Listar tarefas\n");
    printf("3. Marcar como concluida\n");
    printf("4. Remover tarefa\n");
    printf("5. Editar tarefa\n");
    printf("6. Estatisticas\n");
    printf("7. Sair\n");
}

//--------------------------------------------------------------------------------------------------------
void adicionarTarefa(){

    if(totalTarefas >= MAX_TAREFAS){
        printf("Limite de tarefas atingido!\n");

        return;
    }

    Tarefa nova; // escopo de struct atinge funcs
    nova.id = proximoId++ ;
    nova.concluida = 0;

    printf("Titulo da tarefa: ");
    fgets(nova.titulo, TAM_TITULO, stdin);
    nova.titulo[strcspn(nova.titulo, "\n")] = 0; // Remove o \n

    tarefas[totalTarefas] = nova;
    totalTarefas++;

    printf("Tarefa adicionada com ID: %d\n", nova.id);

}
//--------------------------------------------------------------------------------------------------------
void listarTarefas(){

    if(totalTarefas == 0){
        printf("Nenhuma tarefa cadastrada!\n");
        return;}

    printf("\n=== LISTA DE TAREFAS ===\n");
    printf("ID | Status | Titulo\n");
    printf("------------------------\n");

    for(int i=0; i < totalTarefas; i++){

        // Define o status (espacamento fixo)
        char status[6];  // espaco para x ou " "
                         //
        if(tarefas[i].concluida == 1){
            strcpy(status, "  x  "); // 5 caracteres: espaco espaco, x , espaco espaco
        }

        else{
            strcpy(status, "     "); // 5  espacos
        }

    // VERIFICANDO SE TITULO NAO E NULO e garantir que nao tem \n para quebra de linha - formatacao

        char tituloExibicao[TAM_TITULO];
        if(strlen(tarefas[i].titulo) == 0){
            strcpy(tituloExibicao, "[Sem titulo]");
        }

        else{
            strcpy(tituloExibicao, tarefas[i].titulo);
        }



        printf("%2d | %s | %s\n",
                tarefas[i].id,
                status,
                tituloExibicao);
    }
}

//---------------------------------------------------------------------------------------------------------
void marcarConcluida(){

    int id;
    printf("Digite o ID da tarefa: ");
    scanf("%d", &id);
    limparBuffer();

    for(int i = 0; i < totalTarefas; i++){

        if(tarefas[i].id == id){
            tarefas[i].concluida = 1;

            printf("Tarefa marcada como concluida!\n");
            return;
        }
    }

    printf("Tarefa nao econtrada!\n");
}
//--------------------------------------------------------------------------------------------------------
void removerTarefa(){

    int id;
    printf("Digite o ID da tarefa: ");
    scanf("%d", &id);
    limparBuffer();

    for(int i = 0; i < totalTarefas; i++){

        if(tarefas[i].id == id){

            //MOVE TODAS AS TAREFAS UMA POSITION PARA TAREFAS
            for(int j = i; j < totalTarefas - 1; j++){
                tarefas[j] = tarefas[j + 1]; // a tarefa a ser apagada sera sobre posta
            }

            totalTarefas--;
            printf("Tarefa removida!\n");
            return;
        }
    }
    printf("Tarefa nao econtrada!\n");
}
//----------------------------------------------------------------------------------------------------------
void editarTarefa(){
    int id;
    printf("Digite o ID da tarefa: ");
    scanf("%d", &id);
    limparBuffer();

    for(int i = 0; i < totalTarefas; i++){
        if(tarefas[i].id == id){
            printf("Novo titulo: ");
            fgets(tarefas[i].titulo, TAM_TITULO, stdin);
            tarefas[i].titulo[strcspn(tarefas[i].titulo, "\n")] = 0;
            printf("Tarefa editada!\n");
            return;
        }
    }
    printf("Tarefa nao encontrada!\n");
}

//------------------------------------------------------------------------------------------------------------
void estatisticas(){
    int concluidas = 0;

    for(int i = 0; i < totalTarefas; i++){
        if(tarefas[i].concluida) concluidas++;
    }

    printf("\n=== ESTATISTICAS ===\n");
    printf("Total de tarefas: %d", totalTarefas);
    printf("Concluidas: %d\n", concluidas);
    printf("Pendentes: %d\n", totalTarefas - concluidas);

    if(totalTarefas > 0){
        printf("Progresso: %.1f%%\n",
                (concluidas * 100.0) / totalTarefas);
    }

}

// -----------------------------------------------------------------------------------------------------------
void salvarEmArquivo(){
    FILE *arquivo = fopen("tarefas.dat", "wb");
    if(arquivo == NULL){
        printf("Erro ao salvar arquivo!\n");
        return;
    }

    fwrite(&totalTarefas, sizeof(int), 1, arquivo);
    fwrite(&proximoId, sizeof(int), 1, arquivo);
    fwrite(tarefas, sizeof(Tarefa), totalTarefas, arquivo);

    fclose(arquivo);
    printf("Dados salvos!\n");
}

void carregarDoArquivo(){
    FILE *arquivo = fopen("tarefas.dat", "rb");
    if(arquivo == NULL){
        printf("Nenhum arquivo de dados encontrado.\n");
        return;
    }

    fread(&totalTarefas, sizeof(int), 1, arquivo);
    fread(&proximoId, sizeof(int), 1, arquivo);
    fread(tarefas, sizeof(Tarefa), totalTarefas, arquivo);

    fclose(arquivo);
    printf("Dados carregados com sucesso!\n");

}

void limparBuffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}


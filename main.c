#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  int dia;
  int mes;
  int ano;
  int hora;
  int minuto;
} Data;

typedef struct evento{
  int codigo;
  Data dataEvento;
  float duracao;
  char descricao[100];
  struct evento *esquerda;
  struct evento *direita;
} Evento;

int compararDatas(Data data1, Data data2) {
  if (data1.ano < data2.ano)
    return -1;
  else if (data1.ano > data2.ano)
    return 1;
  if (data1.mes < data2.mes)
    return -1;
  else if (data1.mes > data2.mes)
    return 1;
  if (data1.dia < data2.dia)
    return -1;
  else if (data1.dia > data2.dia)
    return 1;
  if (data1.hora < data2.hora)
    return -1;
  else if (data1.hora > data2.hora)
    return 1;
  if (data1.minuto < data2.minuto)
    return -1;
  else if (data1.minuto > data2.minuto)
    return 1;
  return 0;
}

Evento *criarEvento(int codigo, Data dataEvento, float duracao, char descricao[100]){
  Evento *novoEvento=(Evento *)malloc(sizeof(Evento));
  novoEvento->codigo=codigo;
  novoEvento->dataEvento=dataEvento;
  novoEvento->duracao=duracao;
  strcpy(novoEvento->descricao,descricao);
  novoEvento->esquerda=NULL;
  novoEvento->direita=NULL;
  return novoEvento;
}

Evento *inserirEvento(Evento *raiz, int codigo, Data dataEvento, float duracao, char descricao[100]) {
  if (raiz == NULL) {
    return criarEvento(codigo, dataEvento, duracao, descricao);
  }

  int comparacao = compararDatas(dataEvento, raiz->dataEvento);

  if (comparacao < 0) {
    raiz->esquerda = inserirEvento(raiz->esquerda, codigo, dataEvento, duracao, descricao);
  } else if (comparacao > 0) {
    raiz->direita = inserirEvento(raiz->direita, codigo, dataEvento, duracao, descricao);
  } else {
    printf("\nJá existe um agendamento para a data e hora fornecidas.\n");
    return raiz;
  }
  return raiz;
}

Evento *buscarEvento(Evento *raiz, Data dataEvento, int hora, int minuto) {
  if (raiz == NULL) {
    return NULL;
  }
  int comparacao = compararDatas(dataEvento, raiz->dataEvento);
  if (hora == -1 && minuto == -1) {
    if (comparacao == 0) {
      return raiz;
    }
  } else {
    if (comparacao == 0 && hora == raiz->dataEvento.hora && minuto == raiz->dataEvento.minuto) {
      return raiz;
    }
  }
  if (comparacao < 0) {
    return buscarEvento(raiz->esquerda, dataEvento, hora, minuto);
  } else {
    return buscarEvento(raiz->direita, dataEvento, hora, minuto);
  }
}

Evento *encontrarMinimo(Evento *raiz) {
  Evento *atual=raiz;
  while (atual->esquerda!=NULL) {
    atual=atual->esquerda;
  }
  return atual;
}

Evento *removerEvento(Evento *raiz, Data dataEvento, int hora, int minuto) {
  if (raiz == NULL) {
    printf("Evento não encontrado na agenda.\n");
    return NULL;
  }

  int comparacao = compararDatas(dataEvento, raiz->dataEvento);

  if (comparacao < 0) {
    raiz->esquerda = removerEvento(raiz->esquerda, dataEvento, hora, minuto);
  } else if (comparacao > 0) {
    raiz->direita = removerEvento(raiz->direita, dataEvento, hora, minuto);
  } else {
    if (raiz->esquerda == NULL) {
      Evento *temp = raiz->direita;
      free(raiz);
      return temp;
    } else if (raiz->direita == NULL) {
      Evento *temp = raiz->esquerda;
      free(raiz);
      return temp;
    }

    if (dataEvento.minuto < raiz->dataEvento.minuto) {
      raiz->esquerda = removerEvento(raiz->esquerda, dataEvento, hora, minuto);
    } else if (dataEvento.minuto > raiz->dataEvento.minuto) {
      raiz->direita = removerEvento(raiz->direita, dataEvento, hora, minuto);
    } else {
      if (hora < raiz->dataEvento.hora) {
        raiz->esquerda = removerEvento(raiz->esquerda, dataEvento, hora, minuto);
      } else if (hora > raiz->dataEvento.hora) {
        raiz->direita = removerEvento(raiz->direita, dataEvento, hora, minuto);
      } else {
        Evento *temp = encontrarMinimo(raiz->direita);
        raiz->codigo = temp->codigo;
        raiz->dataEvento = temp->dataEvento;
        raiz->duracao = temp->duracao;
        raiz->esquerda = removerEvento(raiz->esquerda, temp->dataEvento, temp->dataEvento.hora, temp->dataEvento.minuto);
      }
    }
  }
  return raiz;
}

void liberarAgenda(Evento *raiz) {
  if (raiz!=NULL){
    liberarAgenda(raiz->esquerda);
    liberarAgenda(raiz->direita);
    free(raiz);
  }
}

void listarEventosOrdenados(Evento *raiz) {
  if (raiz != NULL) {
    listarEventosOrdenados(raiz->esquerda);
    printf("\nCódigo: %d\n", raiz->codigo);
    printf("Data: %02d/%02d/%04d\n",raiz->dataEvento.dia,raiz->dataEvento.mes,raiz->dataEvento.ano);
    printf("Hora: %02d:%02d\n",raiz->dataEvento.hora,raiz->dataEvento.minuto);
    printf("Duração: %.1f\n",raiz->duracao);
    printf("Descrição: %s\n",raiz->descricao);
    listarEventosOrdenados(raiz->direita);
  }
}

void consultarEventoPorData(Evento *raiz, Data dataEvento) {
  if (raiz != NULL) {
    consultarEventoPorData(raiz->esquerda, dataEvento);
    if (raiz->dataEvento.dia == dataEvento.dia && raiz->dataEvento.mes == dataEvento.mes && raiz->dataEvento.ano == dataEvento.ano) {
      printf("\nCódigo: %d\n", raiz->codigo);
      printf("Data: %02d/%02d/%04d\n", raiz->dataEvento.dia, raiz->dataEvento.mes, raiz->dataEvento.ano);
      printf("Hora: %02d:%02d\n", raiz->dataEvento.hora, raiz->dataEvento.minuto);
      printf("Duração: %.1f\n", raiz->duracao);
      printf("Descrição: %s\n", raiz->descricao);
    }
    consultarEventoPorData(raiz->direita, dataEvento);
  }
}

void consultarEventoPorDataEHora(Evento *raiz, Data dataEvento, int hora, int minuto) {
  Evento *evento = buscarEvento(raiz, dataEvento, hora, minuto);
  if (evento != NULL) {
    printf("\nCódigo: %d\n", evento->codigo);
    printf("Data: %02d/%02d/%04d\n", evento->dataEvento.dia, evento->dataEvento.mes, evento->dataEvento.ano);
    printf("Hora: %02d:%02d\n", evento->dataEvento.hora, evento->dataEvento.minuto);
    printf("Duração: %.1f\n", evento->duracao);
    printf("Descrição: %s\n", evento->descricao);
  } else {
    printf("\nEvento não encontrado para a data e hora fornecidas.");
  }
}

void alterarEvento(Evento *raiz, Data dataEvento, int hora, int minuto) {
  Evento *eventoParaAlterar = buscarEvento(raiz, dataEvento, hora, minuto);
  if (eventoParaAlterar != NULL) {
    printf("\nDigite a nova descrição: ");
    scanf(" %[^\n]", eventoParaAlterar->descricao);
    printf("Digite a nova duração: ");
    scanf("%f", &(eventoParaAlterar->duracao));
    printf("\nEvento alterado com sucesso.\n");
  } else {
    printf("\nEvento não encontrado.\n");
  }
}

int main() {
  int opcao,consulta;
  int codigo;
  Data dataEvento;
  float duracao;
  char descricao[100];
  int hora, minuto;
  Evento *agenda=NULL;

  do {
    printf("\n==== UFFSAgenda ====\n\n");
    printf("1. Incluir\n");
    printf("2. Consultar\n");
    printf("3. Alterar\n");
    printf("4. Excluir\n");
    printf("5. Listar todos\n");
    printf("6. Sair\n\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch(opcao){
    case 1:
      printf("\nCódigo do evento: ");
      scanf("%d", &codigo);
      printf("Data do evento (dia/mês/ano): ");
      scanf("%d/%d/%d",&(dataEvento.dia),&(dataEvento.mes),&(dataEvento.ano));
      printf("Hora do evento (hora:minutos): ");
      scanf("%d:%d",&(dataEvento.hora),&(dataEvento.minuto));
      printf("Duração do evento (em horas): ");
      scanf("%f",&duracao);
      printf("Descrição do evento: ");
      scanf(" %[^\n]",descricao);
      agenda=inserirEvento(agenda,codigo,dataEvento,duracao,descricao);
      break;
      
    case 2:
      if (agenda==NULL) {
        printf("\nAgenda vazia.\n");
        break;
      }
      printf("\nCONSULTAR EVENTO: \n");
      printf("1. Por data\n");
      printf("2. Por data e hora\n");
      printf("3. Voltar\n\n");
      printf("Escolha uma opção: ");
      scanf("%d", &consulta);
      switch (consulta){
        case 1:
          printf("\nData do evento (dia/mês/ano): ");
          scanf("%d/%d/%d",&(dataEvento.dia),&(dataEvento.mes),&(dataEvento.ano));
          printf("\nEventos encontrados nessa data:\n");
          consultarEventoPorData(agenda,dataEvento);
          break;
        case 2:
          printf("\nData do evento (dia/mês/ano): ");
          scanf("%d/%d/%d",&(dataEvento.dia),&(dataEvento.mes),&(dataEvento.ano));
          printf("Hora do evento (hora:minutos): ");
          scanf("%d:%d", &(dataEvento.hora), &(dataEvento.minuto));          
          printf("\nEvento encontrado nessa data e hora:\n");
          consultarEventoPorDataEHora(agenda,dataEvento,dataEvento.hora,dataEvento.minuto);
          break;
        case 3:
          break;
        default:
          printf("\nOpção inválida.\n");
      }
      break;
      
    case 3:
      if (agenda == NULL) {
        printf("\nAgenda vazia.\n");
      } else {
        printf("\nData do evento a ser alterado (dia/mês/ano): ");
        scanf("%d/%d/%d", &(dataEvento.dia), &(dataEvento.mes), &(dataEvento.ano));
        printf("Hora do evento a ser alterado (hora:minutos): ");
        scanf("%d:%d", &hora, &minuto);
        alterarEvento(agenda, dataEvento, hora, minuto);
      }
      break;
      
    case 4:
      if (agenda == NULL) {
        printf("\nAgenda vazia.\n");
        break;
      }
      printf("\nData do evento a ser excluído (dia/mês/ano): ");
      scanf("%d/%d/%d", &(dataEvento.dia), &(dataEvento.mes),
            &(dataEvento.ano));
      printf("Hora do evento a ser excluído (hora:minutos): ");
      scanf("%d:%d", &hora, &minuto);
      Evento *eventoParaExcluir=buscarEvento(agenda, dataEvento, hora, minuto);
      if (eventoParaExcluir != NULL) {
        agenda = removerEvento(agenda, dataEvento, hora, minuto);
        printf("\nEvento excluído com sucesso.\n");
      } else {
        printf("\nEvento não encontrado na agenda.\n");
      }
      break;
      
    case 5:
      if (agenda==NULL) {
        printf("\nAgenda vazia.\n");
      } else {
        printf("\n==== Lista de Eventos ====\n");
        listarEventosOrdenados(agenda);
      }
      break;
  
    case 6:
      liberarAgenda(agenda);
      printf("\nEncerrando o programa.\n\n");
      break;
      
    default:
      printf("\nOpção inválida.\n");
      break;
    }
  } while (opcao!=6);
  return 0;
}

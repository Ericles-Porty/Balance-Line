/*
 * balanceLine.h
 *
 *  Modified on: Sep 12, 2021
 *      Author: raphael oliveira
 */


#include <stdio.h> /* define FILE */
#include <stdlib.h>
#include <math.h>


int tamanho_bits_clientes() {
    return (sizeof(int) + sizeof(char) * 40 + sizeof(char) * 12);
}

int tamanho_bits_transacoes() {
    return (sizeof(int) + sizeof(char) + sizeof(char) * 40 + sizeof(char) * 40);
}

int total_clientes(FILE *fMestre) {

    fseek(fMestre, 0, SEEK_END);
    int tam = trunc(ftell(fMestre) / tamanho_bits_clientes());
    return tam;

}


int total_transacoes(FILE *fTransacao) {

    fseek(fTransacao, 0, SEEK_END);
    int tam = trunc(ftell(fTransacao) / tamanho_bits_transacoes());
    return tam;

}

int menorChave(TCliente *cliente, TTransacao *transacao) {
    if (cliente->codCliente < transacao->codCliente) {
        //printf("Chave Mestre menor -> %d\n", chaveMestre);
        return cliente->codCliente;
    } else {
        //printf("Chave Transacao menor -> %d\n", chaveTransacao);
        return transacao->codCliente;
    }
}

int maiorChave(FILE *fMestre, FILE *fTransacao) {
    TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
    TTransacao *transacao = (TTransacao *) malloc(sizeof(TTransacao));

    int bitUltimaChaveMestre, bitUltimaChaveTransacao;
    int maior;

    bitUltimaChaveMestre = tamanho_bits_clientes() * (total_clientes(fMestre) - 1);
    bitUltimaChaveTransacao = tamanho_bits_transacoes() * (total_transacoes(fTransacao) - 1);

    //printf("Tamanho em bits Mestre -> %d\n", bitUltimaChaveMestre);

    fseek(fMestre, bitUltimaChaveMestre, SEEK_SET);
    fread(&cliente->codCliente, sizeof(int), 1, fMestre);

    fseek(fTransacao, bitUltimaChaveTransacao, SEEK_SET);
    fread(&transacao->codCliente, sizeof(int), 1, fTransacao);

    //printf("Mestre -> %d\n", cliente->codCliente);
    //printf("Transacao -> %d\n", transacao->codCliente);

    if (cliente->codCliente > transacao->codCliente)
        maior = cliente->codCliente;
    else
        maior = transacao->codCliente;


    free(cliente);
    free(transacao);

    return maior;
}

void balanceLine(FILE *fMestre, FILE *fTransacao, FILE *fNovoMestre, FILE *fErro) {
    //TODO
    //implementar o algoritmo do Balance Line

    printf("\n\n-------------BALANCELINE--------------\n\n");

    TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
    TCliente *clienteNovo = (TCliente *) malloc(sizeof(TCliente));
    TTransacao *transacao = (TTransacao *) malloc(sizeof(TTransacao));

    int error = 0;
    int alocado = 0;

    int highValor = maiorChave(fMestre, fTransacao);


    fseek(fMestre, 0, SEEK_SET);
    fseek(fTransacao, 0, SEEK_SET);
    fseek(fNovoMestre, 0, SEEK_SET);


    cliente = leCliente(fMestre);
    transacao = leTransacao(fTransacao);

    //salvaCliente(cliente, fNovoMestre);

    int chaveAtual = menorChave(cliente, transacao);

    int cont = 0;

    while (chaveAtual <= highValor) { //< ou !=


        if (chaveAtual == cliente->codCliente)
            alocado = 1;

        // ??
        if (alocado) {
            clienteNovo = cliente; //mestre = registro_velho
            cliente = leCliente(fMestre);
            // ??

        }

        while (transacao->codCliente == chaveAtual) {
            error = 0;
            //printf("Cont -> %d | Clientpos-> %d | TransicaoPos-> %d\n", cont++, cliente->codCliente,
            //       transacao->codCliente);

            if (transacao->tipoTransacao == 'E') {
                if (!alocado) //tentando excluir arquivo inexistente
                    error = 1;
                else
                    alocado = 0;
            } else {
                if (transacao->tipoTransacao == 'I') {
                    if (alocado) //tentando inserir arquivo j� existente
                        error = 2;
                    else {
                        alocado = 1;

                        //mestre = registro_transacao
                        clienteNovo->codCliente = transacao->codCliente;
                        strcpy(clienteNovo->nome, transacao->valor01);
                        strcpy(clienteNovo->dataNascimento, transacao->valor02);

                        //

                    }
                } else {
                    if (transacao->tipoTransacao == 'M') {
                        if (!alocado) //tentando modificar registro inexistente
                            error = 3;
                        else {
                            //atualizarAtributo
                            if (strcmp("Nome", transacao->valor01) == 0) {
                                strcpy(clienteNovo->nome, transacao->valor02);

                            } else {
                                strcpy(clienteNovo->dataNascimento, transacao->valor02);
                            }
                            alocado = 1;
                            //
                        }
                    } else { //transcao com caracter inv�ildo
                        error = 4;
                    }
                }
            }
            //printf("Transacao atual -> %d\n",transacao->codCliente);
            transacao = leTransacao(fTransacao);
            cliente = leCliente(fMestre);
        }
        if (alocado) {
            salvaCliente(clienteNovo, fNovoMestre);
        }


        if (cliente)
            if (cliente->codCliente > transacao->codCliente) {
                if (transacao->tipoTransacao == 'I') {
                    //mestre = registro_transacao
                    clienteNovo->codCliente = transacao->codCliente;
                    strcpy(clienteNovo->nome, transacao->valor01);
                    strcpy(clienteNovo->dataNascimento, transacao->valor02);
                    salvaCliente(clienteNovo, fNovoMestre);
                } else if (transacao->tipoTransacao == 'M') {
                    error = 2; //?
                } else if (transacao->tipoTransacao == 'E') {
                    error = 1; //?
                } else
                    error = 4;

                transacao = leTransacao(fTransacao);
            }

        if (cliente && transacao) {
            chaveAtual = menorChave(cliente, transacao);
        }

        //transacao = leTransacao(fTransacao);



        if (!cliente) {
            printf("Mamaco");
            while (transacao) {
                if (transacao->tipoTransacao == 'I') {
                    clienteNovo->codCliente = transacao->codCliente;
                    strcpy(clienteNovo->nome, transacao->valor01);
                    strcpy(clienteNovo->dataNascimento, transacao->valor02);

                    salvaCliente(clienteNovo, fNovoMestre);
                }
                transacao = leTransacao(fTransacao);
            }
            break;
        }

        if (!transacao) {
            printf("Chimpanz�");
            while (cliente) {
                salvaCliente(cliente, fNovoMestre);
                cliente = leCliente(fMestre);
            }
            break;
        }


    }

}


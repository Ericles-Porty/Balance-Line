/*
 * criaTransacoes.h
 *
 *  Modified on: Sep 12, 2021
 *      Author: raphael oliveira
 */


/* Grava dados formatados num arquivo TRANSAÇÕES */

#include <stdio.h> /* define FILE */
#include <stdlib.h>

void criaTransacoes(){
 FILE *fTransacoes; /* ponteiro para arquivo */

 fTransacoes = fopen("Transacoes.dat","w");

 if(fTransacoes == NULL) {
     printf("\n Erro ao abrir o arquivo TRANSACOES");
     exit(1);
 }

 TTransacao *t1 = Transacao(11,'E', "", "");
 TTransacao *t2 = Transacao(17,'M', "Nome", "Maria");
 TTransacao *t3 = Transacao(18,'I', "Andrea", "02/04/1990");
 TTransacao *t4 = Transacao(25,'I', "Jonas", "03/07/2002"); //alterei o código de 26 para 25
 TTransacao *t5 = Transacao(56,'I', "Andre", "12/11/2011"); //era 56

 //TTransacao *t6 = Transacao(62,'I', "Richarlison", "01/05/2000"); //criado por mim

 salvaTransacao(t1, fTransacoes);
 salvaTransacao(t2, fTransacoes);
 salvaTransacao(t3, fTransacoes);
 salvaTransacao(t4, fTransacoes);
 salvaTransacao(t5, fTransacoes);
 //salvaTransacao(t6, fTransacoes);

 fclose(fTransacoes);
}

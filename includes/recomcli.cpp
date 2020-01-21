#include "recomcli.h"

RecomCli::RecomCli(long id_cliente, int max_recom)
{
	max = max_recom;
	reset();
	_id_cliente = id_cliente;
}

void RecomCli::reset(bool limpa_tail, bool limpa_cliente)
{
	size = 0;
	menor_coef = 0;
	root_prod = nullptr;
	if(limpa_tail)
		tail_prod = nullptr;
	if(limpa_cliente)
		_id_cliente = 0;
}

RecomCli::~RecomCli()
{
	recursive_free_prod(root_prod);  // limpa todos os ponteiros recursivamente
}

void RecomCli::recursive_free_prod(Produto* prod)
{
	if(!prod)
		return;

	if(prod->proximo())
		recursive_free_prod(prod->proximo());
	free(prod);
	return;
}

bool RecomCli::insert(long id, int grupo_id, double coef)
{
	// RESSALVA: APENAS UMA RECOMENDAÇÃO POR GRUPO ID
	// Grupo Id 0 (zero) significa que não há agrupamento para o item.

	bool lista_cheia = size >= max;
	if(lista_cheia && coef < menor_coef)
		return false;  // não insere se o coeficiente for insuficiente

	// verifica se esse produto cabe na recomendação do seu group id
	if(grupo_id)
	{
		std::unordered_map<int, double>::iterator it;
		it = menor_coef_pgrp.find(grupo_id);
		// abaixo: grupo já existe e o coeficiente dele é maior que o item atual
		if(it != menor_coef_pgrp.end())
		{
			if(it->second > coef)
				return false;

			// neste ponto indica que deve-se remover o item do mesmo grupo para
			// inserir o novo
			remove_especifico(prod_grupo[grupo_id]);
			lista_cheia = false;
		}
	}

	// VAI FAZER A INSERÇÃO
	// - Todos os testes a respeito se deve ou não inserir têm de ser feitos
	//   acima deste ponto.

	Produto* novo_item = new Produto(id, grupo_id, coef);
	if(!size)  // inserindo o primeiro item
	{
		root_prod = novo_item;
		tail_prod = root_prod;
		menor_coef = coef;
	}
	else
	{  // inserindo em qualquer lugar 
		if(lista_cheia)  // deve remover o tail
			remove_tail();

		Produto* prod = root_prod;
		Produto* dlyd = nullptr;
		while(prod)  // itera na lista e para quando atingir o ponto de inserção
		{
			if(coef > prod->coef())
				break;
			dlyd = prod;
			prod = prod->proximo();  // pega o próximo
		}

		novo_item->set_proximo(prod);
		novo_item->set_anterior(dlyd);

		if(!dlyd)  // inserindo no root
		{
			root_prod = novo_item;
			prod->set_anterior(novo_item);
		}
		else if(!prod)  // está inserindo no tail
		{
			tail_prod = novo_item;
			menor_coef = coef;
		}
		else
			prod->set_anterior(novo_item);

		if(dlyd)
			dlyd->set_proximo(novo_item);
	}

	if(grupo_id)
	{
		menor_coef_pgrp[grupo_id] = coef;
		prod_grupo[grupo_id] = novo_item;
	}

	size++;
	return true;
}

bool RecomCli::remove_tail()
{
	if(!tail_prod)  // lista vazia
		return false;

	if(tail_prod->grupo_id())  // remove o grupo
	{
		menor_coef_pgrp.erase(tail_prod->grupo_id());
		prod_grupo.erase(tail_prod->grupo_id());
	}

	Produto* novo_tail = tail_prod->anterior();
	if(!novo_tail)  // tail e root são o mesmo cara
		reset(false, false);
	else
	{
		novo_tail->set_proximo(nullptr);
		menor_coef = novo_tail->coef();
		tail_prod->set_anterior(nullptr);
		size--;
	}

	free(tail_prod);
	tail_prod = novo_tail;

	return true;
}

bool RecomCli::remove_especifico(Produto* prod)
{
	if(!prod)  // produto inválido
		return false;
	if(!size)  // sem itens na lista
		return false;
	if(prod == tail_prod)  // se o produto passado for o tail
		return remove_tail();

	Produto* anterior = prod->anterior();
	Produto* proximo = prod->proximo();

	if(prod->grupo_id())  // remove o grupo
	{
		menor_coef_pgrp.erase(prod->grupo_id());
		prod_grupo.erase(prod->grupo_id());
	}

	if(!anterior)  // removendo o root
		root_prod = proximo;
	else
		anterior->set_proximo(proximo);
	proximo->set_anterior(anterior);
	size--;

	prod->set_proximo(nullptr);
	prod->set_anterior(nullptr);
	free(prod);

	return true;
}

void RecomCli::print(bool inverse)
{
	Produto* prod = root_prod;
	if(prod)
	{
		std::cout << "RecomCli " << _id_cliente << std::endl;
		if(inverse)
			std::cout << "(inverse)" << std::endl;
		std::cout << "mcoef " << menor_coef << " tam " << size << std::endl;
		std::cout << "\t" << "prodid, grupoid, coef" << std::endl;
	}
	else
		return;

	if(inverse)
		prod = tail_prod;

	while(prod)
	{
		std::cout << "\t" << prod->id() << ", " << prod->grupo_id() << ", ";
		std::cout << prod->coef() << std::endl;

		if(!inverse)
			prod = prod->proximo();
		else
			prod = prod->anterior();
	}
}

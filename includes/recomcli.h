#ifndef RECOMCLI
#define RECOMCLI

#include <unordered_map>
#include <cstdlib>
#include <iostream>

#include "produto.h"

class RecomCli {
public:
	int size;
	double menor_coef;
	int max;
	std::unordered_map<int, double> menor_coef_pgrp;
	std::unordered_map<int, Produto*> prod_grupo;
	// lista duplamente encadeada
	Produto* root_prod;
	Produto* tail_prod;
	long _id_cliente;

	RecomCli(long id_cliente, int max_recom);
	~RecomCli();
	bool insert(long id, int grupo_id, double coef);
	void print(bool inverse = false);

private:
	void recursive_free_prod(Produto* prod);
	void reset(bool limpa_tail = true, bool limpa_cliente = true);
	bool remove_tail();
	bool remove_especifico(Produto* prod);
};

#endif  // RECOMCLI

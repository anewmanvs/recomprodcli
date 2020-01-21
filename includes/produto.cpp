#include "produto.h"

Produto::Produto(long id, int grupo_id, double coef)
{
	_id = id;
	_grupo_id = grupo_id;
	_coef = coef;
	_proximo = nullptr;
	_anterior = nullptr;
}

Produto::~Produto()
{
	if(_proximo)
		free(_proximo);
	if(_anterior)
		free(_anterior);
}

long Produto::id()
{
	return _id;
}

int Produto::grupo_id()
{
	return _grupo_id;
}

double Produto::coef()
{
	return _coef;
}

Produto* Produto::proximo()
{
	return _proximo;
}

void Produto::set_proximo(Produto* produto)
{
	_proximo = produto;
}

Produto* Produto::anterior()
{
	return _anterior;
}

void Produto::set_anterior(Produto* produto)
{
	_anterior = produto;
}

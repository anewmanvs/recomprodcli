#ifndef PRODUTORECOM
#define PRODUTORECOM

#include <cstdlib>

class Produto {
public:
	long _id;
	int _grupo_id;
	double _coef;
	Produto* _proximo;
	Produto* _anterior;

	Produto(long id, int grupo_id, double coef);
	~Produto();

	long id();
	int grupo_id();
	double coef();
	Produto* proximo();
	Produto* anterior();
	void set_proximo(Produto* produto);
	void set_anterior(Produto* produto);
};

#endif  // PRODUTORECOM

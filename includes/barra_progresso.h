#include <iostream>
#include <chrono>
#include <iomanip>

class Progresso{
public:
	Progresso(int max_iter, bool silent = false, double bar_width = 70);
	void registra_iteracao();
	void finaliza();

private:
	double progresso;
	int _max_iter;
	double _bar_width;
	double step;
	int niter;
	bool _silent;

	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;
	std::chrono::duration<double> acc;

	void exibe(std::chrono::duration<double>);
};

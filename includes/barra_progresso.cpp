#include "barra_progresso.h"

Progresso::Progresso(int max_iter, bool silent, double bar_width)
{
	progresso = 0.0;
	_max_iter = max_iter;
	_bar_width = bar_width;
	step = 1.0/_max_iter;
	niter = 0;
	_silent = silent;

	if(!_silent)
		exibe(std::chrono::duration<double>(0));

	start = std::chrono::high_resolution_clock::now();
	acc = std::chrono::duration<double>(0);
}

void Progresso::registra_iteracao()
{
	progresso += step;

	std::chrono::duration<double> duracao(0);
	if(niter)
	{  // segunda chamada em diante
		t2 = std::chrono::high_resolution_clock::now();
		duracao = t2 - t1;
		acc += duracao;
	}
	t1 = std::chrono::high_resolution_clock::now();
	if(!_silent)
		exibe(duracao);
	niter++;
}

void Progresso::exibe(std::chrono::duration<double> duracao)
{
	if(_silent)
		return;

	std::cout << std::fixed << std::setprecision(5);

	std::cout << "8";
	int pos = _bar_width * progresso;
	for(int i = 0; i < _bar_width; i++)
	{
		if(i < pos)
			std::cout << "=";
		else if(i == pos)
			std::cout << "D";
		else
			std::cout << " ";
	}
	std::cout << "(‿*‿) " << int(progresso * 100) << "% ";

	if(niter)
		std::cout << "it/" << duracao.count() << "s ";
	else
		std::cout << "it/ndf ";

	auto agora = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> total = agora - start;
	std::cout << total.count() << "s/" << acc.count()/(niter + 1) * _max_iter;
	std::cout << "s (ETA)\r";

	std::cout.flush();
}

void Progresso::finaliza()
{
	if(_silent)
		return;

	std::cout << std::endl;
	progresso = 1.0;
}

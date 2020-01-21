#include "ctrlcsv.h"

// Funções
std::time_t hoje()
{
	return std::time(0);
}

int diffsemana(std::time_t _hoje, std::string strdt)
{
	std::tm dt = {0};
	strptime(strdt.c_str(), "%Y-%m-%d", &dt);
	return std::difftime(_hoje, std::mktime(&dt))/SECS_SEMANA;
}

std::fstream read_csv(std::string path)
{
	std::fstream fin;
	fin.open(path, std::ios::in);
	return fin;
}

std::unordered_map<lll_t, double, khash_lllt, kequal_lllt>
read_csv_from_clientes_vendas(std::string path, std::time_t _hoje,
							  std::vector<std::vector<long> > clientes,
							  char sep, bool jump_header, unsigned int step)
{
	std::unordered_map<lll_t, double, khash_lllt, kequal_lllt> info;
	unsigned long start, end;
	short offset;

	if(clientes.empty())
		return info;

	auto lambda = [sep](char c){return c == sep;};

	unsigned short sz_plchdr = 2;
	size_t pos_start = path.find("{}");
	if(pos_start == std::string::npos)
		return info;
	size_t pos_end = path.find("{}", pos_start+1);
	if(pos_end == std::string::npos)
		return info;

	for(size_t i = 0; i < clientes.size(); i++)
	{
		std::vector<long> subcli = clientes.at(i);
		if(subcli.empty())  // se não tem dado, tem algo errado. Mas pula.
			continue;

		// encontra os limites inferior e superior do grupo de usuários
		start = (unsigned long)(subcli.at(0) / step) * step;
		end = start + step;
		offset = start ? ceil(log10(end)) - sz_plchdr : -sz_plchdr + 1;

		// estabelece o nome do arquivo que deve ser aberto
		std::string filename = path;
		filename.replace(pos_start, sz_plchdr, std::to_string(start));
		filename.replace(pos_end + offset, sz_plchdr, std::to_string(end));

		// faz a abertura do arquivo e lê apenas os dados dos usuários selecionados
		std::fstream fin = read_csv(filename);
		if(!fin.is_open())
			std::cout << "Impossível abrir arquivo " + filename << std::endl;

		std::string line;
		bool _jump_header = jump_header;
		while(getline(fin, line))
		{
			if(_jump_header)
			{
				_jump_header = false;
				continue;  // pula o header
			}

			std::vector<std::string> contents;  // separa o conteúdo da linha
			boost::split(contents, line, lambda);

			long id_cliente = std::stoll(contents[CLIENTE]);
			if(!boost::algorithm::any_of_equal(subcli, id_cliente))
				continue;  // linha atual não é de nenhum cliente em questão
			// insere campo lido no vetor
			std::tuple<long, long, long> key;
			key = std::make_tuple(id_cliente, std::stoll(contents[PRODUTO]),
								  std::stoll(contents[CATEGORIA]));
			double ponderacao = std::stod(contents[QTDE]) * \
								exp(0.05 * diffsemana(_hoje, contents[DATA]));
			if(info.find(key) == info.end())
				info[key] = ponderacao;  // elemento novo
			else
				info[key] += ponderacao;  // já existe
		}
		fin.close();
	}

	return info;
}

std::unordered_map<long, std::tuple<long, std::string, std::vector<std::string> > >
read_csv_produtos(std::string path, char sep, bool jump_header)
{
	std::unordered_map<long,
		std::tuple<long, std::string, std::vector<std::string> > > prods;

	std::fstream fin = read_csv(path);
	if(!fin.is_open())
		return prods;

	auto lambda = [sep](char c){return c == sep;};

	std::string line;
	while(getline(fin, line))
	{
		if(jump_header)
		{
			jump_header = false;
			continue;  // pula o header
		}

		std::vector<std::string> contents;
		boost::split(contents, line, lambda);

		// insere campo lido no vetor
		long id_produto = std::stoll(contents[PPROD]);
		std::vector<std::string> tokens_prod;
		boost::split(tokens_prod, contents[DPROD], boost::is_any_of(" "));
		prods[id_produto] = std::make_tuple(std::stoll(contents[PCAT]),
											contents[DPROD], tokens_prod);
	}

	fin.close();
	return prods;
}

std::unordered_map<long, std::string> read_csv_catg(std::string path, char sep,
														 bool jump_header)
{
	std::unordered_map<long, std::string> catgs;

	std::fstream fin = read_csv(path);
	if(!fin.is_open())
		return catgs;

	auto lambda = [sep](char c){return c == sep;};

	std::string line;
	while(getline(fin, line))
	{
		if(jump_header)
		{
			jump_header = false;
			continue;  // pula o header
		}

		std::vector<std::string> contents;
		boost::split(contents, line, lambda);

		// insere campo lido no vetor
		long id_cat = std::stoll(contents[CCAT]);
		catgs[id_cat] = contents[CDESC];
	}

	fin.close();
	return catgs;
}

std::unordered_map<long, int> read_csv_prodsprom(std::string path, char sep,
													  bool jump_header)
{
	std::unordered_map<long, int> prodsprom;

	std::fstream fin = read_csv(path);
	if(!fin.is_open())
		return prodsprom;

	auto lambda = [sep](char c){return c == sep;};

	std::string line;
	while(getline(fin, line))
	{
		if(jump_header)
		{
			jump_header = false;
			continue;  // pula o header
		}

		std::vector<std::string> contents;
		boost::split(contents, line, lambda);

		// insere campo lido no vetor
		long id_produto = std::stoll(contents[PROMPROD]);
		prodsprom[id_produto] = std::stoi(contents[PROMGRP]);
	}

	fin.close();
	return prodsprom;
}

std::vector<std::vector<long> > read_csv_clientes(std::string path, char sep,
													   bool jump_header,
													   unsigned int step)
{
	std::vector<std::vector<long> > grupo_cli;

	std::fstream fin = read_csv(path);
	if(!fin.is_open())
		return grupo_cli;

	auto lambda = [sep](char c){return c == sep;};

	std::string line;
	std::vector<long> clientes;
	while(getline(fin, line))
	{
		if(jump_header)
		{
			jump_header = false;
			continue;  // pula o header
		}

		clientes.push_back(std::stoll(line));
	}

	fin.close();
	if(clientes.empty())
		return grupo_cli;

	// Organiza os grupos de clientes
	// ordena os clientes para diminuir abertura de arquivos
	std::sort(clientes.begin(), clientes.end());
	std::vector<long> grupo;
	unsigned int next_step = (unsigned int)(clientes.at(0)/step)*step + step;
	for(size_t i = 0; i < clientes.size(); i++)
	{
		long id_cliente = clientes.at(i);
		if(id_cliente < next_step)
			grupo.push_back(id_cliente);
		else
		{
			if(!grupo.empty())
			{
				grupo_cli.push_back(grupo);
				grupo = std::vector<long>();
				grupo.push_back(id_cliente);
			}
			next_step += step;
		}
	}

	if(grupo_cli.empty() && !grupo.empty())
		grupo_cli.push_back(grupo);

	return grupo_cli;
}

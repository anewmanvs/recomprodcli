#include <fstream>
#include <ctime>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>

#include "confg_tpl.h"
#include "generic_tuple_map.h"

#define SECS_SEMANA 604800  // segundos em uma semana
#define STEP 5000

enum {CLIENTE, PRODUTO, CATEGORIA, QTDE, DATA, CUPOM, SEQUENCIA};  // csv vendas
enum {PPROD, DPROD, PCAT};  // csv de descrições de produtos
enum {CCAT, CDESC};  // csv de descrições de categorias
enum {PROMPROD, PROMGRP};  // csv de produtos em promoção

std::time_t hoje();
int diffsemana(std::time_t _hoje, std::string strdt);
std::fstream read_csv(std::string path);
std::unordered_map<long, std::tuple<long, std::string, std::vector<std::string> > >
read_csv_produtos(std::string path, char sep = ';', bool jump_header = true);
std::unordered_map<long, std::string> read_csv_catg(
	std::string path, char sep = ';', bool jump_header = true);
std::unordered_map<long, int> read_csv_prodsprom(
	std::string path, char sep = ';', bool jump_header = true);
std::vector<std::vector<long> > read_csv_clientes(std::string path,
													   char sep = ';',
													   bool jump_header = true,
													   unsigned int step = STEP);
std::unordered_map<lll_t, double, khash_lllt, kequal_lllt>
read_csv_from_clientes_vendas(std::string path, std::time_t _hoje,
							  std::vector<std::vector<long> > clientes,
							  char sep = ';', bool jump_header = true,
							  unsigned int step = STEP);

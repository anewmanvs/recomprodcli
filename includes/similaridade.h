#include <unordered_map>
#include <tuple>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "confg_tpl.h"
#include "jaro_winkler.h"
#include "recomcli.h"
#include "barra_progresso.h"
#include "generic_tuple_map.h"

std::tuple<long, long> generate_key(long id_produto_hist,
											  long id_produto_prom);

std::unordered_map<long, RecomCli*> matriz_similaridade(
	std::unordered_map<ll_t, double, khash_llt, kequal_llt> &mat_sim,
	std::unordered_map<lll_t, double, khash_lllt, kequal_lllt> &hist_vend,
	std::unordered_map<long, std::tuple<long,
		std::string, std::vector<std::string> > > &prods,
	std::unordered_map<long, int> &prods_promocao,
	std::unordered_map<ll_t, bool, khash_llt, kequal_llt> &mat_sim_ignores,
	std::unordered_map<ll_t, double, khash_llt, kequal_llt> &mat_sim_cat,
	std::unordered_map<long, std::string> &catgs,
	unsigned short max_recomendacoes = 10, bool verbose = false);

#include "similaridade.h"

using namespace std;

tuple<long, long> generate_key(long id_produto_hist,
							   long id_produto_prom)
{
	long menor, maior;
	if(id_produto_hist < id_produto_prom)
	{
		menor = id_produto_hist;
		maior = id_produto_prom;
	}
	else
	{
		menor = id_produto_prom;
		maior = id_produto_hist;
	}
	return make_tuple(menor, maior);
}

unordered_map<long, RecomCli*> matriz_similaridade(
	unordered_map<ll_t, double, khash_llt, kequal_llt> &mat_sim,
	unordered_map<lll_t, double, khash_lllt, kequal_lllt> &hist_vend,
	unordered_map<long, tuple<long, string, vector<string> > > &prods,
	unordered_map<long, int> &prods_promocao,
	unordered_map<ll_t, bool, khash_llt, kequal_llt> &mat_sim_ignores,
	unordered_map<ll_t, double, khash_llt, kequal_llt> &mat_sim_cat,
	unordered_map<long, string> &catgs,
	unsigned short max_recomendacoes, bool verbose)
{
	unordered_map<long, RecomCli*> recomendacoes;  // map das recomendações

	unordered_map<lll_t, double, khash_lllt, kequal_lllt>::iterator it;
	it = hist_vend.begin();
	size_t i, j;

	Progresso barra(hist_vend.size() * prods_promocao.size(), !verbose);
	while(it != hist_vend.end())  // itera sobre o histórico de compra
	{
		long id_cliente = get<TPL_CLIENTE>(it->first);
		long id_produto_hist = get<TLP_PRODUTO>(it->first);
		long id_categoria_hist = get<TLP_CATEGORIA>(it->first);
		string desc_hist = get<TLP_PDESC>(prods[id_produto_hist]);
		vector<string> tokens_hist = get<TLP_PTKS>(prods[id_produto_hist]);
		string desc_cat_hist = catgs[id_categoria_hist];
		double ponderacao = it->second;

		// verifica se este usuário já têm alguma recomendação aberta
		unordered_map<long, RecomCli*>::iterator it_recom;
		it_recom = recomendacoes.find(id_cliente);
		if(it_recom == recomendacoes.end())  // não tem recomendação ainda
			recomendacoes[id_cliente] = new RecomCli(id_cliente, max_recomendacoes);
		RecomCli* local_recom = recomendacoes[id_cliente];

		// itera sobre os produtos em promoção
		unordered_map<long, int>::iterator it_prods_prom;
		it_prods_prom = prods_promocao.begin();
		while(it_prods_prom != prods_promocao.end())
		{
			long id_produto_prom = it_prods_prom->first;
			tuple<long, long> key, catkey;
			key = generate_key(id_produto_hist, id_produto_prom);

			// key já foi marcada como ignore
			if(mat_sim_ignores[key])
			{
				it_prods_prom++;
				barra.registra_iteracao();
				continue;
			}

			int grupo_prod_prom = it_prods_prom->second;
			long id_cat_prom = get<TLP_PCAT>(prods[id_produto_prom]);
			catkey = generate_key(id_categoria_hist, id_cat_prom);
			double coef, catjw = 0;

			// verifica se a similaridade já foi calculada para o item
			unordered_map<ll_t, double, khash_llt, kequal_llt>::iterator mat_it;
			unordered_map<ll_t, double, khash_llt, kequal_llt>::iterator mat_it_cat;

			mat_it = mat_sim.find(key);
			if(mat_it == mat_sim.end())
			{  // ainda não foi calculada
				// coleta descrições
				string desc_prom = get<TLP_PDESC>(prods[id_produto_prom]);
				// calcula o jaro winkler
				coef = jwratio(desc_hist, desc_prom);

				// ignora se o coeficiente de similaridade for menor que 0.75
				if(coef < 0.75)
				{
					// aponta que este é ignorado
					mat_sim_ignores[key] = true;
					it_prods_prom++;
					barra.registra_iteracao();
					continue;
				}

				// calcula tokens coincidentes
				vector<string> tokens_prom = get<TLP_PTKS>(prods[id_produto_prom]);
				unsigned short matches = 0;
				for(j = 0; j < tokens_hist.size(); j++)
					matches += count(tokens_prom.begin(), tokens_prom.end(),
										  tokens_hist.at(j));
				coef *= matches ? sqrt(matches) : 0.5;  // incorpora token

				// verifica se a similaridade já foi calculada para a categoria
				mat_it_cat = mat_sim_cat.find(catkey);
				if(mat_it_cat == mat_sim_cat.end())
				{  // ainda não foi calculada
					catjw = jwratio(desc_cat_hist, catgs[id_cat_prom]);
					coef *= catjw;
					// salva o coeficiente
					mat_sim_cat[catkey] = catjw;
				}
				else
					catjw = mat_it_cat->second;

				coef *= catjw;
				// salva o coeficiente
				mat_sim[key] = coef;
			}
			else  // similaridade já foi calculada
				coef = mat_it->second;

			coef *= ponderacao;  // faz a ponderação do cliente
			// guarda a recomendação de acordo com as regras do objeto
			local_recom->insert(id_produto_prom, grupo_prod_prom, coef);

			barra.registra_iteracao();
			it_prods_prom++;  // incrementa iterador de produtos em promoção
		}

		it++;  // incrementa iterador de histórico dos clientes
	}

	barra.finaliza();

	return recomendacoes;
}

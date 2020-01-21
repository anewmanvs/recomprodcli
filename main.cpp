#include <iostream>
#include <unordered_map>

#include "includes/ctrlcsv.h"
#include "includes/similaridade.h"
#include "includes/generic_tuple_map.h"

// get resources used:
// /usr/bin/time -f "stdout: %O\nRAM: %M\nCPU: %P\nt(s): %e" ./a.out
// get time (refer to real):
// time ./a.out

int main(int argc, char* argv[])
{
    std::unordered_map<lll_t, double, khash_lllt, kequal_lllt> hist_vendas;
    std::unordered_map<long,
        std::tuple<long, std::string, std::vector<std::string> > > prods;
    std::unordered_map<long, std::string> catgs;
    std::unordered_map<long, int> prodsprom;
    std::vector<std::vector<long> > clientes;

    // Leitura dos dados
    std::time_t _hoje = hoje();
    prods = read_csv_produtos("files/descs_cleaned.csv");
    catgs = read_csv_catg("files/categorias.csv");
    prodsprom = read_csv_prodsprom("files/prods_prom.csv");
    clientes = read_csv_clientes("files/usuarios.csv");
    hist_vendas = read_csv_from_clientes_vendas("files/{}_{}_user_hist.csv", _hoje,
                                                clientes);

    std::cout << "Processando um total de " << clientes.size() << " clientes com ";
    std::cout << hist_vendas.size() << " informações de venda" << std::endl;
    // Calculo da similaridade
    // matriz de similaridade dos produtos
    std::unordered_map<ll_t, double, khash_llt, kequal_llt> mat_sim;
    std::unordered_map<ll_t, bool, khash_llt, kequal_llt> mat_sim_ignores;
    std::unordered_map<ll_t, double, khash_llt, kequal_llt> mat_sim_cat;

    std::unordered_map<long, RecomCli*> recomendacoes = matriz_similaridade(
        mat_sim, hist_vendas, prods, prodsprom, mat_sim_ignores, mat_sim_cat,
        catgs, 10, true);

    std::unordered_map<long, RecomCli*>::iterator it = recomendacoes.begin();
    while(it != recomendacoes.end())
    {
        it->second->print();
        it++;
    }
    return 0;
}

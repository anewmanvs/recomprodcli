#include "jaro_winkler.h"

double lev_jaro_ratio(size_t len1, size_t len2, const std::string str1,
					  const std::string str2)
{
    size_t i, j;
    std::string menor, maior;

    menor = str1;
    maior = str2;

    if(len1 > len2)
    {
        i = len1;
        len1 = len2;
        len2 = i;

        menor = str2;
        maior = str1;
    }

    size_t idx[len1] = {0}; // inicializa array com zeros
    size_t halflen = (len1 + 1)/2;
    size_t match = 0;
    for(i = 0; i < halflen; i++)
    {
        for(j = 0; j <= i + halflen; j++)
        {
            if(menor[j] == maior[i] && !idx[j])
            {
                idx[j] = ++match;
                break;
            }
        }
    }
    size_t limit = len1+halflen < len2 ? len1+halflen : len2;
    for(i = halflen; i < limit; i++)
    {
        for(j = i-halflen; j < len1; j++)
        {
            if(menor[j] == maior[i] && !idx[j])
            {
                idx[j] = ++match;
                break;
            }
        }
    }

    if(!match)
        return 0;

    i = 0;
    size_t trans = 0;
    for(j = 0; j < len1; j++)
    {
        if(idx[j])
        {
            i++;
            if(idx[j] != i)
                trans++;
        }
    }

    double md = match;
    return (md/len1 + md/len2 + 1.0 - trans/md/2.0)/3.0;
}

double jwratio(const std::string str1, const std::string str2)
{
    size_t p, m, len1, len2;

    len1 = str1.length();
    len2 = str2.length();
    m = len1 < len2 ? len1 : len2;
    double j = lev_jaro_ratio(len1, len2, str1, str2);
    for(p = 0; p < m; p++)
    {
        if(str1[p] != str2[p])
            break;
    }
    j += (1.0 - j) * p * 0.1;
    return j > 1.0 ? 1.0 : j;
}

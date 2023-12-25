#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

// thank you wikipedia

auto globalMinCut(std::vector<std::vector<int>> mat) -> std::pair<int, std::vector<int>> {
    std::pair<int, std::vector<int>> best = {INT_MAX, {}};
    int n = mat.size();
    std::vector<std::vector<int>> co(n);

    for (int i = 0; i < n; i++)
        co[i] = {i};

    for (int ph = 1; ph < n; ph++) {
        std::vector<int> w = mat[0];
        size_t s = 0, t = 0;
        for (int it = 0; it < n - ph; it++) { // O(V^2) -> O(E log V) with prio. queue
            w[t] = INT_MIN;
            s = t, t = max_element(w.begin(), w.end()) - w.begin();
            for (int i = 0; i < n; i++) w[i] += mat[t][i];
        }
        best = min(best, {w[t] - mat[t][t], co[t]});
        co[s].insert(co[s].end(), co[t].begin(), co[t].end());
        for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
        for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
        mat[0][t] = INT_MIN;
    }

    return best;
}
#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <deque>

void dfs_path(const std::map<std::size_t, std::vector<std::size_t>>& graph,   
    std::set<std::size_t>& unqueued,
    std::deque<std::size_t>& queue,
    std::size_t start) {

    if(unqueued.empty()) return;                // done
    if(!unqueued.contains(start)) return;       // if we're queued already then return

    // go to the back of the queue

    queue.push_back(start);
    unqueued.erase(start);

    // repeat over all unqueued children

    for(const auto & i : graph.at(start)){
        if(unqueued.contains(i)) {
            dfs_path(graph,unqueued,queue,i);
        }
    }
}

int main (int argc, char **argv)
{

    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    for(std::size_t i = 0; i < Context.scanners.size(); i++) {
        assert(test_distinct_fingerprint(Context.scanners[i]));
    }

    // store fingerprints and build an index of scanner[fingerprint]=beacon_pair

    std::vector< std::vector<aoc::fingerprint> > fingerprints;
    std::vector<aoc::fingerprint> c_fing;

    std::vector< std::map<aoc::fingerprint,std::pair<std::size_t,std::size_t>> > lookups;
    std::map<aoc::fingerprint,std::pair<std::size_t,std::size_t>> c_lookup;

    for(std::size_t i = 0; i < Context.scanners.size(); i++) {
        for(std::size_t j = 0; j < Context.scanners[i].size(); j++) {
            for(std::size_t k = j + 1; k < Context.scanners[i].size(); k++) {
                c_fing.emplace_back(Context.scanners[i][j], Context.scanners[i][k]);
                c_lookup[c_fing.back()] = std::make_pair(j,k);
            }
        }
        std::sort(c_fing.begin(), c_fing.end());
        fingerprints.push_back(std::move(c_fing));
        c_fing.clear();

        lookups.push_back(std::move(c_lookup));
        c_lookup.clear();
    }

    // ok for each pair of scanners, how many pairs of detected beacons overlap? 
    // cunning: we will have 11 + 10 + ... + 1 = 11 * 12 / 2 = 66 pair overlaps if 12 beacons overlap
    // want to use scanner 0 as root
    // and problem statement states that the region is contiguous i.e. only one root 
    // so: construct a graph of overlaps
    // memoise which beacon pairs are common 

    std::map<std::size_t, std::vector<std::size_t>> adjacency_graph;
    std::map<std::pair<std::size_t,std::size_t>, std::vector<aoc::fingerprint> > common_fingerprints;

    for(std::size_t i = 0; i < fingerprints.size(); i++) {
        for(std::size_t j = i+1; j < fingerprints.size(); j++) {
            std::vector<aoc::fingerprint> overlap;
            std::set_intersection(
                fingerprints[i].begin(), fingerprints[i].end(), 
                fingerprints[j].begin(), fingerprints[j].end(),
                std::back_insert_iterator(overlap)
            );
            if(overlap.size() >= 66) {
                // std::cout << i << " " << j << " " << overlap.size() << std::endl;
                adjacency_graph[i].push_back(j);
                adjacency_graph[j].push_back(i);
                common_fingerprints[std::make_pair(i,j)] = overlap;
                common_fingerprints[std::make_pair(j,i)] = overlap;
            }
        }
    }

    for(auto & i : adjacency_graph) std::sort(i.second.begin(), i.second.end());

    // arbitrarily select 0 as root
    // form a queue of scanners to visit
    // select first scanner that neighbours a rotated scanner
    // for rotated scanner, store (orientated!) set of pair direction vectors
    // for new scanner, iterate over rotations until we get a matching set
    // store orientation

    std::set<std::size_t> unqueued_scanners;
    std::deque<std::size_t> rotation_queue;

    for(std::size_t i = 0; i < Context.scanners.size(); i++) unqueued_scanners.insert(i);

    dfs_path(adjacency_graph,unqueued_scanners,rotation_queue,0);
    
    std::vector<int> needed_rotations; 
    needed_rotations.resize(Context.scanners.size());
    needed_rotations[0] = 1;

    std::vector<aoc::point> displacements;
    displacements.resize(Context.scanners.size());

    for(std::size_t i = 1; i < rotation_queue.size(); i++) {

        std::size_t current_scanner, prior_scanner;

        current_scanner = rotation_queue[i];
        for(std::size_t z = 0; z < adjacency_graph[current_scanner].size(); z++) {
            if(needed_rotations[adjacency_graph[current_scanner][z]] != 0) {
                prior_scanner = adjacency_graph[current_scanner][z];
                break;
            }
        }

        std::vector<aoc::fingerprint> common_pair_fingerprints =
            common_fingerprints[std::make_pair(prior_scanner, current_scanner)];

        // Idea: we know that if a point is in common, then
        // x_0 = R(x_1) + d
        // i.e. rotation and shift. need three points for uniqueness
        //
        // so: use fingerprints to find two beacon pairs with a common beacon
        // try rotations until rotated pair 1 and rotated pair 2 parallel with equivalents in origin

        aoc::fingerprint finger_1, finger_2;
        std::size_t index_1, index_2, index_3, index_4;

        aoc::point common_prior, longer_prior, shorter_prior;
        aoc::point common_current, longer_current, shorter_current;
        aoc::point the_root, the_end, the_other_end;

        long long mag_1, mag_2;

        bool done = false;

        for(auto j = common_pair_fingerprints.begin(); j != common_pair_fingerprints.end(); j++) {
            for(auto k = std::next(j); k != common_pair_fingerprints.end(); k++) {
                index_1 = lookups[current_scanner][*j].first;
                index_2 = lookups[current_scanner][*j].second;
                index_3 = lookups[current_scanner][*k].first;
                index_4 = lookups[current_scanner][*k].second;

                if(index_1 == index_3)
                {   
                    the_root = Context.scanners[current_scanner][index_1];
                    the_end = Context.scanners[current_scanner][index_2];
                    the_other_end = Context.scanners[current_scanner][index_4];
                } else if(index_1 == index_4)
                {   
                    the_root = Context.scanners[current_scanner][index_1];
                    the_end = Context.scanners[current_scanner][index_2];
                    the_other_end = Context.scanners[current_scanner][index_3];
                } else if(index_2 == index_4)
                {   
                    the_root = Context.scanners[current_scanner][index_2];
                    the_end = Context.scanners[current_scanner][index_1];
                    the_other_end = Context.scanners[current_scanner][index_3];
                }else if(index_2 == index_3)
                {   
                    the_root = Context.scanners[current_scanner][index_2];
                    the_end = Context.scanners[current_scanner][index_1];
                    the_other_end = Context.scanners[current_scanner][index_4];
                }
                else {
                    continue;
                }
                mag_1 = magnitude_sq(the_end-the_root);
                mag_2 = magnitude_sq(the_other_end-the_root);
                if(mag_1 == mag_2) {
                    continue; // want different lengths to avoid any ambiguity
                }

                common_current = the_root;
                if(mag_1 > mag_2) {
                    longer_current = the_end;
                    shorter_current = the_other_end;
                } else { // guaranteed strict comparison
                    longer_current = the_other_end;
                    shorter_current = the_end;
                }

                finger_1 = *j;
                finger_2 = *k;

                done = true;
                break;
            }
            if(done) break;
        }

        // now get prior, but know the lookups already hooray

        index_1 = lookups[prior_scanner][finger_1].first;
        index_2 = lookups[prior_scanner][finger_1].second;
        index_3 = lookups[prior_scanner][finger_2].first;
        index_4 = lookups[prior_scanner][finger_2].second;

        if(index_1 == index_3)
        {   
            the_root = Context.scanners[prior_scanner][index_1];
            the_end = Context.scanners[prior_scanner][index_2];
            the_other_end = Context.scanners[prior_scanner][index_4];
        } else if(index_1 == index_4)
        {   
            the_root = Context.scanners[prior_scanner][index_1];
            the_end = Context.scanners[prior_scanner][index_2];
            the_other_end = Context.scanners[prior_scanner][index_3];
        } else if(index_2 == index_4)
        {   
            the_root = Context.scanners[prior_scanner][index_2];
            the_end = Context.scanners[prior_scanner][index_1];
            the_other_end = Context.scanners[prior_scanner][index_3];
        }else if(index_2 == index_3)
        {   
            the_root = Context.scanners[prior_scanner][index_2];
            the_end = Context.scanners[prior_scanner][index_1];
            the_other_end = Context.scanners[prior_scanner][index_4];
        }
        else {
            assert(false); // should never happen
        }
        mag_1 = magnitude_sq(the_end-the_root);
        mag_2 = magnitude_sq(the_other_end-the_root);
        if(mag_1 == mag_2) {
            continue; // want different lengths to avoid any ambiguity
        }

        common_prior = the_root;
        if(mag_1 > mag_2) {
            longer_prior = the_end;
            shorter_prior = the_other_end;
        } else { // guaranteed strict comparison
            longer_prior = the_other_end;
            shorter_prior = the_end;
        }

        // ok. rotate the current root and translate it to match the prior root.
        // apply the rotation and translation to the other two points.
        // if it works - great! else iterate

        aoc::point rotated_translated_prior_root = rotate(common_prior,needed_rotations[prior_scanner]) 
            + displacements[prior_scanner];
        aoc::point rotated_translated_prior_longer = rotate(longer_prior,needed_rotations[prior_scanner])
            + displacements[prior_scanner];
        aoc::point rotated_translated_prior_shorter = rotate(shorter_prior,needed_rotations[prior_scanner])
            + displacements[prior_scanner];

        for(int i = 1; i <= 24; i++) {
            aoc::point rotated_current_root = rotate(common_current,i);
            aoc::point implied_translation = rotated_translated_prior_root - rotated_current_root;

            aoc::point rotated_translated_current_shorter = rotate(shorter_current,i) + implied_translation;
            aoc::point rotated_translated_current_longer = rotate(longer_current,i) + implied_translation;

            if(rotated_translated_prior_longer == rotated_translated_current_longer
                && rotated_translated_current_shorter == rotated_translated_prior_shorter) {
                    needed_rotations[current_scanner] = i;
                    displacements[current_scanner] = implied_translation;
                    break;
            }
        }
    }

    std::set<aoc::point> beacon_locations;

    for(std::size_t i = 0; i < Context.scanners.size(); i++) {
        for(const auto & j : Context.scanners[i]) {
            beacon_locations.insert(rotate(j,needed_rotations[i]) + displacements[i]);
        }
    }

    std::cout << beacon_locations.size() << std::endl;

    long long largest_manhattan_distance = 0, temp_dist;

    for(std::size_t i = 0; i < displacements.size(); i++) {
        for(std::size_t j = i+1; j < displacements.size(); j++) {
            temp_dist = manhattan_distance(displacements[i],displacements[j]);
            largest_manhattan_distance = std::max(temp_dist, largest_manhattan_distance);
        }
    }

    std::cout << largest_manhattan_distance << std::endl;

    return 0;

}
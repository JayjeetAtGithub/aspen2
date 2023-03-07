#include "../graph/api.h"
#include "../trees/utils.h"
#include "../lib_extensions/sparse_table_hash.h"
#include "../graph/tree_plus/immutable_graph_tree_plus.h"
#include "../pbbslib/random_shuffle.h"

#include <cstring>

#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>


#include <iostream>
#include <fstream>

#include "rmat_util.h"


int main(int argc, char* argv[]) {
    auto tree_plus_graph = empty_treeplus_graph();  

    // Inserting key/value pairs one at a time
    std::vector<std::tuple<uintV, uintV>> stream_of_kv;
    auto r = pbbs::random(200000);
    for (int i = 0; i < 100000; i++) {
        stream_of_kv.push_back(std::make_tuple(r.ith_rand(2*i), r.ith_rand((2*i)+1)));        
    }
    
    timer st;
    st.start();
    for (auto kv : stream_of_kv) {
        std::cout << "Inserting: " << std::get<0>(kv) << ", " << std::get<1>(kv) << std::endl;
        tree_plus_graph.insert_edges_batch(1, &kv);
    }
    double runtime = st.stop();
    std::cout << "runtime (one-at-a-time) = " << runtime << std::endl;

    // Inserting key/value pairs in batches
    st.start();
    tree_plus_graph.insert_edges_batch(stream_of_kv.size(), stream_of_kv.data());
    runtime = st.stop();
    std::cout << "runtime (batch) = " << runtime << std::endl;

    //// using just the tree plus 
    
    
    sym_immutable_graph_tree_plus tree_plus;
    st.start();
    tree_plus.insert_edges_batch(stream_of_kv.size(), stream_of_kv.data());
    runtime = st.stop();
    std::cout << "runtime (tree plus) = " << runtime << std::endl;
    std::cout << "size: " << tree_plus.size_in_bytes() << std::endl;

}

// void parallel_updates(commandLine& P) {
//   string update_fname = P.getOptionValue("-update-file", "updates.dat");

//   auto VG = initialize_treeplus_graph(P);

//   cout << "calling acquire_version" << endl;
//   auto S = VG.acquire_version();
//   cout << "acquired!" << endl;
//   const auto& GA = S.graph;
//   size_t n = GA.num_vertices();
//   cout << "n = " << n << endl;
//   VG.release_version(std::move(S));

//   using pair_vertex = tuple<uintV, uintV>;

//   auto r = pbbs::random();
//   // 2. Generate the sequence of insertions and deletions

//   auto update_sizes = pbbs::sequence<size_t>(10);
//   update_sizes[0] = 10;
//   update_sizes[1] = 100;
//   update_sizes[2] = 1000;
//   update_sizes[3] = 10000;
//   update_sizes[4] = 100000;
//   update_sizes[5] = 1000000;
//   update_sizes[6] = 10000000;
//   update_sizes[7] = 100000000;
//   update_sizes[8] = 1000000000;
//   update_sizes[9] = 2000000000;

//   auto update_times = std::vector<double>();
//   size_t n_trials = 3;

//   size_t start = 0;
//   for (size_t us=start; us<update_sizes.size(); us++) {
//     double avg_insert = 0.0;
//     double avg_delete = 0.0;
//     cout << "Running bs: " << update_sizes[us] << endl;

//     if (update_sizes[us] < 100000000) {
//       n_trials = 20;
//     }
//     else {
//       n_trials = 3;
//     }

//     for (size_t ts=0; ts<n_trials; ts++) {
//       size_t updates_to_run = update_sizes[us];
//       auto updates = pbbs::sequence<pair_vertex>(updates_to_run);

//       double a = 0.5;
//       double b = 0.1;
//       double c = 0.1;
//       size_t nn = 1 << (pbbs::log2_up(n) - 1);
//       auto rmat = rMat<uintV>(nn, r.ith_rand(0), a, b, c);

//       parallel_for(0, updates.size(), [&] (size_t i) {
//         updates[i] = rmat(i);
//       });

//       {
//         //cout << "Inserting" << endl;
//         timer st; st.start();
//         VG.insert_edges_batch(update_sizes[us], updates.begin(), false, true, nn, false);
//         double batch_time = st.stop();

//         // cout << "batch time = " << batch_time << endl;
//         avg_insert += batch_time;
//       }

//       {
//         // cout << "Deleting" << endl;
//         timer st; st.start();
//         VG.delete_edges_batch(update_sizes[us], updates.begin(), false, true, nn, false);
//         double batch_time = st.stop();

//         // cout << "batch time = " << batch_time << endl;
//         avg_delete += batch_time;
//       }

//     }
//     // cout << "Finished bs: " << update_sizes[us] << endl;
//     cout << "Avg insert: " << (avg_insert / n_trials) << endl;
//     cout << "Avg delete: " << (avg_delete / n_trials) << endl << endl;
//   }
// }

// int main(int argc, char** argv) {
//   cout << "Running with " << num_workers() << " threads" << endl;
//   commandLine P(argc, argv, "./test_graph [-f file -m (mmap) <testid>]");

//   parallel_updates(P);
// }

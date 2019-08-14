# include <iostream>
# include <cstdlib>
# include <ctime>
# include <iomanip>
# include <iostream>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
# include <map>
# include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <mpi.h>
#include <math.h>
#include <chrono>
#include <cstdlib>


//****************************************************************************80
using namespace std;

const int kProfileLength = 30;
const int kNumAlph = 20;

std::array<char, kNumAlph> kAlphabets = {'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K',
                                          'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V',
                                          'W', 'Y'};

std::map<char, int> letter_int_map;
std::map<int, char> int_letter_map;



struct Matrix {
  std::string initial_segment;
  std::string origin; // id of the origin
  int Kmatches;	   //number of segments with score > score(p)
  double score;  //score(p)
//  std::array<std::array<double, kNumAlph>, kProfileLength> freq;
  std::array<std::array<double, kNumAlph>, kProfileLength> freq;
//  double* freq[kProfileLength*kNumAlph];
};


int random_index_30[11][30] = {{21, 1, 12, 4, 15, 8, 13, 16, 13, 13, 24, 23, 9, 3, 11, 27, 28, 28, 15, 2, 14,
                                                                                                              15, 17, 15, 4, 14, 8, 14, 2, 20},
                               {13, 10, 25, 20, 22, 2, 13, 11, 21, 20, 10, 15, 1, 10, 24, 10, 17, 6, 5, 20,
                                                                                                          16, 2, 9, 3, 5, 9, 26, 25, 9, 17},
                               {12, 15, 5, 7, 17, 19, 25, 22, 19, 15, 29, 24, 7, 29, 21, 29, 20, 5, 8, 28, 28,
                                                                                                              5, 27, 14, 28, 26, 10, 12, 4, 8},
                               {6, 16, 23, 18, 16, 26, 3, 20, 15, 13, 20, 18, 25, 19, 6, 17, 5, 2, 23, 25, 14,
                                                                                                              14, 6, 19, 23, 28, 1, 20, 29, 21},
                               {10, 14, 22, 10, 17, 14, 17, 20, 4, 14, 20, 4, 27, 16, 4, 25, 9, 15, 4, 5, 20,
                                                                                                              21, 12, 1, 13, 10, 21, 11, 25, 13},
                               {18, 2, 1, 16, 23, 20, 11, 10, 29, 8, 22, 25, 1, 6, 1, 25, 6, 12, 25, 15, 3,
                                                                                                              17, 1, 9, 23, 22, 13, 24, 20, 26},
                               {18, 19, 8, 17, 29, 21, 21, 14, 14, 28, 13, 9, 11, 17, 18, 26, 29, 25, 18, 8,
                                 7, 16, 12, 27, 24, 27, 8, 24, 19, 10},
                               {28, 20, 20, 25, 22, 27, 21, 17, 14, 25, 22, 10, 13, 1, 18, 17, 23, 8, 14, 11,
                                 17, 9, 19, 14, 23, 29, 29, 9, 5, 7},
                               {27, 27, 20, 3, 18, 25, 21, 10, 11, 3, 29, 5, 10, 15, 20, 11, 18, 23, 26, 2,
                                 26, 5, 27, 9, 20, 21, 14, 13, 1, 24},
                               {19, 22, 8, 29, 4, 10, 16, 8, 14, 11, 20, 21, 17, 9, 9, 1, 4, 12, 3, 22, 16,
                                                                                                              21, 24, 20, 20, 4, 1, 1, 11, 27},
                               {8, 3, 10, 17, 6, 18, 19, 14, 5, 9, 23, 14,
                                 16, 10, 13, 11, 21, 11, 11, 2, 20, 21, 6, 9,
                                                                                                                          15, 27, 6, 8, 7, 8}};


//int random_index_50[11][50] =
//  {{46,20,41,29,21,26,37,13,17,36,47,8,25,43,9,48,39,24,1,5,34,45,32,18,14,40,38,28,7,6,11,27,0,44,33,19,16,35,23,4,42,49,2,15,30,12,3,10,22,31},
//   {26,41,10,42,36,32,44,16,21,5,20,34,37,33,35,0,46,45,19,18,39,9,27,1,7,13,8,14,28,15,4,43,48,12,29,25,24,40,31,30,38,47,23,11,3,22,2,49,17,6},
//   {17,10,21,31,40,18,44,39,30,29,33,41,38,34,15,7,5,23,36,35,47,42,25,19,48,11,3,37,20,43,9,46,1,26,28,22,12,0,4,24,27,14,32,8,16,6,45,49,2,13},
//   {48,1,42,34,15,25,30,35,38,2,46,43,45,47,33,0,20,11,14,7,21,32,49,23,37,5,13,39,18,36,28,26,9,22,31,41,19,10,24,8,6,17,44,40,16,29,4,3,27,12},
//   {35,48,29,36,47,45,40,11,33,24,9,8,32,15,43,4,34,5,21,6,14,30,13,28,2,26,39,19,31,46,37,41,1,18,38,49,10,17,44,23,22,3,27,20,25,16,12,42,7,0},
//   {37,2,8,0,19,36,39,20,13,47,22,31,7,48,32,26,11,42,17,41,44,10,5,49,45,40,1,30,38,29,24,18,43,6,15,3,14,35,34,46,4,23,33,12,28,21,16,27,25,9},
//   {31,37,39,41,44,12,18,26,47,24,36,16,8,15,43,6,30,49,38,35,14,19,22,9,28,34,48,5,45,32,27,23,25,46,21,42,40,11,17,0,20,29,13,10,7,1,2,4,3,33},
//   {2,35,10,39,47,28,31,44,43,29,37,6,46,45,30,1,16,32,5,25,24,18,11,34,26,21,38,14,22,7,20,23,17,42,33,48,0,19,41,49,12,9,40,4,13,8,27,3,36,15},
//   {28,30,33,14,11,48,26,46,21,1,35,49,15,43,37,42,24,16,10,13,34,25,44,23,41,12,9,7,4,20,8,5,29,45,39,17,0,3,31,27,19,36,6,22,40,47,2,32,18,38},
//   {42,33,40,26,0,4,36,22,37,19,8,17,30,46,31,48,44,29,45,28,15,41,13,47,27,16,10,5,24,3,35,38,21,49,34,43,1,25,39,20,12,2,9,7,14,18,6,32,23,11},
//   {27,30,16,49,38,33,11,36,0,32,42,7,26,41,40,44,45,4,21,47,35,12,31,13,28,15,2,19,39,22,3,18,5,34,10,48,43,6,29,24,46,14,37,17,1,25,20,23,8,9}};


void write_Matrices(std::string filename, std::vector<Matrix> matrices) {
  ofstream file;
  file.open(filename);
  for (Matrix matrix: matrices) {
    file << "PROTOTYPE 1\n";
    file << "BEGIN\n";
    file << "SEGMENT " << matrix.initial_segment << "\n";
    char formatted_line[1000];
    std::sprintf(formatted_line, "MATRIX Kmatches=%d Score=%f",
                 matrix.Kmatches, matrix.score);
    file << formatted_line << endl;
    fill(formatted_line, formatted_line+100, 0);
    file << "30 ";
    
    for (char letter: kAlphabets) {
      file << "    " << letter << " ";
    }
    file << "\n";
    for (int i = 0; i < kProfileLength; i++) {
      std::sprintf(formatted_line, "%2d ", i);
//      file << formatted_line;
//      fill(formatted_line, formatted_line+1000, 0);
      for (int j=0;j<kNumAlph;j++){
        int output_int = matrix.freq[i][j];
        std::sprintf(formatted_line+6*j*sizeof(char)+3, "%5d ", output_int);
      }
      file << formatted_line << endl;
      fill(formatted_line, formatted_line+1000, 0);
    }
    file << "END\n";
  }
  file.close();
}

std::vector<double> make_composition(const std::vector<std::vector<int>>&
proteome){
  std::vector<double> composition(kNumAlph);
  double compos_sum = 0.0;
  for (const std::vector<int>& line: proteome){
    for (int letter_i: line){
      composition[letter_i] += 1;
      compos_sum += 1;
    }
  }
  for (double& value: composition){
    value /= compos_sum;
  }
  return composition;
}

template <typename... Args>
void load(const std::string& filename, Args&... outputs){
  std::ifstream file;
  file.open(filename, std::ios_base::binary);
  {
    cereal::BinaryInputArchive iarchive(file); // Create an output archive
    iarchive(outputs...);
  }
}


int main(int argc, char *argv[]) {
  for (int i=0;i<20;i++){
    letter_int_map[kAlphabets[i]] = i;
    int_letter_map[i] = kAlphabets[i];
  }

  int rank;
  int num_p;


  for (int i = 0; i < argc; i++) {
    cout << *argv[i] << endl;
  }
  cout << argc << endl;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &num_p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  for (int i=0;i<kNumAlph;i++){
    letter_int_map[kAlphabets[i]] = i;
    int_letter_map[i] = kAlphabets[i];
  }
  
  int theta = 10;
  
  const double omega = 1.0;
  
  std::string proteome_filename = "proteome_binary";
  std::vector<std::string> headers;
  std::vector<std::vector<int>> sequences;
  cout << "Proteome_size: " << sequences.size() << endl;
  load(proteome_filename, headers, sequences);
  long long int sequence_length = sequences.size();
  long long int seq_denom = sequence_length / 100;


// Encode seed into vector<string>, each of spacing 10
  std::string seed_filename = "seed_seq_binary";
  std::vector<std::vector<int>> seed_seqs;
  load(seed_filename, seed_seqs);
  cout << "seed_seqs_size: " << seed_seqs.size() << endl;
  
  srand(time(NULL) + rank);
// Encode blosum
  std::string blosum_filename = "blosum_binary";
  std::vector<std::vector<double>> kBlosum;
  load(blosum_filename, kBlosum);
  cout << "Blosum_size: " << kBlosum.size() << endl;
  
  const double epsilon = 1;
  std::vector<Matrix> converged_matrices;
  
  int K_min = 1;
  int kLetterX_i = 99;
  
  std::vector<double> composition = make_composition(sequences);
  double composition_sum_squares = 0.0;
  for (double value: composition){
    composition_sum_squares += value * value;
  }

//  std::string initial_seq = load_seed_seq(seed_seq_filename);
//  std::vector<std::string> initial_seqs = split_seq(initial_seq, 10,
//    kProfileLength);
  for (std::vector<int> seed_seq: seed_seqs) {
    cout << "\nnext" << endl;
    cout << "seed_seq: ";
    for (int i=0;i<seed_seq.size();i++){
      cout << int_letter_map[seed_seq[i]];
    }
    cout << endl;
  
    auto* PSSM = new double[kProfileLength*kNumAlph];
    auto* M = new double[kProfileLength*kNumAlph];
    auto* PSSM2 = new double[kProfileLength*kNumAlph];
    auto* M1 = new double[kProfileLength*kNumAlph];
  
    auto* freq = new double[kProfileLength*kNumAlph];
    auto* other_freq = new double[kProfileLength*kNumAlph];
    for (int i=0;i<kProfileLength*kNumAlph;i++){
      PSSM[i] = 0.0;
      PSSM2[i] = 0.0;
      M[i] = 0.0;
    }

    
//    std::array<std::array<double, kNumAlph>, kProfileLength> M = {0};
////    std::array<std::array<double, kNumAlph>, kProfileLength> PSSM = {0};
//    std::array<std::array<double, kNumAlph>, kProfileLength> PSSM2 = {0};
    
    for (int i = 0; i < kProfileLength; i++) {
      int j = seed_seq[i];
      if (j != kLetterX_i){
        M[i*kNumAlph+j] = 1.0;
        for (int k=0;k<kNumAlph; k++){
          PSSM[i*kNumAlph+k] = kBlosum[j][k];
        }
      }
    }
    
    int converged = 0;
    double S;
    std::array<double, kProfileLength> Information = {0};
    int iteration = 0;
    auto t1 = std::chrono::high_resolution_clock::now();

    while (converged == 0) {
      for (int i = 0; i < kProfileLength; i++) {
        double H_sum = 0.0;
        for (int j = 0; j < kNumAlph; j++) {
          if (M[i*kNumAlph+j] > 0.0) {
            H_sum += M[i*kNumAlph+j] * (log(M[i*kNumAlph+j]) / log(2));
          }
        }
        Information[i] = log(kNumAlph) / log(2) + H_sum;
        
        if (Information[i] < 1.0) {
          Information[i] = 0.0;
        }
      }
  
  
      double maxS[num_p];
      maxS[rank] = (double) - INFINITY;
      
      /////////////////// BACKGROUND DISTRIBUTIONS ///////////////////
      int kNumberRandomisations = 10;
      for (int rand_count = 0;
           rand_count < kNumberRandomisations; rand_count++) {
        for (int seq_i=0;seq_i<sequences.size()/(kNumberRandomisations*num_p);
        seq_i++) {
          int selected_i = seq_i*(kNumberRandomisations*num_p) + rand()%(kNumberRandomisations*num_p);
          const std::vector<int>& protein = sequences[selected_i];
//        for (const std::vector<int>& protein: sequences) {
          int protein_length = (int) protein.size();
          for (int m = 0; m + kProfileLength <= protein_length; m++) {
            S = 0.0;
            for (int j = 0; j < kProfileLength; j++) {
              // compare 50 residues in one fragment from proteome with reshuffled matrix position
              if (protein[m + j] != kLetterX_i) {
                int rand_i = random_index_30[rand_count][j];
                S += Information[rand_i] * PSSM[rand_i*kNumAlph+protein[m + j]];
              }
            }
            S /= kProfileLength;
            if (S > maxS[rank]) {
              maxS[rank] = S;
            }
          }
        }
      }
      if (rank != 0){
        MPI_Send(maxS+rank, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
      } else{
        for (int i=1;i<num_p;i++){
          MPI_Recv(maxS+i, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
        }
      }
      auto final_maxS = (double) -INFINITY;
      if (rank == 0){
        for (int i=0;i<num_p;i++){
          if (maxS[i] > final_maxS){
            final_maxS = maxS[i];
          }
        }
      }
      MPI_Bcast(&final_maxS, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      int Kmatches = 0;
      
      for (int i=0;i<kProfileLength*kNumAlph;i++){
        freq[i] = 0.0;
      }
      
//      std::array<std::array<double, kNumAlph>, kProfileLength> freq = {0};

      for (int seq_i=0;seq_i<sequences.size()/num_p;seq_i++) {
        const std::vector<int>& protein = sequences[seq_i*num_p+rank];
        int protein_length = (int) protein.size();
        for (int m = 0; m + kProfileLength <= protein_length; m++) {
          S = 0.0;
          for (int j = 0; j < kProfileLength; j++) {
            // compare 50 residues in one fragment from proteome with reshuffled matrix position
            if (protein[m + j] != kLetterX_i) {
              S += Information[j] * PSSM[j*kNumAlph+protein[m + j]];
            }
          }
          S /= kProfileLength;
          if (S > final_maxS) {
            Kmatches++;
            for (int j = 0; j < kProfileLength; j++) {
              if (protein[m + j] != kLetterX_i) {
                freq[j*kNumAlph+protein[m + j]]++;
              }
            }
          }
        }
      }
      if (rank != 0){
        MPI_Send(&Kmatches, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
      } else{
        int other_Kmatches = 0;
        for (int i=1;i<num_p;i++){
          MPI_Recv(&other_Kmatches, 1, MPI_INT, i, 2, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
          Kmatches += other_Kmatches;
        }
      }
      if (rank != 0){
        MPI_Send(freq, kProfileLength*kNumAlph, MPI_DOUBLE, 0, 3,
          MPI_COMM_WORLD);
      } else{
        for (int i=1;i<num_p;i++){
          MPI_Recv(other_freq, kProfileLength*kNumAlph, MPI_DOUBLE, i, 3,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          for (int i=0;i<kProfileLength*kNumAlph;i++){
            freq[i] += other_freq[i];
          }
        }
      }
      
      for (int i=0;i<kProfileLength*kNumAlph;i++){
        M1[i] = 0.0;
      }
      if (rank == 0){
        for (int q=0;q<kNumAlph;q++) {
          for (int i=0;i<kProfileLength;i++) {
            M1[i*kNumAlph+q] = (freq[i*kNumAlph+q] + omega * composition[q] *
              composition[q]) / (Kmatches + omega * composition_sum_squares);
            PSSM[i*kNumAlph+q] = log(M1[i*kNumAlph+q] / composition[q]);
          }
        }

        double distance = 0.0;
        for (int i=0;i<kProfileLength;i++) {
          double sum = 0.0;
          for (int q=0;q<kNumAlph;q++) {
            sum += pow(M1[i*kNumAlph+q] - M[i*kNumAlph+q], 2);
          }
          distance += sqrt(sum);
        }
        iteration++;
        if (distance < epsilon || iteration >= theta) {
          converged = 1;
          if (Kmatches < K_min) {
            break;
          }
          Matrix matrix;
          vector<char> seed_seq_v;
          for(int letter_i: seed_seq){
            seed_seq_v.push_back(int_letter_map[letter_i]);
          }
          std::string seed_seq_letters(seed_seq_v.begin(), seed_seq_v.end());
          matrix.initial_segment = seed_seq_letters;
          matrix.Kmatches = Kmatches;
          matrix.score = final_maxS;
          for (int i=0;i<kProfileLength;i++){
            for (int j=0;j<kNumAlph;j++){
              matrix.freq[i][j] = freq[i*kNumAlph+j];
            }
          }
          converged_matrices.push_back(matrix);
        } else {
          M = M1;
        }
      }
      MPI_Bcast(M, kNumAlph*kProfileLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//      MPI_Bcast(M1, kNumAlph*kProfileLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(PSSM, kNumAlph*kProfileLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&converged, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "duration: " << duration << endl;
  }
  if (rank == 0) {
    write_Matrices("output.1.matrix", converged_matrices);
  }
  MPI_Finalize();
  return 0;
}











//
//
//
//# include <iostream>
//# include <cstdlib>
//# include <ctime>
//# include <iomanip>
//# include <iostream>
//# include <mpi.h>
//#include <cereal/types/vector.hpp>
//#include <cereal/types/string.hpp>
//#include <cereal/archives/binary.hpp>
//
//# include <string>
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include <set>
//#include <stdlib.h>
//#include <assert.h>
//#include <math.h>
//#include <stdio.h>
//
//
////todo: what if we really have X? How do we deal with it? Also, need to add
//// qualifier for each call to proteome/seed_seq that if it is 99, to skip.
//
//
////****************************************************************************80
//using namespace std;
//
//std::array<char, 20> kAlphabets = {'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K',
//                              'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V',
//                              'W', 'Y'};
//std::map<char, int> letter_int_map;
//std::map<int, char> int_letter_map;
//
//const int kProfileLength = 30;
//
//struct Matrix {
//  std::string initial_segment;
//  std::string origin; // id of the origin
////  int id;	   //matrix id
//  int K;	   //number of segments with score > score(p)
//  int N;	   //number of segments in proteome
////  int L;	   //length of profile in amino acids <=MAX_PSSM_LENGTH
//  double p;	   //p-value
//  double score;  //score(p)
//  double omega;  //pseudocount coefficient
//  std::vector<std::vector<double>> freq;
//  //frequencies:
//  // 0..1
//};
//
//
//
//int random_index_30[11][30] = {{21, 1, 12, 4, 15, 8, 13, 16, 13, 13, 24, 23, 9, 3, 11, 27, 28, 28, 15, 2, 14,
//                                                                                                              15, 17, 15, 4, 14, 8, 14, 2, 20},
//                               {13, 10, 25, 20, 22, 2, 13, 11, 21, 20, 10, 15, 1, 10, 24, 10, 17, 6, 5, 20,
//                                                                                                          16, 2, 9, 3, 5, 9, 26, 25, 9, 17},
//                               {12, 15, 5, 7, 17, 19, 25, 22, 19, 15, 29, 24, 7, 29, 21, 29, 20, 5, 8, 28, 28,
//                                                                                                              5, 27, 14, 28, 26, 10, 12, 4, 8},
//                               {6, 16, 23, 18, 16, 26, 3, 20, 15, 13, 20, 18, 25, 19, 6, 17, 5, 2, 23, 25, 14,
//                                                                                                              14, 6, 19, 23, 28, 1, 20, 29, 21},
//                               {10, 14, 22, 10, 17, 14, 17, 20, 4, 14, 20, 4, 27, 16, 4, 25, 9, 15, 4, 5, 20,
//                                                                                                              21, 12, 1, 13, 10, 21, 11, 25, 13},
//                               {18, 2, 1, 16, 23, 20, 11, 10, 29, 8, 22, 25, 1, 6, 1, 25, 6, 12, 25, 15, 3,
//                                                                                                              17, 1, 9, 23, 22, 13, 24, 20, 26},
//                               {18, 19, 8, 17, 29, 21, 21, 14, 14, 28, 13, 9, 11, 17, 18, 26, 29, 25, 18, 8,
//                                 7, 16, 12, 27, 24, 27, 8, 24, 19, 10},
//                               {28, 20, 20, 25, 22, 27, 21, 17, 14, 25, 22, 10, 13, 1, 18, 17, 23, 8, 14, 11,
//                                 17, 9, 19, 14, 23, 29, 29, 9, 5, 7},
//                               {27, 27, 20, 3, 18, 25, 21, 10, 11, 3, 29, 5, 10, 15, 20, 11, 18, 23, 26, 2,
//                                 26, 5, 27, 9, 20, 21, 14, 13, 1, 24},
//                               {19, 22, 8, 29, 4, 10, 16, 8, 14, 11, 20, 21, 17, 9, 9, 1, 4, 12, 3, 22, 16,
//                                                                                                              21, 24, 20, 20, 4, 1, 1, 11, 27},
//                               {8, 3, 10, 17, 6, 18, 19, 14, 5, 9, 23, 14,
//                                16, 10, 13, 11, 21, 11, 11, 2, 20, 21, 6, 9,
//                                15, 27, 6, 8, 7, 8}};
//
//
////int random_index_50[11][50] =
////  {{46,20,41,29,21,26,37,13,17,36,47,8,25,43,9,48,39,24,1,5,34,45,32,18,14,40,38,28,7,6,11,27,0,44,33,19,16,35,23,4,42,49,2,15,30,12,3,10,22,31},
////   {26,41,10,42,36,32,44,16,21,5,20,34,37,33,35,0,46,45,19,18,39,9,27,1,7,13,8,14,28,15,4,43,48,12,29,25,24,40,31,30,38,47,23,11,3,22,2,49,17,6},
////   {17,10,21,31,40,18,44,39,30,29,33,41,38,34,15,7,5,23,36,35,47,42,25,19,48,11,3,37,20,43,9,46,1,26,28,22,12,0,4,24,27,14,32,8,16,6,45,49,2,13},
////   {48,1,42,34,15,25,30,35,38,2,46,43,45,47,33,0,20,11,14,7,21,32,49,23,37,5,13,39,18,36,28,26,9,22,31,41,19,10,24,8,6,17,44,40,16,29,4,3,27,12},
////   {35,48,29,36,47,45,40,11,33,24,9,8,32,15,43,4,34,5,21,6,14,30,13,28,2,26,39,19,31,46,37,41,1,18,38,49,10,17,44,23,22,3,27,20,25,16,12,42,7,0},
////   {37,2,8,0,19,36,39,20,13,47,22,31,7,48,32,26,11,42,17,41,44,10,5,49,45,40,1,30,38,29,24,18,43,6,15,3,14,35,34,46,4,23,33,12,28,21,16,27,25,9},
////   {31,37,39,41,44,12,18,26,47,24,36,16,8,15,43,6,30,49,38,35,14,19,22,9,28,34,48,5,45,32,27,23,25,46,21,42,40,11,17,0,20,29,13,10,7,1,2,4,3,33},
////   {2,35,10,39,47,28,31,44,43,29,37,6,46,45,30,1,16,32,5,25,24,18,11,34,26,21,38,14,22,7,20,23,17,42,33,48,0,19,41,49,12,9,40,4,13,8,27,3,36,15},
////   {28,30,33,14,11,48,26,46,21,1,35,49,15,43,37,42,24,16,10,13,34,25,44,23,41,12,9,7,4,20,8,5,29,45,39,17,0,3,31,27,19,36,6,22,40,47,2,32,18,38},
////   {42,33,40,26,0,4,36,22,37,19,8,17,30,46,31,48,44,29,45,28,15,41,13,47,27,16,10,5,24,3,35,38,21,49,34,43,1,25,39,20,12,2,9,7,14,18,6,32,23,11},
////   {27,30,16,49,38,33,11,36,0,32,42,7,26,41,40,44,45,4,21,47,35,12,31,13,28,15,2,19,39,22,3,18,5,34,10,48,43,6,29,24,46,14,37,17,1,25,20,23,8,9}};
//
//
//void write_Matrices(std::string filename, std::vector<Matrix> matrices) {
//  ofstream file;
//  file.open (filename);
//  for (Matrix matrix: matrices) {
//    file << "PROTOTYPE 1\n";
//    file << "BEGIN\n";
//    file << "SEGMENT " << matrix.initial_segment << "\n";
//    char formatted_line[1000];
//    std::sprintf(formatted_line, "MATRIX F=%d N=%d P=%f S=%f W=%f", matrix.K,
//      matrix.N, matrix.p, matrix.score, matrix.omega);
//    file << formatted_line << endl;
//    fill(formatted_line, formatted_line+100, 0);
////    std::fill(formatted_line, formatted_line)
////    file << "MATRIX K=" << matrix.K << " N=" << matrix.N << " P=" <<
////         matrix.p << " S=" << matrix.score << " W=" << matrix.omega << "\n";
//    file << "30 ";
//
//    for (char letter: kAlphabets) {
//      file << "    " << letter << " ";
//    }
//    file << "\n";
//    for (int i = 0; i < kProfileLength; i++) {
//      std::sprintf(formatted_line, "%2d ", i);
////      file << formatted_line;
////      fill(formatted_line, formatted_line+1000, 0);
//      for (int j=0;j<kAlphabets.size();j++){
//        char letter = *std::next(kAlphabets.begin(), j);
//        int letter_i = letter - 'A';
//        int output_int = matrix.freq[i][letter_i];
//        std::sprintf(formatted_line+6*j*sizeof(char)+3, "%5d ", output_int);
//      }
//      file << formatted_line << endl;
//      fill(formatted_line, formatted_line+1000, 0);
//    }
//    file << "END\n";
//  }
//  file.close();
//}
//
//std::vector<std::string> read_file(std::string const &fileName) {
//  std::vector<std::string> vecOfStrs;
//  // Open the File
//  std::ifstream in(fileName.c_str());
//
//  std::string str;
//  // Read the next line from File untill it reaches the end.
//  while (std::getline(in, str)) {
//    // Line contains string of length > 0 then save it in vector
//    if (!str.empty()) {
//      vecOfStrs.push_back(str);
//    }
//  }
//  in.close();
//  return vecOfStrs;
//}
//
//
//struct Sequence {
//  std::string description;
//  std::string sequence;
//};
//
//
//
//std::vector<double> make_composition(const std::vector<std::vector<int>>&
//  proteome){
//  std::vector<double> composition(26);
//  for (const std::vector<int>& line: proteome){
//    for (int letter_i: line){
//      composition[letter_i] += 1;
//    }
//  }
//  return composition;
//}
//
//template <typename... Args>
//void load(const std::string& filename, Args&... outputs){
//  std::ifstream file;
//  file.open(filename, std::ios_base::binary);
//  {
//    cereal::BinaryInputArchive iarchive(file); // Create an output archive
//    iarchive(outputs...);
//  }
//}
//
//int main(int argc, char *argv[]) {
//  for (int i=0;i<20;i++){
//    letter_int_map[kAlphabets[i]] = i;
//    int_letter_map[i] = kAlphabets[i];
//  }
//
//  int id;
//  int ierr;
//  int p;
//  int theta = 10;
//
//  const double omega = 1.0;
//
//  for (int i = 0; i < argc; i++) {
//    cout << *argv[i] << endl;
//  }
//  cout << argc << endl;
//
//  ierr = MPI_Init(&argc, &argv);
//  ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);
//  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);
//
//
//  std::string proteome_filename = "proteome_binary";
//  std::vector<std::string> headers;
//  std::vector<std::vector<int>> sequences;
//  cout << proteome_filename << endl;
//  load(proteome_filename, headers, sequences);
//
//
//// Encode seed into vector<string>, each of spacing 10
//  std::string seed_filename = "seed_seq_binary";
//  std::vector<std::vector<int>> seed_seqs;
//  load(seed_filename, seed_seqs);
//  cout << seed_seqs.size() << endl;
//
//
//
//// Encode blosum
//  std::string blosum_filename = "blosum_binary";
//  std::vector<std::vector<double>> kBlosum;
//  load(blosum_filename, kBlosum);
////  cout << kBlosum.size() << endl;
//  MPI_Barrier(MPI_COMM_WORLD);
//  MPI_Finalize();
//
//  return 0;
//
//  std::string const output_prefix = "output/output.matrix.%d";
//  std::string const composition_filename = "composition.csv";
//  const double epsilon = 1;
//  std::vector<Matrix> converged_matrices;
//
//  double E_value = 1;
//  int K_min = 1;
//  int kLetterX_i = 99;
////  int kProfileLength = 30;
//
////  std::vector<Sequence> proteome = load_fasta_sequences(proteome_filename);
//
////  std::vector<std::vector<double>> kBlosum = read_blosum("BLOSUM62");
//  std::vector<double> composition = make_composition(sequences);
//  double composition_sum_squares = 0.0;
//  for (double value: composition){
//    composition_sum_squares += value * value;
//  }
//
//
////  std::string initial_seq = load_seed_seq(seed_seq_filename);
////  std::vector<std::string> initial_seqs = split_seq(initial_seq, 10,
////    kProfileLength);
//
//  for (std::vector<int> seed_seq: seed_seqs) {
//    std::vector<std::vector<double>> M(kProfileLength, std::vector<double>(20));
//    std::vector<std::vector<double>> PSSM(kProfileLength,
//                                          std::vector<double>(20));
//    std::vector<std::vector<double>> PSSM2(kProfileLength,
//                                          std::vector<double>(20));
//    for (int i = 0; i < kProfileLength; i++) {
//      int j = seed_seq[i];
//      if (j != kLetterX_i){
//        M[i][j] = 1.0;
//        //      pssm for this row will be all 0, if I skip X.
//        PSSM[i] = kBlosum[j];
//      }
//    }
//
//
////    //        debugging purpose
////    for (std::vector<double> line: kBlosum){
////      for (double value: line){
////        cout << value << " ";
////      }
////      cout << "\n";
////    }
////    cout << "\n" << endl;
//////    debugging end
////    //        debugging purpose
////    for (std::vector<double> line: PSSM){
////      for (double value: line){
////        cout << value << " ";
////      }
////      cout << "\n";
////    }
////    cout << "\n" << endl;
//////    debugging end
//
//
//    bool converged = false;
//    double S;
//    std::vector<double> Information(kProfileLength);
//    int iteration = 0;
//    while (!converged) {
//      for (int i = 0; i < kProfileLength; i++) {
//        double H_sum = 0.0;
//        for (int j = 0; j < kProfileLength; j++) {
//          for (int k = 0; k < 20; k++) {
//            if (M[i][k] > 0.0) {
//              H_sum += M[i][k] * (log(M[i][k]) / log(2));
//            }
//          }
//        }
//        Information[i] = log(20) / log(2) + H_sum;
//
//        if (Information[i] < 1.0) {
//          Information[i] = 0.0;
//        }
//      }
//
//
//      double maxS = 0.0;
//      auto sampled_maxS = (double) -INFINITY;
//      /////////////////// BACKGROUND DISTRIBUTIONS ///////////////////
//      int kNumberRandomisations = 10;
//      S = 0.0;
//      for (int rand_count = 0;
//           rand_count < kNumberRandomisations; rand_count++) {
//        for (const std::vector<int>& protein: sequences) {
//          int protein_length = (int) protein.size();
//          for (int m = 0; m + kProfileLength <= protein_length; m++) {
//            S = 0.0;
//            for (int j = 0; j < kProfileLength; j++) {
//              // compare 50 residues in one fragment from proteome with reshuffled matrix position
//              if (protein[m + j] != kLetterX_i) {
//                int rand_i = random_index_30[rand_count][j];
//
////                cout << PSSM[rand_i][letter] << " " << endl;
//                S += Information[rand_i] * PSSM[rand_i][protein[m + j]];
////                cout << PSSM[rand_i][letter] << " " << Information[rand_i] <<
////                endl;
////                debugging
//                assert (PSSM[rand_i][protein[m + j]] != 999);
//              }
//            }
//
////            cout << PSSM[rand_i] << endl;
//            S /= kProfileLength;
//            if (S > maxS) {
//              maxS = S;
//            }
//          }
//        }
//        if (sampled_maxS < maxS){
//          sampled_maxS = maxS;
//        }
//      }
//      int Kmatches = 0;
//
//      cout << "Max_S " << maxS << endl;
//      cout << "Sampled_MaxS " << sampled_maxS << endl;
//      maxS = sampled_maxS;
//      std::vector<std::vector<double>> F(kProfileLength,
//                                         std::vector<double>(26));
//      for (const std::vector<int>& protein: sequences) {
//        int protein_length = (int) protein.size();
//        for (int m = 0; m + kProfileLength <= protein_length; m++) {
//          S = 0.0;
//          for (int j = 0; j < kProfileLength; j++) {
//            // compare 50 residues in one fragment from proteome with reshuffled matrix position
//            if (protein[m + j] != kLetterX_i) {
//              S += Information[j] * PSSM[j][protein[m + j]];
//              assert (PSSM[j][protein[m + j]] != 999);
//            }
//          }
//          S /= kProfileLength;
////          cout << maxS << endl;
//          if (S > maxS) {
//            Kmatches++;
//            for (int j = 0; j < kProfileLength; j++) {
//              if (protein[m + j] != kLetterX_i) {
//                F[j][protein[m + j]]++;
//              }
//            }
//          }
//        }
//      }
//      std::vector<std::vector<double>> M1(kProfileLength, std::vector<double>
//        (20));
////      for (int i = 0; i < kProfileLength; i++) {
////        std::fill(PSSM[i].begin(), PSSM[i].end(), 0.0);
////      }
//
//      for (int q=0;q<20;q++) {
//        for (int i=0;i<kProfileLength;i++) {
//          M1[i][q] = (F[i][q] + omega * composition[q] * composition[q]) /
//                     (Kmatches + omega * composition_sum_squares);
//          PSSM[i][q] = log(M1[i][q] / composition[q]);
//        }
//      }
//
//      double distance = 0.0;
//      for (int i=0;i<kProfileLength;i++) {
//        double sum = 0.0;
//        for (int q=0;q<20;q++) {
//          sum += (M1[i][q] - M[i][q]) * (M1[i][q] - M[i][q]);
//        }
//        cout << sum << " ";
//        distance += sqrt(sum);
//      }
//      cout << distance;
//      cout << endl;
////      if (distance < epsilon || iteration >= theta) {
//      iteration++;
//      cout << distance << " " << epsilon << endl;
//      cout << "Kmatches: " << Kmatches << " " << K_min << endl;
//      if (distance < epsilon || iteration >= theta) {
//        converged = true;
//
//        if (Kmatches < K_min) {
//          break;
//        }
//        Matrix matrix;
//        vector<char> seed_seq_v;
//        for(int letter_i: seed_seq){
//          seed_seq_v.push_back(int_letter_map[letter_i]);
//        }
//        std::string seed_seq_letters(seed_seq_v.begin(), seed_seq_v.end());
//        matrix.initial_segment = seed_seq_letters;
//        matrix.K = Kmatches;
////        matrix.L = kProfileLength;
////        todo: figure out what N is
//        matrix.N = 100000;
//        matrix.p = p;
////        matrix.id = 0; //sequential number of the origin segment or matrix
//        matrix.omega = omega;
//        matrix.score = maxS;
//        matrix.freq = F;
//        converged_matrices.push_back(matrix);
//      } else {
//        M = M1;
//      }
//    }
//  }
//
//  write_Matrices("output.1.matrix", converged_matrices);
////  write_VariableMatrixFreq("output.4.matrix", converged_matrices, rank, composition);
//
//  MPI_Barrier(MPI_COMM_WORLD);
//  MPI_Finalize();
//
//  return 0;
//}
//
//
////if (!((PSSM[rand_i][letter] > -10) && (PSSM[rand_i][letter]
////< 20))){
////cout << "\n\n";
////for (int i = 0; i < kProfileLength; i++) {
////cout << seed_seq[i] << " ";
////}
////cout << endl;
////
////
//////        debugging purpose
////for (std::vector<double> line: PSSM){
////for (double value: line){
////cout << value << " ";
////}
////cout << "\n";
////}
////cout << "\n" << endl;
//////    debugging end
////cout << PSSM[rand_i][letter] << " " << rand_i << " " <<
////protein[m + j] << endl;
////}
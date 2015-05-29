/*
 * singled-end read mapping
 * */
#ifndef MAPPING_HPP_
#define MAPPING_HPP_

#include "reference.hpp"

#include <string>
#include <vector>

/* BestMatch records the match position with the minimal number
 * of mismatches. times records how many genome positions with
 * this minimal number of mismatches. If times equals 1, means
 * the read is uniquely matched to the genome with the minimal
 * number of mismatches. */
struct BestMatch {
  BestMatch(const uint32_t& _genome_pos = 0, const uint32_t& _times = 0,
            const char& _strand = '+', const uint32_t& _mismatch = MAX_UINT32)
      : genome_pos(_genome_pos),
        times(_times),
        strand(_strand),
        mismatch(_mismatch) {
  }

  uint32_t genome_pos;
  uint32_t times;
  char strand;
  uint32_t mismatch;
};

/* count the number of uniquely mapped, ambiguous mapped and unmapped reads */
struct StatSingleReads {
  StatSingleReads(const bool& _ambiguous, const bool& _unmapped,
                  const string& output_file)
      : ambiguous(_ambiguous),
        unmapped(_unmapped) {
    total_reads = 0;
    unique_mapped_reads = 0;
    ambiguous_mapped_reads = 0;
    unmapped_reads = 0;

    if (ambiguous) {
      fambiguous = fopen(string(output_file + "_ambiguous").c_str(), "w");
    }
    if (unmapped) {
      funmapped = fopen(string(output_file + "_unmapped").c_str(), "w");
    }
  }
  ~StatSingleReads() {
    if (ambiguous) {
      fclose(fambiguous);
    }
    if (unmapped) {
      fclose(funmapped);
    }
  }

  uint32_t total_reads;
  uint32_t unique_mapped_reads;
  uint32_t ambiguous_mapped_reads;
  uint32_t unmapped_reads;

  FILE * fambiguous;
  FILE * funmapped;

  bool ambiguous;
  bool unmapped;
};

/* load reads from reads file, each time load n_reads_to_process reads,
 * start from  read_start_idx */
void LoadReadsFromFastqFile(FILE * fin, const uint32_t read_start_idx,
                            const uint32_t n_reads_to_process,
                            const string& adaptor, uint32_t& num_of_reads,
                            vector<string>& read_names,
                            vector<string>& read_seqs,
                            vector<string>& read_scores);

/* reads from _1 file, Cs are transfered to Ts*/
void C2T(const string& org_read, const uint32_t& read_len, string& read);

/* reads from _2 file, Gs are transfered to As*/
void G2A(const string& org_read, const uint32_t& read_len, string& read);

/* find the region of index where those positions started with the seed */
void IndexRegion(const string& read, const Genome& genome,
                 const HashTable& hash_table, const uint32_t& seed_len,
                 pair<uint32_t, uint32_t>& region);

/* output the uniquely mapped reads or ambiguously mapped reads */
void OutputUniquelyAndAmbiguousMapped(FILE * fout, const BestMatch best_match,
                                      const string& read_name,
                                      const string& read_seq,
                                      const string& read_score,
                                      const Genome& genome);

/* output the unmapped reads */
void OutputUnmapped(FILE * fout, const string& read_name,
                    const string& read_seq, const string& read_score);

/* singled-end read */
void ProcessSingledEndReads(const string& index_file,
                            const string& reads_file_s,
                            const string& output_file,
                            const uint32_t& n_reads_to_process,
                            const uint32_t& max_mismatches,
                            const string& adaptor, const bool& AG_WILDCARD,
                            const bool& ambiguous, const bool& unmapped);

#endif /* MAPPING_HPP_ */

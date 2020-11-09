// Copyright 2011 Google Inc. All Rights Reserved.
// Author: dehao@google.com (Dehao Chen)

// Create AutoFDO profile.

#ifndef AUTOFDO_PROFILE_CREATOR_H_
#define AUTOFDO_PROFILE_CREATOR_H_

#include "addr2line.h"
#include "sample_reader.h"
#include "symbol_map.h"
#include "profile_writer.h"

namespace devtools_crosstool_autofdo {

class ProfileCreator {
 public:
  explicit ProfileCreator(const std::string &binary)
      : sample_reader_(nullptr),
        binary_(binary),
        use_discriminator_encoding_(false) {}

  ~ProfileCreator() {
    delete sample_reader_;
  }

  void set_use_discriminator_encoding(bool use_discriminator_encoding) {
    use_discriminator_encoding_ = use_discriminator_encoding;
  }

  // Returns the total sample counts from a text profile.
  static uint64 GetTotalCountFromTextProfile(
      const std::string &input_profile_name);

  // Creates AutoFDO profile, returns true if success, false otherwise.
  bool CreateProfile(const std::string &input_profile_name,
                     const std::string &profiler,
                     devtools_crosstool_autofdo::ProfileWriter *writer,
                     const std::string &output_profile_name,
                     bool store_sym_list_in_profile = false);

  // Reads samples from the input profile.
  bool ReadSample(const std::string &input_profile_name,
                  const std::string &profiler);

  // Returns total number of samples collected.
  uint64 TotalSamples();

  // Returns the SampleReader pointer.
  const devtools_crosstool_autofdo::SampleReader &sample_reader() {
    return *sample_reader_;
  }

  // Computes the profile and updates the given symbol map and addr2line
  // instance.
  bool ComputeProfile(devtools_crosstool_autofdo::SymbolMap *symbol_map);

 private:
  bool ConvertPrefetchHints(const std::string &profile_file,
                            SymbolMap *symbol_map);
  bool CheckAndAssignAddr2Line(SymbolMap *symbol_map, Addr2line *addr2line);

  SampleReader *sample_reader_;
  std::string binary_;
  bool use_discriminator_encoding_;
};

bool MergeSample(const std::string &input_file,
                 const std::string &input_profiler, const std::string &binary,
                 const std::string &output_file);
}  // namespace devtools_crosstool_autofdo

#endif  // AUTOFDO_PROFILE_CREATOR_H_

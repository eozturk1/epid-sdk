/*############################################################################
  # Copyright 2016-2018 Intel Corporation
  #
  # Licensed under the Apache License, Version 2.0 (the "License");
  # you may not use this file except in compliance with the License.
  # You may obtain a copy of the License at
  #
  #     http://www.apache.org/licenses/LICENSE-2.0
  #
  # Unless required by applicable law or agreed to in writing, software
  # distributed under the License is distributed on an "AS IS" BASIS,
  # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  # See the License for the specific language governing permissions and
  # limitations under the License.
  ############################################################################*/

/*!
 * \file
 * \brief VerifyBasicSig unit tests.
 */

#include <cstring>
#include <vector>

#include "epid/common-testhelper/epid_gtest-testhelper.h"
#include "gtest/gtest.h"

extern "C" {
#include "epid/verifier/api.h"
#include "epid/verifier/src/verifybasic.h"
}

#include "epid/common-testhelper/errors-testhelper.h"
#include "epid/common-testhelper/verifier_wrapper-testhelper.h"
#include "epid/verifier/unittests/verifier-testhelper.h"

namespace {

TEST_F(EpidVerifierTest, VerifyBasicSigFailsGivenNullPtr) {
  VerifierCtxObj verifier(this->kGrp01Key);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrp01Member0Sha256RandombaseTest1NoSigRl.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto& msg = this->kTest1;

  EXPECT_EQ(kEpidBadArgErr,
            EpidVerifyBasicSig(nullptr, &basic_sig, msg.data(), msg.size()));
  EXPECT_EQ(kEpidBadArgErr,
            EpidVerifyBasicSig(verifier, nullptr, msg.data(), msg.size()));
  EXPECT_EQ(kEpidBadArgErr,
            EpidVerifyBasicSig(verifier, &basic_sig, nullptr, msg.size()));
}

TEST_F(EpidVerifierTest, VerifyBasicSigCanVerifyValidSignatureWithSHA256) {
  VerifierCtxObj verifier(this->kGrp01Key);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrp01Member0Sha256RandombaseTest1NoSigRl.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto& msg = this->kTest1;

  EXPECT_EQ(kEpidNoErr,
            EpidVerifyBasicSig(verifier, &basic_sig, msg.data(), msg.size()));
}

TEST_F(EpidVerifierTest, VerifyBasicSigCanVerifyValidSignatureWithSHA384) {
  GroupPubKey pub_key = this->kGrp01Key;
  pub_key.gid.data[1] = 1;
  VerifierCtxObj verifier(pub_key);

  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrp01Member0Sha384RandombaseTest1NoSigRl.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto& msg = this->kTest1;
  EXPECT_EQ(kEpidNoErr,
            EpidVerifyBasicSig(verifier, &basic_sig, msg.data(), msg.size()));
}

TEST_F(EpidVerifierTest, VerifyBasicSigCanVerifyValidSignatureWithSHA512) {
  GroupPubKey pub_key = this->kGrp01Key;
  pub_key.gid.data[1] = 2;
  VerifierCtxObj verifier(pub_key);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrp01Member0Sha512RandombaseTest1NoSigRl.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto& msg = this->kTest1;
  EXPECT_EQ(kEpidNoErr,
            EpidVerifyBasicSig(verifier, &basic_sig, msg.data(), msg.size()));
}

TEST_F(EpidVerifierTest, VerifyBasicSigCanVerifyValidSignatureWithSHA512256) {
  GroupPubKey pub_key = this->kGrpXKey;
  pub_key.gid.data[1] = 3;
  VerifierCtxObj verifier(pub_key);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrpXMember0Sha512256RandombaseMsg0NoSigRl.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto& msg = this->kMsg0;
  EXPECT_EQ(kEpidNoErr,
            EpidVerifyBasicSig(verifier, &basic_sig, msg.data(), msg.size()));
}

TEST_F(EpidVerifierTest,
       VerifyBasicSigDetectsInvalidSignatureGivenMatchingMessage) {
  VerifierCtxObj verifier(this->kGrp01Key);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrp01Member0Sha256RandombaseTest1NoSigRl.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto& msg = this->kTest1;
  BasicSignature corrupted_basic_sig = basic_sig;
  corrupted_basic_sig.B.x.data.data[0]++;
  EXPECT_NE(kEpidNoErr, EpidVerifyBasicSig(verifier, &corrupted_basic_sig,
                                           msg.data(), msg.size()));
}

TEST_F(EpidVerifierTest,
       VerifyBasicSigDetectsInvalidSignatureGivenMessageMismatch) {
  VerifierCtxObj verifier(this->kGrp01Key);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrp01Member0Sha256RandombaseTest1NoSigRl.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto msg = this->kTest1;
  msg[0]++;  // change message for signature verification to fail
  EXPECT_EQ(kEpidSigInvalid,
            EpidVerifyBasicSig(verifier, &basic_sig, msg.data(), msg.size()));
}

TEST_F(EpidVerifierTest, VerifyBasicSigCanVerifyWithBasename) {
  VerifierCtxObj verifier(this->kGrp01Key);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrp01Member0Sha256Basename1Test1NoSigRl.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto& msg = this->kTest1;
  auto& basename = this->kBasename1;
  THROW_ON_EPIDERR(
      EpidVerifierSetBasename(verifier, basename.data(), basename.size()));
  EXPECT_EQ(kEpidNoErr,
            EpidVerifyBasicSig(verifier, &basic_sig, msg.data(), msg.size()));
}

TEST_F(EpidVerifierTest, VerifyBasicSigCanVerifyWithBasenameUsingIkgfData) {
  VerifierCtxObj verifier(this->kPubKeyIkgfStr);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigSha256Basename1Test1NoSigRlIkgf.data();
  const BasicSignature basic_sig = sig->sigma0;
  auto& msg = this->kTest1;
  auto& basename = this->kBasename1;
  THROW_ON_EPIDERR(
      EpidVerifierSetBasename(verifier, basename.data(), basename.size()));
  EXPECT_EQ(kEpidNoErr,
            EpidVerifyBasicSig(verifier, &basic_sig, msg.data(), msg.size()));
}

TEST_F(EpidVerifierTest, VerifyBasicSigCanVerifyMsgContainingAllPossibleBytes) {
  GroupPubKey pub_key = this->kPubKeySigRlVerify;
  // Initialize pubkey.gid to sha512
  pub_key.gid.data[1] = 2;
  VerifierCtxObj verifier(pub_key);
  auto const& sig = (EpidNonSplitSignature const*)this
                        ->kSigGrp01Member0Sha512kBsn0Data_0_255.data();
  const BasicSignature basic_sig = sig->sigma0;

  auto& msg = this->kData_0_255;
  auto& basename = this->kBsn0;
  THROW_ON_EPIDERR(
      EpidVerifierSetBasename(verifier, basename.data(), basename.size()));
  EXPECT_EQ(kEpidNoErr,
            EpidVerifyBasicSig(verifier, &basic_sig, msg.data(), msg.size()));
}

}  // namespace

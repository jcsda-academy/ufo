/*
 * (C) Copyright 2019 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#ifndef TEST_UFO_OBSDIAGNOSTICS_H_
#define TEST_UFO_OBSDIAGNOSTICS_H_

#include <memory>
#include <string>
#include <vector>

#define ECKIT_TESTING_SELF_REGISTER_CASES 0

#include "eckit/config/LocalConfiguration.h"
#include "eckit/testing/Test.h"
#include "ioda/ObsSpace.h"
#include "ioda/ObsVector.h"
#include "oops/../test/TestEnvironment.h"
#include "oops/base/Variables.h"
#include "oops/runs/Test.h"
#include "ufo/GeoVaLs.h"
#include "ufo/ObsBias.h"
#include "ufo/ObsDiagnostics.h"
#include "ufo/ObsOperator.h"

namespace ufo {
namespace test {

// -----------------------------------------------------------------------------

void testObsDiagnostics() {
  const eckit::LocalConfiguration conf = ::test::TestEnvironment::config();

  //  Setup ObsSpace
  util::DateTime bgn(conf.getString("window_begin"));
  util::DateTime end(conf.getString("window_end"));
  const eckit::LocalConfiguration obsconf(conf, "ObsSpace");
  ioda::ObsSpace ospace(obsconf, bgn, end);
  const size_t nlocs = ospace.nlocs();

  // initialize observation operator (set variables requested from the model,
  // variables simulated by the observation operator, other init)
  eckit::LocalConfiguration obsopconf(conf, "ObsOperator");
  ObsOperator hop(ospace, obsopconf);

  // read geovals from the file
  eckit::LocalConfiguration gconf(conf, "GeoVaLs");
  const GeoVaLs gval(gconf, ospace, hop.variables());

  // initialize bias correction
  const ObsBias ybias(conf);

  // create obsvector to hold H(x)
  ioda::ObsVector hofx(ospace);

  // create diagnostics to hold HofX diags
  eckit::LocalConfiguration diagconf(conf, "ObsDiag");
  oops::Variables diagvars(diagconf);
  EXPECT(diagvars.size() > 0);
  Locations * locs = hop.locations(bgn, end);
  ObsDiagnostics diags(ospace, *locs, diagvars);

  // call H(x) to compute diagnostics
  hop.simulateObs(gval, hofx, ybias, diags);

  // read tolerance and reference Diagnostics
  const double tol = conf.getDouble("tolerance");
  eckit::LocalConfiguration diagrefconf(conf, "Reference ObsDiag");
  ObsDiagnostics diagref(diagrefconf, ospace, diagvars);

  // loop over all diag variables and levels and compare with reference
  for (size_t ivar = 0; ivar < diagvars.size(); ivar++) {
    const size_t nlevs = diags.nlevs(diagvars[ivar]);
    EXPECT(nlevs == diagref.nlevs(diagvars[ivar]));
    for (size_t ilev = 0; ilev < nlevs; ilev++) {
      std::vector<float> ref(nlocs);
      std::vector<float> computed(nlocs);
      diags.get(computed, diagvars[ivar], ilev+1);
      diagref.get(ref, diagvars[ivar], ilev+1);

      float rms = 0.0;
      for (size_t iloc = 0; iloc < nlocs; iloc++) {
        ref[iloc] -= computed[iloc];
        rms += ref[iloc] * ref[iloc];
      }
      rms = sqrt(rms / nlocs);

      EXPECT(rms < 100*tol);
      oops::Log::info() << diagvars[ivar] << ", level " << ilev <<
          ": difference between reference and computed: " << ref << std::endl;
    }
  }
}

// -----------------------------------------------------------------------------

class ObsDiagnostics : public oops::Test {
 public:
  ObsDiagnostics() {}
  virtual ~ObsDiagnostics() {}
 private:
  std::string testid() const {return "ufo::test::ObsDiagnostics";}

  void register_tests() const {
    std::vector<eckit::testing::Test>& ts = eckit::testing::specification();

    ts.emplace_back(CASE("ufo/ObsDiagnostics/testObsDiagnostics")
      { testObsDiagnostics(); });
  }
};

// -----------------------------------------------------------------------------

}  // namespace test
}  // namespace ufo

#endif  // TEST_UFO_OBSDIAGNOSTICS_H_
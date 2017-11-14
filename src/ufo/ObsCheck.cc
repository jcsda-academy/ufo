/*
 * (C) Copyright 2017 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "ObsCheck.h"

#include "eckit/config/Configuration.h"
#include "ObsSpace.h"
#include "Fortran.h"
#include "Variables.h"
#include "GeoVaLs.h"
#include "ObsVector.h"
#include "util/Logger.h"

namespace ufo {

// -----------------------------------------------------------------------------

ObsCheck::ObsCheck(const ObsSpace & obsdb, const Variables & var,
                 const util::DateTime & t1, const util::DateTime & t2) {
  oops::Log::trace() << "ObsCheck contructor starting " << t1 << " " << t2 << std::endl;
  const util::DateTime * p1 = &t1;
  const util::DateTime * p2 = &t2;
//  ufo_obsdb_getobscheck_f90(obsdb.toFortran(), var.toFortran(), &p1, &p2, keyObsCheck_);
  oops::Log::trace() << "ObsCheck contructor key = " << keyObsCheck_ << std::endl;
}

// -----------------------------------------------------------------------------

ObsCheck::ObsCheck(const eckit::Configuration & config) {
  oops::Log::trace() << "ObsCheck contructor config starting" << std::endl;
//  ufo_obscheck_create_f90(keyObsCheck_);
  const eckit::Configuration * conf = &config;
//  ufo_obscheck_read_file_f90(keyObsCheck_, &conf);
  oops::Log::trace() << "ObsCheck contructor config key = " << keyObsCheck_ << std::endl;
}

// -----------------------------------------------------------------------------

ObsCheck::ObsCheck(const ObsSpace & os) {
  oops::Log::trace() << "ObsCheck ObsSpace starting" << std::endl;
  oops::Log::trace() << "ObsCheck ObsSpace end " << std::endl;
}

// -----------------------------------------------------------------------------

ObsCheck::~ObsCheck() {
//  ufo_obscheck_delete_f90(keyObsCheck_);
}

// -----------------------------------------------------------------------------

void ObsCheck::print(std::ostream & os) const {
  os << "ObsCheck::print not implemented";
}

// -----------------------------------------------------------------------------

void ObsCheck::postFilter(const GeoVaLs & gv, const ObsVector & ov, const ObsSpace & os) const {
  oops::Log::trace() << "ObsCheck postFilter starting" << std::endl;
  ufo_postFilter_f90(gv.toFortran(), ov.toFortran());
  oops::Log::trace() << "ObsCheck postFilter end" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsCheck::priorFilter(const GeoVaLs & gv, const ObsVector & ov, const ObsSpace & os) const {
  oops::Log::trace() << "ObsCheck priorFilter starting" << std::endl;
  ufo_priorFilter_f90(gv.toFortran(), ov.toFortran());
  oops::Log::trace() << "ObsCheck priorFilter end" << std::endl;
}

// -----------------------------------------------------------------------------
}  // namespace ufo

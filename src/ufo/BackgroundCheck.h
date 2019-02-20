/*
 * (C) Copyright 2017-2018 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#ifndef UFO_BACKGROUNDCHECK_H_
#define UFO_BACKGROUNDCHECK_H_

#include <ostream>
#include <string>

#include "oops/base/Variables.h"
#include "oops/util/ObjectCounter.h"
#include "oops/util/Printable.h"
#include "ufo/BackgroundCheck.interface.h"

namespace eckit {
  class Configuration;
}

namespace oops {
  class Variables;
}

namespace ioda {
  class ObsSpace;
  class ObsVector;
}

namespace ufo {
  class GeoVaLs;

/// BackgroundCheck: check observation closeness to background

class BackgroundCheck : public util::Printable,
                        private util::ObjectCounter<BackgroundCheck> {
 public:
  static const std::string classname() {return "ufo::BackgroundCheck";}

  BackgroundCheck(const ioda::ObsSpace &, const eckit::Configuration &);
  ~BackgroundCheck();

  void priorFilter(const GeoVaLs &) const;
  void postFilter(const ioda::ObsVector &) const;

  const oops::Variables & requiredGeoVaLs() const {return geovars_;}

 private:
  void print(std::ostream &) const;
  F90bgcheck key_;
  const oops::Variables geovars_;
};

}  // namespace ufo

#endif  // UFO_BACKGROUNDCHECK_H_
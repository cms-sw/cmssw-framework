
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "Math/Cartesian3D.h" 
#include "DataFormats/Math/interface/Vector3D.h" 
#include "Math/Polar3D.h" 
#include "Math/CylindricalEta3D.h" 
#include "DataFormats/Math/interface/Vector.h" 

namespace DataFormats_BeamSpot {
  struct dictionary {
    reco::BeamSpot b;
    edm::Wrapper<reco::BeamSpot> b_w;
  };      
}

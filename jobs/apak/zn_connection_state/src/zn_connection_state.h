#ifndef ZN_CONNECTION_STATE_H
#define ZN_CONNECTION_STATE_H

#include "zn_connection_state_global.h"

#include "../../../../../Modus/global/job/sv_abstract_job.h"
#include "../../../../../Modus/global/signal/sv_signal.h"

#include "zn_connection_state_defs.h"

extern "C" {

  ZN_CONNECTION_STATE_EXPORT modus::SvAbstractJob* create();

}

namespace zncs {


  const QMap<QString, modus::SvSignal* signal> SupportedSignals = {{}, {}, {}};

  class ZnConnectionState: public modus::SvAbstractJob
  {

  public:
    ZnConnectionState();

    bool configure(modus::JobConfig* config) override;
    bool signalIsSupported(modus::SvSignal* signal) override;

    void start() override;

  private:
    zncs::Params       m_params;

  };


}

#endif // ZN_CONNECTION_STATE_H

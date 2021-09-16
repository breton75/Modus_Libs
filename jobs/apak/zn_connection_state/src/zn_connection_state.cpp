#include "zn_connection_state.h"


zncs::ZnConnectionState::ZnConnectionState()
{
}


bool zncs::ZnConnectionState::configure(modus::JobConfig* config)
{
  try {

    p_config = config;

    m_params = zncs::Params::fromJsonString(p_config->params);


  }
  catch(SvException& e) {

    p_last_error = e.error;
    return false;
  }

  return true;
}

bool zncs::ZnConnectionState::signalIsSupported(modus::SvSignal* signal)
{

}

void zncs::ZnConnectionState::start()
{

}

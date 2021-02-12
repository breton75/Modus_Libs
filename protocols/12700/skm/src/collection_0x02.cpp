#include "collection_0x02.h"

skm::Type0x02::Type0x02():
  SvAbstractSignalCollection()
{

}

void skm::Type0x02::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try
  {
    skm::SignalParams_0x02 p = skm::SignalParams_0x02::fromJson(signal->config()->params);

    quint32 uid = getUid(p.byte, p.bit);

    if(m_signals.contains(uid))
      throw SvException(QString("Не уникальные значения параметров: '%1'")
                        .arg(signal->config()->params));

    m_signals.insert(uid, signal);

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name).arg(e.error));
  }
}

void skm::Type0x02::updateSignals(const skm::DATA* data)
{
  if(!data)
    return;

  if(data->data.size() < 3)
    return;

  int b[3];
  b[0] = 0;
  b[1] = check_1F_2F_55(data->data.at(b[0])) ? b[0] + 2 : b[0] + 1;
  if(b[1] >= data->data.size())
    return;

  b[2] = check_1F_2F_55(data->data.at(b[1])) ? b[1] + 2 : b[1] + 1;
  if(b[2] >= data->data.size())
    return;

  for(quint8 i = 0; i < 3; i++ ) {
    for(quint8 j = 0; j < 8; j++) {

      quint32 uid = getUid(i, j);

      if(m_signals.contains(uid))
        m_signals.value(uid)->setValue(((quint8)data->data.at(b[i]) >> j) & 1);
    }
  }
}

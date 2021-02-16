#include "collection_0x01.h"

skm::Type0x01::Type0x01():
  SvAbstractSignalCollection()
{

}

void skm::Type0x01::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try
  {
    skm::SignalParams_0x01 p = skm::SignalParams_0x01::fromJson(signal->config()->params);

    if(m_signals.contains(getUid(0, 0, p.vin, p.faktor)))
      throw SvException(QString("Не уникальные значения параметров: '%1'")
                        .arg(signal->config()->params));

    m_signals.insert(getUid(0, 0, p.vin, p.faktor), signal);

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name, e.error));
  }
}

void skm::Type0x01::updateSignals(const skm::DATA* data)
{
  if(!data)
    return;

  quint8  faktor_count  = 0;
  quint16 offset        = 0;
  quint8  vin;

  while(data->len - offset > 2) {

    vin          = (quint8)data->data[offset++];
    faktor_count = (quint8)data->data[offset++];

    if(data->len - offset < faktor_count)
      break;

    while(faktor_count) {

        quint8 faktor = data->data[offset++];
        if(check_1F_2F_55(faktor)) offset++;

        quint32 uid = getUid(0, 0, vin, faktor);

        if(m_signals.contains(uid))
          m_signals.value(uid)->setValue(1);

        faktor_count--;
    }
  }
}
